#pragma once
#include "Parser.h"

namespace Shado {
	using namespace Shado;

	std::shared_ptr<VM> FromScriptFile(const std::string& filename);
	std::shared_ptr<VM> FromCode(const std::string& code);

}
