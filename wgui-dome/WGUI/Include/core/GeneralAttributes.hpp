#pragma once
#ifndef _WGUI_GENERIC_ATTRIBUTES_
#define _WGUI_GENERIC_ATTRIBUTES_

#include "Framework.hpp"

_WGUI_BEGIN

// 控件矩形类
class CTRL_RECT
	: protected RECT
{
public:
	CTRL_RECT()noexcept;

	CTRL_RECT(LONG _Left, LONG _Top, LONG _Width, LONG _Height)noexcept;

	CTRL_RECT(POINT _Point, SIZE _Size)noexcept;

	CTRL_RECT(RECT _Rect)noexcept;

	CTRL_RECT(const CTRL_RECT& _Right)noexcept;

	RECT* operator&()noexcept;
	
	operator RECT& ()noexcept;
	
	CTRL_RECT& operator=(const CTRL_RECT& _Right)noexcept;
	
public:
	LONG& Left;
	LONG& Top;
	LONG& Width;
	LONG& Height;
};

/**************************************************************/

// 通用属性
class Attri_General
{
protected:
	Attri_General(HWND& hWnd)noexcept;

	~Attri_General()noexcept;
	
private:
	void _property_get(int _Symbol)noexcept;

	bool _property_set(int _Symbol)noexcept;

public:
	// 本属性指定控件在父控件的位置及尺寸
	_property<Attri_General, CTRL_RECT> Rect;

	// 本属性指定控件的可视状态。
	_property<Attri_General, bool> Visibled;

	// 本属性指定控件的可用状态。
	_property<Attri_General, bool> Disabled;

private:
	HWND& m_hWnd;
};


// 标题属性
class Attri_Caption
{
protected:
	Attri_Caption(HWND& hWnd)noexcept;

	~Attri_Caption()noexcept;

private:
	void _property_get(int _Symbol)noexcept;

	bool _property_set(int _Symbol)noexcept;
	
public:
	// 本属性指定控件的标题
	_property<Attri_Caption, WStr> Caption;
	
private:
	HWND& m_hWnd;
};

_WGUI_END

#endif // !_WGUI_GENERIC_ATTRIBUTES_
