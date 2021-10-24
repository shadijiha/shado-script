#pragma once
#include "Parser.h"

#define RETURN_VOID std::make_any<int>(0);
#define TO_NUMBER(X) std::any_cast<double>(X)

namespace Shado {
	using namespace Shado;
	
	VM& LoadScriptFile(const std::string& filename);
	
}
