#pragma once

#include "Object.h"

#include <exception>
#include <string>

class Exception : public Object, public std::exception
{
private:
	
	int m_Line;
	std::string m_File;
	HRESULT m_HR;
	bool m_HasHRResult;
	std::string m_Message;
	Exception* m_InnerException;

protected:

	mutable std::string m_WhatBuffer;

public:

	Exception(int line, const char* file) noexcept;
	Exception(int line, const char* file, HRESULT hr) noexcept;
	Exception(int line, const char* file, HRESULT hr, const std::string& message) noexcept;
	Exception(int line, const char* file, const std::string& message) noexcept;
	Exception(int line, const char* file, HRESULT hr, const std::string& message, Exception* const innerException) noexcept;
	Exception(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	~Exception() noexcept;

	virtual const char* what() const noexcept override;
	const int GetLine() const noexcept;
	HRESULT GetErrorCode() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetErrorSpot() const noexcept;

	static const std::string TranslateErrorCode(HRESULT hr) noexcept;
};