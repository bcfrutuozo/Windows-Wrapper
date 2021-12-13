#include "InvalidEnumArgumentException.h"

InvalidEnumArgumentException::InvalidEnumArgumentException(int line, const char* file) noexcept
	:
	ArgumentException(line, file, "Arg_InvalidEnumException")
{

}

InvalidEnumArgumentException::InvalidEnumArgumentException(int line, const char* file, const std::string& message) noexcept
	:
	ArgumentException(line, file, message)
{

}

InvalidEnumArgumentException::InvalidEnumArgumentException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	ArgumentException(line, file, message, innerException)
{

}

InvalidEnumArgumentException::InvalidEnumArgumentException(int line, const char* file, const std::string& argumentName, int invalidValue, const std::string& enumTypeClassName) noexcept
	:
	ArgumentException(line, file, "InvaliEnumArgument:" + argumentName + "_" + "InvalidValue:" + std::to_string(invalidValue) + "_" + "TypeClass:" + enumTypeClassName)
{

}