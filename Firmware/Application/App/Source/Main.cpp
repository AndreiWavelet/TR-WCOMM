#include <App/Main.h>

#include <main.h>

#include <App/Tasks.h>

using namespace App;

Main::Main() : m_prevTicks(0), m_baseTicks(0){
	return;
}

Main::~Main(){
	return;
}

Main* Main::GetInstance(){
	static Main mn;
	return &mn;
}

void Main::Run(){
	Setup();
	for(;;){
		Loop();
	}
}

void Main::Setup(){
	m_prevTicks = HAL_GetTick();

	App::TaskManager::Setup();
}

void Main::Loop(){
	//ST Library HAL_GetTick will overflow after 2^32-1 ticks.
	//Detect this event and increment 1 on m_baseTicks so that Millis() can return a 64 bit integer.
	uint32_t millis = HAL_GetTick();
	if(millis<m_prevTicks){
		m_baseTicks ++;
	}
	m_prevTicks = millis;

	App::TaskManager::Loop();
}

uint64_t Main::Millis() const{
	//There is a small chance that HAL_GetTick overflows between a Loop() call and a Millis() call. Take care of it.
	uint32_t millis = HAL_GetTick();
	uint32_t bticks = m_baseTicks;
	if(millis<m_prevTicks){
		bticks++;
	}
	return (((uint64_t)bticks)<<32) + millis;
}
