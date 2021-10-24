#pragma once
#include <string>

#include "VM.h"

namespace Shado {
	class Parser {
	public:
		Parser(const std::string& filename);

		VM* GetVM() { return vm; }
		
	private:
		VM* vm;
		int lineNum = 0;
	};
}
