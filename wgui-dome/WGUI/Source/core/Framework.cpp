#include "../../Include/core/Framework.hpp"

_WGUI_BEGIN

#pragma region ��Сдת��

// ת��Ϊ��д
inline void StrToUpper(char* const str)
{
	int len = ::strlen(str);
	for (int i = 0; i < len; i++)
	{
		str[i] = ::toupper(str[i]);
	}
}
// ת��ΪСд
inline void StrToLower(char* const str)
{
	int len = ::strlen(str);
	for (int i = 0; i < len; i++) {
		str[i] = ::tolower(str[i]);
	}
}

// ת��Ϊ��д
inline void StrToUpper(wchar_t* const str)
{
	int len = ::wcslen(str);
	for (int i = 0; i < len; i++)
	{
		str[i] = ::toupper(str[i]);
	}
}

// ת��ΪСд
inline void StrToLower(wchar_t* const str)
{
	int len = ::wcslen(str);
	for (int i = 0; i < len; i++) {
		str[i] = ::tolower(str[i]);
	}
}

#pragma endregion

#pragma region ����ʱ��ṹ

DATETIME::DATETIME(DATE _Date, TIME _Time)
	: Date(_Date)
	, Time(_Time)
{}

DATETIME::DATETIME(WORD _Year, WORD _Month, WORD _Day,
				   WORD _Hour, WORD _Minute, WORD _Second,
				   WORD _Milliseconds)
	: DATETIME({ _Year, _Month, _Day }, { _Hour, _Minute, _Second, _Milliseconds })
{}

DATETIME::DATETIME(const SYSTEMTIME& _SysTime)
	: DATETIME(_SysTime.wYear, _SysTime.wMonth, _SysTime.wDay,
			   _SysTime.wHour, _SysTime.wMinute, _SysTime.wSecond,
			   _SysTime.wMilliseconds)
{}

DATETIME::operator SYSTEMTIME()const
{
	return { Year, Month, Day, Hour, Minute, Second, Milliseconds };
}

#pragma endregion


_WGUI_END