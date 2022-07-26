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
#pragma region ���캯������������

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

	// ����_property���Ա���
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

#pragma region ��������

	// �����ַ�������
	inline WSTR_POS Length()const noexcept
	{
		return ::wcslen(m_Ptr);
	}

	// ��ȡ��ǰ�������Ĵ洢�ռ��ܱ�����ַ�����
	inline WSTR_SIZE Capacity()const noexcept
	{
		if (m_Ptr == nullptr)
			return 0;
		return m_Size - 1;
	}

	// ���õ�ǰ��������ౣ���ַ�������
	// @param _NewCap  ������
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

	// ���������ַ�������
	// @param  _NewLength  �ַ������µĳ��ȣ����С���ַ������ȣ���������ڵ��ַ�
	inline void ResetLength(WSTR_POS _NewLength)noexcept
	{
		if (Empty() || Length() <= _NewLength)
			return;
		::memset(m_Ptr + _NewLength, 0, (Capacity() - _NewLength) * sizeof(wchar_t));
	}

	// ����ַ����Ƿ�Ϊ��
	inline bool Empty()const noexcept
	{
		if (m_Ptr != nullptr)
			return Length() == 0;
		return false;
	}

#pragma endregion

#pragma region Ԫ�ط��ʺ���

	// �������ַ�
	inline wchar_t Front()const noexcept
	{
		if (Empty())
			return 0;
		return m_Ptr[0];
	}

	// ���������ַ�
	inline wchar_t Back()const noexcept
	{
		if (Empty())
			return 0;
		return m_Ptr[Length() - 1];
	}

	// ��ʽת��wchar_t*
	inline operator wchar_t* ()noexcept
	{
		return m_Ptr;
	}

	// ��ʽת��const wchar_t*
	inline operator const wchar_t* ()const noexcept
	{
		return m_Ptr;
	}

	// ����ָ��λ�õ��ַ�
	inline wchar_t operator[](WSTR_POS _Index)const noexcept
	{
		if (Empty() || Length() <= _Index)
			return 0;
		return m_Ptr[_Index];
	}

	// ȡ��ָ��������ַ���
	// @param _OffPos  ��ʼλ��
	// @param _Count   ȡ������
	// @return ���ر�����ָ�������ַ����Ķ���
	inline WStr SubStr(WSTR_POS _OffPos, WSTR_SIZE _Count = DEF_COUNT)const noexcept
	{
		if (Empty() || _Count == 0)
			return WStr();
		if (_OffPos + _Count >= Length())
			return WStr();

		return WStr(m_Ptr + _OffPos, _Count);
	}

	// ����C�����ַ���ָ��
	inline const wchar_t* CStr()const noexcept
	{
		return m_Ptr;
	}

	// ����C�����ַ���ָ��
	inline wchar_t* Data()noexcept
	{
		return m_Ptr;
	}

#pragma endregion

#pragma region ��������

	// ��������ַ�
	// @param _Ch �����ַ���Ĭ��Ϊ\0 
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

	// ����ָ��λ���ַ���
	// @param _Index         �������ݵ���λ��
	// @param _InsetString   Ҫ������ַ���
	// @param _Count         Ҫ������ַ���
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

	// ��ָ��λ�ò���ָ���������ַ�
	// @param _Index         �������ݵ���λ��
	// @param _InsetString   Ҫ������ַ�
	// @param _Count         Ҫ������ַ�����Ŀ
	inline WStr& Insert(WSTR_POS _Index, wchar_t _Ch, WSTR_SIZE _Count)noexcept
	{
		if (_Ch == 0 || _Count == 0)
			return *this;

		wchar_t* temp = new wchar_t[_Count + 1]{ _Ch };
		temp[_Count] = 0;
		Insert(_Index, temp, _Count);
		return *this;
	}

	// �Ƴ��ַ�
	// @param _Index   Ҫ�Ƴ����ַ�λ��
	// @param _Count   Ҫ�Ƴ����ַ�����
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

	// �滻�ַ���
	// @param  _Index   �����滻���Ӵ���ʼλ��
	// @param  _String  �����滻���ַ���
	// @param  _Count   �����滻���ַ����ĳ���
	inline WStr& Replace(WSTR_POS _Index, const wchar_t* _String, WSTR_SIZE _Count = DEF_COUNT)noexcept
	{
		return Replace(_Index, _Count, _String, _Count);
	}

	// �滻�ַ���
	// @param  _Index         �����滻���Ӵ���ʼλ��
	// @param  _ReplaceLen    �����滻���Ӵ�����
	// @param  _String        �����滻���ַ���
	// @param  _Count         �����滻���ַ����ĳ���
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

	// �滻�ַ���
	// @param  _Index   �����滻���Ӵ���ʼλ��
	// @param  _Ch      �����滻���ַ�
	// @param  _Count   �����滻���ַ�������
	inline WStr& Replace(WSTR_POS _Index, wchar_t _Ch, WSTR_SIZE _Count = 1)noexcept
	{
		return Replace(_Index, _Count, _Ch, _Count);
	}

	// �滻�ַ���
	// @param  _Index         �����滻���Ӵ���ʼλ��
	// @param  _ReplaceLen    �����滻���Ӵ�����
	// @param  _Ch            �����滻���ַ�
	// @param  _Count         �����滻���ַ�������
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

	// �������ַ���
	inline WStr& operator=(WStr& _Right)noexcept
	{
		operator=(_Right.m_Ptr);
		return *this;
	}

	// �������ַ���
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

	// �����ַ���
	inline WStr& operator+=(const wchar_t* _AddString)noexcept
	{
		return Insert(Length(), _AddString);
	}

	// �����ַ�
	inline WStr& operator+=( wchar_t _Add)noexcept
	{
		wchar_t s[2] = { 0 };
		s[0] = _Add;
		return Insert(Length(), s);
	}

	// �Ƚ��ַ���
	inline int Compare(const wchar_t* _String)const noexcept
	{
		if (Empty() || _String == nullptr)
			return 0;
		return ::wcscmp(m_Ptr, _String);
	}

	// ���������ַ����Ƿ���ͬ��ģ�����ָ���Ƿ����ִ�Сд
	inline bool operator==(const wchar_t* _Str)const noexcept
	{
		if (_Str == m_Ptr)
			return true;

		if (m_Ptr == nullptr || _Str == nullptr)
			return false;

		return 0 == ::wcscmp(m_Ptr, _Str);
	}

	// ���������ַ����Ƿ���ͬ��ģ�����ָ���Ƿ����ִ�Сд
	inline bool operator!=(const wchar_t* _Str)const noexcept
	{
		return !operator==(_Str);
	}

	// ��������
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

#pragma region ���Һ���

	// Ѱ���Ӵ��״γ��ֵ�λ��
	// @param _FindStr  Ҫ�������Ӵ�
	// @param _Index    ������ʼ��λ��
	// @param _Count    Ҫ�������Ӵ��ĳ���
	// @returns �����ҵ����Ӵ��״γ��ֵ�λ�ã����Ҳ����Ӵ���ΪNPOS
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

	// Ѱ���Ӵ����һ�γ��ֵ�λ��
	// @param _FindStr  Ҫ�������Ӵ�
	// @param _Index    ������ʼ��λ��
	// @param _Count    Ҫ�������Ӵ��ĳ���
	// @returns �����ҵ����Ӵ����һ�γ��ֵ�λ�ã����Ҳ����Ӵ���ΪNPOS
	inline WSTR_POS FindOfEnd(const wchar_t* _FindStr, WSTR_POS _Index = BEGIN_POS, WSTR_SIZE _Count = DEF_COUNT)const noexcept
	{
		WSTR_POS Find = NPOS;
		WSTR_POS Last = NPOS;

		while ((Find = FindOfFirst(_FindStr, Find, _Count)) != NPOS)
			Last = Find++;
		return Last;
	}

	// Ѱ���ַ��״γ��ֵ�λ��
	// @param _FindChar          Ҫ�������ַ�
	// @param _OffPos            �����Ŀ�ʼλ��
	// @param _EndPos            �����Ľ���λ��
	// @param _NotCaseSensitive  ��ȱ�ִ�Сд
	// @returns �����ҵ����ַ��״γ��ֵ�λ�ã����Ҳ����ַ���ΪNPOS
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

	// Ѱ���ַ����һ�γ��ֵ�λ��
	// @param _FindChar          Ҫ�������ַ�
	// @param _OffPos            �����Ŀ�ʼλ��
	// @param _EndPos            �����Ľ���λ��
	// @param _NotCaseSensitive  ��ȱ�ִ�Сд
	// @returns �����ҵ����ַ����һ�γ��ֵ�λ�ã����Ҳ����ַ���ΪNPOS
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

	// Ѱ���Ӵ��״β�ƥ����ַ���λ��
	// @param _FindStr  Ҫ�������Ӵ�
	// @param _Index    ������ʼ��λ��
	// @param _Count    Ҫ�������Ӵ��ĳ���
	// @returns �����ַ��״β�ƥ����ַ���λ�ã���һ��ƥ���򷵻�NPOS
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

	// Ѱ���ַ��״β�ƥ����ַ���λ��
	// @param _Char              Ҫ�������ַ�
	// @param _OffPos            ������ʼ��λ��
	// @param _EndPos            Ҫ�������Ӵ��ĳ���
	// @param _NotCaseSensitive  ��ȱ�ִ�Сд
	// @returns �����ַ��״β�ƥ����ַ���λ�ã���һ��ƥ���򷵻�NPOS
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

	// Ѱ���ַ����һ�β�ƥ����ַ���λ��
	// @param _Char              Ҫ�������ַ�
	// @param _OffPos            ������ʼ��λ��
	// @param _EndPos            Ҫ�������Ӵ��ĳ���
	// @param _NotCaseSensitive  ��ȱ�ִ�Сд
	// @returns �����ַ����һ�β�ƥ����ַ���λ�ã���һ��ƥ���򷵻�NPOS
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

// ��������ַ����Ƿ���ͬ
inline bool operator==(const WStr& _Left, const WStr& _Right)noexcept
{
	return ::wcscmp(_Left,_Right) == 0;
}

// ��������ַ����Ƿ���ͬ
inline bool operator!=(const WStr& _Left, const WStr& _Right)noexcept
{
	return ::wcscmp(_Left, _Right) != 0;
}



_WGUI_END

#endif // !_WGUI_WSTR_
