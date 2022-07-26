#include "../../Include/core/GeneralAttributes.hpp"

_WGUI_BEGIN

#pragma region 控件矩形类

CTRL_RECT::CTRL_RECT()noexcept
	: CTRL_RECT(0, 0, 0, 0)
{}

CTRL_RECT::CTRL_RECT(LONG _Left, LONG _Top, LONG _Width, LONG _Height)noexcept
	: RECT({ _Left, _Top, _Width, _Height })
	, Left(left)
	, Top(top)
	, Width(right)
	, Height(bottom)
{}

CTRL_RECT::CTRL_RECT(POINT _Point, SIZE _Size)noexcept
	: CTRL_RECT(_Point.x, _Point.y, _Size.cx, _Size.cy)
{}

CTRL_RECT::CTRL_RECT(RECT _Rect)noexcept
	: CTRL_RECT(_Rect.left, _Rect.top, _Rect.right, _Rect.bottom)
{}

CTRL_RECT::CTRL_RECT(const CTRL_RECT& _Right)noexcept
	: CTRL_RECT(_Right.left, _Right.top, _Right.right, _Right.bottom)
{}

RECT* CTRL_RECT::operator&()noexcept
{
	return this;
}

CTRL_RECT::operator RECT& ()noexcept
{
	return *this;
}

CTRL_RECT& CTRL_RECT::operator=(const CTRL_RECT& _Right)noexcept
{
	left = _Right.left;
	top = _Right.top;
	right = _Right.right;
	bottom = _Right.bottom;
	return *this;
}

#pragma endregion

#pragma region 通用属性类

Attri_General::Attri_General(HWND& hWnd)noexcept
	: m_hWnd(hWnd)
	, Rect(PROPERTY_INIT(Attri_General))
	, Visibled(PROPERTY_INIT(Attri_General), true)
	, Disabled(PROPERTY_INIT(Attri_General), false)
{}

Attri_General::~Attri_General()noexcept
{}

void Attri_General::_property_get(int _Symbol)noexcept
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
		// 更新窗口显示状态属性的值
		Visibled.value = (bool)::IsWindowVisible(m_hWnd);
	}
	else if (_Symbol == Disabled.symbol)
	{
		// 更新窗口禁用状态属性的值
		Disabled.value = !(bool)::IsWindowEnabled(m_hWnd);
	}

}

bool Attri_General::_property_set(int _Symbol)noexcept
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
		// 更新窗口显示状态属性的值
		return ::ShowWindow(m_hWnd, Visibled.value ? SW_SHOW : SW_HIDE);
	}
	else if (_Symbol == Disabled.symbol)
	{
		// 更新窗口禁用状态属性的值
		return ::EnableWindow(m_hWnd, !Disabled.value);
	}
	else return false;
	return true;
}

#pragma endregion

#pragma region 标题属性

Attri_Caption::Attri_Caption(HWND& hWnd)noexcept
	: m_hWnd(hWnd)
	, Caption(PROPERTY_INIT(Attri_Caption), WStr(260))
{}

Attri_Caption::~Attri_Caption()noexcept
{}

void Attri_Caption::Attri_Caption::_property_get(int _Symbol)noexcept
{
	if (::IsWindow(m_hWnd) == FALSE)
		return;

	if (_Symbol == Caption.symbol)
	{
		// 获取窗口标题长度
		DWORD len = ::GetWindowTextLengthW(m_hWnd) + 1;

		// 设置容量
		Caption.value.SetCapacity(len);

		// 更新窗口标题属性的值
		::GetWindowTextW(m_hWnd, Caption.value, Caption.value.Capacity());
	}
}

bool Attri_Caption::_property_set(int _Symbol)noexcept
{
	if (::IsWindow(m_hWnd) == FALSE)
		return true;

	if (_Symbol == Caption.symbol)
	{
		// 更新窗口标题属性的值
		return ::SetWindowTextW(m_hWnd, Caption.value);
	}
	else return false;
}

#pragma endregion

_WGUI_END