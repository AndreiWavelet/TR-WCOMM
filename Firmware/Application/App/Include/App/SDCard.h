#pragma once

#include <cstdint>

namespace App {

	class SdFile final {
	public:
		static constexpr uint32_t MAX_FILES = 2;

		SdFile();
		~SdFile();

		/**
		 * Opens file for reading or writing. Then opened file points to the start of the file.
		 * @param path Path to the file.
		 * @param write true to enable writing, false otherwise.
		 * @param read true to enable reading, flase otherwise.
		 * @param onlyIfExists true to open only a existing file and fail otherwise, false to create file if it doesn't exist.
		 * @return true if file could be open, false otherwise.
		 */
		bool Open(const char* path, bool write = true, bool read = true, bool onlyIfExists = false);

		/**
		 * Closes the file.
		 */
		void Close();

		/**
		 * @return true if the file is open, false otherwise.
		 */
		bool IsOpen() const;

		/**
		 * @return Current size of the file.
		 */
		uint32_t GetSize() const;

		/**
		 * @return The read/write point location relative to the start of the file.
		 */
		uint32_t GetPosition() const;

		/**
		 * Sets the location of the read/write pointer.
		 * @return The actual position of the pointer after the operation.
		 */
		uint32_t Seek(uint32_t position);

		/**
		 * Truncates the file to a specified size.
		 * @return The actual position of the pointer after the operation.
		 */
		uint32_t Truncate(uint32_t size);

		/**
		 * Writes data to file.
		 * @param data Pointer to the data to write.
		 * @param size Size of the data to write.
		 * @return The amount of written data.
		 */
		uint32_t Write(const void* data, uint32_t size);

		/**
		 * Reads data from file.
		 * @param data Pointer to where to write read data.
		 * @param size Amount of data to read.
		 * @return The amount of read data.
		 */
		uint32_t Read(void* data, uint32_t size);

		/**
		 * Reads a line terminated by \n, \r, \0 or until reach the end of the file or until there is no more space on the output buffer.
		 * @param line Pointer to where to write read data.
		 * @param maximumSize Buffer size.
		 * @return Amount of read bytes.
		 */
		uint32_t ReadLine(char* line, uint32_t maximumSize);

	private:
		void* m_handle;

		void GetHandle();
		void ReleaseHandle();
	};

	class SDCard final {
	public:
		~SDCard();

		static SDCard* GetInstance();

		bool Enable(bool en);
		bool IsEnabled() const;

	private:
		bool m_enabled;

		SdFile* m_fileHandlersOwners[SdFile::MAX_FILES];

		SDCard();

		friend class SdFile;
	};

}

