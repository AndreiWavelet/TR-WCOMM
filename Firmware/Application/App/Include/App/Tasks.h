#pragma once

//Use this macro to build a task list, then declare the task object at Tasks.cpp.
#define TASK(taskname, classNamespace, className) \
namespace classNamespace { \
	class className; \
} \
namespace Tasks { \
	extern classNamespace::className taskname; \
}

//Task list...
TASK(Battery,Driver,Battery);
TASK(Leds,Driver,Leds);
TASK(NRF24L01p,Driver,NRF24L01p);
TASK(SpiManager,Driver,SpiManager);
TASK(Switches,Driver,Switches);

TASK(Control,App,Control);

namespace App {

	class TaskManager final {
	public:
		TaskManager() = delete;

		static void Setup();
		static void Loop();

	};

}

