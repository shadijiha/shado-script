#include "VM.h"
#include <iostream>
#include <vector>

namespace Shado {


	VM::VM() {
		// Define standard functions
		auto printfn = [](const std::vector<std::any>& args)
		{
			try {
				std::cout << TO_NUMBER(args[0]);
			} catch (const std::bad_any_cast & e) {
				std::cout << TO_STRING(args[0]);
			}

			return RETURN_VOID;
		};

		DefineFunc("print", printfn);
		DefineFunc("println", [printfn](const auto& args)
			{
				printfn(args);
				std::cout << std::endl;
				return RETURN_VOID
			});

		DefineFunc("inspect_vm", [this](const auto& args) {
			for (auto& function : functions) {
				std::cout << "Function\t-->\t" << function.first << std::endl;
			}
			return RETURN_VOID;
			});
	}

	VM::~VM() {
	}

	void VM::DefineFunc(const std::string& name, const Function& func) {
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

	const std::unordered_map<std::string, VM::Function>& VM::GetFunctions() const {
		return functions;
	}

	std::shared_ptr<VM> VM::Create() {
		return std::make_shared<VM>();
	}

}
