#include <Drivers/Power.h>

#include <App/TickTimer.h>

#include <algorithm>
#include <lptim.h>

using namespace Driver;

#define HLPTIM (&hlptim1)

static constexpr uint32_t LPTIM_CLOCK = 37000;
static constexpr uint32_t LPTIM_DIV = 32;
static constexpr uint32_t MIN_SLEEP_TIME = 3;

Power* Power::GetInstance(){
	static Power instance;
	return &instance;
}

Power::Power(){
	return;
}

Power::~Power(){
	return;
}

void Power::LowPowerSleep(uint32_t time){
	if(time<MIN_SLEEP_TIME){
		return;
	}

	//Find amount of clock cycles of the LPTIM will be needed to make the specified delay.
	uint32_t clkTime = (uint32_t)((uint64_t)time*LPTIM_CLOCK/(1000*LPTIM_DIV));
	clkTime = std::clamp<uint32_t>(clkTime,0,65535);

	//Start counter.
	HAL_LPTIM_Counter_Start_IT(HLPTIM, clkTime);

	//Suspend tick interrupt.
	HAL_SuspendTick();
	auto start = hlptim1.Instance->CNT;
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	auto end = hlptim1.Instance->CNT;
	HAL_ResumeTick();

	//Evaluate differente in time and compensate tick count.
	auto delta = end - start;
	uint32_t timeDeltaMs = (uint32_t)delta * 1000 * LPTIM_DIV / LPTIM_CLOCK;

	while(timeDeltaMs--){
		HAL_IncTick();
	}
}

void Power::PowerOff(){
	HAL_GPIO_WritePin(PWR_HOLD_GPIO_Port, PWR_HOLD_Pin, GPIO_PIN_SET);
	for(;;);
}
