#pragma once
#ifndef _WGUI_TEXTBOX_
#define _WGUI_TEXTBOX_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

// �ı���
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
	// �ؼ�����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateBefore_())
			return false;

		// ָ��������뷽ʽ
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

		// ָ��������뷽ʽ
		switch (BaseVAlignMode::VertAlignMode.value)
		{
			case AlignMode::Near:
				break;

			case AlignMode::Far:
				// Ŀǰ��Ч
				break;

			case AlignMode::Center:
				BaseWindow::m_dwStyle |= SS_CENTERIMAGE;
				break;
		}

		// ָ���߿���ʽ
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
		
		// ָ������ʾģʽ
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

	// �ؼ�������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateAfter_())
			return false;

		return true;
	}

private:
	// �ؽ��ؼ�
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
			// ��ȡ�������ģʽ

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
			// ��ȡ�������ģʽ

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
			// ���ú������ģʽ

			// ��ȡ�ؼ���񣬲�ȥ�����ж�����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~SS_CENTER;
			BaseWindow::m_dwStyle &= ~SS_RIGHT;

			// �������ö�����
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
			// �����������ģʽ

			// ��ȡ�ؼ���񣬲�ȥ�����ж�����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~SS_CENTERIMAGE;

			// �������ö�����
			switch (BaseVAlignMode::VertAlignMode.value)
			{
				case AlignMode::Near:
					
					break;

				case AlignMode::Far:
					// Ŀǰ��Ч
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
			

			// Ŀǰֻ���ع��ؼ����ܸĶ��ؼ��߿�����
			return ReCreate();
		}
		else if (_Symbol == TextShowMode.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~SS_SIMPLE;
			BaseWindow::m_dwStyle &= ~SS_ENDELLIPSIS;
			
			// Ŀǰֻ���ع��ؼ����ܸĶ��ؼ��߿�����
			return ReCreate();

		}
		else return false;
		return true;
	}
	
public:
	// ������ָ���ؼ��ı߿�����
	// 0���ޱ߿�
	// 1������߿�WS_EX_CLIENTEDGE
	// 2��͹��߿�WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE
	// 3��ǳ���߿�SS_SUNKEN
	// 4�����߱߿�WS_BORDER
	_property<TextBox, DWORD> BorderStyle;

	// ������ָ���Ƿ�������ʱ����ʾ��ʽ
	// 0�������ı���������Ĳ���ʾ��SS_SIMPLE
	// 1��β�������ı���ʡ�Ժ������ǿ�Ƶ��С�SS_ENDELLIPSIS
	// 2��������ʾ
	_property<TextBox, DWORD> TextShowMode;
};

_WGUI_END

#endif // !_WGUI_TEXTBOX_
