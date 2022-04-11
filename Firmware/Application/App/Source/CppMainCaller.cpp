#include <App/Main.h>

extern "C" {

	void CppMainCaller(){
		auto mn = App::Main::GetInstance();
		mn->Run();
		//Never returns.
	}

}

