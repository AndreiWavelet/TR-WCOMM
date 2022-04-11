#include <Drivers/Battery.h>

#include <adc.h>
#include <algorithm>
#include <main.h>

#define HADC (&hadc)

using namespace Driver;

static constexpr uint32_t VREFINT = 1224; //[mv]
static constexpr uint32_t START_VOLTAGE = 3600; //[mv]
static constexpr uint32_t MIN_LEVEL = 3100; //[mv]
static constexpr uint32_t MAX_LEVEL = 4200; //[mv]
static constexpr uint32_t LOW_LEVEL = 3300; //[mv]

static constexpr uint32_t BATTERY_DIVIDER_HIGH = 200;
static constexpr uint32_t BATTERY_DIVIDER_LOW = 100;
static constexpr uint32_t BATTERY_DIVIDER_STAB_TIME = 10; //[ms]

#define ENABLE_BAT_FB(en) (HAL_GPIO_WritePin(BAT_FB_GPIO_Port, BAT_FB_Pin,(en)?(GPIO_PIN_RESET):(GPIO_PIN_SET)))

typedef enum {
	bsIDLE, bsCALIBRATE, bsWAIT_DIVIDER_STAB, bsSTART, bsRUNNING
} State;

volatile static Battery* m_batteryCbInstance;

namespace Driver {
	void OnADCCallback(){
		m_batteryCbInstance->m_done = true;
	}
}

extern "C" {
	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
		OnADCCallback();
	}
}

Battery::Battery() : m_lastVoltage(START_VOLTAGE), m_state(bsIDLE){
}

Battery::~Battery(){
	return;
}

bool Battery::RequestVoltage(std::function<void(uint32_t voltage_mv)> callback){
	if(IsBusy()){
		return false;
	}
	m_req = true;
	m_callback = callback;
	return true;
}

uint32_t Battery::GetVoltage(){
	return m_lastVoltage;
}
uint32_t Battery::GetLevel(){
	auto lv = std::clamp(m_lastVoltage,MIN_LEVEL,MAX_LEVEL);
	return (lv - MIN_LEVEL)*100/(MAX_LEVEL-MIN_LEVEL);
}

bool Battery::IsLow(){
	return m_lastVoltage<=LOW_LEVEL;
}

bool Battery::IsBusy(){
	return m_state!=bsIDLE || m_req;
}

void Battery::Loop(){
	switch(m_state){
		case bsIDLE:
		{
			if(m_req){
				m_req = false;
				HAL_ADC_Init(HADC);
				m_state = bsCALIBRATE;
				ENABLE_BAT_FB(true);
				m_tim.Set(0);
				m_tim.SetAlarm(BATTERY_DIVIDER_STAB_TIME);
			}
			break;
		}
		case bsCALIBRATE:
		{
			if(HAL_ADCEx_Calibration_Start(HADC, ADC_SINGLE_ENDED)==HAL_OK){
				m_state = bsWAIT_DIVIDER_STAB;
			}
			break;
		}
		case bsWAIT_DIVIDER_STAB:
		{
			if(m_tim.Expired()){
				m_state = bsSTART;
			}
			break;
		}
		case bsSTART:
		{
			m_done = false;
			m_batteryCbInstance = this;
			if(HAL_ADC_Start_DMA(HADC, (uint32_t*)m_buffer,2)==HAL_OK){
				m_state = bsRUNNING;
			}
			break;
		}
		case bsRUNNING:
		{
			if(m_done){
				//Acquire voltage from reference.
				auto rawVoltage = ((uint32_t)m_buffer[0])*VREFINT/m_buffer[1];

				//Adjust battery voltage from divider.
				m_lastVoltage = rawVoltage * (BATTERY_DIVIDER_HIGH+BATTERY_DIVIDER_LOW) / BATTERY_DIVIDER_LOW;

				m_state = bsIDLE;
				HAL_ADC_DeInit(HADC); //Save power.
				ENABLE_BAT_FB(false);
				if(m_callback){
					auto cb = m_callback;
					cb(m_lastVoltage);
				}
			}
			break;
		}
	}
}

