#pragma once

#include <cstdint>
#include <functional>

#include <App/TaskInterface.h>
#include <App/TickTimer.h>

namespace Driver {

	class Switches final : public App::Task {
	public:
		Switches();
		~Switches();

		typedef enum {
			POWER, NUM_SWITCHES
		} Sw;

		/**
		 * @return true if the switch is pressed, false otherwise.
		 */
		bool Get(Sw sw);

		/**
		 * @return time, in ms, since the switch was pressed.
		 */
		uint64_t TimeSincePressed(Sw sw) const;

		/**
		 * Sets the callback to be called when the switch state changes.
		 * @param callback The callback to be called once the switch state changes.
		 */
		void SetCallback(Sw sw, std::function<void(bool state)> callback);

		/**
		 * Resets switch state.
		 * @param sw Switch to reset state.
		 * @remark If the switch if pressed, it will need to be released to detect a press again.
		 */
		void Reset(Sw sw);

		/**
		 * Loop task function.
		 */
		void Loop() override;
	private:
		App::TickTimer m_debounceTim[NUM_SWITCHES];
		App::TickTimer m_eventTim[NUM_SWITCHES];

		uint64_t m_lastPressTime[NUM_SWITCHES];
		bool m_state[NUM_SWITCHES];
		bool m_reset[NUM_SWITCHES];

		std::function<void(bool state)> m_swCallback[NUM_SWITCHES];
	};

}


