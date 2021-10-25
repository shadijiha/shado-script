#pragma once
#include <string>

#include "VM.h"

namespace Shado {
	class Parser {
	public:
		Parser(const std::string& filename);

		std::shared_ptr<VM> GetVM() { return vm; }

	private:
		std::shared_ptr<VM> vm;
		int lineNum = 0;
	};
}
