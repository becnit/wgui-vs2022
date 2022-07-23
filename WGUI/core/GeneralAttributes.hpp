#pragma once
#ifndef _WGUI_GENERIC_ATTRIBUTES_
#define _WGUI_GENERIC_ATTRIBUTES_

#include "Framework.hpp"

_WGUI_BEGIN

class WNDRECT
	: protected RECT
{
public:
	inline WNDRECT()noexcept
		: WNDRECT(0, 0, 0, 0)
	{}

	inline WNDRECT(LONG _Left, LONG _Top, LONG _Width, LONG _Height)noexcept
		: RECT({ _Left, _Top, _Width, _Height })
		, Left(left)
		, Top(top)
		, Width(right)
		, Height(bottom)
	{}

	inline WNDRECT(POINT _Point, SIZE _Size)noexcept
		: WNDRECT(_Point.x, _Point.y, _Size.cx, _Size.cy)
	{}

	inline WNDRECT(RECT _Rect)noexcept
		: WNDRECT(_Rect.left, _Rect.top, _Rect.right, _Rect.bottom)
	{}

	inline WNDRECT(const WNDRECT& _Right)noexcept
		: WNDRECT(_Right.left, _Right.top, _Right.right, _Right.bottom)
	{}

	inline RECT* operator&()noexcept
	{
		return this;
	}
	
	inline operator RECT&()noexcept
	{
		return *this;
	}
	
	inline WNDRECT& operator=(const WNDRECT& _Right)noexcept
	{
		left = _Right.left;
		top = _Right.top;
		right = _Right.right;
		bottom = _Right.bottom;
		return *this;
	}
	
public:
	LONG& Left;
	LONG& Top;
	LONG& Width;
	LONG& Height;
};

/**************************************************************/

// ͨ������
class Attri_General
{
protected:
	inline Attri_General(HWND& hWnd)noexcept
		: m_hWnd(hWnd)
		, Rect(PROPERTY_INIT(Attri_General))
		, Visibled(PROPERTY_INIT(Attri_General), true)
		, Disabled(PROPERTY_INIT(Attri_General), false)
	{}

	inline ~Attri_General()noexcept
	{}
	
private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (::IsWindow(m_hWnd) == FALSE)
			return;
		if (_Symbol == Rect.symbol)
		{
			::GetWindowRect(m_hWnd, &Rect.value);

			Rect.value.Width -= Rect.value.Left;
			Rect.value.Height -= Rect.value.Top;
		}
		else if (_Symbol == Visibled.symbol)
		{
			// ���´�����ʾ״̬���Ե�ֵ
			Visibled.value = (bool)::IsWindowVisible(m_hWnd);
		}
		else if (_Symbol == Disabled.symbol)
		{
			// ���´��ڽ���״̬���Ե�ֵ
			Disabled.value = !(bool)::IsWindowEnabled(m_hWnd);
		}

	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (::IsWindow(m_hWnd) == FALSE)
			return true;

		if (_Symbol == Rect.symbol)
		{
			return ::MoveWindow(m_hWnd, 
								Rect.value.Left, 
								Rect.value.Top, 
								Rect.value.Width, 
								Rect.value.Height, 
								TRUE);
		}
		else if (_Symbol == Visibled.symbol)
		{
			// ���´�����ʾ״̬���Ե�ֵ
			return ::ShowWindow(m_hWnd, Visibled.value ? SW_SHOW : SW_HIDE);
		}
		else if (_Symbol == Disabled.symbol)
		{
			// ���´��ڽ���״̬���Ե�ֵ
			return ::EnableWindow(m_hWnd, !Disabled.value);
		}
		else return false;
		return true;
	}

public:
	// ������ָ���ؼ��ڸ��ؼ���λ�ü��ߴ�
	_property<Attri_General, WNDRECT> Rect;

	// ������ָ���ؼ��Ŀ���״̬��
	_property<Attri_General, bool> Visibled;

	// ������ָ���ؼ��Ŀ���״̬��
	_property<Attri_General, bool> Disabled;

private:
	HWND& m_hWnd;
};


// ��������
class Attri_Caption
{
protected:
	inline Attri_Caption(HWND& hWnd)noexcept
		: m_hWnd(hWnd)
		, Caption(PROPERTY_INIT(Attri_Caption), WStr(260))
	{}

	inline ~Attri_Caption()noexcept
	{}

private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (::IsWindow(m_hWnd) == FALSE)
			return;

		if (_Symbol == Caption.symbol)
		{
			// ��ȡ���ڱ��ⳤ��
			DWORD len = ::GetWindowTextLengthW(m_hWnd) + 1;

			// ��������
			Caption.value.SetCapacity(len);
				
			// ���´��ڱ������Ե�ֵ
			::GetWindowTextW(m_hWnd, Caption.value, Caption.value.Capacity());
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (::IsWindow(m_hWnd) == FALSE)
			return true;

		if (_Symbol == Caption.symbol)
		{
			// ���´��ڱ������Ե�ֵ
			return ::SetWindowTextW(m_hWnd, Caption.value);
		}
		else return false;
	}
	
public:
	// ������ָ���ؼ��ı���
	_property<Attri_Caption, WStr> Caption;
	
private:
	HWND& m_hWnd;
};

_WGUI_END

#endif // !_WGUI_GENERIC_ATTRIBUTES_
