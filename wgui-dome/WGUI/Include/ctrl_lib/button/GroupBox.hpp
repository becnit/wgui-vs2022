#pragma once
#ifndef _WGUI_GROUPBOX_
#define _WGUI_GROUPBOX_

#include "../../core/BaseControl.hpp"

_WGUI_BEGIN


class GroupBox
	: public BaseControl<GroupBox>
	, public BaseHAlignMode<GroupBox>
	, public Attri_Caption
{
public:

	inline GroupBox()noexcept
		: BaseControl<GroupBox>(WC_BUTTONW)
		, BaseHAlignMode<GroupBox>(BaseWindow::m_hWnd, PROPERTY_INIT(GroupBox))
		, Attri_Caption(BaseWindow::m_hWnd)
	{
		BaseWindow::m_dwStyle |= BS_GROUPBOX;
	}

	virtual ~GroupBox()noexcept
	{	}

	virtual bool Create(const BaseWindow& _ParentWnd)noexcept
	{
		return BaseControl::Create(_ParentWnd, WStr(Caption));
	}
	
private:
	// ����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateBefore_())
			return false;

		// ��ʼ���������ģʽ
		switch (BaseHAlignMode::HorzAlignMode.value)
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
		
		return true;
	}

	// ������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateAfter_())
			return false;

		return true;
	}

	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;

		if (_Symbol == BaseHAlignMode::HorzAlignMode.symbol)
		{
			// ��ȡ�������ģʽ

			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			if (BaseWindow::m_dwStyle & BS_LEFT)
				BaseHAlignMode::HorzAlignMode = AlignMode::Near;
			
			else if (BaseWindow::m_dwStyle & BS_RIGHT)
				BaseHAlignMode::HorzAlignMode = AlignMode::Far;
			
			else if (BaseWindow::m_dwStyle & BS_CENTER)
				BaseHAlignMode::HorzAlignMode = AlignMode::Center;
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
			BaseWindow::m_dwStyle &= ~BS_LEFT;
			BaseWindow::m_dwStyle &= ~BS_RIGHT;

			// �������ö�����
			switch (HorzAlignMode.value)
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
			return WindowAPI::WndLong(GWL_STYLE, BaseWindow::m_dwStyle);
		}
		else return false;
		return true;
	}
};

_WGUI_END

#endif // !_WGUI_GROUPBOX_
