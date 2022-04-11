#pragma once

#include <cstdint>

#include <App/SDCard.h>
#include <App/TaskInterface.h>
#include <App/TickTimer.h>

namespace App {

	class Control final : public App::Task {
	public:
		Control();
		~Control();

		bool Sending() const;

		void Setup() override;
		void Loop() override;

	private:
		typedef struct __attribute__((packed)) {
			uint8_t pktType;
			uint32_t size;
			char filename[16];
		} FileDescStruct;

		typedef struct __attribute__((packed)) {
			uint8_t pktType;
			uint32_t ptr;
			uint8_t data[26];
		} FileDataStruct;

		uint32_t m_state;
		bool m_sendRequest;
		bool m_poffRequest;
		bool m_writeSuccess;
		bool m_fileAcceptRequest;
		bool m_sendFailureRequest;

		FileDataStruct m_pktSend;
		uint32_t m_pktSendSize;

		App::TickTimer m_sleepTim;
		App::TickTimer m_fileTransferTim;
		App::TickTimer m_fileRetryTim;

		char m_fileToSend[16];

		bool m_hasFileRxRequest;
		uint32_t m_fileSize;
		char m_fileToWrite[16];

		bool m_fileCancelRequest;
		bool m_fileCompleteRequest;

		App::SdFile m_fil;

		bool DoParseConfig();
		void ParseValuePair(const char* first, const char* second);
		void ConvertToLowerCase(char* dt);
		void OnReceiveRadioPacket(const void* data, uint32_t size);
	};

}
