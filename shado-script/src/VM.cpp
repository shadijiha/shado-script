#include "VM.h"
#include <iostream>

namespace Shado {

	
	VM::VM() {
	}

	VM::~VM() {
	}

	void VM::DefineFunc(const std::string& name, Function func) {
		functions[name] = func;
	}

	void VM::DefineGlobalVar(const std::string& name, std::any value) {
		globalVariables[name] = value;
	}

	std::any VM::Call(const std::string& name, const std::vector<std::any>& args) {
		using namespace std::literals::string_literals;
		
		if (functions.find(name) == functions.end()) {
			std::cout << "Function \""s + name + "\" is not defined" << std::endl;
			return std::make_any<int>(0);
		}
		
		return functions[name](args);			
	}

	std::shared_ptr<VM> VM::Create() {
		return std::make_shared<VM>();
	}
	
}
