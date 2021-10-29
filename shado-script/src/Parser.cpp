#include "Parser.h"
#include <regex>
#include <memory>


#include "Expression.h"
#include "ShadoScript.h"

namespace Shado {
	
	Parser::Parser(const std::string& content)
	{
		vm = std::make_shared<VM>();

		// Parse text
		std::unordered_map<std::string, std::string> funcBlocks;
		auto lines = Split(content, "\n");
		for (auto& line : lines) {

			if (IsComment(line)) {
				goto NextIt;
			}

			trimRef(line);
			if (Matches(line, "\\w+\\s+\\w+\\(.*\\)\\s*.*")) {
				std::string block = ExtractFunction(SubString(lines, lineNum + 1));

				// Exract function name
				auto name = Split(Split(line, "\\s+")[1], "\\(")[0];
				funcBlocks[name] = block;
			}

		NextIt:
			lineNum++;
		}

		// Convert each extracted function code blocks to c++ code
		for (auto& func_block : funcBlocks) {
			// See if it is a function call
			std::vector<std::string> funcLines = Split(func_block.second, "\n");

			std::vector<std::function<void()>> nativeCodeLines;

			for (const auto& line : funcLines) {

				std::string trimmedLine = line;
				trimRef(trimmedLine);

				if (IsComment(line)) {
					continue;
				}

				nativeCodeLines.emplace_back(Expression(line).Execute(*this));
				
			}

			// Now push the function to the VM
			vm->DefineFunc(func_block.first, [nativeCodeLines](std::vector<std::any>) {

				for (auto& native_code : nativeCodeLines) {
					native_code();
				}

				return std::make_any<int>(0);
				});
		}
	}



}
