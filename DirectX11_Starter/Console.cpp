#include "Console.h"

#include <windows.h>
#include <assert.h>
#include <cstdio>
#include <iostream>

using std::wstring;
using std::streambuf;
using std::cout;

namespace {
	FILE *outstream;
}

// Thank you based StackOverflow
// https://stackoverflow.com/questions/311955/redirecting-cout-to-a-console-in-windows#424236
class ConsoleOutBuff : public streambuf
{
public:
	ConsoleOutBuff()
	{
		// all output gets overflowed
		setp(nullptr, nullptr);
	}

	virtual int_type overflow(int_type c = traits_type::eof())
	{
		return fputc(c, outstream) == EOF ? traits_type::eof() : c;
	}
};

namespace {
	bool console_exists = false;
	streambuf *originalCout;
	ConsoleOutBuff outBuff;
}

Console::Console(wstring consoleTitle)
{
	assert(!console_exists);

	if (AllocConsole()) {
		console_exists = true;
		freopen_s(&outstream, "CONOUT$", "w", stdout);
		SetConsoleTitle(consoleTitle.c_str());
	}

	originalCout = cout.rdbuf(&outBuff);
}


Console::~Console()
{
	cout.rdbuf(originalCout);
}