#pragma once

#include <cstdint>

#include <App/TaskInterface.h>
#include <App/TickTimer.h>

namespace Driver {

	class Leds final : public App::Task {
	public:
		Leds();
		~Leds();

		typedef enum {
			RED, YELLOW, GREEN, NUM_LEDS
		} Ld;

		/**
		 * Sets led to turn continually on or off.
		 * @param led Led to control.
		 * @param on true to turn on, false to turn off.
		 */
		void Set(Ld led, bool on);

		/**
		 * Configures a led to blink.
		 * @param led Led to control.
		 * @param numBlinks Amount of times to blink.
		 * @param timeOn Time for the led to stay on, in ms.
		 * @param timeOff Time for the led to stay off, in ms.
		 * @param timeToStart Time after the function call until the led first turns on.
		 */
		void Blink(Ld led, uint32_t numBlinks = 1, uint64_t timeOn = 100, uint64_t timeOff = 0, uint64_t timeToStart = 50);

		/**
		 * @return true if led is blinking, false otherwise.
		 */
		bool IsBlinking(Ld led) const;

		/**
		 * Task loop function.
		 */
		void Loop();
	private:
		bool m_state[NUM_LEDS];
		App::TickTimer m_tim[NUM_LEDS];
		uint32_t m_blinkCount[NUM_LEDS];
		uint64_t m_timeOn[NUM_LEDS];
		uint64_t m_timeOff[NUM_LEDS];

		void DoSet(Ld led, bool on);
	};

}

