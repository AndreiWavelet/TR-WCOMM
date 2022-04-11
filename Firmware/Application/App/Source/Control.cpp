#include <App/Control.h>

#include <App/Main.h>
#include <App/SDCard.h>
#include <App/Tasks.h>

#include <Drivers/Battery.h>
#include <Drivers/Leds.h>
#include <Drivers/NRF24L01p.h>
#include <Drivers/Power.h>
#include <Drivers/SpiManager.h>
#include <Drivers/Switches.h>

#include <algorithm>
#include <cctype>
#include <string.h>

using namespace App;

static constexpr uint32_t POWER_ON_BLINKS = 3;
static constexpr uint32_t POWER_OFF_BLINKS = 5;
static constexpr uint32_t NRF_ERROR_BLINKS = 7;
static constexpr uint32_t CANT_OPEN_FILE_BLINKS = 3;
static constexpr uint32_t SDCARD_MISSING_BLINKS = 2;
static constexpr uint32_t NO_FILE_RECEPTOR_BLINKS = 1;

static constexpr uint32_t DEFAULT_BLINK_ON_TIME = 50; //[ms]
static constexpr uint32_t DEFAULT_BLINK_OFF_TIME = 350; //[ms]

static constexpr uint32_t SEND_REQUEST_TIME = 1000; //[ms]
static constexpr uint32_t POWER_OFF_TIME = 3000; //[ms]

static constexpr uint32_t SLEEP_TIME = 3000; //[ms]
static constexpr uint32_t RADIO_RX_TIME = 1000; //[ms]
static constexpr uint32_t FILE_TIMEOUT = 10000; //[ms]
static constexpr uint32_t FILE_ACCEPT_TIMEOUT = 300; //[ms]

static constexpr char WCOMM_CFG_FILE[] = "wcomm.cfg";
static constexpr char DEFAULT_FILE_TO_SEND[] = "data.dat";

static constexpr char MAX_FILE_LINE_LENGTH = 64;

typedef enum {
	csPOWER_ON, csWAIT_PERIPHERALS, csPARSE_CONFIG, csWAIT_TO_SLEEP, csSLEEP, csWAIT_SLEEP,
	csSTART_RADIO_RX, csWAIT_FILE_REQ_RX,
	csSEND_FILE_ACCEPT, csWAIT_SEND_FILE_ACCEPT, csRECEIVE_FILE,
	csSEND_FILE_TRANSFER_REQ, csWAIT_FILE_TRANSFER_REQ,
	csWAIT_FILE_ACCEPT,
	csSEND_FILE_PART, csWAIT_SEND_FILE_PART,
	csPOWER_OFF, csWAIT_POWER_OFF,
} State;

typedef enum {
	ptSEND_FILE, ptFILE_DATA, ptCANCEL, ptDONE, ptACCEPT_FILE
} PacketType;

Control::Control() : m_state(csPOWER_ON){
	strncpy(m_fileToSend,DEFAULT_FILE_TO_SEND,sizeof(m_fileToSend));
	return;
}

Control::~Control(){
	return;
}

bool Control::DoParseConfig(){
	//Basic parsing function.

	//Open config file.
	bool ok = m_fil.Open(WCOMM_CFG_FILE, false, true, true);
	if(!ok){
		return false;
	}

	//Scan all lines in file.
	while(m_fil.GetPosition()<m_fil.GetSize()){
		//Read next line.
		char dt[MAX_FILE_LINE_LENGTH];
		auto size = m_fil.ReadLine(dt, sizeof(dt));
		dt[size] = 0;

		//Setup parser.
		char* firstValue = nullptr;
		char* secondValue = nullptr;

        bool foundElem = false;
		char* firstAlnum = dt;
		char* lastAlnum = dt-1;

		//Find tokens.
		char* ch = dt;
		while(*ch!='\0'){
			//Find first and last alphanumeric character before '=' or '\0'.
			if(isalnum(*ch)){
				if(!foundElem){
					firstAlnum = ch;
					foundElem = true;
				}
				lastAlnum = ch;
			}

			if(!firstValue && *ch == '='){
				if(firstValue==nullptr){
					firstValue = firstAlnum;
					*(lastAlnum+1) = '\0';
					foundElem = false;
				}
				firstAlnum = ch;
				lastAlnum = ch-1;
			}
			ch++;
		}

		if(foundElem){
		    secondValue = firstAlnum;
			*(lastAlnum+1) = '\0';
		}

		if(firstValue && secondValue){
			ConvertToLowerCase(firstValue);
			ParseValuePair(firstValue, secondValue);
		}
	}

	m_fil.Close();

	return true;
}

void Control::ParseValuePair(const char* first, const char* second){
	//Parse value pairs.
	if(strncmp(first,"file",MAX_FILE_LINE_LENGTH)==0){
		strncpy(m_fileToSend,second,sizeof(m_fileToSend));
	}else if(strncmp(first,"txaddr",MAX_FILE_LINE_LENGTH)==0
			||strncmp(first,"dest",MAX_FILE_LINE_LENGTH)==0){
		Driver::NRF24L01p::Address addr;
		memset(addr.value,0,sizeof(addr.value));
		for(uint32_t t = 0;second[t]!='\0';addr.value[t] = second[t], t++);
		Tasks::NRF24L01p.SetTxAddress(addr);
	}else if(strncmp(first,"rxaddr",MAX_FILE_LINE_LENGTH)==0
			||strncmp(first,"addr",MAX_FILE_LINE_LENGTH)==0){
		Driver::NRF24L01p::Address addr;
		memset(addr.value,0,sizeof(addr.value));
		for(uint32_t t = 0;second[t]!='\0';addr.value[t] = second[t], t++);
		Tasks::NRF24L01p.SetRxAddress(addr);
	}else if(strncmp(first,"channel",MAX_FILE_LINE_LENGTH)==0
			||strncmp(first,"ch",MAX_FILE_LINE_LENGTH)==0){
		uint32_t ch = atoi(second);
		Tasks::NRF24L01p.SetChannel(ch);
	}
}

void Control::ConvertToLowerCase(char* dt){
	while(*(dt)!='\0'){
		*(dt) = tolower(*dt);
		dt++;
	}
}

bool Control::Sending() const {
	switch(m_state){
		case csSEND_FILE_TRANSFER_REQ:
		case csWAIT_FILE_TRANSFER_REQ:
		case csWAIT_FILE_ACCEPT:
		case csSEND_FILE_PART:
		case csWAIT_SEND_FILE_PART:
			return true;
		default:
			return false;
	}
}

void Control::OnReceiveRadioPacket(const void* data, uint32_t size){
	switch(m_state){
		case csWAIT_FILE_REQ_RX:
		{
			auto pkt = (const FileDescStruct*)data;
			if(size<sizeof(FileDescStruct) || pkt->pktType != ptSEND_FILE){
				return;
			}
			m_hasFileRxRequest = true;
			m_fileSize = size;
			strncpy(m_fileToWrite,pkt->filename,sizeof(m_fileToWrite));
			break;
		}
		case csRECEIVE_FILE:
		case csWAIT_SEND_FILE_ACCEPT:
		{
			if(size<1){
				return;
			}
			uint8_t type = *((const uint8_t*)data);
			switch(type){
				case ptFILE_DATA:
				{
					auto pkt = (const FileDataStruct*)data;
					const uint32_t PACKET_OVERHEAD = (sizeof(pkt) - sizeof(pkt->data));
					if(size<=PACKET_OVERHEAD){
						return;
					}
					auto dtWriteSize = size - PACKET_OVERHEAD;
					auto newpos = m_fil.Seek(pkt->ptr);
					if(newpos!=pkt->ptr){
						return;
					}

					auto written = m_fil.Write(pkt->data, dtWriteSize);
					if(written==dtWriteSize){
						m_writeSuccess = true;
					}
					break;
				}
				case ptCANCEL:
				{
					m_fileCancelRequest = true;
					break;
				}
				case ptDONE:
				{
					m_fileCompleteRequest = true;
					break;
				}
				case ptSEND_FILE:
				{
					m_fileCancelRequest = true;
					break;
				}
			}
			break;
		}
		case csWAIT_FILE_ACCEPT:
		{
			if(size<1){
				return;
			}
			uint8_t type = *((const uint8_t*)data);
			if(type==ptACCEPT_FILE){
				m_fileAcceptRequest = true;
			}
			break;
		}
	}
}

void Control::Setup(){
	Tasks::Switches.SetCallback(Driver::Switches::POWER, [this](bool state){
		if(state){
			return;
		}
		auto time = Tasks::Switches.TimeSincePressed(Driver::Switches::POWER);
		if(time<SEND_REQUEST_TIME){
			if(Sending()){
				m_fileCancelRequest = true;
			}else{
				m_sendRequest = true;
			}

		}else if(time>POWER_OFF_TIME){
			m_poffRequest = true;
			m_fileCancelRequest = true;
		}
	});
	Tasks::Switches.Reset(Driver::Switches::POWER);
}

void Control::Loop(){

	/**
	 * Send file normal flow:
	 * csSTART_RADIO_RX -> csWAIT_FILE_REQ_RX -> csSEND_FILE_TRANSFER_REQ -> csWAIT_FILE_TRANSFER_REQ -> csWAIT_FILE_ACCEPT -> csSEND_FILE_PART -> csWAIT_SEND_FILE_PART -> csSEND_FILE_PART (...)
	 * Once there is no more data to send:
	 * csSEND_FILE_PART -> csSTART_RADIO_RX
	 *
	 *
	 * Receive file normal flow:
	 * csSTART_RADIO_RX -> csWAIT_FILE_REQ_RX -> csSEND_FILE_ACCEPT -> csWAIT_SEND_FILE_ACCEPT -> csRECEIVE_FILE -> csRECEIVE_FILE (...)
	 * Once system receives a DONE or CANCEL mesage, or there is a timeout:
	 * csRECEIVE_FILE -> csSTART_RADIO_RX
	 *
	 * If there is no file to receive, nor a file to transmit, this is the state flow:
	 * csSTART_RADIO_RX -> csWAIT_FILE_REQ_RX -> csSLEEP -> csWAIT_TO_SLEEP
	 *
	 * If there is a event while sleeping:
	 * csWAIT_TO_SLEEP -> csPARSE_CONFIG -> csSTART_RADIO_RX
	 *
	 * The normal startup sequence is:
	 * csPOWER_ON -> csWAIT_PERIPHERALS -> csPARSE_CONFIG -> csSTART_RADIO_RX
	 *
	 */

	switch(m_state){
		case csPOWER_ON:
		{
			//Signal startup.
			Tasks::Battery.RequestVoltage(nullptr);
			Tasks::Leds.Blink(Driver::Leds::RED, POWER_ON_BLINKS,DEFAULT_BLINK_ON_TIME,DEFAULT_BLINK_OFF_TIME);
			Tasks::Leds.Blink(Driver::Leds::YELLOW, POWER_ON_BLINKS,DEFAULT_BLINK_ON_TIME,DEFAULT_BLINK_OFF_TIME);
			Tasks::Leds.Blink(Driver::Leds::GREEN, POWER_ON_BLINKS,DEFAULT_BLINK_ON_TIME,DEFAULT_BLINK_OFF_TIME);
			m_state = csWAIT_PERIPHERALS;
			break;
		}
		case csWAIT_PERIPHERALS:
		{
			//Wait peripherals to be ready.
			if(Tasks::NRF24L01p.Error()){
				//There is a error at NRF24L01p. Sinalize with RED led.
				if(!Tasks::Leds.IsBlinking(Driver::Leds::RED)){
					Tasks::Leds.Blink(Driver::Leds::RED, NRF_ERROR_BLINKS, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 2000);
				}
			}else if(Tasks::NRF24L01p.Idle() && !Tasks::Battery.IsBusy() && !Tasks::Leds.IsBlinking(Driver::Leds::RED)){
				//Peripherals are working properly, go to parse.
				m_state = csPARSE_CONFIG;
				m_sendRequest = false;
			}
			break;
		}
		case csPARSE_CONFIG:
		{
			//Request battery voltage.
			Tasks::Battery.RequestVoltage(nullptr);

			//Powerup SD Card.
			auto sdInst = App::SDCard::GetInstance();
			auto ok = sdInst->Enable(true);

			if(!ok){
				//SD Card failure. Signal with RED led.
				Tasks::Leds.Blink(Driver::Leds::RED,SDCARD_MISSING_BLINKS, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME);
				m_state = csSLEEP;
				break;
			}

			//SD Card OK. Parse config file if it exists.
			DoParseConfig();

			//Blink led to show activity.
			if(Tasks::Battery.IsLow()){
				Tasks::Leds.Blink(Driver::Leds::YELLOW,1, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME);
			}else{
				Tasks::Leds.Blink(Driver::Leds::GREEN,1, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME);
			}

			//Go receive data.
			m_state = csSTART_RADIO_RX;
			break;
		}
		case csSLEEP:
		{
			//Setup sleep time.
			m_sleepTim.Set(0);
			m_sleepTim.SetAlarm(SLEEP_TIME);
			auto sdInst = App::SDCard::GetInstance();
			sdInst->Enable(false);
			m_state = csWAIT_TO_SLEEP;
			break;
		}
		case csWAIT_TO_SLEEP:
		{
			auto remaining = m_sleepTim.GetAlarm() - m_sleepTim.Get();
			if(m_sleepTim.Expired()){
				m_state = csPARSE_CONFIG;
				break;
			}

			if(m_poffRequest){
				m_state = csPOWER_OFF;
				break;
			}

			bool ledBlinking = Tasks::Leds.IsBlinking(Driver::Leds::RED) || Tasks::Leds.IsBlinking(Driver::Leds::YELLOW) || Tasks::Leds.IsBlinking(Driver::Leds::GREEN);
			bool spiBusy = Tasks::SpiManager.Busy();
			bool nrfBusy = !Tasks::NRF24L01p.Idle();
			bool batBusy = Tasks::Battery.IsBusy();
			bool swBusy = Tasks::Switches.Get(Driver::Switches::POWER);

			//Only sleep with everyone idle.
			if(!ledBlinking && !spiBusy && !nrfBusy && !batBusy && !swBusy){
				Tasks::SpiManager.Shutdown();
				Tasks::Switches.Reset(Driver::Switches::POWER);
				Driver::Power::GetInstance()->LowPowerSleep(remaining);
				//LowPowerSleep will return only after a interrupt or when the specified time passes.
				m_state = csPARSE_CONFIG;
				break;
			}

			break;
		}
		case csSTART_RADIO_RX:
		{
			//Only take action when NRF24L01p is ready.
			if(!Tasks::NRF24L01p.Idle()){
				break;
			}
			//Enable RX.
			Tasks::NRF24L01p.EnableRx([this](const void* data, uint32_t size){
				OnReceiveRadioPacket(data,size);
			});
			//Schedule maximum time to keep hearing.
			m_sleepTim.Set(0);
			m_sleepTim.SetAlarm(RADIO_RX_TIME);
			m_state = csWAIT_FILE_REQ_RX;
			break;
		}
		case csWAIT_FILE_REQ_RX:
		{
			if(m_poffRequest){
				m_state = csPOWER_OFF;
				break;
			}

			if(m_hasFileRxRequest){
				//Received file request. Try to open new file at SDCard.
				m_hasFileRxRequest = false;
				bool ok = m_fil.Open(m_fileToWrite, true, false, false);
				if(ok){
					m_fil.Truncate(0);
				}

				if(!ok){
					//Can't open SD Card file. Signal failure.
					Tasks::Leds.Blink(Driver::Leds::RED, CANT_OPEN_FILE_BLINKS, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 0);
				}else{
					//SD Card file open. Send file accept.
					Tasks::NRF24L01p.EnableRx(nullptr);
					m_state = csSEND_FILE_ACCEPT;
				}
				break;
			}

			if(m_sendRequest){
				//Received file send request. Disable RX.
				Tasks::NRF24L01p.EnableRx(nullptr);
				m_state = csSEND_FILE_TRANSFER_REQ;
				m_fileCancelRequest = false;
				m_sendRequest = false;
				m_fileRetryTim.Set(0);
				m_fileRetryTim.SetAlarm(FILE_TIMEOUT);
				break;
			}

			if(m_sleepTim.Expired()){
				//Nothing happened. Go sleep.
				Tasks::NRF24L01p.EnableRx(nullptr);
				m_state = csSLEEP;
			}

			break;
		}
		case csSEND_FILE_ACCEPT: //RX
		{
			if(Tasks::NRF24L01p.Idle()){
				//Send file accept message when NRF becomes IDLE.
				m_fileCompleteRequest = false;
				m_fileCancelRequest = false;
				m_writeSuccess = true;

				static constexpr uint8_t CONF_DATA = ptACCEPT_FILE;
				Tasks::NRF24L01p.SendData(&CONF_DATA, 1, nullptr);
				m_state = csWAIT_SEND_FILE_ACCEPT;
			}
			break;
		}
		case csWAIT_SEND_FILE_ACCEPT: //RX
		{
			if(Tasks::NRF24L01p.Idle()){
				//Enable NRF to receive remaining file data.
				Tasks::NRF24L01p.EnableRx([this](const void* data, uint32_t size){
					OnReceiveRadioPacket(data,size);
				});
				m_state = csRECEIVE_FILE;
				m_fileTransferTim.Set(0);
				m_fileTransferTim.SetAlarm(FILE_TIMEOUT);
			}
			break;
		}
		case csRECEIVE_FILE: //RX
		{
			if(m_writeSuccess){
				//A write success happened. Signal it with the led.
				m_writeSuccess = false;
				m_fileTransferTim.Set(0);
				if(!Tasks::Leds.IsBlinking(Driver::Leds::GREEN)){
					Tasks::Leds.Blink(Driver::Leds::GREEN, 1, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 250);
				}
			}
			if(m_fileCompleteRequest){
				//File completed by the sender. Close it and signal with the led.
				m_fileCompleteRequest = false;
				m_fil.Close();
				Tasks::Leds.Blink(Driver::Leds::GREEN, 1, 1000, 0, 0);
				Tasks::NRF24L01p.EnableRx(nullptr);
				m_state = csSTART_RADIO_RX;
				break;
			}
			if(m_fileCancelRequest || m_fileTransferTim.Expired()){
				//File cancelled by the sender or there was a timeout. Close file and signal with the led.
				m_fileCancelRequest = false;
				m_fil.Close();
				Tasks::Leds.Blink(Driver::Leds::YELLOW, 1, 1000, 0, 0);
				Tasks::NRF24L01p.EnableRx(nullptr);
				m_state = csSTART_RADIO_RX;
				break;
			}
			break;
		}
		case csSEND_FILE_TRANSFER_REQ: //TX
		{
			//Send a request to send a file.
			if(m_fileRetryTim.Expired()){
				//Has been trying for too long. Give up.
				Tasks::Leds.Blink(Driver::Leds::RED, NO_FILE_RECEPTOR_BLINKS, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 250);
				m_state = csSTART_RADIO_RX;
				break;
			}
			if(!Tasks::NRF24L01p.Idle()){
				break;
			}

			//Open file and fetch it's stats.
			bool ok = m_fil.Open(m_fileToSend, false, true, true);
			if(!ok){
				//Failure: can't open file to write.
				Tasks::Leds.Blink(Driver::Leds::RED, CANT_OPEN_FILE_BLINKS, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 250);
				m_state = csSTART_RADIO_RX;
				break;
			}

			//Build request and send.
			FileDescStruct desc;
			desc.pktType = ptSEND_FILE;
			desc.size = m_fil.GetSize();
			strncpy(desc.filename, m_fileToSend,sizeof(desc.filename));

			Tasks::NRF24L01p.SendData(&desc, sizeof(desc), nullptr);
			Tasks::Leds.Blink(Driver::Leds::GREEN, 1, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 0);

			m_state = csWAIT_FILE_TRANSFER_REQ;

			break;
		}
		case csWAIT_FILE_TRANSFER_REQ: //TX
		{
			if(!Tasks::NRF24L01p.Idle()){
				break;
			}

			//Wait for accept.
			Tasks::NRF24L01p.EnableRx([this](const void* data, uint32_t size){
				OnReceiveRadioPacket(data,size);
			});

			m_state = csWAIT_FILE_ACCEPT;
			m_fileTransferTim.Set(0);
			m_fileTransferTim.SetAlarm(FILE_ACCEPT_TIMEOUT);

			break;
		}
		case csWAIT_FILE_ACCEPT: //TX
		{
			if(m_fileAcceptRequest){
				//File accepted. Start transfer.
				Tasks::NRF24L01p.EnableRx(nullptr);
				m_sendFailureRequest = false;
				m_state = csSEND_FILE_PART;
				m_fileTransferTim.Set(0);
				m_fileTransferTim.SetAlarm(FILE_TIMEOUT);
				Tasks::Leds.Blink(Driver::Leds::GREEN, 1, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 0);
				break;
			}
			if(m_fileTransferTim.Expired()){
				//File not accepted. Try again.
				Tasks::Leds.Blink(Driver::Leds::YELLOW, 1, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 0);
				Tasks::NRF24L01p.EnableRx(nullptr);
				m_state = csSEND_FILE_TRANSFER_REQ;
				break;
			}
			break;
		}
		case csSEND_FILE_PART: //TX
		{
			if(Tasks::NRF24L01p.Idle()){
				if(m_fileCancelRequest || m_fileTransferTim.Expired()){
					//Received a cancel request or timed out. Signal led and send cancel packet.
					m_fileCancelRequest = false;
					uint8_t msg = ptCANCEL;
					Tasks::NRF24L01p.SendData(&msg, sizeof(msg),nullptr);
					Tasks::Leds.Blink(Driver::Leds::YELLOW, 1, 1000, DEFAULT_BLINK_OFF_TIME, 0);
					m_fil.Close();
					m_state = csSTART_RADIO_RX;
					break;
				}

				if(!m_sendFailureRequest){
					m_fileTransferTim.Set(0);
					//Last packed was sent successfully. Fetch next data.
					uint32_t filePos = m_fil.GetPosition();
					uint32_t fileEnd = m_fil.GetSize();
					if(filePos>=fileEnd){
						//Data sent.
						uint8_t msg = ptDONE;
						Tasks::NRF24L01p.SendData(&msg, sizeof(msg),nullptr);
						Tasks::Leds.Blink(Driver::Leds::GREEN, 1, 1000, DEFAULT_BLINK_OFF_TIME, 0);
						m_fil.Close();
						m_state = csSTART_RADIO_RX;
						break;
					}

					uint32_t remaining = std::min<uint32_t>(fileEnd - filePos,sizeof(m_pktSend.data));

					m_pktSend.pktType = ptFILE_DATA;
					m_pktSend.ptr = filePos;
					auto read = m_fil.Read(m_pktSend.data,remaining);
					if(read!=remaining){
						//Failure to read file.
						uint8_t msg = ptCANCEL;
						Tasks::NRF24L01p.SendData(&msg, sizeof(msg),nullptr);
						Tasks::Leds.Blink(Driver::Leds::RED, 1, 1000, DEFAULT_BLINK_OFF_TIME, 0);
						m_fil.Close();
						m_state = csSTART_RADIO_RX;
						break;
					}
					const uint32_t PACKET_OVERHEAD = (sizeof(m_pktSend) - sizeof(m_pktSend.data));
					m_pktSendSize =  PACKET_OVERHEAD + remaining;
				}

				//Send data.
				Tasks::NRF24L01p.SendData(&m_pktSend, m_pktSendSize, [this](bool success){
					m_sendFailureRequest = !success;
				});

				m_sendFailureRequest = true;
				m_state = csWAIT_SEND_FILE_PART;
				break;
			}
			break;
		}
		case csWAIT_SEND_FILE_PART: //TX
		{
			if(Tasks::NRF24L01p.Idle()){
				//Send next part.
				m_state = csSEND_FILE_PART;
				if(m_sendFailureRequest){
					if(!Tasks::Leds.IsBlinking(Driver::Leds::YELLOW)){
						Tasks::Leds.Blink(Driver::Leds::YELLOW, 1, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 200);
					}
				}else{
					if(!Tasks::Leds.IsBlinking(Driver::Leds::GREEN)){
						Tasks::Leds.Blink(Driver::Leds::YELLOW, 1, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 200);
					}
				}
			}
			break;
		}
		case csPOWER_OFF:
		{
			//Disable SD Card and signal power off.
			auto sdInst = App::SDCard::GetInstance();
			sdInst->Enable(false);
			Tasks::Leds.Blink(Driver::Leds::RED, POWER_OFF_BLINKS, DEFAULT_BLINK_ON_TIME, DEFAULT_BLINK_OFF_TIME, 100);
			m_state = csWAIT_POWER_OFF;
			break;
		}
		case csWAIT_POWER_OFF:
		{
			//Wait until signalling is done.
			if(!Tasks::Leds.IsBlinking(Driver::Leds::RED)){
				Driver::Power::GetInstance()->PowerOff();
			}
			break;
		}
	}
}
