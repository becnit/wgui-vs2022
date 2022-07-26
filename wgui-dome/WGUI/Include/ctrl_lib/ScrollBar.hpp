#pragma once
#ifndef _WGUI_SCROLL_BAR_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

enum class ScrollBarType
{
	Horz = 0x0000,	// 水平滚动条
	Vert = 0x0001	// 垂直滚动条
};

// 滚动条
class ScrollBar
	: public BaseControl<ScrollBar>
{
public:
	inline ScrollBar(ScrollBarType _Type)noexcept
		: BaseControl<ScrollBar>(WC_SCROLLBARW)
		, CurPos(PROPERTY_INIT(ScrollBar), 0)
		, MinPos(PROPERTY_INIT(ScrollBar), 0)
		, MaxPos(PROPERTY_INIT(ScrollBar), 100)
		, ShowArrows(PROPERTY_INIT(ScrollBar), true)
	{
		BaseWindow::m_dwStyle |= (DWORD)_Type;
	}

	inline ~ScrollBar()noexcept
	{}

	virtual bool Create(const BaseWindow& _ParentWnd)noexcept
	{
		return BaseControl::Create(_ParentWnd);
	}

protected:
	// 创建之前的初始化操作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数
		if (!BaseControl::_CreateBefore_())
			return false;
		
		return true;
	}

	// 创建之后的工作初始化操作
	virtual bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数
		if (!BaseControl::_CreateAfter_())
			return false;

		// 指定滚动条的初始位置、最小值和最大值
		SCROLLINFO Info = { 0 };
		
		Info.cbSize = sizeof(Info);
		Info.fMask = SIF_POS | SIF_RANGE;
		Info.nMin = MinPos;
		Info.nMax = MaxPos;
		Info.nPos = CurPos;
		SetScrollInfo(BaseWindow::m_hWnd, SB_CTL, &Info, true);
		
		return true;
	}

private:
	
	inline void _property_get(int _Symbol)noexcept
	{
		if (!BaseWindow::IsCreate())
			return;

		if (_or(_Symbol, { CurPos.symbol, MinPos.symbol, MaxPos.symbol }))
		{
			SCROLLINFO Info = { 0 };

			Info.cbSize = sizeof(Info);
			::GetScrollInfo(BaseWindow::m_hWnd, SB_CTL, &Info);

			CurPos.value = Info.nPos;
			MinPos.value = Info.nMin;
			MaxPos.value = Info.nMax;
		}
		else if (_Symbol == ShowArrows.symbol)
		{
			/*
			SBM_ENABLE_ARROWS		启用或禁用滚动条控件的一个或两个箭头

			ESB_DISABLE_BOTH		禁用滚动条上的两个箭头
			ESB_ENABLE_BOTH			启用滚动条上的两个箭头

			ESB_DISABLE_LEFT		禁用水平滚动条上的向左箭头
			ESB_DISABLE_RIGHT		禁用水平滚动条上的向右箭头

			ESB_DISABLE_UP			禁用垂直滚动条上的向上箭头
			ESB_DISABLE_DOWN		禁用垂直滚动条上的向下箭头

			ESB_DISABLE_LTUP		禁用水平滚动条上的向左箭头或垂直滚动条上的向上箭头
			ESB_DISABLE_RTDN		禁用水平滚动条上的向右箭头或垂直滚动条上的向下箭头
			*/
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!BaseWindow::IsCreate())
			return true;

		if (_Symbol == CurPos.symbol)
		{
			WindowAPI::SendMsg(SBM_SETPOS, CurPos.value, TRUE);
		}
		else if (_Symbol == MinPos.symbol)
		{
			WindowAPI::SendMsg(SBM_SETRANGE, 0, &MaxPos.value);
			WindowAPI::SendMsg(SBM_SETRANGEREDRAW, MinPos.value, MaxPos.value);
		}
		else if (_Symbol == MaxPos.symbol)
		{
			WindowAPI::SendMsg(SBM_SETRANGE, &MinPos.value);
			WindowAPI::SendMsg(SBM_SETRANGEREDRAW, MinPos.value, MaxPos.value);
		}
		else if (_Symbol == ShowArrows.symbol)
		{
			/*
			SBM_ENABLE_ARROWS		启用或禁用滚动条控件的一个或两个箭头

			ESB_DISABLE_BOTH		禁用滚动条上的两个箭头
			ESB_ENABLE_BOTH			启用滚动条上的两个箭头

			ESB_DISABLE_LEFT		禁用水平滚动条上的向左箭头
			ESB_DISABLE_RIGHT		禁用水平滚动条上的向右箭头

			ESB_DISABLE_UP			禁用垂直滚动条上的向上箭头
			ESB_DISABLE_DOWN		禁用垂直滚动条上的向下箭头

			ESB_DISABLE_LTUP		禁用水平滚动条上的向左箭头或垂直滚动条上的向上箭头
			ESB_DISABLE_RTDN		禁用水平滚动条上的向右箭头或垂直滚动条上的向下箭头
			*/
			
			return  (bool)WindowAPI::SendMsg(SBM_ENABLE_ARROWS,
											 ShowArrows.value ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
		}
		return false;
	}
public:
	// 本属性指定滚动框的位置
	_property<ScrollBar, INT>		CurPos;

	// 本属性指定滚动的最小值
	_property<ScrollBar, INT>		MinPos;

	// 本属性指定滚动的最大值
	_property<ScrollBar, INT>		MaxPos;

	// 本属性指定是否显示滚动条上的箭头
	_property<ScrollBar, bool>		ShowArrows;
};

_WGUI_END

#endif // !_WGUI_SCROLL_BAR_
