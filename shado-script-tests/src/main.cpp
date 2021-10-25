#include <chrono>
#include <iostream>
#define SHADO_SCRIPT_EXPERIMENTAL
#include "ShadoScript.h"

int main() {
	using namespace Shado;
	auto vm = FromScriptFile("hello.sscript");

	vm->DefineFunc("CreatePlatform", [](const auto& args)
		{
			std::cout << TO_NUMBER(args[0]) * 10;
			return RETURN_VOID;
		});

	// Function here
	vm->Call("Setup");
}
