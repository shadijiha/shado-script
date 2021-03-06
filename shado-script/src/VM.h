#pragma once
#include <functional>
#include <memory>
#include <string>
#include <any>
#include <vector>
#include <unordered_map>

#define RETURN_VOID std::make_any<int>(0);
#define TO_NUMBER(X) std::any_cast<double>(X)
#define TO_STRING(X) std::any_cast<std::string>(X)

namespace Shado {
	class VM final {
	public:
		using Function = std::function<std::any(const std::vector<std::any>&)>;

		VM();
		VM(const VM&) = delete;
		~VM();

		void DefineFunc(const std::string& name, const Function& func);
		void DefineGlobalVar(const std::string& name, std::any value);
		bool FuncExists(const std::string& name);
		bool GlobalVarExists(const std::string& name);
		
		std::any Call(const std::string& name, const std::vector<std::any>& args = {});
		const std::any& GetVar(const std::string& name);

		const std::unordered_map<std::string, Function>& GetFunctions() const;

		static std::shared_ptr<VM> Create();

	protected:
		void AddCleanUp(std::vector<std::any>* argVector);

	private:
		std::unordered_map<std::string, Function> functions;
		std::unordered_map<std::string, std::any> globalVariables;

		std::vector<std::vector<std::any>*> toClearnUp;
		
		friend class Parser;
		friend class Expression;
	};
}
