#pragma once

#include <vector>
#include <unordered_map>
#include <string>


typedef std::vector<std::string> CommandList;
typedef std::unordered_map<std::string, std::string> CommandMap;

class CommandLine
{
public:
	static const CommandLine& Get();
	static void Initialize(int argc, char* argv[]);
	static const bool Contains(std::string argument);
	static const CommandList& GetList();
	static const CommandMap& GetArgPairs();
	static const std::string* GetValue(const std::string& key);
private:
	CommandLine(int argc, char* argv[]);
	CommandList Args;
	CommandMap ArgMap;
};