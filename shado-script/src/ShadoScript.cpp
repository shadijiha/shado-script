#include "ShadoScript.h"
#include <fstream>
#include <numeric>
#include <stack>
#include <sstream>
#include <regex>

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

	/////////////////////////////// UTILITY ///////////////////////////////

	// trim from start (in place)
	void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
			}));
	}

	// trim from end (in place)
	void rtrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	// trim from both ends (in place)
	void trimRef(std::string& s) {
		ltrim(s);
		rtrim(s);
	}

	std::string trim(const std::string& s) {
		auto temp = s;
		trimRef(temp);
		return temp;
	}
	
	bool IsNumber(const std::string& s)
	{
		try
		{
			std::stod(s);
		} catch (const std::exception& e)
		{
			return false;
		}
		return true;
	}

	bool IsComment(const std::string& line) {
		std::string __temp = line;
		trimRef(__temp);

		// If comment
		if (__temp.find("//") == 0) {
			return true;
		}
		return false;
	}

	std::string ExtractFunction(const std::string& block) {
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

	std::vector<std::string> Split(const std::string& str, const std::string& regex) {
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

	bool Matches(const std::string& subject, const std::string& regex) {
		return std::regex_match(subject, std::regex(regex));
	}

	std::string SubString(const std::vector<std::string>& lines, int startAt) {
		auto first = lines.begin() + startAt;
		auto last = lines.end();
		std::vector<std::string> x(first, last);

		return std::accumulate(std::begin(x), std::end(x), std::string(),
			[](std::string& ss, std::string& s)
			{
				return ss.empty() ? s : ss + "\n" + s;
			});
	}
}
