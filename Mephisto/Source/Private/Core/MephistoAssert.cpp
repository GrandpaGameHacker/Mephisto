#include <Core/MephistoAssert.h>
#include <spdlog/spdlog.h>
#include <assert.h>
void AssertionHandler(const char* File, const char* Function, const char* Assertion, int LineNumber)
{
	spdlog::error("Assertion Failure Detected!");
	spdlog::error("{} at line {}", File, LineNumber);
	spdlog::error("{} -> {}", Function, Assertion);
	abort();
}

