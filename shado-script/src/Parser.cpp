#include "Parser.h"
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <stack>
#include <sstream>

namespace Shado {
	static std::vector<std::string> Split(const std::string& str, const std::string& regex);
	static bool Matches(const std::string& subject, const std::string& regex);
	static std::string ExtractFunction(const std::string& block);
	static std::string SubString(const std::vector<std::string>& lines, int startAt);
	static void trim(std::string& s);
	static bool IsNumber(const std::string& s);
	static bool IsComment(const std::string& line);
	
	Parser::Parser(const std::string& filename)
	{
		vm = new VM();
		
		// Read the file
		std::ifstream ifs(filename);
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

		// Parse text
		std::unordered_map<std::string, std::string> funcBlocks;
		auto lines = Split(content, "\n");
		for (auto& line : lines) {

			if (IsComment(line)) {
				goto NextIt;
			}
			
			if (Matches(line, "\\w+\\s+\\w+\\(.*\\)\\s*.*")) {
				std::string block = ExtractFunction(SubString(lines, lineNum + 1));

				// Exract function name
				auto name = Split(Split(line, "\\s+")[1], "\\(")[0];
				funcBlocks[name] = block;
			}

			NextIt:
				lineNum++;
		}

		// Convert code blocks to function code
		for (auto& func_block : funcBlocks) {
			// See if it is a function call
			std::vector<std::string> funcLines = Split(func_block.second, "\n");

			std::vector<std::function<void()>> nativeCodeLines;
			
			for (const auto& line : funcLines) {

				std::string trimmedLine = line;
				trim(trimmedLine);

				if (IsComment(line)) {
					continue;
				}
				
				// Function call
				if (Matches(trimmedLine, "\\w+(.*)")) {
					// Extract the callee name
					auto tokens = Split(trimmedLine, "\\(");
					std::string callee = tokens[0];
					auto rawArgs = tokens[1].replace(tokens[1].rfind(')'), 1, "");

					const int semiColPos = tokens[1].rfind(';');
					if (semiColPos != std::string::npos)
						rawArgs.replace(semiColPos, 1, "");

					auto splittedArgs = Split(rawArgs, ",");
					
					std::vector<std::any> parssedArgs;
					for (auto& arg : splittedArgs) {
						trim(arg);
						if (IsNumber(arg))
							parssedArgs.emplace_back(std::make_any<double>(std::stod(arg)));
						else
							parssedArgs.emplace_back(std::make_any<std::string>(arg));
					}

					// convert function to C++
					VM* wrapper = this->vm;
					nativeCodeLines.emplace_back([wrapper, callee, parssedArgs]() {
						wrapper->Call(callee, parssedArgs);
					});
				}
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

	VM& LoadScriptFile(const std::string& filename) {
		Parser* parser = new Parser(filename);
		
		VM* res = parser->GetVM();
		//delete parser;
		
		return *res;
	}
	
	static std::string ExtractFunction(const std::string& block) {
		std::stack<char> stack;
		std::stringstream buffer;
		
		stack.push('}');
		
		for (const char& c : block) {
			if (c == '{')
				stack.push('{');
			else if (c == '}') {
				
				stack.pop();
				if (stack.empty()) 
					return buffer.str();
				
			}

			buffer << c;
		}
	}

	static std::vector<std::string> Split(const std::string& str, const std::string& regex) {
		std::regex rgx(regex);
		std::sregex_token_iterator iter(str.begin(),
			str.end(),
			rgx,
			-1);
		std::sregex_token_iterator end;
		
		std::vector<std::string> result;
		
		for (; iter != end; ++iter)
			result.push_back(*iter);

		return result;
	}

	static bool Matches(const std::string& subject, const std::string& regex) {
		return std::regex_match(subject, std::regex(regex));
	}

	static std::string SubString(const std::vector<std::string>& lines, int startAt) {
		auto first = lines.begin() + startAt;
		auto last = lines.end();
		std::vector<std::string> x(first, last);

		return std::accumulate(std::begin(x), std::end(x), std::string(),
			[](std::string& ss, std::string& s)
			{
				return ss.empty() ? s : ss + "\n" + s;
			});
	}

	// trim from start (in place)
	static void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
			}));
	}

	// trim from end (in place)
	static void rtrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	// trim from both ends (in place)
	static void trim(std::string& s) {
		ltrim(s);
		rtrim(s);
	}

	static bool IsNumber(const std::string& s)
	{
		try
		{
			std::stod(s);
		} catch (...)
		{
			return false;
		}
		return true;
	}

	static bool IsComment(const std::string& line) {
		std::string __temp = line;
		trim(__temp);

		// If comment
		if (__temp.find("//") == 0) {
			return true;
		}
		return false;
	}
}
