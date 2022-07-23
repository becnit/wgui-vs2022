#pragma once
#ifndef _PROPERTY_
#define _PROPERTY_

#define __property(get_func, set_func) __declspec(property(get = get_func, put = set_func))

#define PROPERTY_INIT(_Elem) this, &##_Elem##::_property_get, &##_Elem##::_property_set

// 类成员属性类的标识符
class _property_symbol
{
protected:
	inline _property_symbol()noexcept
		: symbol(m_basic_symbol++)
	{}

	const int symbol;	// 属性变量标识符

private:
	// 类成员属性类自增标识符
	static int m_basic_symbol;
};
int _property_symbol::m_basic_symbol = 0;




// 类成员属性类
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

	// 获取值
	inline operator const _Ty&()const noexcept
	{
		(cls_this->*fn_get)(symbol);	// 通知获取实时值
		return value;
	}

	// 设置新值
	inline _property& operator=(_Ty _Val)noexcept
	{
		_Ty old_val = value;

		value = _Val;
		if (!(cls_this->*fn_set)(symbol))	// 通知设置新值
			value = old_val;	// 设置新值失败，恢复旧值
		return *this;
	}

	// 设置新值
	inline _property& operator=(const _property& _Val)noexcept
	{
		return operator=(_Val.operator const _Ty & ());
	}

	// 获取值的常指针
	inline const _Ty* operator->()const noexcept
	{
		(cls_this->*fn_get)(symbol);	// 通知获取实时值
		return &value;
	}

	// 不允许外部获取指针
	inline _Ty* operator&() = delete;

private:
	_Ty				value;		// 属性值
	_Elem* const	cls_this;	// 属性所属类指针

	FN_GET const fn_get;
	FN_SET const fn_set;
	
};

// 兼容iostream
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
