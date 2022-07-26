#pragma once
#ifndef _WGUI_WSTR_
#define _WGUI_WSTR_

#include <stdio.h>
#include "_property.hpp"

_WGUI_BEGIN

class WStr
{
	using WSTR_POS = unsigned int;
	using WSTR_SIZE = unsigned int;
	using WSTR_POS = unsigned int;

public:
#pragma region 构造函数与析构函数

	inline WStr(const wchar_t* _Src = nullptr)noexcept
		: WStr(_Src, DEF_COUNT)
	{}

	inline WStr(const wchar_t* _Src, WSTR_SIZE _Size)noexcept
		: m_Ptr(nullptr)
		, m_Size(0)
	{
		if (_Size)
			m_Size = _Size == DEF_COUNT ? (_Src != nullptr ? ::wcslen(_Src) : m_Increment) + 1 : _Size + 1;
		m_Ptr = new wchar_t[m_Size + m_Increment]{ 0 };
		if (_Src != nullptr)
			::wcsncat_s(m_Ptr, m_Size, _Src, _Size);
	}

	inline WStr(WSTR_SIZE _InitCapacity)noexcept
		: m_Ptr(nullptr)
		, m_Size(_InitCapacity + 1)
	{
		if (m_Size <= 1)
			m_Size = m_Increment;
		m_Ptr = new wchar_t[m_Size] {0};
	}

	inline WStr(WStr& _Right)noexcept
		: m_Ptr(_Right.m_Ptr)
		, m_Size(_Right.m_Size)
	{
		_Right.m_Ptr = nullptr;
		_Right.m_Size = 0;
	}

	// 兼容_property属性变量
	template<class _Elem>
	inline WStr(const _property<_Elem, WStr>& _Src)noexcept
		: WStr(_Src->CStr(), DEF_COUNT)
	{}
	

	inline ~WStr()noexcept
	{
		if (m_Ptr)
			delete[]m_Ptr;
		m_Ptr = nullptr;
		m_Size = 0;
	}

#pragma endregion

#pragma region 容量函数

	// 返回字符串长度
	inline WSTR_POS Length()const noexcept
	{
		return ::wcslen(m_Ptr);
	}

	// 获取当前对象分配的存储空间能保存的字符数量
	inline WSTR_SIZE Capacity()const noexcept
	{
		if (m_Ptr == nullptr)
			return 0;
		return m_Size - 1;
	}

	// 设置当前对象能最多保存字符的数量
	// @param _NewCap  新容量
	inline void SetCapacity(WSTR_SIZE _NewCap)noexcept
	{
		if (_NewCap == m_Size - 1)
			return;
		m_Size = _NewCap + 1;

		wchar_t* NewPtr = new wchar_t[m_Size]{ 0 };
		::wcscat_s(NewPtr, m_Size, m_Ptr);
		delete[]m_Ptr;
		m_Ptr = NewPtr;
	}

	// 重新设置字符串长度
	// @param  _NewLength  字符串的新的长度，如果小于字符串长度，则清除多于的字符
	inline void ResetLength(WSTR_POS _NewLength)noexcept
	{
		if (Empty() || Length() <= _NewLength)
			return;
		::memset(m_Ptr + _NewLength, 0, (Capacity() - _NewLength) * sizeof(wchar_t));
	}

	// 检查字符串是否为空
	inline bool Empty()const noexcept
	{
		if (m_Ptr != nullptr)
			return Length() == 0;
		return false;
	}

#pragma endregion

#pragma region 元素访问函数

	// 访问首字符
	inline wchar_t Front()const noexcept
	{
		if (Empty())
			return 0;
		return m_Ptr[0];
	}

	// 访问最后的字符
	inline wchar_t Back()const noexcept
	{
		if (Empty())
			return 0;
		return m_Ptr[Length() - 1];
	}

	// 隐式转换wchar_t*
	inline operator wchar_t* ()noexcept
	{
		return m_Ptr;
	}

	// 隐式转换const wchar_t*
	inline operator const wchar_t* ()const noexcept
	{
		return m_Ptr;
	}

	// 访问指定位置的字符
	inline wchar_t operator[](WSTR_POS _Index)const noexcept
	{
		if (Empty() || Length() <= _Index)
			return 0;
		return m_Ptr[_Index];
	}

	// 取出指定区域的字符串
	// @param _OffPos  开始位置
	// @param _Count   取出数量
	// @return 返回保存有指定区域字符串的对象
	inline WStr SubStr(WSTR_POS _OffPos, WSTR_SIZE _Count = DEF_COUNT)const noexcept
	{
		if (Empty() || _Count == 0)
			return WStr();
		if (_OffPos + _Count >= Length())
			return WStr();

		return WStr(m_Ptr + _OffPos, _Count);
	}

	// 返回C风格的字符串指针
	inline const wchar_t* CStr()const noexcept
	{
		return m_Ptr;
	}

	// 返回C风格的字符串指针
	inline wchar_t* Data()noexcept
	{
		return m_Ptr;
	}

#pragma endregion

#pragma region 操作函数

	// 清空所有字符
	// @param _Ch 填充的字符，默认为\0 
	inline void Clear(wchar_t _Ch = L'\0')noexcept
	{
		if (m_Ptr == nullptr)
			return;
		if (_Ch == L'\0')
			::memset(m_Ptr, 0, m_Size * sizeof(wchar_t));
		else
			for (WSTR_POS i = 0; i < m_Size; i++)
				m_Ptr[i] = _Ch;
	}

	// 插入指定位置字符串
	// @param _Index         插入内容到的位置
	// @param _InsetString   要插入的字符串
	// @param _Count         要插入的字符数
	inline WStr& Insert(WSTR_POS _Index, const wchar_t* _InsetString, WSTR_SIZE _Count = DEF_COUNT)noexcept
	{
		_Count = _Count == -1 ? ::wcslen(_InsetString) : _Count;

		if (_InsetString == nullptr)
			return *this;
		if (::wcslen(_InsetString) == 0 || _Count == 0)
			return *this;
		if (_Count > ::wcslen(_InsetString))
			return *this;

		wchar_t* temp = nullptr;
		WSTR_SIZE len = Length();

		if (Capacity() < len + _Count)
		{
			m_Size = len + _Count + m_Increment + 1;
			temp = new wchar_t[m_Size];
			::wcsncpy_s(temp, m_Size, m_Ptr + _Index, len);
			delete[]m_Ptr;
		}
		else
		{
			temp = m_Ptr;	
		}
		
		WSTR_SIZE bak_size = ::wcslen(temp) - _Index + 1;
		wchar_t* bak = new wchar_t[bak_size]{ 0 };
		
		::wcsncpy_s(bak, bak_size, m_Ptr + _Index, bak_size - 1);
		::wcsncpy_s(temp + _Index, Capacity() - _Index, _InsetString, _Count);
		::wcsncpy_s(temp + _Index + _Count, Capacity() - _Index - _Count, bak, ::wcslen(bak));

		m_Ptr = temp;
		delete[]bak;

		return *this;
	}

	// 在指定位置插入指定数量的字符
	// @param _Index         插入内容到的位置
	// @param _InsetString   要插入的字符
	// @param _Count         要插入的字符的数目
	inline WStr& Insert(WSTR_POS _Index, wchar_t _Ch, WSTR_SIZE _Count)noexcept
	{
		if (_Ch == 0 || _Count == 0)
			return *this;

		wchar_t* temp = new wchar_t[_Count + 1]{ _Ch };
		temp[_Count] = 0;
		Insert(_Index, temp, _Count);
		return *this;
	}

	// 移除字符
	// @param _Index   要移除的字符位置
	// @param _Count   要移除的字符数量
	inline WStr& Erase(WSTR_POS _Index, WSTR_SIZE _Count)noexcept
	{
		WSTR_SIZE len = Length();

		if (Empty() || _Index >= len || _Count == 0)
			return *this;

		WSTR_SIZE bak_size = len - _Index - _Count + 1;
		wchar_t* bak = new wchar_t[bak_size]{ 0 };

		::wcsncpy_s(bak, bak_size, m_Ptr + _Index + _Count, bak_size - 1);
		m_Ptr[_Index] = 0;
		::wcscat_s(m_Ptr, m_Size, bak);
		return *this;
	}

	// 替换字符串
	// @param  _Index   将被替换的子串起始位置
	// @param  _String  用于替换的字符串
	// @param  _Count   用于替换的字符串的长度
	inline WStr& Replace(WSTR_POS _Index, const wchar_t* _String, WSTR_SIZE _Count = DEF_COUNT)noexcept
	{
		return Replace(_Index, _Count, _String, _Count);
	}

	// 替换字符串
	// @param  _Index         将被替换的子串起始位置
	// @param  _ReplaceLen    将被替换的子串长度
	// @param  _String        用于替换的字符串
	// @param  _Count         用于替换的字符串的长度
	inline WStr& Replace(WSTR_POS _Index, WSTR_POS _ReplaceLen, const wchar_t* _String, WSTR_SIZE _Count = DEF_COUNT)noexcept
	{
		_Count = _Count == -1 ? ::wcslen(_String) : _Count;

		if (Empty() || _String == nullptr)
			return *this;
		if (::wcslen(_String) == 0 || _Count == 0)
			return *this;
		if (_Count > ::wcslen(_String))
			return *this;

		Erase(_Index, _ReplaceLen);
		return Insert(_Index, _String, _Count);
	}

	// 替换字符串
	// @param  _Index   将被替换的子串起始位置
	// @param  _Ch      用于替换的字符
	// @param  _Count   用于替换的字符的数量
	inline WStr& Replace(WSTR_POS _Index, wchar_t _Ch, WSTR_SIZE _Count = 1)noexcept
	{
		return Replace(_Index, _Count, _Ch, _Count);
	}

	// 替换字符串
	// @param  _Index         将被替换的子串起始位置
	// @param  _ReplaceLen    将被替换的子串长度
	// @param  _Ch            用于替换的字符
	// @param  _Count         用于替换的字符的数量
	inline WStr& Replace(WSTR_POS _Index, WSTR_POS _ReplaceLen, wchar_t _Ch, WSTR_SIZE _Count = 1)noexcept
	{
		if (_Count == 0)
			return *this;

		wchar_t* s = new wchar_t[_Count + 1]{ 0 };
		for (WSTR_POS i = 0; i <= _Count; i++)
		{
			s[i] = _Ch;
		}
		Replace(_Index, _ReplaceLen, s, _Count);
		delete[]s;
		return *this;
	}

	// 设置新字符串
	inline WStr& operator=(WStr& _Right)noexcept
	{
		operator=(_Right.m_Ptr);
		return *this;
	}

	// 设置新字符串
	inline WStr& operator=(const wchar_t* _NewString)noexcept
	{
		if (_NewString == nullptr)
			return *this;

		WSTR_SIZE cb = ::wcslen(_NewString);

		if (m_Size <= cb)
		{
			if (m_Ptr)
				delete[]m_Ptr;
			m_Ptr = nullptr;
		}

		if (m_Ptr == nullptr)
		{
			m_Size = cb + m_Increment;
			m_Ptr = new wchar_t[m_Size] {0};
		}
		Clear();

		::wcscat_s(m_Ptr, m_Size, _NewString);
		return *this;
	}

	// 增加字符串
	inline WStr& operator+=(const wchar_t* _AddString)noexcept
	{
		return Insert(Length(), _AddString);
	}

	// 增加字符
	inline WStr& operator+=( wchar_t _Add)noexcept
	{
		wchar_t s[2] = { 0 };
		s[0] = _Add;
		return Insert(Length(), s);
	}

	// 比较字符串
	inline int Compare(const wchar_t* _String)const noexcept
	{
		if (Empty() || _String == nullptr)
			return 0;
		return ::wcscmp(m_Ptr, _String);
	}

	// 检索两串字符串是否相同，模板参数指定是否不区分大小写
	inline bool operator==(const wchar_t* _Str)const noexcept
	{
		if (_Str == m_Ptr)
			return true;

		if (m_Ptr == nullptr || _Str == nullptr)
			return false;

		return 0 == ::wcscmp(m_Ptr, _Str);
	}

	// 检索两串字符串是否不相同，模板参数指定是否不区分大小写
	inline bool operator!=(const wchar_t* _Str)const noexcept
	{
		return !operator==(_Str);
	}

	// 交换数据
	inline void Swap(WStr& _Right)noexcept
	{
		auto ptr = _Right.m_Ptr;
		auto cb = _Right.m_Size;

		_Right.m_Ptr = m_Ptr;
		_Right.m_Size = m_Size;

		m_Ptr = ptr;
		m_Size = cb;
	}

#pragma endregion

#pragma region 查找函数

	// 寻找子串首次出现的位置
	// @param _FindStr  要搜索的子串
	// @param _Index    搜索开始的位置
	// @param _Count    要搜索的子串的长度
	// @returns 返回找到的子串首次出现的位置，若找不到子串则为NPOS
	inline WSTR_POS FindOfFirst(const wchar_t* _FindStr, WSTR_POS _Index = BEGIN_POS, WSTR_SIZE _Count = DEF_COUNT)const noexcept
	{
		if (Empty() || _FindStr == nullptr)
			return NPOS;

		_Count = _Count == -1 ? ::wcslen(_FindStr) : _Count;
		if (::wcslen(_FindStr) < _Count || _Count == 0)
			return NPOS;

		if (_Index + _Count > Length())
			return NPOS;

		wchar_t* Temp = nullptr;
		wchar_t* Find = nullptr;
		
		Temp = new wchar_t[_Count + 1]{ 0 };
		::wcsncpy_s(Temp, _Count + 1, _FindStr, _Count);
		Find = ::wcsstr(m_Ptr + _Index, Temp);
		delete[]Temp;
		return Find == nullptr ? NPOS : (WSTR_POS)(Find - m_Ptr);
	}

	// 寻找子串最后一次出现的位置
	// @param _FindStr  要搜索的子串
	// @param _Index    搜索开始的位置
	// @param _Count    要搜索的子串的长度
	// @returns 返回找到的子串最后一次出现的位置，若找不到子串则为NPOS
	inline WSTR_POS FindOfEnd(const wchar_t* _FindStr, WSTR_POS _Index = BEGIN_POS, WSTR_SIZE _Count = DEF_COUNT)const noexcept
	{
		WSTR_POS Find = NPOS;
		WSTR_POS Last = NPOS;

		while ((Find = FindOfFirst(_FindStr, Find, _Count)) != NPOS)
			Last = Find++;
		return Last;
	}

	// 寻找字符首次出现的位置
	// @param _FindChar          要搜索的字符
	// @param _OffPos            搜索的开始位置
	// @param _EndPos            搜索的结束位置
	// @param _NotCaseSensitive  不缺分大小写
	// @returns 返回找到的字符首次出现的位置，若找不到字符则为NPOS
	inline WSTR_POS FindOfFirst(wchar_t _Char, WSTR_POS _OffPos = BEGIN_POS, WSTR_POS _EndPos = END_POS, bool _NotCaseSensitive = false)const noexcept
	{
		WSTR_POS Len = Length();

		_EndPos = _EndPos == NPOS ? Len - 1 : _EndPos;
		if (Empty() || _Char == 0 || _OffPos == _EndPos || _OffPos == NPOS)
			return NPOS;

		if (_OffPos >= Len || _EndPos >= Len)
			return NPOS;

		for (WSTR_POS Index = _OffPos; Index <= _EndPos; ++Index)
		{
			if (_NotCaseSensitive ? (m_Ptr[Index] == _Char || m_Ptr[Index] == (_Char + 32) || m_Ptr[Index] == (_Char - 32)) : m_Ptr[Index] == _Char)
				return Index;
		}
		return NPOS;
	}

	// 寻找字符最后一次出现的位置
	// @param _FindChar          要搜索的字符
	// @param _OffPos            搜索的开始位置
	// @param _EndPos            搜索的结束位置
	// @param _NotCaseSensitive  不缺分大小写
	// @returns 返回找到的字符最后一次出现的位置，若找不到字符则为NPOS
	inline WSTR_POS FindOfEnd(wchar_t _Char, WSTR_POS _OffPos = BEGIN_POS, WSTR_POS _EndPos = NPOS, bool _NotCaseSensitive = false)const noexcept
	{
		WSTR_POS Len = Length();

		_EndPos = _EndPos == NPOS ? Len - 1 : _EndPos;
		if (Empty() || _Char == 0 || _OffPos == _EndPos || _OffPos == NPOS)
			return NPOS;

		if (_OffPos >= Len || _EndPos >= Len)
			return NPOS;

		for (WSTR_POS Index = _EndPos; Index >= _OffPos; --Index)
		{
			if (_NotCaseSensitive ? (m_Ptr[Index] == _Char || m_Ptr[Index] == (_Char + 32) || m_Ptr[Index] == (_Char - 32)) : m_Ptr[Index] == _Char)
				return Index;
		}
		return NPOS;
	}

	// 寻找子串首次不匹配的字符的位置
	// @param _FindStr  要搜索的子串
	// @param _Index    搜索开始的位置
	// @param _Count    要搜索的子串的长度
	// @returns 返回字符首次不匹配的字符的位置，若一致匹配则返回NPOS
	WSTR_POS FindFirstNotOf(const wchar_t* _FindStr, WSTR_POS _Index = BEGIN_POS, WSTR_SIZE _Count = DEF_COUNT)const noexcept
	{
		if (Empty() || _FindStr == nullptr)
			return NPOS;

		_Count = _Count == -1 ? ::wcslen(_FindStr) : _Count;
		if (::wcslen(_FindStr) < _Count || _Count == 0)
			return NPOS;

		if (_Index + _Count > Length())
			return NPOS;

		while (_Index + _Count < Length())
		{
			for (WSTR_POS Pos = _Index; Pos < _Index + _Count; ++Pos)
			{
				if (m_Ptr[Pos] != _FindStr[Pos - _Index])
					return Pos;
			}
			_Index += _Count;
		}
		return NPOS;
	}

	// 寻找字符首次不匹配的字符的位置
	// @param _Char              要搜索的字符
	// @param _OffPos            搜索开始的位置
	// @param _EndPos            要搜索的子串的长度
	// @param _NotCaseSensitive  不缺分大小写
	// @returns 返回字符首次不匹配的字符的位置，若一致匹配则返回NPOS
	WSTR_POS FindFirstNotOf(wchar_t _Char, WSTR_POS _OffPos = BEGIN_POS, WSTR_SIZE _EndPos = END_POS, bool _NotCaseSensitive = false)const noexcept
	{
		WSTR_POS Len = Length();

		_EndPos = _EndPos == NPOS ? Len - 1 : _EndPos;
		if (Empty() || _Char == 0 || _OffPos == _EndPos || _OffPos == NPOS)
			return NPOS;

		if (_OffPos >= Len || _EndPos >= Len)
			return NPOS;

		for (WSTR_POS Index = _OffPos; Index <= _EndPos; ++Index)
		{
			if (m_Ptr[Index] != _Char)
				return Index;
		}
		return NPOS;
	}

	// 寻找字符最后一次不匹配的字符的位置
	// @param _Char              要搜索的字符
	// @param _OffPos            搜索开始的位置
	// @param _EndPos            要搜索的子串的长度
	// @param _NotCaseSensitive  不缺分大小写
	// @returns 返回字符最后一次不匹配的字符的位置，若一致匹配则返回NPOS
	WSTR_POS FindEndNotOf(wchar_t _Char, WSTR_POS _OffPos = BEGIN_POS, WSTR_SIZE _EndPos = END_POS, bool _NotCaseSensitive = false)const noexcept
	{
		WSTR_POS Len = Length();

		_EndPos = _EndPos == NPOS ? Len - 1 : _EndPos;
		if (Empty() || _Char == 0 || _OffPos == _EndPos || _OffPos == NPOS)
			return NPOS;

		if (_OffPos >= Len || _EndPos >= Len)
			return NPOS;

		for (WSTR_POS Index = _EndPos; Index >= _OffPos; --Index)
		{
			if (_NotCaseSensitive ? (m_Ptr[Index] == _Char || m_Ptr[Index] == (_Char + 32) || m_Ptr[Index] == (_Char - 32)) : m_Ptr[Index] == _Char)
				return Index;
		}
		return NPOS;
	}

#pragma endregion

private:
	wchar_t* m_Ptr;
	unsigned int m_Size;
	static constexpr WSTR_POS m_Increment{ 64 };
public:
	static constexpr WSTR_POS NPOS{ 0xFFFFFFFFU };
	static constexpr WSTR_POS BEGIN_POS{ 0x00000000U };
	static constexpr WSTR_POS END_POS{ 0xFFFFFFFFU };
	static constexpr WSTR_POS DEF_COUNT{ 0xFFFFFFFFU };

};

#ifdef _IOSTREAM_

inline std::wostream& operator<<(std::wostream& _Os, const WStr& _WStr)noexcept
{
	_Os << _WStr.operator const wchar_t* ();
	return _Os;
}

inline std::wistream& operator<<(std::wistream& _Is, WStr& _WStr)noexcept
{
	_Is.get(_WStr, _WStr.Capacity());
	return _Is;
}

#endif // _IOSTREAM_

// 检查两个字符串是否相同
inline bool operator==(const WStr& _Left, const WStr& _Right)noexcept
{
	return ::wcscmp(_Left,_Right) == 0;
}

// 检查两个字符串是否不相同
inline bool operator!=(const WStr& _Left, const WStr& _Right)noexcept
{
	return ::wcscmp(_Left, _Right) != 0;
}



_WGUI_END

#endif // !_WGUI_WSTR_
