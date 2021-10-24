#include "VM.h"
#include <iostream>

namespace Shado {

	
	VM::VM() {
		// Define standard functions
		DefineFunc("print", [](auto args) { std::cout << TO_STRING(args[0]);  return RETURN_VOID});
		DefineFunc("println", [](auto args) { std::cout << TO_STRING(args[0]) << std::endl;  return RETURN_VOID});

		DefineFunc("inspect_vm", [this](auto args) {
			for (auto& function : functions) {
				std::cout << "Function\t-->\t" << function.first << std::endl;
			}
			return RETURN_VOID;
		});
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

#if !SHADO_DIST
			std::cout << "Function \""s + name + "\" is not defined" << std::endl;
#endif
			
#if SHADO_DEBUG
			__debugbreak();
#else
			return std::make_any<int>(0);
#endif
		}
		
		return functions[name](args);			
	}

	std::shared_ptr<VM> VM::Create() {
		return std::make_shared<VM>();
	}
	
}
