#pragma once

#include "ArgumentException.h"
#include "ArgumentNullException.h"
#include "ArgumentOutOfRangeException.h"
#include "ArithmeticException.h"
#include "ControlException.h"
#include "DivideByZeroException.h"
#include "Exception.h"
#include "InvalidOperationException.h"
#include "NotImplementedException.h"
#include "NotSupportedException.h"

/**************************************************************************************************************************************
ArgumentException macro translations:

ArgumentException()
ArgumentException(const std::string& message)
ArgumentException(const std::string& message, Exception* const innerException)
ArgumentException(const std::string& message, const std::string& para)
ArgumentException(const std::string& message, const std::string& param, Exception* const innerException)
**************************************************************************************************************************************/
#define ArgumentException(...) ArgumentException(__LINE__, __FILE__, ##__VA_ARGS__)

/**************************************************************************************************************************************
ArgumentNullException macro translations:

ArgumentNullException()
ArgumentNullException(const std::string& param)
ArgumentNullException(const std::string& message, Exception* const innerException)
ArgumentNullException(const std::string& message, const std::string& param)
**************************************************************************************************************************************/
#define ArgumentNullException(...) ArgumentNullException(__LINE__, __FILE__, ##__VA_ARGS__)

/**************************************************************************************************************************************
ArgumentNullException macro translations:

ArgumentNullException()
ArgumentNullException(const std::string& param)
ArgumentNullException(const std::string& message, Exception* const innerException)
ArgumentNullException(const std::string& message, const std::string& param)
**************************************************************************************************************************************/
#define ArgumentOutOfRangeException(...) ArgumentOutOfRangeException(__LINE__, __FILE__, ##__VA_ARGS__)

/**************************************************************************************************************************************
ArithmeticException macro translations:

ArithmeticException()
ArithmeticException(const std::string& message)
ArithmeticException(const std::string& message, Exception* const innerException)
**************************************************************************************************************************************/
#define ArithmeticException(...) ArithmeticException(__LINE__, __FILE__, ##__VA_ARGS__)

/**************************************************************************************************************************************
ControlException macro translations:

ControlException()
ControlException(HRESULT hr)
ControlException(HRESULT hr, const std::string& message)
**************************************************************************************************************************************/
#define ControlException(...) ControlException(__LINE__, __FILE__, ##__VA_ARGS__)
#define CTL_LAST_EXCEPT() ControlException(GetLastError())

/**************************************************************************************************************************************
DivideByZeroException macro translations:

DivideByZeroException()
DivideByZeroException(const std::string& message)
DivideByZeroException(const std::string& message, Exception* const innerException)
**************************************************************************************************************************************/
#define DivideByZeroException(...) DivideByZeroException(__LINE__, __FILE__, ##__VA_ARGS__)

/**************************************************************************************************************************************
Exception macro translations:

Exception()
Exception(HRESULT hr)
Exception(HRESULT hr, const std::string& message)
Exception(const std::string& message)
Exception(HRESULT hr, const std::string& message, Exception* const innerException)
Exception(const std::string& message, Exception* const innerException)
**************************************************************************************************************************************/
#define Exception(...) Exception( __LINE__, __FILE__, ##__VA_ARGS__)
#define LAST_EXCEPTION() Exception(GetLastError())

/**************************************************************************************************************************************
InvalidOperationException macro translations:

InvalidOperationException()
InvalidOperationException(const std::string& message)
InvalidOperationException(const std::string& message, Exception* const innerException)
**************************************************************************************************************************************/
#define InvalidOperationException(...) InvalidOperationException(__LINE__, __FILE__, ##__VA_ARGS__)

/**************************************************************************************************************************************
NotImplementedException macro translations:

NotImplementedException()
NotImplementedException(const std::string& message)
NotImplementedException(const std::string& message, Exception* const innerException)
**************************************************************************************************************************************/
#define NotImplementedException(...) NotImplementedException(__LINE__, __FILE__, ##__VA_ARGS__)

/**************************************************************************************************************************************
NotSupportedException macro translations:

NotSupportedException()
NotSupportedException(const std::string& message)
NotSupportedException(const std::string& message, Exception* const innerException)
**************************************************************************************************************************************/
#define NotSupportedException(...) NotSupportedException(__LINE__, __FILE__, ##__VA_ARGS__)

/**************************************************************************************************************************************
!!! Important !!!

Because SystemException serves as the base class of a variety of exception types, your code should not throw a SystemException
exception, nor should it attempt to handle a SystemException exception unless you intend to re-throw the original exception.

SYSTEMEXCEPTION USES THE HRESULT COR_E_SYSTEM, THAT HAS THE VALUE 0X80131501.

SystemException macro translations:

SystemException()
SystemException(const std::string& message)
SystemException(const std::string& message, Exception* const innerException)
**************************************************************************************************************************************/
#define SystemException(...) SystemException(__LINE__, __FILE__, ##__VA_ARGS__)