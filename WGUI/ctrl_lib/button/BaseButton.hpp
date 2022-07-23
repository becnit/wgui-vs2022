#pragma once
#ifndef _WGUI_BASEBUTTON_
#define _WGUI_BASEBUTTON_

#include "../../core/BaseControl.hpp"

_WGUI_BEGIN

// 按钮基类部分实现
class BaseButtonAchieve
	: public BaseHAlignMode<BaseButtonAchieve>
	, public BaseVAlignMode<BaseButtonAchieve>
	, public Attri_Caption
{
public:
	inline BaseButtonAchieve(HWND& _hWnd)noexcept
		: BaseHAlignMode<BaseButtonAchieve>(_hWnd, PROPERTY_INIT(BaseButtonAchieve))
		, BaseVAlignMode<BaseButtonAchieve>(_hWnd, PROPERTY_INIT(BaseButtonAchieve))
		, Attri_Caption(_hWnd)
		, m_hWnd(_hWnd)
	{
		BaseHAlignMode::HorzAlignMode.value = AlignMode::Center;
		BaseVAlignMode::VertAlignMode.value = AlignMode::Center;
	}

	inline ~BaseButtonAchieve()noexcept
	{
		BaseHAlignMode::HorzAlignMode.value = AlignMode::Center;
		BaseVAlignMode::VertAlignMode.value = AlignMode::Center;
	}

	// 后台模拟点击
	inline void Click()const noexcept
	{
		::PostMessageW(m_hWnd, BM_CLICK, 0, 0);
	}

private:

	inline void _property_get(int _Symbol)noexcept
	{
		if (!::IsWindow(m_hWnd))
			return;

		if (_Symbol == BaseHAlignMode::HorzAlignMode.symbol)
		{
			// 获取横向对齐模式

			DWORD dwStyle = ::GetWindowLongW(m_hWnd, GWL_STYLE);

			if (dwStyle & BS_LEFT)
				BaseHAlignMode::HorzAlignMode.value = AlignMode::Near;
			
			else if (dwStyle & BS_RIGHT)
				BaseHAlignMode::HorzAlignMode.value = AlignMode::Far;
			
			else if (dwStyle & BS_CENTER)
				BaseHAlignMode::HorzAlignMode.value = AlignMode::Center;
		}
		else if (_Symbol == BaseVAlignMode::VertAlignMode.symbol)
		{
			// 获取纵向对齐模式

			DWORD dwStyle = ::GetWindowLongW(m_hWnd, GWL_STYLE);

			if (dwStyle & BS_TOP)
				BaseVAlignMode::VertAlignMode.value = AlignMode::Near;
			
			else if (dwStyle & BS_BOTTOM)
				BaseVAlignMode::VertAlignMode.value = AlignMode::Far;
			
			else if (dwStyle & BS_VCENTER)
				BaseVAlignMode::VertAlignMode.value = AlignMode::Center;
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!::IsWindow(m_hWnd))
			return true;

		if (_Symbol == BaseHAlignMode::HorzAlignMode.symbol)
		{
			// 设置横向对齐模式

			// 获取控件风格，并去掉已有对齐标记
			DWORD dwStyle = ::GetWindowLongW(m_hWnd, GWL_STYLE);
			
			// 清除已有对齐标记
			dwStyle &= ~BS_LEFT;
			dwStyle &= ~BS_RIGHT;

			// 重新设置对齐标记
			switch (BaseHAlignMode::HorzAlignMode.value)
			{
				case AlignMode::Near:
					dwStyle |= BS_LEFT;
					break;

				case AlignMode::Far:
					dwStyle |= BS_RIGHT;
					break;

				case AlignMode::Center:
					dwStyle |= BS_CENTER;
					break;
			}
			return ::SetWindowLongW(m_hWnd, GWL_STYLE, dwStyle);
		}
		else if (_Symbol == BaseVAlignMode::VertAlignMode.symbol)
		{
			// 设置纵向对齐模式

			// 获取控件风格，并去掉已有对齐标记
			DWORD dwStyle = ::GetWindowLongW(m_hWnd, GWL_STYLE);
			
			// 清除已有对齐标记
			dwStyle &= ~BS_TOP;
			dwStyle &= ~BS_BOTTOM;

			// 重新设置对齐标记
			switch (BaseVAlignMode::VertAlignMode.value)
			{
				case AlignMode::Near:
					dwStyle |= BS_TOP;
					break;

				case AlignMode::Far:
					dwStyle |= BS_BOTTOM;
					break;

				case AlignMode::Center:
					dwStyle |= BS_VCENTER;
					break;
			}
			return ::SetWindowLongW(m_hWnd, GWL_STYLE, dwStyle);
		}
		else return false;
		return true;
	}

private:
	HWND& m_hWnd;
};

// 按钮基类
template<class _ElemCtrl>
class BaseButton
	: public BaseControl<_ElemCtrl>
	, public BaseButtonAchieve
{
protected:
	inline BaseButton(DWORD _Style)noexcept
		: BaseControl<_ElemCtrl>(WC_BUTTONW)
		, BaseButtonAchieve(BaseWindow::m_hWnd)
	{
		BaseWindow::m_dwStyle |= _Style;
	}

public:
	virtual ~BaseButton()noexcept
	{
	}

	// 创建按钮
	virtual bool Create(const BaseWindow& _ParentWnd)noexcept
	{
		return BaseControl<_ElemCtrl>::Create(_ParentWnd, Caption);
	}

protected:
	
	// 控件创建前的初始化工作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl<_ElemCtrl>::_CreateBefore_())
			return false;

		// 初始化横向对齐模式
		switch (BaseHAlignMode::HorzAlignMode)
		{
			case AlignMode::Near:
				BaseWindow::m_dwStyle |= BS_LEFT;
				break;

			case AlignMode::Far:
				BaseWindow::m_dwStyle |= BS_RIGHT;
				break;

			case AlignMode::Center:
				BaseWindow::m_dwStyle |= BS_CENTER;
				break;
		}

		// 初始化纵向对齐模式
		switch (BaseVAlignMode::VertAlignMode)
		{
			case AlignMode::Near:
				BaseWindow::m_dwStyle |= BS_TOP;
				break;

			case AlignMode::Far:
				BaseWindow::m_dwStyle |= BS_BOTTOM;
				break;

			case AlignMode::Center:
				BaseWindow::m_dwStyle |= BS_VCENTER;
				break;
		}
		return true;
	}

	// 控件创建后的初始化工作
	virtual bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl<_ElemCtrl>::_CreateAfter_())
			return false;
		
		return true;
	}
};

_WGUI_END
#endif // !_WGUI_BASEBUTTON_
