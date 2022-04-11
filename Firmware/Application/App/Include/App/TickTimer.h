#pragma once

#include <cstdint>

namespace App {

	class TickTimer final {
	public:
		TickTimer(uint64_t alarm = 0);
		~TickTimer();

		void SetAlarm(uint64_t alarm);
		uint64_t GetAlarm() const;

		void Set(uint64_t time);
		uint64_t Get() const;

		bool Expired() const;
	private:
		uint64_t m_startTime;
		uint64_t m_alarmDuration;
	};

}

