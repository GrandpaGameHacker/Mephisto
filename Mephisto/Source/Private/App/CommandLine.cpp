#include <App/CommandLine.h>
#include <Strings/StringUtilities.h>
#include <spdlog/spdlog.h>
CommandLine* CmdLine = nullptr;

CommandLine::CommandLine(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++) {
		Args.push_back(argv[i]);
	}

	for (const auto& Arg : Args)
	{
		std::string Left, Right;
		if (StrUtils::Split(Arg, '=', Left, Right))
		{
			ArgMap.try_emplace(Left, Right);
		}
		else
		{
			continue;
		}

	}
}
const CommandLine& CommandLine::Get()
{
	if (CmdLine)
	{
		return *CmdLine;
	}
	
}

void CommandLine::Initialize(int argc, char* argv[])
{
	if (!CmdLine) {
		CmdLine = new CommandLine(argc, argv);
	}
	else
	{
		spdlog::error("Tried to initialize CommandLine twice!");
		abort();
	}
}

const bool CommandLine::Contains(std::string argument)
{
	return false;
}

const CommandList& CommandLine::GetList()
{
	return Get().Args;
}

const CommandMap& CommandLine::GetArgPairs()
{
	return Get().ArgMap;
}

const std::string* CommandLine::GetValue(const std::string& key)
{
	auto pair = Get().ArgMap.find(key);
	if (pair != Get().ArgMap.end()) {
		return &pair->second;
	}
	return nullptr;
}
