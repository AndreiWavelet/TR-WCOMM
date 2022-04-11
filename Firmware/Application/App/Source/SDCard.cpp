#include <App/SDCard.h>

#include <fatfs.h>
#include <fatfs_sd.h>
#include <ff.h>
#include <ff_gen_drv.h>

using namespace App;

FATFS m_fs;
FIL m_file[SdFile::MAX_FILES];

SDCard::SDCard() : m_enabled(false){
	for(uint32_t i=0;i<SdFile::MAX_FILES;i++){
		m_fileHandlersOwners[i] = nullptr;
	}
	return;
}

SDCard::~SDCard(){
	Enable(false);
}

SDCard* SDCard::GetInstance(){
	static SDCard instance;
	return &instance;
}

bool SDCard::Enable(bool en){
	if(IsEnabled()==en){
		return true;
	}


	if(en){
		FATFS_LinkDriver(&USER_Driver, USERPath);

		auto res = f_mount(&m_fs,"/",1);
		if(res!=FR_OK){
			return false;
		}
	}else{
		for(uint32_t i=0;i<SdFile::MAX_FILES;i++){
			if(m_fileHandlersOwners[i]){
				m_fileHandlersOwners[i]->Close();
			}
		}

		FATFS_UnLinkDriver(USERPath);

		SD_disk_shutdown();
	}

	m_enabled = en;
	return true;
}

bool SDCard::IsEnabled() const{
	return m_enabled;
}

SdFile::SdFile() : m_handle(nullptr){
	return;
}

SdFile::~SdFile(){
	Close();
}

void SdFile::GetHandle(){
	auto sdc = SDCard::GetInstance();
	for(uint32_t i=0;i<MAX_FILES;i++){
		if(!sdc->m_fileHandlersOwners[i]){
			sdc->m_fileHandlersOwners[i] = this;
			m_handle = &m_file[i];
		}
	}
}

void SdFile::ReleaseHandle(){
	auto sdc = SDCard::GetInstance();
	for(uint32_t i=0;i<MAX_FILES;i++){
		if(sdc->m_fileHandlersOwners[i] == this){
			sdc->m_fileHandlersOwners[i] = nullptr;
		}
	}
	m_handle = nullptr;
}

bool SdFile::Open(const char* path, bool write, bool read, bool onlyIfExists){
	auto sdc = SDCard::GetInstance();
	if(!sdc->IsEnabled()){
		return false;
	}

	if(IsOpen()){
		Close();
	}

	GetHandle();

	if(!m_handle){
		return false;
	}

	uint32_t flags = 00;
	if(write){
		flags |= FA_WRITE;
	}
	if(read){
		flags |= FA_READ;
	}
	if(onlyIfExists){
		flags |= FA_OPEN_EXISTING;
	}else{
		flags |= FA_OPEN_ALWAYS;
	}

	auto res = f_open((FIL*)m_handle, path, flags);
	if(res!=FR_OK){
		ReleaseHandle();
		return false;
	}

	return true;
}

void SdFile::Close(){
	auto sdc = SDCard::GetInstance();
	if(!sdc->IsEnabled()){
		return;
	}

	if(!IsOpen()){
		return;
	}

	f_close((FIL*)m_handle);

	ReleaseHandle();
}

bool SdFile::IsOpen() const{
	return m_handle!=nullptr;
}

uint32_t SdFile::GetSize() const{
	if(!IsOpen()){
		return 0;
	}
	return f_size((FIL*)m_handle);
}

uint32_t SdFile::GetPosition() const{
	if(!IsOpen()){
		return 0;
	}
	return f_tell((FIL*)m_handle);
}

uint32_t SdFile::Seek(uint32_t position){
	if(!IsOpen()){
		return 0;
	}
	auto res = f_lseek((FIL*)m_handle,position);
	if(res!=FR_OK){
		return 0;
	}
	return GetPosition();
}

uint32_t SdFile::Truncate(uint32_t size){
	if(!IsOpen()){
		return 0;
	}
	auto prevPos = GetPosition();
	Seek(size);
	auto res = f_truncate((FIL*)m_handle);
	if(res!=FR_OK){
		return 0;
	}
	Seek(prevPos);
	return GetPosition();
}

uint32_t SdFile::Write(const void* data, uint32_t size){
	if(!IsOpen()){
		return 0;
	}

	UINT written = 0;
	auto res = f_write((FIL*)m_handle, data, size, &written);
	if(res!=FR_OK || written!=size){
		return 0;
	}
	return written;
}

uint32_t SdFile::Read(void* data, uint32_t size){
	if(!IsOpen()){
		return 0;
	}

	UINT read = 0;
	auto res = f_read((FIL*)m_handle, data, size, &read);
	if(res!=FR_OK || read!=size){
		return false;
	}
	return read;
}

uint32_t SdFile::ReadLine(char* line, uint32_t maximumSize){
	if(maximumSize==0){
		return 0;
	}
	uint32_t dtRd = 0;
	while(--maximumSize){
		auto rd = Read(line,1);
		dtRd += rd;
		if(rd==0){
			break;
		}
		if(*line == '\r' || *line == '\n' || *line=='\0'){
			break;
		}
		line++;
	}

	*line = '\0';

	return dtRd;
}



