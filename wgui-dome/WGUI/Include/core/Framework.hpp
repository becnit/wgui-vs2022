#pragma once
#ifndef _WGUI_FRAMEWORK_
#define _WGUI_FRAMEWORK_

#define _WGUI_BEGIN	namespace WGUI {
#define _WGUI_END	}

#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <initializer_list>

#pragma comment(lib, "Comctl32.lib")

#include "_auto_ptr.hpp"
#include "_property.hpp"
#include "WStr.hpp"

_WGUI_BEGIN

template<typename _Ty>
using INIT_LIST = std::initializer_list<_Ty>;

#pragma region 逻辑运算增强

// 批量逻辑或运算
template<typename _Ty, SIZE_T _Size>
inline bool _or(_Ty A, _Ty(&& B)[_Size])
{
	bool b = false;

	for (int i = 0; i < _Size; i++)
	{
		b = b && (A == B[i]);
	}
	return b;
}

// 批量逻辑与运算
template<typename _Ty, SIZE_T _Size>
inline bool _and(_Ty A, _Ty(&& B)[_Size])
{
	bool b = false;

	bool b = false;

	for (int i = 0; i < _Size; i++)
	{
		b = b || (A == B[i]);
	}
	return b;
}

#pragma endregion

#pragma region 自定义结构

// 选择范围
typedef struct SELECT_RANGE
{
	LONG Start;
	LONG End;
}*PSELECT_RANGE;

// 滚动区域
typedef struct SCROLL_RANGE
{
	LONG Max;
	LONG Min;
}*PSCROLL_RANGE;

// 日期结构
typedef struct DATE
{
	WORD Year;
	WORD Month;
	WORD Day;
}*PDATE;

// 时间结构
typedef struct TIME
{
	WORD Hour;
	WORD Minute;
	WORD Second;
	WORD Milliseconds;
}*PTIME;

// 日期时间结构
typedef union DATETIME
{
	struct
	{
		WORD Year;
		WORD Month;
		WORD Day;
		WORD Hour;
		WORD Minute;
		WORD Second;
		WORD Milliseconds;
	};
	struct
	{
		DATE Date;
		TIME Time;
	};

	DATETIME(DATE _Date, TIME _Time);

	DATETIME(WORD _Year = 0, WORD _Month = 0, WORD _Day = 0,
			 WORD _Hour = 0, WORD _Minute = 0, WORD _Second = 0,
			 WORD _Milliseconds = 0);
	
	DATETIME(const SYSTEMTIME& _SysTime);
	
	operator SYSTEMTIME()const;
	
}*PDATETIME;

#pragma endregion

#pragma region 大小写转换

// 转换为大写
void StrToUpper(char* const str);

// 转换为小写
void StrToLower(char* const str);

// 转换为大写
void StrToUpper(wchar_t* const str);

// 转换为小写
void StrToLower(wchar_t* const str);

#pragma endregion

_WGUI_END

#endif // !_WGUI_FRAMEWORK_
