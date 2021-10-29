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
		for (auto* vec : toClearnUp) {
			delete vec;
		}
	}

	void VM::DefineFunc(const std::string& name, const Function& func) {
		functions[name] = func;
	}

	void VM::DefineGlobalVar(const std::string& name, std::any value) {
		globalVariables[name] = value;
	}

	bool VM::FuncExists(const std::string& name) {
		return functions.find(name) != functions.end();
	}

	bool VM::GlobalVarExists(const std::string& name) {
		return globalVariables.find(name) != globalVariables.end();
	}

	const std::any& VM::GetVar(const std::string& name) {

		if (!GlobalVarExists(name)) {

#if !SHADO_DIST
			using namespace std::string_literals;
			std::cout << "Global variable \""s + name + "\" is not defined" << std::endl;
#endif

#if SHADO_DEBUG
			__debugbreak();
#else
			return std::make_any<int>(0);
#endif
		}
		return globalVariables[name];
	}

	std::any VM::Call(const std::string& name, const std::vector<std::any>& args) {
		using namespace std::literals::string_literals;

		if (!FuncExists(name)) {

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

	void VM::AddCleanUp(std::vector<std::any>* argVector) {
		this->toClearnUp.push_back(argVector);
	}

}
