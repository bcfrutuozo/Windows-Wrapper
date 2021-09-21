#include "Exception.h"

#include <sstream>

Exception::Exception(int line, const char* file) noexcept
	:
	m_Line(line),
	m_File(file),
	m_HasHRResult(false)
{}

Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	m_Line(line),
	m_File(file),
	m_HR(hr),
	m_HasHRResult(true)
{ }

Exception::Exception(int line, const char* file, HRESULT hr, const std::string & message) noexcept
	:
	m_Line(line),
	m_File(file),
	m_HR(hr),
	m_HasHRResult(true),
	m_Message(message)
{
}

Exception::Exception(int line, const char* file, const std::string& message) noexcept
	:
	m_Line(line),
	m_File(file),
	m_HasHRResult(false),
	m_Message(message)
{
}

Exception::Exception(int line, const char* file, HRESULT hr, const std::string& message, Exception* const innerException) noexcept
	:
	m_Line(line),
	m_File(file),
	m_HR(hr),
	m_HasHRResult(true),
	m_Message(message),
	m_InnerException(innerException)
{
}

Exception::Exception(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	m_Line(line),
	m_File(file),
	m_HasHRResult(false),
	m_Message(message),
	m_InnerException(innerException)
{
}

Exception::~Exception() noexcept
{
	if (m_InnerException != nullptr)
	{
		delete m_InnerException;
		m_InnerException = nullptr;
	}
}

const char* Exception::what() const noexcept
{
	std::ostringstream oss;

	if (!m_HasHRResult)
	{
		if (m_Message.empty())
		{
			oss << ToString() << std::endl << GetErrorSpot();
		}
		else
		{
			oss << ToString() << std::endl 
				<< "Message: " << m_Message << std::endl
				<< GetErrorSpot();
		}
	}
	else
	{
		if (m_Message.empty())
		{
			oss << ToString() << std::endl
				<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
				<< std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << std::endl
				<< "Description: " << TranslateErrorCode(m_HR) << std::endl
				<< GetErrorSpot();
		}
		else
		{
			oss << ToString() << std::endl
				<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
				<< std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << std::endl
				<< "Description: " << TranslateErrorCode(m_HR) << std::endl
				<< "Message: " << m_Message << std::endl
				<< GetErrorSpot();
		}
	}

	if (m_InnerException != nullptr)
	{
		oss << std::endl
			<< "--- Inner Exception ---" << std::endl
			<< "\t" << m_InnerException->what();
	}

	m_WhatBuffer = oss.str();

	return m_WhatBuffer.c_str();
}

const int Exception::GetLine() const noexcept
{
	return m_Line;
}

HRESULT Exception::GetErrorCode() const noexcept
{
	return m_HR;
}

const std::string& Exception::GetFile() const noexcept
{
	return m_File;
}

std::string Exception::GetErrorSpot() const noexcept
{
	std::ostringstream oss;
	oss << "File: " << m_File << std::endl << "Line: " << m_Line;
	return oss.str();
}

const std::string Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMessageBuffer = nullptr;

	// Windows will allocate memory for error string and make our pointer point to it
	const DWORD nMessageLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMessageBuffer),
		0,
		nullptr);

	// String length as 0 indicates a failure
	if (nMessageLength == 0)
	{
		return "Unidentified error code";
	}

	// Copy error string from windows allocated buffer to string
	std::string errorString = pMessageBuffer;

	// Free windows buffer
	LocalFree(pMessageBuffer);

	return errorString;
}
