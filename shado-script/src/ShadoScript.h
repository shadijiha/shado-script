#pragma once
#include "Parser.h"

namespace Shado {
	using namespace Shado;
	
	VM& FromScriptFile(const std::string& filename);
	VM& FromCode(const std::string& code);
}
