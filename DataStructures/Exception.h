#pragma once

#include <exception>
#include <string>

class Exception : public std::exception
{

private:
	
	int m_Line;
	std::string m_File;

protected:
	
	mutable std::string m_WhatBuffer;

public:

	Exception(int line, const char* file) noexcept;

	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	const int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetErrorSpot() const noexcept;
};