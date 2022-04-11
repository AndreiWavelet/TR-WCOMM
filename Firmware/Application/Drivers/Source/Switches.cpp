#include <Drivers/Switches.h>

#include <main.h>

using namespace Driver;

static constexpr uint32_t DEBOUNCE_TIME = 20; //[ms]

GPIO_TypeDef* m_swPorts[Switches::NUM_SWITCHES] = {PWR_nBTN_GPIO_Port};
uint16_t m_swPins[Switches::NUM_SWITCHES] = {PWR_nBTN_Pin};
GPIO_PinState m_swActiveLevels[Switches::NUM_SWITCHES] = {GPIO_PIN_RESET};

Switches::Switches(){
	for(uint32_t i=0; i<NUM_SWITCHES;i++){
		m_state[i] = false;
		m_debounceTim[i].SetAlarm(DEBOUNCE_TIME);
		m_lastPressTime[i] = 0;
	}
}

Switches::~Switches(){
	for(uint32_t i=0; i<NUM_SWITCHES;i++){
		m_state[i] = false;
	}
}

bool Switches::Get(Sw sw){
	if(sw>=NUM_SWITCHES){
		return false;
	}
	return m_state[sw] && !m_reset[sw];
}

uint64_t Switches::TimeSincePressed(Sw sw) const{
	if(sw>=NUM_SWITCHES){
		return 0;
	}
	if(!m_state[sw]){
		return m_lastPressTime[sw];
	}
	return m_eventTim[sw].Get();
}

void Switches::SetCallback(Sw sw, std::function<void(bool state)> callback){
	if(sw>=NUM_SWITCHES){
		return;
	}
	m_swCallback[sw] = callback;
}

void Switches::Reset(Sw sw){
	if(sw>=NUM_SWITCHES){
		return;
	}
	m_reset[sw] = true;
}

void Switches::Loop(){
	for(uint32_t i=0;i<NUM_SWITCHES;i++){
		bool state = HAL_GPIO_ReadPin(m_swPorts[i], m_swPins[i])==m_swActiveLevels[i];
		if(state==m_state[i]){
			m_debounceTim[i].Set(0);
		}else{
			if(m_debounceTim[i].Expired()){
				m_state[i] = state;
				auto wasReset = m_reset[i];
				if(!state){
					m_lastPressTime[i] = m_eventTim[i].Get();
					m_reset[i] = false;
				}
				m_eventTim[i].Set(0);
				if(m_swCallback[i] && !wasReset){
					auto cb = m_swCallback[i];
					cb(state);
				}
			}
		}
	}
}
