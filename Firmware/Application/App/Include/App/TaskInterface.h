#pragma once


namespace App {

	class Task {
	public:
		virtual void Setup() {
			return;
		}
		virtual void Loop(){
			return;
		}

	};

}

