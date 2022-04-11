#pragma once

#include <cstdint>
#include <functional>

#include <App/TaskInterface.h>
#include <App/Tasks.h>
#include <App/TickTimer.h>

namespace Driver {

	class NRF24L01p final : public App::Task {
	public:
		enum class Speed {
			S250K, S1M, S2M
		};

		enum class Power {
			Pm18, Pm12, Pm6, P0
		};

		typedef struct {
			uint8_t value[5];
		} Address;

		NRF24L01p();
		~NRF24L01p();

		/**
		 * @return true if there is an error, false otherwise. The driver will try to recover from the error automatically.
		 */
		bool Error() const;

		/**
		 * @return true if the driver is idle, false otherwise.
		 */
		bool Idle() const;

		/**
		 * Setup tx address.
		 * @param addr Tx address.
		 */
		void SetTxAddress(const Address& addr);

		/**
		 * Setup rx address.
		 * @param addr Rx address.
		 */
		void SetRxAddress(const Address& addr);

		/**
		 * Setup operating channel.
		 * @param chn Channel, 0-127.
		 */
		void SetChannel(uint8_t chn);

		/**
		 * Transmits data with confirmation.
		 * @param data Pointer to data to be transmitted.
		 * @param size Size of the data to be transmitted. Maximum of 32 bytes.
		 * @param callback Callback to be called when transmission finishes.
		 * @return true if operation can be started, false otherwise.
		 */
		bool SendData(const void* data, uint32_t size, std::function<void(bool success)> callback);

		/**
		 * Enables/disables reception.
		 * @param callback Callback to be called when there is new data, nullptr to disable.
		 */
		bool EnableRx(std::function<void(const void* data, uint32_t size)> callback);

		/**
		 * Task loop function.
		 */
		void Loop() override;
	private:
		static constexpr uint32_t MAX_MULTI_OP = 16;
		static constexpr uint32_t MAX_TRANSACTION_SIZE = 32;

		typedef struct {
			bool read;
			uint8_t address;

			bool externalData;
			uint8_t dataSize;
			union {
				void* extDataPtr;
				uint8_t intData[5];
			};

			uint32_t delayAfterOp;
		} MultiRegOpStrt;

		uint8_t m_txAddress[5];
		uint8_t m_rxAddress[5];
		uint8_t m_channel;
		Power m_power;
		Speed m_speed;
		uint8_t m_maxRetx;

		bool m_error;

		bool m_configureRequest;

		bool m_txRequest;
		uint8_t m_txReqBuffer[32];
		uint8_t m_txSize;
		std::function<void(bool success)> m_txCallback;

		bool m_rxRequest;
		bool m_rxStopRequest;
		uint8_t m_rxReqBuffer[32];
		uint8_t m_rxSize[5];
		uint8_t m_rxStatus;
		std::function<void(const void* data, uint32_t size)> m_rxCallback;

		uint8_t m_txStatus;
		uint8_t m_txObserver;

		uint8_t m_testAddress[5];
		uint8_t m_readTestAddress[5];

		uint32_t m_state;
		uint8_t m_txBuffer[MAX_TRANSACTION_SIZE+1];
		uint8_t m_rxBuffer[MAX_TRANSACTION_SIZE+1];

		App::TickTimer m_tim;
		App::TickTimer m_mopTim;

		MultiRegOpStrt m_multiOp[MAX_MULTI_OP];
		uint32_t m_numMultiOp;
		uint32_t m_currentMultiOp;
		uint32_t m_multiOpState;

		bool m_exti;

		bool MultiOpBusy() const;
		bool SetupMultiOp(const MultiRegOpStrt* multiOp, uint32_t numOp);
		void LoopMultiOp();
		void InitMultiOpStr(MultiRegOpStrt* str, bool read, uint8_t addr, bool extData, const void* data, uint32_t dataSize, uint32_t delayAfterOf);

		friend void OnExtiCallback();
	};

}

