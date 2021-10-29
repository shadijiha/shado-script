#include "Expression.h"

#include "ShadoScript.h"

namespace Shado {

	Expression::Expression(const std::string& raw)
		: line(raw)
	{
	}

	Expression::~Expression() {
	}

	std::function<void()> Expression::Execute(Parser& parser) {
		this->line = trim(line);

		if (Matches(line, "\\w+((\\s+)?\\*|\\&)?\\s+\\w+\\s+=\\s+.*")) {
			return ExecuteVarAssign(parser);
		}
		
		if (Matches(line, "\\w+(.*)")) {
			return ExecuteFuncCall(parser);
		}
		
	}

	std::function<void()> Expression::ExecuteFuncCall(Parser& parser) const {
		// Function call
		
			// Extract the callee name
			auto tokens = Split(line, "\\(");
			std::string callee = tokens[0];
			auto rawArgs = tokens[1].replace(tokens[1].rfind(')'), 1, "");

			const int semiColPos = tokens[1].rfind(';');
			if (semiColPos != std::string::npos)
				rawArgs.replace(semiColPos, 1, "");

			auto splittedArgs = Split(rawArgs, ",");

			auto* parssedArgs = new std::vector<std::any>();
			for (auto& arg : splittedArgs) {
				trim(arg);
				if (IsNumber(arg))
					parssedArgs->emplace_back(std::make_any<double>(std::stod(arg)));
				else
					parssedArgs->emplace_back(std::make_any<std::string>(arg));
			}

			// convert function to C++
			const auto vm = parser.GetVM();
			vm->AddCleanUp(parssedArgs);
		
			std::shared_ptr<VM> wrapper = vm;		
			return [wrapper, callee, parssedArgs]() {

				// Now on call, see if the arg is actually a variable
				int i = 0;
				for (auto& arg : *parssedArgs) {

					using namespace std::string_literals;
					if (std::string(arg.type().name()) == std::string(typeid(std::string).name()) && wrapper->GlobalVarExists(TO_STRING(arg))) {
						(*parssedArgs)[i] = wrapper->GetVar(TO_STRING(arg));
					}

					i++;
				}
				wrapper->Call(callee, *parssedArgs);
			};
	}

	std::function<void()> Expression::ExecuteVarAssign(Parser& parser) const {
		return []() {};
	}
}
