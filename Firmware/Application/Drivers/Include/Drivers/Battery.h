#pragma once

#include <cstdint>
#include <functional>

#include <App/TaskInterface.h>
#include <App/TickTimer.h>

namespace Driver {

	class Battery final : public App::Task {
	public:
		Battery();
		~Battery();

		bool RequestVoltage(std::function<void(uint32_t voltage_mv)> callback);

		uint32_t GetVoltage();
		uint32_t GetLevel();
		bool IsLow();
		bool IsBusy();

		void Loop() override;
	private:
		uint32_t m_lastVoltage;

		App::TickTimer m_tim;

		std::function<void(uint32_t voltage_mv)> m_callback;

		uint32_t m_state;
		bool m_req;
		bool m_done;
		uint16_t m_buffer[2];

		friend void OnADCCallback();
	};

}
