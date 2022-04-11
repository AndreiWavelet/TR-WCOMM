#include <App/Tasks.h>

#include <Drivers/Battery.h>
#include <Drivers/Leds.h>
#include <Drivers/NRF24L01p.h>
#include <Drivers/SpiManager.h>
#include <Drivers/Switches.h>

#include <App/Control.h>

namespace Tasks {
::Driver::Battery Battery;
::Driver::Leds Leds;
::Driver::NRF24L01p NRF24L01p;
::Driver::SpiManager SpiManager;
::Driver::Switches Switches;

::App::Control Control;
}

using namespace App;

void TaskManager::Setup(){
	Tasks::Battery.Setup();
	Tasks::Leds.Setup();
	Tasks::NRF24L01p.Setup();
	Tasks::SpiManager.Setup();
	Tasks::Switches.Setup();

	Tasks::Control.Setup();
}

void TaskManager::Loop(){
	Tasks::Battery.Loop();
	Tasks::Leds.Loop();
	Tasks::NRF24L01p.Loop();
	Tasks::SpiManager.Loop();
	Tasks::Switches.Loop();

	Tasks::Control.Loop();
}

