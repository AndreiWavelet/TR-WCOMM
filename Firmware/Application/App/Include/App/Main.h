#pragma once

#include <cstdint>

namespace App {
	
	class Main {
		public:
			~Main();
		
			static Main* GetInstance();
			
			void Run();
			
			uint64_t Millis() const;
			
			
		private:
			uint32_t m_prevTicks;
			uint32_t m_baseTicks;
		
			void Setup();
			void Loop();
			Main();
	};

}

