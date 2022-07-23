#pragma once
#ifndef _PROPERTY_
#define _PROPERTY_

#define __property(get_func, set_func) __declspec(property(get = get_func, put = set_func))

#define PROPERTY_INIT(_Elem) this, &##_Elem##::_property_get, &##_Elem##::_property_set

// ���Ա������ı�ʶ��
class _property_symbol
{
protected:
	inline _property_symbol()noexcept
		: symbol(m_basic_symbol++)
	{}

	const int symbol;	// ���Ա�����ʶ��

private:
	// ���Ա������������ʶ��
	static int m_basic_symbol;
};
int _property_symbol::m_basic_symbol = 0;




// ���Ա������
template<class _Elem, typename _Ty>
class _property final
	: protected _property_symbol
{
	friend _Elem;
	using FN_GET = void(_Elem::*)(int);
	using FN_SET = bool(_Elem::*)(int);
	
public:
	inline _property(_Elem* _This,
					 FN_GET _Get,
					 FN_SET _Set,			 
					 _Ty _InitVal = _Ty())noexcept
		: _property_symbol()
		, value(_InitVal)
		, cls_this(_This)
		, fn_get(_Get)
		, fn_set(_Set)
	{
	}

	inline ~_property()noexcept
	{}

	// ��ȡֵ
	inline operator const _Ty&()const noexcept
	{
		(cls_this->*fn_get)(symbol);	// ֪ͨ��ȡʵʱֵ
		return value;
	}

	// ������ֵ
	inline _property& operator=(_Ty _Val)noexcept
	{
		_Ty old_val = value;

		value = _Val;
		if (!(cls_this->*fn_set)(symbol))	// ֪ͨ������ֵ
			value = old_val;	// ������ֵʧ�ܣ��ָ���ֵ
		return *this;
	}

	// ������ֵ
	inline _property& operator=(const _property& _Val)noexcept
	{
		return operator=(_Val.operator const _Ty & ());
	}

	// ��ȡֵ�ĳ�ָ��
	inline const _Ty* operator->()const noexcept
	{
		(cls_this->*fn_get)(symbol);	// ֪ͨ��ȡʵʱֵ
		return &value;
	}

	// �������ⲿ��ȡָ��
	inline _Ty* operator&() = delete;

private:
	_Ty				value;		// ����ֵ
	_Elem* const	cls_this;	// ����������ָ��

	FN_GET const fn_get;
	FN_SET const fn_set;
	
};

// ����iostream
#if defined(_IOSTREAM_)

template<class _Elem, typename _Ty>
inline std::wostream& operator<<(std::wostream& _Os, const _property<_Elem, _Ty>& _Val)
{
	_Os << _Val.operator const _Ty&();
	return _Os;
}

template<class _Elem, typename _Ty>
inline std::wistream& operator>>(std::wistream& _Is, _property<_Elem, _Ty>& _Val)
{
	_Is >> _Val.operator _Ty&();
	return _Is;
}

#endif // !_IOSTREAM_

#endif // !_PROPERTY_
