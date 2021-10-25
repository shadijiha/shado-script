#include "ShadoScript.h"
#include <fstream>

namespace Shado {
	std::shared_ptr<VM> FromScriptFile(const std::string& filename) {
		// Read the file
		std::ifstream ifs(filename);
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		ifs.close();

		Parser* parser = new Parser(content);

		auto res = parser->GetVM();
		delete parser;

		return res;
	}

	std::shared_ptr<VM> FromCode(const std::string& code) {
		Parser* parser = new Parser(code);

		auto res = parser->GetVM();
		delete parser;

		return res;
	}
}
