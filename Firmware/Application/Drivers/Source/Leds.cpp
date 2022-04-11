#include <Drivers/Leds.h>

#include <limits>

#include <main.h>

using namespace Driver;

static constexpr uint64_t FOREVER = std::numeric_limits<uint64_t>::max();
static constexpr uint32_t BLINK_ETERNAL =  std::numeric_limits<uint32_t>::max();

GPIO_TypeDef* m_ledPorts[Leds::NUM_LEDS] = {LED_R_GPIO_Port,LED_Y_GPIO_Port, LED_G_GPIO_Port};
uint16_t m_ledPins[Leds::NUM_LEDS] = {LED_R_Pin,LED_Y_Pin, LED_G_Pin};
GPIO_PinState m_ledActiveLevels[Leds::NUM_LEDS] = {GPIO_PIN_SET,GPIO_PIN_SET,GPIO_PIN_SET};

Leds::Leds(){
	for(uint32_t i=0;i<NUM_LEDS;i++){
		Set((Ld)i,false);
	}
}

Leds::~Leds(){
	for(uint32_t i=0;i<NUM_LEDS;i++){
		Set((Ld)i,false);
	}
}

void Leds::Set(Ld led, bool on){
	if(led>=NUM_LEDS){
		return;
	}
	m_state[led] = on;
	m_tim[led].Set(0);
	m_tim[led].SetAlarm(FOREVER);
	m_blinkCount[led] = BLINK_ETERNAL;
	m_timeOn[led] = (on)?(FOREVER):(0);
	m_timeOff[led] = (!on)?(FOREVER):(0);
	DoSet(led,on);
}

void Leds::DoSet(Ld led, bool on){
	HAL_GPIO_WritePin(m_ledPorts[led], m_ledPins[led], (on)?(m_ledActiveLevels[led]):((m_ledActiveLevels[led]==GPIO_PIN_SET)?(GPIO_PIN_RESET):(GPIO_PIN_SET)));
}

void Leds::Blink(Ld led, uint32_t numBlinks, uint64_t timeOn, uint64_t timeOff, uint64_t timeToStart){
	if(led>=NUM_LEDS){
		return;
	}
	if(numBlinks==0){
		Set(led, false);
		return;
	}
	m_state[led] = false;
	m_blinkCount[led] = numBlinks;
	m_timeOn[led] = timeOn;
	m_timeOff[led] = timeOff;
	m_tim[led].Set(0);
	m_tim[led].SetAlarm(timeToStart);
	DoSet(led,false);
}

bool Leds::IsBlinking(Ld led) const{
	if(led>=NUM_LEDS){
		return false;
	}
	return m_tim[led].GetAlarm()!=FOREVER && m_blinkCount[led]>0;
}

void Leds::Loop(){
	for(uint32_t i=0;i<NUM_LEDS;i++){
		if(m_tim[i].GetAlarm()==FOREVER){
			//Led will never change state.
			continue;
		}
		if(m_tim[i].Expired()){
			if(m_state[i]){
				m_state[i] = false;
				DoSet((Ld)i, false);

				if(m_blinkCount[i]!=BLINK_ETERNAL){
					--m_blinkCount[i];
				}

				if(m_blinkCount[i]==0){
					Set((Ld)i, false);
				}else{
					m_tim[i].Set(0);
					m_tim[i].SetAlarm(m_timeOff[i]);
				}
			}else{
				m_state[i] = true;
				DoSet((Ld)i, true);
				m_tim[i].Set(0);
				m_tim[i].SetAlarm(m_timeOn[i]);
			}
		}
	}
}


