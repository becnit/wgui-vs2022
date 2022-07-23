#pragma once
#ifndef _WGUI_TEXTBOX_
#define _WGUI_TEXTBOX_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

// 文本框
class TextBox
	: public BaseControl<TextBox>
	, public Attri_Caption
	, public BaseHAlignMode<TextBox>
	, public BaseVAlignMode<TextBox>
{
public:
	inline TextBox()noexcept
		: BaseControl<TextBox>(WC_STATICW)
		, Attri_Caption(BaseWindow::m_hWnd)
		, BaseHAlignMode<TextBox>(BaseWindow::m_hWnd, PROPERTY_INIT(TextBox))
		, BaseVAlignMode<TextBox>(BaseWindow::m_hWnd, PROPERTY_INIT(TextBox))
		, BorderStyle(PROPERTY_INIT(TextBox))
		, TextShowMode(PROPERTY_INIT(TextBox))
	{
		BaseHAlignMode::HorzAlignMode.value = AlignMode::Center;
		BaseVAlignMode::VertAlignMode.value = AlignMode::Center;
	}

	virtual ~TextBox()noexcept
	{}

	virtual bool Create(const BaseWindow& _ParantWnd)noexcept
	{
		return BaseControl::Create(_ParantWnd, WStr(Attri_Caption::Caption));
	}

protected:
	// 控件创建前的初始化工作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl::_CreateBefore_())
			return false;

		// 指定横向对齐方式
		switch (BaseHAlignMode::HorzAlignMode.value)
		{
			case AlignMode::Near:
				BaseWindow::m_dwStyle |= SS_LEFT;
				break;

			case AlignMode::Far:
				BaseWindow::m_dwStyle |= SS_RIGHT;
				break;

			case AlignMode::Center:
				BaseWindow::m_dwStyle |= SS_CENTER;
				break;
		}

		// 指定纵向对齐方式
		switch (BaseVAlignMode::VertAlignMode.value)
		{
			case AlignMode::Near:
				break;

			case AlignMode::Far:
				// 目前无效
				break;

			case AlignMode::Center:
				BaseWindow::m_dwStyle |= SS_CENTERIMAGE;
				break;
		}

		// 指定边框样式
		switch (BorderStyle.value)
		{
			case 1:
				BaseWindow::m_dwExStyle |= WS_EX_CLIENTEDGE;
				break;
			
			case 2:
				BaseWindow::m_dwExStyle |= WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE;
				break;

			case 3:
				BaseWindow::m_dwStyle |= SS_SUNKEN;
				break;

			case 4:
				BaseWindow::m_dwStyle |= WS_BORDER;
				break;

			default:
				break;
		}
		
		// 指标题显示模式
		switch (TextShowMode.value)
		{
			case 0:
				BaseWindow::m_dwStyle |= SS_SIMPLE;
				break;
				
			case 1:
				BaseWindow::m_dwStyle |= SS_ENDELLIPSIS;
				break;
		}
		
		return true;
	}

	// 控件创建后的初始化工作
	virtual bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl::_CreateAfter_())
			return false;

		return true;
	}

private:
	// 重建控件
	virtual bool ReCreate()noexcept
	{
		BaseWindow* ParentWnd = BaseWindow::GetParentCtrl();
		if (ParentWnd == nullptr)
			return false;

		::DestroyWindow(BaseWindow::m_hWnd);
		BaseWindow::m_hWnd = nullptr;
		return Create(*ParentWnd);
	}
	
	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;

		if (_Symbol == BaseHAlignMode::HorzAlignMode.symbol)
		{
			// 获取横向对齐模式

			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			if (BaseWindow::m_dwStyle & SS_LEFT)
				BaseHAlignMode::HorzAlignMode.value = AlignMode::Near;

			else if (BaseWindow::m_dwStyle & SS_RIGHT)
				BaseHAlignMode::HorzAlignMode.value = AlignMode::Far;

			else if (BaseWindow::m_dwStyle & SS_CENTER)
				BaseHAlignMode::HorzAlignMode.value = AlignMode::Center;
		}
		else if (_Symbol == BaseVAlignMode::VertAlignMode.symbol)
		{
			// 获取纵向对齐模式

			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			if (BaseWindow::m_dwStyle & BS_VCENTER)
				BaseVAlignMode::VertAlignMode.value = AlignMode::Center;
			else BaseVAlignMode::VertAlignMode.value = AlignMode::Near;
		}
		else if (_Symbol == BorderStyle.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);

			if (BaseWindow::m_dwStyle & WS_BORDER)
				BorderStyle.value = 4;
			else if (BaseWindow::m_dwStyle & SS_SUNKEN)
				BorderStyle.value = 3;
			else if (BaseWindow::m_dwExStyle & (WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE))
				BorderStyle.value = 2;
			else if (BaseWindow::m_dwExStyle & WS_EX_CLIENTEDGE)
				BorderStyle.value = 1;
			else BorderStyle.value = 0;
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == BaseHAlignMode::HorzAlignMode.symbol)
		{
			// 设置横向对齐模式

			// 获取控件风格，并去掉已有对齐标记
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~SS_CENTER;
			BaseWindow::m_dwStyle &= ~SS_RIGHT;

			// 重新设置对齐标记
			switch (BaseHAlignMode::HorzAlignMode.value)
			{
				case AlignMode::Near:
					BaseWindow::m_dwStyle |= SS_LEFT;
					break;

				case AlignMode::Far:
					BaseWindow::m_dwStyle |= SS_RIGHT;
					break;

				case AlignMode::Center:
					BaseWindow::m_dwStyle |= SS_CENTER;
					break;
			}
			return WindowAPI::WndLong(GWL_STYLE, BaseWindow::m_dwStyle);
		}
		else if (_Symbol == BaseVAlignMode::VertAlignMode.symbol)
		{
			// 设置纵向对齐模式

			// 获取控件风格，并去掉已有对齐标记
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~SS_CENTERIMAGE;

			// 重新设置对齐标记
			switch (BaseVAlignMode::VertAlignMode.value)
			{
				case AlignMode::Near:
					
					break;

				case AlignMode::Far:
					// 目前无效
					break;

				case AlignMode::Center:
					BaseWindow::m_dwStyle |= SS_CENTERIMAGE;
					break;
			}
			return WindowAPI::WndLong(GWL_STYLE, BaseWindow::m_dwStyle);
		}
		else if (_Symbol == BorderStyle.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);

			BaseWindow::m_dwStyle &= ~WS_BORDER;
			BaseWindow::m_dwStyle &= ~SS_SUNKEN;
			BaseWindow::m_dwStyle &= ~WS_EX_DLGMODALFRAME;
			BaseWindow::m_dwStyle &= ~WS_EX_WINDOWEDGE;
			BaseWindow::m_dwStyle &= ~WS_EX_CLIENTEDGE;
			

			// 目前只能重构控件才能改动控件边框类型
			return ReCreate();
		}
		else if (_Symbol == TextShowMode.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~SS_SIMPLE;
			BaseWindow::m_dwStyle &= ~SS_ENDELLIPSIS;
			
			// 目前只能重构控件才能改动控件边框类型
			return ReCreate();

		}
		else return false;
		return true;
	}
	
public:
	// 本属性指定控件的边框类型
	// 0：无边框
	// 1：凹入边框，WS_EX_CLIENTEDGE
	// 2：凸起边框，WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE
	// 3：浅凹边框，SS_SUNKEN
	// 4：单线边框，WS_BORDER
	_property<TextBox, DWORD> BorderStyle;

	// 本属性指定是否标题过长时的显示方式
	// 0：单行文本，多出来的不显示，SS_SIMPLE
	// 1：尾部部分文本以省略号替代，强制单行。SS_ENDELLIPSIS
	// 2：换行显示
	_property<TextBox, DWORD> TextShowMode;
};

_WGUI_END

#endif // !_WGUI_TEXTBOX_
