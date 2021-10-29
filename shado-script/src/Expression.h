#pragma once
#include <functional>
#include <string>
#include "VM.h"

namespace Shado {
	class Expression {
	public:
		Expression(const std::string& line);
		~Expression();

		std::function<void()> Execute(Parser& parser);

	private:
		std::function<void()> ExecuteFuncCall(Parser& parser) const;
		std::function<void()> ExecuteVarAssign(Parser& parser) const;

	private:
		std::string line;
	};
}
