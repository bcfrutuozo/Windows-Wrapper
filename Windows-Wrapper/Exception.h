#pragma once

#include "CommonObject.h"
#include "_HResults.h"

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

	virtual const char* what() const noexcept override;

public:

	Exception(int line, const char* file) noexcept;
	Exception(int line, const char* file, const std::string& message) noexcept;
	Exception(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	~Exception() noexcept;

	const int GetLine() const noexcept;
	HRESULT GetErrorCode() const noexcept;
	void SetErrorCode(HRESULT hr) noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetErrorSpot() const noexcept;
	const char* GetExceptionMessage() const noexcept;
	void SetMessage(const std::string& message) noexcept;

	static const std::string TranslateErrorCode(HRESULT hr) noexcept;
};