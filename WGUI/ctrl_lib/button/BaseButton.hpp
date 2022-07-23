#pragma once
#ifndef _WGUI_BASEBUTTON_
#define _WGUI_BASEBUTTON_

#include "../../core/BaseControl.hpp"

_WGUI_BEGIN

// ��ť���ಿ��ʵ��
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

	// ��̨ģ����
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
			// ��ȡ�������ģʽ

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
			// ��ȡ�������ģʽ

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
			// ���ú������ģʽ

			// ��ȡ�ؼ���񣬲�ȥ�����ж�����
			DWORD dwStyle = ::GetWindowLongW(m_hWnd, GWL_STYLE);
			
			// ������ж�����
			dwStyle &= ~BS_LEFT;
			dwStyle &= ~BS_RIGHT;

			// �������ö�����
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
			// �����������ģʽ

			// ��ȡ�ؼ���񣬲�ȥ�����ж�����
			DWORD dwStyle = ::GetWindowLongW(m_hWnd, GWL_STYLE);
			
			// ������ж�����
			dwStyle &= ~BS_TOP;
			dwStyle &= ~BS_BOTTOM;

			// �������ö�����
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

// ��ť����
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

	// ������ť
	virtual bool Create(const BaseWindow& _ParentWnd)noexcept
	{
		return BaseControl<_ElemCtrl>::Create(_ParentWnd, Caption);
	}

protected:
	
	// �ؼ�����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl<_ElemCtrl>::_CreateBefore_())
			return false;

		// ��ʼ���������ģʽ
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

		// ��ʼ���������ģʽ
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

	// �ؼ�������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl<_ElemCtrl>::_CreateAfter_())
			return false;
		
		return true;
	}
};

_WGUI_END
#endif // !_WGUI_BASEBUTTON_
