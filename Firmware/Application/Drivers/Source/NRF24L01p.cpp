#include <Drivers/NRF24L01p.h>
#include <Drivers/NRF24L01p_Defs.h>

#include <Drivers/SpiManager.h>
#include <App/Tasks.h>

#include <cstring>
#include <main.h>
#include <gpio.h>

using namespace Driver;

#define RADIO_EN(en) (HAL_GPIO_WritePin(RADIO_EN_GPIO_Port, RADIO_EN_Pin, (en)?(GPIO_PIN_SET):(GPIO_PIN_RESET)))

static constexpr uint32_t STARTUP_TIME = 10; //[ms]
static constexpr uint32_t ERROR_RESTART_TIME = 2000; //[ms]

typedef enum {
	nsSTARTUP, nsWAIT_STARTUP,
	nsTEST_REG, nsWAIT_TEST_REG, nsWAIT_TEST_CLEANUP,
	nsENTER_ERROR, nsERROR,
	nsDEVICE_SETUP, nsWAIT_DEVICE_SETUP,
	nsIDLE,
	nsSETUP_TX, nsWAIT_SETUP_TX, nsWAIT_TX, nsFETCH_TX_INFO, nsWAIT_TX_INFO,
	nsSETUP_RX, nsWAIT_SETUP_RX, nsWAIT_RX, nsFETCH_RX_INFO, nsWAIT_RX_INFO, nsDISABLE_RX, nsWAIT_DISABLE_RX,
} State;

typedef enum {
	mosIDLE, mosSETUP, mosWAIT, mosWAIT_POST_DELAY,
} MultiOpState;

static NRF24L01p* m_nrfExtiInstance = nullptr;

namespace Driver {
	void OnExtiCallback(){
		if(m_nrfExtiInstance){
			m_nrfExtiInstance->m_exti = true;
		}
	}
}

extern "C" {
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
		if(GPIO_Pin==RADIO_IRQ_Pin){
			OnExtiCallback();
		}
	}
}

NRF24L01p::NRF24L01p() : m_channel(0),m_power(Power::P0),m_speed(Speed::S250K), m_maxRetx(15),
		m_error(false), m_configureRequest(false), m_txRequest(false), m_rxRequest(false), m_rxStopRequest(false),
		m_state(nsSTARTUP),
		m_numMultiOp(0), m_currentMultiOp(0),m_multiOpState(mosIDLE)
{
	m_nrfExtiInstance = this;
}

NRF24L01p::~NRF24L01p(){
	RADIO_EN(false);
}

bool NRF24L01p::Error() const{
	return m_error;
}

bool NRF24L01p::Idle() const{
	return m_state == nsIDLE;
}

void NRF24L01p::SetTxAddress(const Address& addr){
	memcpy(m_txAddress,addr.value,sizeof(m_txAddress));
	m_configureRequest = true;
}

void NRF24L01p::SetRxAddress(const Address& addr){
	memcpy(m_rxAddress,addr.value,sizeof(m_rxAddress));
	m_configureRequest = true;
}

void NRF24L01p::SetChannel(uint8_t chn){
	m_channel = chn;
	m_configureRequest = true;
}

bool NRF24L01p::SendData(const void* data, uint32_t size, std::function<void(bool success)> callback){
	if(!Idle() || m_txRequest || size>32){
		return false;
	}
	m_txRequest = true;
	m_txSize = size;
	memcpy(m_txBuffer,data,size);
	m_txCallback = callback;
	return true;
}

bool NRF24L01p::EnableRx(std::function<void(const void* data, uint32_t size)> callback){
	if(!callback){
		m_rxStopRequest = true;
	}else{
		if(!Idle()){
			return false;
		}
		m_rxRequest = true;
		m_rxCallback = callback;
	}
	return true;
}

void NRF24L01p::Loop(){
	switch(m_state){
		case nsSTARTUP:
		{
			//Setup startup time.
			m_tim.Set(0);
			m_tim.SetAlarm(STARTUP_TIME);
			Tasks::SpiManager.SetupDevice(Driver::SpiManager::Device::NRF, 4000000);
			RADIO_EN(false);
			m_state = nsWAIT_STARTUP;
			break;
		}
		case nsWAIT_STARTUP:
		{
			//Wait startup.
			if(m_tim.Expired()){
				m_state = nsTEST_REG;
				break;
			}
			break;
		}
		case nsTEST_REG:
		{
			//Write random value to TX_ADDR register and then read it back to check if NRF is healthy.

			NRF24L01_Reg::Setup_Aw_Data aw;
			aw.value = 0;
			aw.AW = NRF24L01_Reg::AW_5BYTES;

			for(uint32_t i=0;i<sizeof(m_testAddress);i++){
				m_testAddress[i] = (uint8_t)rand();
			}

			MultiRegOpStrt reg[3];
			InitMultiOpStr(&reg[0],false,NRF24L01_Reg::SETUP_AW, false, &aw, sizeof(aw),0);
			InitMultiOpStr(&reg[1],false,NRF24L01_Reg::TX_ADDR, false, m_testAddress, sizeof(m_testAddress),2);
			InitMultiOpStr(&reg[2],true,NRF24L01_Reg::TX_ADDR, true, m_readTestAddress, sizeof(m_readTestAddress),0);

			bool ok = SetupMultiOp(reg,sizeof(reg)/sizeof(*reg));
			if(!ok){
				break;
			}

			m_state = nsWAIT_TEST_REG;

			break;
		}
		case nsWAIT_TEST_REG:
		{
			//Wait for test operations to finish.
			if(MultiOpBusy()){
				break;
			}

			//Compare register.
			if(memcmp(m_testAddress,m_readTestAddress,sizeof(m_testAddress))!=0){
				m_state = nsENTER_ERROR;
				m_error = true;
				break;
			}else{
				m_error = false;
			}

			//Device test passed. Cleanup.
			MultiRegOpStrt reg[1];
			InitMultiOpStr(&reg[0],false,NRF24L01_Reg::TX_ADDR, false, m_txAddress, sizeof(m_txAddress),2);

			bool ok = SetupMultiOp(reg,sizeof(reg)/sizeof(*reg));
			if(!ok){
				break;
			}

			m_state = nsWAIT_TEST_CLEANUP;

			break;
		}
		case nsWAIT_TEST_CLEANUP:
		{
			if(MultiOpBusy()){
				break;
			}

			m_state = nsDEVICE_SETUP;
			break;
		}
		case nsDEVICE_SETUP:
		{
			//Setup all registers.
			if(MultiOpBusy()){
				break;
			}

			MultiRegOpStrt reg[15];

			//Enable CRC but not power up.
			NRF24L01_Reg::Config_Data cfg;
			cfg.value = 0;
			cfg.EN_CRC = 1;
			InitMultiOpStr(&reg[0], false, NRF24L01_Reg::CONFIG, false, &cfg, sizeof(cfg), 0);

			//Enable auto acknowledge at pipes 0 and 1.
			NRF24L01_Reg::En_Aa_Data enaa;
			enaa.value = 0;
			enaa.ENAA_P0 = 1;
			enaa.ENAA_P1 = 1;
			InitMultiOpStr(&reg[1], false, NRF24L01_Reg::EN_AA, false, &enaa, sizeof(enaa), 0);

			//Enable RX at pipes 0 and 1.
			NRF24L01_Reg::En_RxAddr_Data erxaddr;
			erxaddr.value = 0;
			erxaddr.ERX_P0 = 1;
			erxaddr.ERX_P1 = 1;
			InitMultiOpStr(&reg[2], false, NRF24L01_Reg::EN_RXADDR, false, &erxaddr, sizeof(erxaddr), 0);

			//Set address size to 5 bytes.
			NRF24L01_Reg::Setup_Aw_Data saw;
			saw.value = 0;
			saw.AW = NRF24L01_Reg::AW_5BYTES;
			InitMultiOpStr(&reg[3], false, NRF24L01_Reg::SETUP_AW, false, &saw, sizeof(saw), 0);

			//Setup acknowledge time and maximum retries.
			NRF24L01_Reg::Setup_Retr_Data sret;
			sret.value = 0;
			sret.ARD = NRF24L01_Reg::Ard_Values::ARD_500us;
			sret.ARC = m_maxRetx;
			InitMultiOpStr(&reg[4], false, NRF24L01_Reg::SETUP_RETR, false, &sret, sizeof(sret), 0);

			//Setup channel index.
			NRF24L01_Reg::Rf_Ch_Data rfch;
			rfch.value = 0;
			rfch.value = m_channel;
			InitMultiOpStr(&reg[5], false, NRF24L01_Reg::RF_CH, false, &rfch, sizeof(rfch), 0);

			//Setup network speed and power.
			NRF24L01_Reg::Rf_Setup_Data rfstp;
			rfstp.value = 0;
			switch(m_speed){
				case Speed::S250K:
				{
					rfstp.RF_DR_LOW = 1;
					rfstp.RF_DR_HIGH = 0;
					break;
				}
				case Speed::S1M:
				{
					rfstp.RF_DR_LOW = 0;
					rfstp.RF_DR_HIGH = 0;
					break;
				}
				case Speed::S2M:
				{
					rfstp.RF_DR_LOW = 0;
					rfstp.RF_DR_HIGH = 1;
					break;
				}
			}
			rfstp.RF_PWR = (uint8_t)m_power;
			InitMultiOpStr(&reg[6], false, NRF24L01_Reg::RF_SETUP, false, &rfstp, sizeof(rfstp), 0);

			//Assert interruptions.
			NRF24L01_Reg::Status_Data sts;
			sts.value = 0;
			sts.RX_DR = 1;
			sts.TX_DS = 1;
			sts.MAX_RT = 1;
			InitMultiOpStr(&reg[7], false, NRF24L01_Reg::STATUS, false, &sts, sizeof(sts), 0);

			//Setup addresses.
			NRF24L01_Reg::RX_TX_ADDR_P01_Data rxAddr0;
			memcpy(rxAddr0.value,m_txAddress,sizeof(rxAddr0.value));
			InitMultiOpStr(&reg[8], false, NRF24L01_Reg::RX_ADDR_P0, false, &rxAddr0, sizeof(rxAddr0), 0);

			NRF24L01_Reg::RX_TX_ADDR_P01_Data rxAddr1;
			memcpy(rxAddr1.value,m_rxAddress,sizeof(rxAddr1.value));
			InitMultiOpStr(&reg[9], false, NRF24L01_Reg::RX_ADDR_P1, false, &rxAddr1, sizeof(rxAddr1), 0);

			NRF24L01_Reg::RX_TX_ADDR_P01_Data txAddr;
			memcpy(txAddr.value,m_txAddress,sizeof(txAddr.value));
			InitMultiOpStr(&reg[10], false, NRF24L01_Reg::TX_ADDR, false, &txAddr, sizeof(txAddr), 0);

			//Enable dynamic payload and auto acknowledge.
			NRF24L01_Reg::Feature_Data feat;
			feat.value = 0;
			feat.EN_DPL = 1;
			feat.EN_ACK_PAY = 1;
			InitMultiOpStr(&reg[11], false, NRF24L01_Reg::FEATURE, false, &feat, sizeof(feat), 0);

			//Enable dynamic payload at pipes 0 and 1.
			NRF24L01_Reg::Dynpd_Data dpd;
			dpd.value = 0;
			dpd.DPL_P0 = 1;
			dpd.DPL_P1 = 1;
			InitMultiOpStr(&reg[12], false, NRF24L01_Reg::DYN_PD, false, &dpd, sizeof(dpd), 0);

			//Flush RX and TX fifos.
			InitMultiOpStr(&reg[13], false, NRF24L01_Reg::FLUSH, false, nullptr, 0, 0);
			InitMultiOpStr(&reg[13], true, NRF24L01_Reg::FLUSH, false, nullptr, 0, 0);

			bool ok = SetupMultiOp(reg, sizeof(reg)/sizeof(*reg));
			if(!ok){
				break;
			}

			m_state = nsWAIT_DEVICE_SETUP;

			break;
		}
		case nsWAIT_DEVICE_SETUP:
		{
			if(MultiOpBusy()){
				break;
			}
			m_state = nsIDLE;
			break;
		}
		case nsIDLE:
		{
			//Handle requests.
			if(m_configureRequest){
				m_configureRequest = false;
				m_state = nsDEVICE_SETUP;
				break;
			}
			if(m_txRequest){
				m_txRequest = false;
				m_state = nsSETUP_TX;
				break;
			}
			if(m_rxRequest){
				m_rxRequest = false;
				m_state = nsSETUP_RX;
				break;
			}
			if(m_rxStopRequest){
				m_rxStopRequest = false;
			}
			break;
		}
		case nsSETUP_TX:
		{
			if(MultiOpBusy()){
				break;
			}

			MultiRegOpStrt reg[2];

			//Setup TX and power up.
			NRF24L01_Reg::Config_Data cfg;
			cfg.value = 0;
			cfg.PWR_UP = 1;
			cfg.EN_CRC = 1;
			cfg.PRIM_RX = 0;
			InitMultiOpStr(&reg[0], false, NRF24L01_Reg::CONFIG, false, &cfg, sizeof(cfg), 0);

			//Write payload fifo.
			InitMultiOpStr(&reg[1], false, NRF24L01_Reg::PAYLOAD_FIFO, true, m_txReqBuffer, m_txSize, 0);

			bool ok = SetupMultiOp(reg, sizeof(reg)/sizeof(*reg));
			if(!ok){
				break;
			}

			m_state = nsWAIT_SETUP_TX;

			break;
		}
		case nsWAIT_SETUP_TX:
		{
			if(!MultiOpBusy()){
				m_exti = false;
				RADIO_EN(true);
				m_state = nsWAIT_TX;
			}
			break;
		}
		case nsWAIT_TX:
		{
			if(m_exti){
				//Interrupt will change m_exti automatically.
				RADIO_EN(false);
				m_state = nsFETCH_TX_INFO;
			}
			break;
		}
		case nsFETCH_TX_INFO:
		{
			if(MultiOpBusy()){
				break;
			}

			MultiRegOpStrt reg[6];
			//Fetch status.
			InitMultiOpStr(&reg[0], true, NRF24L01_Reg::STATUS, true, &m_txStatus, sizeof(m_txStatus), 0);
			InitMultiOpStr(&reg[1], true, NRF24L01_Reg::OBSERVE_TX, true, &m_txObserver, sizeof(m_txObserver), 0);

			//Assert interrupts.
			NRF24L01_Reg::Status_Data sts;
			sts.value = 0;
			sts.RX_DR = 1;
			sts.TX_DS = 1;
			sts.MAX_RT = 1;
			InitMultiOpStr(&reg[2], false, NRF24L01_Reg::STATUS, false, &sts, sizeof(sts), 0);

			//Power off.
			NRF24L01_Reg::Config_Data cfg;
			cfg.value = 0;
			cfg.EN_CRC = 1;
			InitMultiOpStr(&reg[3], false, NRF24L01_Reg::CONFIG, false, &cfg, sizeof(cfg), 0);

			//Flush FIFO.
			InitMultiOpStr(&reg[4], false, NRF24L01_Reg::FLUSH, false, nullptr, 0, 0);
			InitMultiOpStr(&reg[5], true, NRF24L01_Reg::FLUSH, false, nullptr, 0, 0);

			bool ok = SetupMultiOp(reg, sizeof(reg)/sizeof(*reg));
			if(!ok){
				break;
			}

			m_state = nsWAIT_TX_INFO;

			break;
		}
		case nsWAIT_TX_INFO:
		{
			if(MultiOpBusy()){
				break;
			}

			NRF24L01_Reg::Status_Data sts;
			sts.value = m_txStatus;

			m_state = nsIDLE;

			//Callback with result.
			if(m_txCallback){
				auto cb = m_txCallback;
				cb(sts.TX_DS && !sts.MAX_RT);
			}

			break;
		}
		case nsSETUP_RX:
		{
			if(MultiOpBusy()){
				break;
			}

			MultiRegOpStrt reg[1];

			//Setup RX and power up.
			NRF24L01_Reg::Config_Data cfg;
			cfg.value = 0;
			cfg.PWR_UP = 1;
			cfg.EN_CRC = 1;
			cfg.PRIM_RX = 1;
			InitMultiOpStr(&reg[0], false, NRF24L01_Reg::CONFIG, false, &cfg, sizeof(cfg), 0);

			bool ok = SetupMultiOp(reg, sizeof(reg)/sizeof(*reg));
			if(!ok){
				break;
			}

			m_state = nsWAIT_SETUP_RX;
			break;
		}
		case nsWAIT_SETUP_RX:
		{
			if(!MultiOpBusy()){
				m_exti = false;
				RADIO_EN(true);
				m_state = nsWAIT_RX;
			}
			break;
		}
		case nsWAIT_RX:
		{
			if(m_exti){
				m_exti = false;
				m_state = nsFETCH_RX_INFO;
				break;
			}
			if(m_rxStopRequest){
				RADIO_EN(false);
				m_state = nsDISABLE_RX;
			}
			break;
		}
		case nsFETCH_RX_INFO:
		{
			if(MultiOpBusy()){
				break;
			}

			//Request all info about RX.
			//TODO: if needed, read data only about the pipe that actually has the data. This will add some more steps to the state machine, but may benefit with less SPI traffic.
			MultiRegOpStrt reg[9];
			InitMultiOpStr(&reg[0], true, NRF24L01_Reg::STATUS, true, &m_rxStatus, sizeof(m_rxStatus), 0);
			InitMultiOpStr(&reg[1], true, NRF24L01_Reg::RX_PW_P0, true, &m_rxSize[0], sizeof(m_rxSize[0]), 0);
			InitMultiOpStr(&reg[2], true, NRF24L01_Reg::RX_PW_P1, true, &m_rxSize[1], sizeof(m_rxSize[1]), 0);
			InitMultiOpStr(&reg[3], true, NRF24L01_Reg::RX_PW_P2, true, &m_rxSize[2], sizeof(m_rxSize[2]), 0);
			InitMultiOpStr(&reg[4], true, NRF24L01_Reg::RX_PW_P3, true, &m_rxSize[3], sizeof(m_rxSize[3]), 0);
			InitMultiOpStr(&reg[5], true, NRF24L01_Reg::RX_PW_P4, true, &m_rxSize[4], sizeof(m_rxSize[4]), 0);
			InitMultiOpStr(&reg[6], true, NRF24L01_Reg::RX_PW_P5, true, &m_rxSize[5], sizeof(m_rxSize[5]), 0);
			InitMultiOpStr(&reg[7], true, NRF24L01_Reg::PAYLOAD_FIFO, true, m_rxBuffer, sizeof(m_rxBuffer), 0);

			//Assert interrupts.
			NRF24L01_Reg::Status_Data sts;
			sts.value = 0;
			sts.RX_DR = 1;
			sts.TX_DS = 1;
			sts.MAX_RT = 1;
			InitMultiOpStr(&reg[8], false, NRF24L01_Reg::STATUS, false, &sts, sizeof(sts), 0);

			bool ok = SetupMultiOp(reg, sizeof(reg)/sizeof(*reg));
			if(!ok){
				break;
			}

			m_state = nsWAIT_RX_INFO;

			break;
		}
		case nsWAIT_RX_INFO:
		{
			if(MultiOpBusy()){
				break;
			}

			NRF24L01_Reg::Status_Data sts;
			sts.value = m_rxStatus;

			if(sts.RX_P_NO != 0b111){
				//There may be more data on FIFO. Fetch it.
				m_state = nsFETCH_RX_INFO;
			}else{
				//No data on fifo. Wait for more.
				m_state = nsWAIT_RX;
			}

			if(sts.RX_P_NO == 0b001){
				//Data on rx pipe. Forward it to user.
				if(m_rxCallback){
					auto cb = m_rxCallback;
					cb(m_rxBuffer,m_rxSize[1]);
				}
			}

			break;
		}
		case nsDISABLE_RX:
		{
			if(MultiOpBusy()){
				break;
			}

			MultiRegOpStrt reg[5];

			//Assert interrupts.
			NRF24L01_Reg::Status_Data sts;
			sts.value = 0;
			sts.RX_DR = 1;
			sts.TX_DS = 1;
			sts.MAX_RT = 1;
			InitMultiOpStr(&reg[1], false, NRF24L01_Reg::STATUS, false, &sts, sizeof(sts), 0);

			//Power off.
			NRF24L01_Reg::Config_Data cfg;
			cfg.value = 0;
			cfg.EN_CRC = 1;
			InitMultiOpStr(&reg[2], false, NRF24L01_Reg::CONFIG, false, &cfg, sizeof(cfg), 0);

			//Flush FIFO.
			InitMultiOpStr(&reg[3], false, NRF24L01_Reg::FLUSH, false, nullptr, 0, 0);
			InitMultiOpStr(&reg[4], true, NRF24L01_Reg::FLUSH, false, nullptr, 0, 0);

			bool ok = SetupMultiOp(reg, sizeof(reg)/sizeof(*reg));
			if(!ok){
				break;
			}

			m_state = nsWAIT_DISABLE_RX;

			break;
		}
		case nsWAIT_DISABLE_RX:
		{
			if(MultiOpBusy()){
				break;
			}
			m_state = nsIDLE;
			break;
		}
		case nsENTER_ERROR:
		{
			m_tim.Set(0);
			m_tim.SetAlarm(ERROR_RESTART_TIME);
			m_state = nsERROR;
			break;
		}
		case nsERROR:
		{
			if(m_tim.Expired()){
				m_state = nsSTARTUP;
			}
			break;
		}
	}

}

bool NRF24L01p::MultiOpBusy() const{
	return m_multiOpState!=mosIDLE || m_currentMultiOp<m_numMultiOp;
}

bool NRF24L01p::SetupMultiOp(const MultiRegOpStrt* multiOp, uint32_t numOp){
	if(MultiOpBusy() || numOp >= MAX_MULTI_OP){
		return false;
	}

	//Validate parameters.
	for(uint32_t i=0;i<numOp;i++){
		if(multiOp[i].dataSize>MAX_TRANSACTION_SIZE || (!multiOp[i].externalData && multiOp[i].dataSize>sizeof(multiOp[i].intData))){
			return false;
		}
	}

	memcpy(m_multiOp, multiOp,numOp*sizeof(*m_multiOp));
	m_numMultiOp = numOp;
	m_currentMultiOp = 0;
	return true;
}

void NRF24L01p::InitMultiOpStr(MultiRegOpStrt* str, bool read, uint8_t addr, bool extData, const void* data, uint32_t dataSize, uint32_t delayAfterOf){
	str->read = read;
	str->address = addr;
	str->externalData = extData;
	str->delayAfterOp = delayAfterOf;
	str->dataSize = dataSize;
	if(extData){
		str->extDataPtr = const_cast<void*>(data);
	}else if(!read){
		memcpy(str->intData,data,dataSize);
	}
}

void NRF24L01p::LoopMultiOp(){
	switch(m_multiOpState){
		case mosIDLE:
		{
			//Verify if there is a multiple register operation to do.
			if(m_currentMultiOp<m_numMultiOp){
				m_multiOpState = mosSETUP;
			}
			break;
		}
		case mosSETUP:
		{
			//If both indexes are the same, we reached the end. Go back to IDLE.
			if(m_currentMultiOp>=m_numMultiOp){
				m_multiOpState = mosIDLE;
				break;
			}

			//Wait until SPI is available.
			if(Tasks::SpiManager.Busy()){
				break;
			}

			memset(m_txBuffer,0,m_multiOp[m_currentMultiOp].dataSize+1);
			memset(m_rxBuffer,0,m_multiOp[m_currentMultiOp].dataSize+1);

			//Some registers are actually commands. Handle them.
			if(m_multiOp[m_currentMultiOp].address == NRF24L01_Reg::PAYLOAD_FIFO){
				m_txBuffer[0] = (m_multiOp[m_currentMultiOp].read)?(NRF24L01_Command::R_RX_PAYLOAD):(NRF24L01_Command::W_TX_PAYLOAD);
			}else if(m_multiOp[m_currentMultiOp].address == NRF24L01_Reg::FLUSH){
				m_txBuffer[0] = (m_multiOp[m_currentMultiOp].read)?(NRF24L01_Command::FLUSH_RX):(NRF24L01_Command::FLUSH_TX);
			}else if(m_multiOp[m_currentMultiOp].address == NRF24L01_Reg::NOP){
				m_txBuffer[0] = NRF24L01_Command::NOP;
			}else{
				//Normal register.
				m_txBuffer[0] = (m_multiOp[m_currentMultiOp].read)?(NRF24L01_Command::R_REGISTER):(NRF24L01_Command::W_REGISTER);
				m_txBuffer[0] |=  (m_multiOp[m_currentMultiOp].address & 0b11111);
			}

			//Copy data to write.
			if(!m_multiOp[m_currentMultiOp].read){
				if(m_multiOp[m_currentMultiOp].externalData){
					memcpy(m_txBuffer+1,m_multiOp[m_currentMultiOp].extDataPtr,m_multiOp[m_currentMultiOp].dataSize);
				}else{
					memcpy(m_txBuffer+1,m_multiOp[m_currentMultiOp].intData,m_multiOp[m_currentMultiOp].dataSize);
				}
			}

			//Enable SPI.
			Tasks::SpiManager.SendReceiveData(Driver::SpiManager::Device::NRF, m_txBuffer, m_rxBuffer, m_multiOp[m_currentMultiOp].dataSize + 1, nullptr);

			m_multiOpState = mosWAIT;

			break;
		}
		case mosWAIT:
		{
			//Wait until SPI is done. When done, wai specified time after operation.
			if(!Tasks::SpiManager.Busy()){
				m_mopTim.Set(0);
				m_mopTim.SetAlarm(m_multiOp[m_currentMultiOp].delayAfterOp);
				m_multiOpState = mosWAIT_POST_DELAY;
			}
			break;
		}
		case mosWAIT_POST_DELAY:
		{
			if(m_mopTim.Expired()){
				//Time after op has passed.

				//Copy read data.
				if(m_multiOp[m_currentMultiOp].read && m_multiOp[m_currentMultiOp].externalData){
					memcpy(m_multiOp[m_currentMultiOp].extDataPtr,m_rxBuffer+1,m_multiOp[m_currentMultiOp].dataSize);
				}

				//Advance to next operation.
				++m_currentMultiOp;
				m_multiOpState = mosSETUP;
			}
			break;
		}
	}
}

