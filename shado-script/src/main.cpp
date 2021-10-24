#include <iostream>
#include "ShadoScript.h"


int main() {
	using namespace Shado;
	VM& vm = LoadScriptFile("hello.sscript");

	vm.DefineFunc("CppFunction", [](auto args) {

		std::cout << TO_NUMBER(args[0]) + TO_NUMBER(args[1]) << std::endl;

		
		return RETURN_VOID;
	});

	vm.Call("Update", {});
}