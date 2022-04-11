#pragma once

#include <cstdint>

namespace Driver {

	class Power {
	public:
		~Power();

		static Power* GetInstance();

		/**
		 * Sleeps for the specified time in low power mode.
		 * Will wakeup if there is a interruption.
		 * @param Time to sleep in ms.
		 */
		void LowPowerSleep(uint32_t time);

		/**
		 * Powers off the device. This function will not return.
		 */
		void PowerOff();

	private:
		Power();
	};


}




