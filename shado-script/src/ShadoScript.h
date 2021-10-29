#pragma once
#include "Parser.h"

namespace Shado {
	using namespace Shado;

	std::shared_ptr<VM> FromScriptFile(const std::string& filename);
	std::shared_ptr<VM> FromCode(const std::string& code);

	// Utility functions
	std::vector<std::string> Split(const std::string& str, const std::string& regex);
	bool Matches(const std::string& subject, const std::string& regex);
	std::string ExtractFunction(const std::string& block);
	std::string SubString(const std::vector<std::string>& lines, int startAt);
	void trimRef(std::string& s);
	std::string trim(const std::string& s);
	bool IsNumber(const std::string& s);
	bool IsComment(const std::string& line);
}
