#pragma once

#include <cstdint>
#include <functional>

#include <App/TaskInterface.h>

namespace Driver {

	class SpiManager final : public App::Task {
	public:
		enum class Device {
			SD, NRF,
			NUM_DEVICES,
			NONE
		};

		SpiManager();
		~SpiManager();

		/**
		 * Shutdowns SPI peripheral to save power.
		 * @return true if shutdown was completed successfully, false otherwise.
		 */
		bool Shutdown();

		/**
		 * Setups baud rate of a device.
		 * @param dev Device to be set up.
		 * @param baud Baud rate, in bits per second.
		 */
		void SetupDevice(Device dev, uint32_t baud);

		/**
		 * Transmits data via SPI to a determined device.
		 * @param dev Device that data will be sent to.
		 * @param data Pointer to data to be sent.
		 * @param dataSize Size of the data to be sent.
		 * @param callback Callback to be called when data is sent.
		 * @return true if operation can be started, false if not.
		 */
		bool SendData(Device dev, const void* data, uint32_t dataSize, std::function<void(void)> callback);
		/**
		 * Receives data via SPI from a determined device.
		 * @param dev Device that data will be received from.
		 * @param data Pointer to where received data will be writeen.
		 * @param dataSize Size of the data to be received.
		 * @param callback Callback to be called when data is received.
		 * @return true if operation can be started, false if not.
		 */
		bool ReceiveData(Device dev, void* data, uint32_t dataSize, std::function<void(void)> callback);
		/**
		 * Transmits and receives data via SPI from a determined device.
		 * @param dev Device that data will be sent and received from.
		 * @param dataSend Pointer to data to be sent.
		 * @param dataReceive Pointer to where received data will be writeen.
		 * @param dataSize Size of the data to be sent/received.
		 * @param callback Callback to be called when operation is done.
		 * @return true if operation can be started, false if not.
		 */
		bool SendReceiveData(Device dev, const void* dataSend, void* dataReceive, uint32_t dataSize, std::function<void(void)> callback);

		/**
		 * Locks device, keeping it's CS line LOW until unlock. No other device can be used while a device is locked.
		 * @param dev Device to be locked/unlocked.
		 * @param lock true to lock, false to unlock.
		 * @return true if success, false otherwise.
		 */
		bool LockDevice(Device dev, bool lock);

		/**
		 * @return true if busy, false otherwise.
		 */
		bool Busy() const;

		/**
		 * Pools until there are no more SPI operations.
		 */
		bool PoolUntilDone();

		/**
		 * Runs the driver state machine.
		 */
		void Loop() override;

	private:
		uint32_t m_deviceFreqDiv[(uint32_t)Device::NUM_DEVICES];
		bool m_devReconfReq;

		bool m_deviceLocked;
		Device m_deviceLock;

		bool m_opRequest;
		bool m_opRunning;
		volatile bool m_opDone;
		Device m_opDevice;
		uint8_t* m_dataToSend;
		uint8_t* m_dataToReceive;
		uint32_t m_dataSize;
		std::function<void(void)> m_opCallback;

		friend void OnSpiCallback();
	};

}

