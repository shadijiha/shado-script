#pragma once
#include <functional>
#include <memory>
#include <string>
#include <any>
#include <vector>
#include <unordered_map>

namespace Shado {
	class VM final {
		using Function = std::function<std::any(std::vector<std::any>)>;
		
	public:
		VM();
		VM(const VM&) = delete;
		~VM();
		
		void DefineFunc(const std::string& name, Function func);
		void DefineGlobalVar(const std::string& name, std::any value);

		std::any Call(const std::string& name, const std::vector<std::any>& args);
		
		static std::shared_ptr<VM> Create();

	private:
		std::unordered_map<std::string, Function> functions;
		std::unordered_map<std::string, std::any> globalVariables;

		friend class Parser;
	};
}
