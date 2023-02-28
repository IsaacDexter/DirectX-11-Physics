#ifndef DEBUG_H
#define DEBUG_H

#include <Windows.h>	//needed for OutputDebugString
#include <stdio.h>		//needed for vsnprintf

/// <summary>This function builds a const char * that is formatted using string formatting symbols, these symbols are populated via the variable arguments list passed into the function Variadic arguments MSDN
/// <para>Variadic functions have an indefinite amount of arguments</summary>
/// <param name="format"></param>
/// <param name="args"></param>
/// <returns></returns>
static int VDebugPrintF(const char* format, va_list args)
{
	const UINT32 MAX_CHARS = 1024;
	static char s_buffer[MAX_CHARS];

	int charsWritten = vsnprintf(s_buffer, MAX_CHARS, format, args);
	OutputDebugStringA(s_buffer);

	return charsWritten;
}

/// <summary>
/// DebugPrintF("deltaTime is %f \n the number is %i \n", accumulator, 2); is an example of how to call this.
/// </summary>
/// <param name="format"></param>
/// <param name=""></param>
/// <returns></returns>
static int DebugPrintF(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);

	int charsWritten = VDebugPrintF(format, argList);
	va_end(argList);

	return charsWritten;
}

#endif // !DEBUG_H