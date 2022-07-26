#pragma once
#ifndef _WGUI_SCROLL_BAR_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

enum class ScrollBarType
{
	Horz = 0x0000,	// ˮƽ������
	Vert = 0x0001	// ��ֱ������
};

// ������
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
	// ����֮ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ������
		if (!BaseControl::_CreateBefore_())
			return false;
		
		return true;
	}

	// ����֮��Ĺ�����ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ������
		if (!BaseControl::_CreateAfter_())
			return false;

		// ָ���������ĳ�ʼλ�á���Сֵ�����ֵ
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
			SBM_ENABLE_ARROWS		���û���ù������ؼ���һ����������ͷ

			ESB_DISABLE_BOTH		���ù������ϵ�������ͷ
			ESB_ENABLE_BOTH			���ù������ϵ�������ͷ

			ESB_DISABLE_LEFT		����ˮƽ�������ϵ������ͷ
			ESB_DISABLE_RIGHT		����ˮƽ�������ϵ����Ҽ�ͷ

			ESB_DISABLE_UP			���ô�ֱ�������ϵ����ϼ�ͷ
			ESB_DISABLE_DOWN		���ô�ֱ�������ϵ����¼�ͷ

			ESB_DISABLE_LTUP		����ˮƽ�������ϵ������ͷ��ֱ�������ϵ����ϼ�ͷ
			ESB_DISABLE_RTDN		����ˮƽ�������ϵ����Ҽ�ͷ��ֱ�������ϵ����¼�ͷ
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
			SBM_ENABLE_ARROWS		���û���ù������ؼ���һ����������ͷ

			ESB_DISABLE_BOTH		���ù������ϵ�������ͷ
			ESB_ENABLE_BOTH			���ù������ϵ�������ͷ

			ESB_DISABLE_LEFT		����ˮƽ�������ϵ������ͷ
			ESB_DISABLE_RIGHT		����ˮƽ�������ϵ����Ҽ�ͷ

			ESB_DISABLE_UP			���ô�ֱ�������ϵ����ϼ�ͷ
			ESB_DISABLE_DOWN		���ô�ֱ�������ϵ����¼�ͷ

			ESB_DISABLE_LTUP		����ˮƽ�������ϵ������ͷ��ֱ�������ϵ����ϼ�ͷ
			ESB_DISABLE_RTDN		����ˮƽ�������ϵ����Ҽ�ͷ��ֱ�������ϵ����¼�ͷ
			*/
			
			return  (bool)WindowAPI::SendMsg(SBM_ENABLE_ARROWS,
											 ShowArrows.value ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
		}
		return false;
	}
public:
	// ������ָ���������λ��
	_property<ScrollBar, INT>		CurPos;

	// ������ָ����������Сֵ
	_property<ScrollBar, INT>		MinPos;

	// ������ָ�����������ֵ
	_property<ScrollBar, INT>		MaxPos;

	// ������ָ���Ƿ���ʾ�������ϵļ�ͷ
	_property<ScrollBar, bool>		ShowArrows;
};

_WGUI_END

#endif // !_WGUI_SCROLL_BAR_
