#include <App/Main.h>
#include <App/TickTimer.h>

using namespace App;

TickTimer::TickTimer(uint64_t alarm){
	Set(0);
	SetAlarm(alarm);
}

TickTimer::~TickTimer(){
	return;
}

void TickTimer::SetAlarm(uint64_t alarm){
	m_alarmDuration = alarm;
}

uint64_t TickTimer::GetAlarm() const{
	return m_alarmDuration;
}

void TickTimer::Set(uint64_t time){
	m_startTime = App::Main::GetInstance()->Millis() - time;
}

uint64_t TickTimer::Get() const {
	return App::Main::GetInstance()->Millis() - m_startTime;
}

bool TickTimer::Expired() const {
	return Get()>=m_alarmDuration;
}
