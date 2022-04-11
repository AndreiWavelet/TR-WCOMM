#include <Drivers/SpiManager.h>

#include <main.h>
#include <spi.h>

#define HSPI (&hspi1)

using namespace Driver;

GPIO_TypeDef* m_spiCsPorts[(uint32_t)SpiManager::Device::NUM_DEVICES] = {SD_nCS_GPIO_Port, RADIO_nCS_GPIO_Port};
uint16_t m_spiCsPins[(uint32_t)SpiManager::Device::NUM_DEVICES] = {SD_nCS_Pin, RADIO_nCS_Pin};

volatile static SpiManager* m_spiMgrCbInstance;

namespace Driver {
	void OnSpiCallback(){
		m_spiMgrCbInstance->m_opDone = true;
	}
}

extern "C" {
	void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
		OnSpiCallback();
	}
	void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){
		OnSpiCallback();
	}
	void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi){
		OnSpiCallback();
	}
}

SpiManager::SpiManager(){
	SetupDevice(Device::SD,2000000);
	SetupDevice(Device::NRF, 2000000);
	m_devReconfReq = true;
	m_deviceLocked = false;
}

SpiManager::~SpiManager(){
	return;
}

bool SpiManager::Shutdown(){
	if(Busy() || m_deviceLocked){
		return false;
	}
	m_devReconfReq = true;
	HAL_SPI_DeInit(HSPI);
	return true;
}

void SpiManager::SetupDevice(Device dev, uint32_t baud){
	if((uint32_t)dev >= (uint32_t)Device::NUM_DEVICES || m_deviceLocked){
		return;
	}

	//Determine best clock division to match baud rate.
	uint32_t baseBusFreq = HAL_RCC_GetPCLK2Freq();
	uint32_t divIdx = 0;
	while(baseBusFreq/(1<<(divIdx+1)) > baud && divIdx < 7){
		divIdx++;
	}

	const uint32_t baudCfgs[] = {SPI_BAUDRATEPRESCALER_2,SPI_BAUDRATEPRESCALER_4,SPI_BAUDRATEPRESCALER_8,SPI_BAUDRATEPRESCALER_16,
			SPI_BAUDRATEPRESCALER_32,SPI_BAUDRATEPRESCALER_64,SPI_BAUDRATEPRESCALER_128,SPI_BAUDRATEPRESCALER_256};

	bool mayReconf = m_deviceFreqDiv[(uint32_t)dev]!=baudCfgs[divIdx];

	m_deviceFreqDiv[(uint32_t)dev] = baudCfgs[divIdx];

	m_devReconfReq = m_devReconfReq || (mayReconf && m_deviceFreqDiv[(uint32_t)dev] != m_deviceFreqDiv[(uint32_t)m_opDevice]);
}

bool SpiManager::SendData(Device dev, const void* data, uint32_t dataSize, std::function<void(void)> callback){
	return SendReceiveData(dev,data,nullptr,dataSize,callback);
}

bool SpiManager::ReceiveData(Device dev, void* data, uint32_t dataSize, std::function<void(void)> callback){
	return SendReceiveData(dev,nullptr,data,dataSize,callback);
}

bool SpiManager::SendReceiveData(Device dev, const void* dataSend, void* dataReceive, uint32_t dataSize, std::function<void(void)> callback){
	if(Busy() || (m_deviceLocked && dev!=m_deviceLock) || ((uint32_t)dev>=(uint32_t)Device::NUM_DEVICES && dev!=Device::NONE)){
		return false;
	}
	//Request reconfiguration if there will be a frequency change.
	m_devReconfReq = m_devReconfReq || (m_opDevice!=dev && m_deviceFreqDiv[(uint32_t)dev]!=m_deviceFreqDiv[(uint32_t)m_opDevice]);

	m_opRequest = true;
	m_opDevice = dev;
	m_opCallback = callback;
	m_dataToSend = const_cast<uint8_t*>((const uint8_t*)dataSend);
	m_dataToReceive = (uint8_t*)dataReceive;
	return true;
}

bool SpiManager::Busy() const{
	return m_opRequest || m_opRunning;
}

bool SpiManager::PoolUntilDone(){
	while(Busy()){
		Loop();
	}
	return true;
}

bool SpiManager::LockDevice(Device dev, bool lock){
	if(Busy() || (m_deviceLocked && m_deviceLock!=dev) || (uint32_t)dev >= (uint32_t)Device::NUM_DEVICES){
		return false;
	}
	m_deviceLock = dev;
	m_deviceLocked = lock;
	if(lock){
		HAL_GPIO_WritePin(m_spiCsPorts[(uint32_t)m_opDevice], m_spiCsPins[(uint32_t)m_opDevice], GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(m_spiCsPorts[(uint32_t)m_opDevice], m_spiCsPins[(uint32_t)m_opDevice], GPIO_PIN_SET);
	}
	return true;
}

void SpiManager::Loop(){
	if(m_opRunning){

		//m_opDone will change on interrupt.
		if(m_opDone){
			m_opDone = false;
			m_opRunning = false;

			if(!m_deviceLocked && m_opDevice!=Device::NONE){
				HAL_GPIO_WritePin(m_spiCsPorts[(uint32_t)m_opDevice], m_spiCsPins[(uint32_t)m_opDevice], GPIO_PIN_SET);
			}

			if(m_opCallback){
				auto cb = m_opCallback;
				cb();
			}
		}

	}else{
		if(m_opRequest){
			m_opRequest = false;
			m_opDone = false;
			m_opRunning = true;
			m_spiMgrCbInstance = this;

			//Reconfigure device if needed...
			if(m_devReconfReq){
				m_devReconfReq = false;
				HSPI->Instance = SPI1;
				HSPI->Init.Mode = SPI_MODE_MASTER;
				HSPI->Init.Direction = SPI_DIRECTION_2LINES;
				HSPI->Init.DataSize = SPI_DATASIZE_8BIT;
				HSPI->Init.CLKPolarity = SPI_POLARITY_LOW;
				HSPI->Init.CLKPhase = SPI_PHASE_1EDGE;
				HSPI->Init.NSS = SPI_NSS_SOFT;
				HSPI->Init.BaudRatePrescaler = m_deviceFreqDiv[(uint32_t)m_opDevice];
				HSPI->Init.FirstBit = SPI_FIRSTBIT_MSB;
				HSPI->Init.TIMode = SPI_TIMODE_DISABLE;
				HSPI->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
				HSPI->Init.CRCPolynomial = 7;
				HAL_SPI_Init(HSPI);
			}

			if(!m_deviceLocked && m_opDevice!=Device::NONE){
				HAL_GPIO_WritePin(m_spiCsPorts[(uint32_t)m_opDevice], m_spiCsPins[(uint32_t)m_opDevice], GPIO_PIN_RESET);
			}

			bool ok = false;
			if(m_dataToReceive && m_dataToSend){
				ok = HAL_SPI_TransmitReceive_DMA(HSPI, m_dataToSend, m_dataToReceive, m_dataSize) == HAL_OK;
			}else if(m_dataToReceive){
				ok = HAL_SPI_Receive_DMA(HSPI, m_dataToReceive, m_dataSize) == HAL_OK;
			}else if(m_dataToSend){
				ok = HAL_SPI_Transmit_DMA(HSPI, m_dataToReceive, m_dataSize) == HAL_OK;
			}

			if(!ok){
				//TODO: add error parameter if it's found to be useful.
				m_opDone = true;
			}
		}
	}
}



