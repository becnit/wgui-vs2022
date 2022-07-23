#pragma once
#ifndef _AUTO_PTR_
#define _AUTO_PTR_

// 自动指针
template<typename _Ty>
class _auto_ptr
{
public:
#pragma region 构造和析构

	// 不允许隐式调用该构造函数
	explicit inline _auto_ptr(_Ty* _Ptr = nullptr, size_t _Size = 0)noexcept
		: m_Ptr(nullptr)
		, m_Size(_Size)
	{
		if (m_Ptr == nullptr || m_Size == 0)
			m_Ptr = nullptr, m_Size = 0;
	}

	inline _auto_ptr(size_t _Size, _Ty _Init = _Ty())noexcept
		: _auto_ptr()
	{
		if (_Size)
		{
			m_Size = _Size;
			m_Ptr = new _Ty[m_Size]{ _Init };
		}
	}

	inline _auto_ptr(_auto_ptr& _right)noexcept
		: _auto_ptr()
	{
		_right.Swap(*this);
	}

	inline ~_auto_ptr()noexcept
	{
		if (m_Ptr)
			delete[] m_Ptr;
		m_Ptr = nullptr;
	}

#pragma endregion

#pragma region 公开函数

	// 交换指针
	inline void Swap(_auto_ptr& _right)noexcept
	{
		_auto_ptr temp;

		temp.m_Ptr = _right.m_Ptr;
		temp.m_Size = _right.m_Size;

		_right.m_Ptr = m_Ptr;
		_right.m_Size = m_Size;

		m_Ptr = temp.m_Ptr;
		m_Size = temp.m_Size;
	}

	// 返回指针
	inline _Ty* ptr()noexcept
	{
		return m_Ptr;
	}

	// 返回常指针
	inline const _Ty* ptr()const noexcept
	{
		return m_Ptr;
	}

	// 返回指针大小
	inline size_t size()const noexcept
	{
		return m_Size;
	}

	// 检查指针是否为nullptr
	inline bool Empty()const noexcept
	{
		return m_Ptr == nullptr;
	}

#pragma endregion

#pragma region 运算符重载

	// 隐式转换指针
	inline operator _Ty* ()noexcept
	{
		return m_Ptr;
	}

	// 隐式转换常指针
	inline operator const _Ty* ()const noexcept
	{
		return m_Ptr;
	}

	// 与另一个对象交换指针
	void operator=(_auto_ptr& _right)noexcept
	{
		_right.Swap(*this);
	}

	// 与另一个对象交换指针
	void operator=(_auto_ptr&& _right)noexcept
	{
		_right.Swap(*this);
	}
#pragma endregion

private:
	_Ty*	m_Ptr;
	size_t	m_Size;
};

#endif // !_AUTO_PTR_
