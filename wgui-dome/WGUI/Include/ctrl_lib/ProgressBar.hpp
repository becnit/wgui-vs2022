#pragma once
#ifndef _WGUI_PROGRESS_BAR_
#define _WGUI_PROGRESS_BAR_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

enum class ProgressBarType : DWORD
{
	Horz = 0x00,		// ˮƽ������
	Vert = 0x04,		// ��ֱ������
};

enum class ProgressBarState : DWORD
{
	Normal	= 0x01,		// ����״̬
	Erroe	= 0x02,		// ����״̬
	Paused	= 0x03,		// ��ͣ״̬
};

// ������
class ProgressBar
	: public BaseControl<ProgressBar>
{
public:
	inline ProgressBar(ProgressBarType _Type)noexcept
		: BaseControl<ProgressBar>(PROGRESS_CLASSW)
		, NowPos(PROPERTY_INIT(ProgressBar), 0)
		, MinPos(PROPERTY_INIT(ProgressBar), 0)
		, MaxPos(PROPERTY_INIT(ProgressBar), 100)
		, StepIncrement(PROPERTY_INIT(ProgressBar), 10)
		, State(PROPERTY_INIT(ProgressBar), ProgressBarState::Normal)
		, MarqueeMode(PROPERTY_INIT(ProgressBar), false)
		, EnableMarquee(PROPERTY_INIT(ProgressBar), false)
	{
		BaseWindow::m_dwStyle |= (DWORD)_Type;
	}

	inline ~ProgressBar()noexcept
	{}

	/// <summary>
	/// ʹ��ǰλ��ǰ��һ�����������λ��ʱ����λ�ý�����Ϊ����ı߽�
	/// </summary>
	/// <returns>������һ��λ��</returns>
	inline UINT Stepit()
	{
		return WindowAPI::SendMsg(PBM_STEPIT);
	}

protected:
	// ����֮ǰ�Ĺ���
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ������
		if (!BaseControl::_CreateBefore_())
			return false;
		
		// ָ������marqueeģʽ
		BaseWindow::m_dwStyle |= (MarqueeMode.value ? PBS_MARQUEE : PBS_SMOOTH);
		
		return true;
	}

	// ����֮��Ĺ���
	bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ������
		if (!BaseControl::_CreateAfter_())
			return false;
		
		// ָ����������
		WindowAPI::SendMsg(PBM_SETSTEP, StepIncrement.value);

		// ָ����ǰֵ
		WindowAPI::SendMsg(PBM_SETPOS, NowPos.value);

		// ָ��״̬
		WindowAPI::SendMsg(PBM_SETSTATE, State.value);
		
		// ָ�����ֵ����Сֵ
		WindowAPI::SendMsg(PBM_SETRANGE32, MinPos.value, MaxPos.value);
		
		// ������marqueeģʽ����ָ����������
		if (EnableMarquee.value)
			WindowAPI::SendMsg(PBM_SETMARQUEE, EnableMarquee.value);

		return true;
	}

private:
	inline bool ReCreate()noexcept
	{
		BaseWindow* ParentWnd = BaseWindow::GetParentCtrl();
		if (ParentWnd == nullptr)
			return false;

		return Create(*ParentWnd);
	}
	
	inline void _property_get(int _Symbol)noexcept
	{
		if (!BaseWindow::IsCreate())
			return;

		if (_Symbol == NowPos.symbol)
		{
			// ��ȡ��ǰλ��
			NowPos.value = WindowAPI::SendMsg(PBM_GETPOS);
		}
		else if (_or(_Symbol, { MinPos.symbol, MaxPos.symbol }))
		{
			// ��ȡ���ֵ����Сֵ
			PBRANGE Range;

			WindowAPI::SendMsg(PBM_GETRANGE, FALSE, &Range);
			MinPos.value = Range.iLow;
			MaxPos.value = Range.iHigh;
		}
		else if (_Symbol == StepIncrement.symbol)
		{
			// ��ȡ��������
			StepIncrement.value = WindowAPI::SendMsg(PBM_GETSTEP);
		}
		else if (_Symbol == State.symbol)
		{
			// ��ȡ״̬
			State.value = WindowAPI::SendMsg<ProgressBarState>(PBM_GETSTATE);
		}
		else if (_Symbol == MarqueeMode.symbol)
		{
			// ��ȡ�Ƿ�����marqueeģʽ
			MarqueeMode.value = (WindowAPI::WndLong(GWL_STYLE) & PBS_MARQUEE);
		}
		else if(_Symbol == EnableMarquee.symbol)
		{
			// ��ȡ�Ƿ�������
			// WindowAPI::SendMsg(PBM_SETMARQUEE, EnableMarquee.value);	// �޷���ȡʵʱֵ
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!BaseWindow::IsCreate())
			return true;

		if (_Symbol == NowPos.symbol)
		{
			// ָ����ǰֵ
			WindowAPI::SendMsg(PBM_SETPOS, NowPos.value);
			return true;
		}
		else if (_or(_Symbol, { MinPos.symbol, MaxPos.symbol }))
		{
			// ָ�����ֵ����Сֵ
			WindowAPI::SendMsg(PBM_SETRANGE32, MinPos.value, MaxPos.value);
		}
		else if (_Symbol == StepIncrement.symbol)
		{
			// ָ����������
			WindowAPI::SendMsg(PBM_SETSTEP, StepIncrement.value);
		}
		else if (_Symbol == State.symbol)
		{
			// ָ��״̬
			WindowAPI::SendMsg(PBM_SETSTATE, (DWORD)State.value);
		}
		else if (_Symbol == MarqueeMode.symbol)
		{
			// ָ������marqueeģʽ
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~PBS_MARQUEE;
			
			return ReCreate();
		}
		else if (_Symbol == EnableMarquee.symbol)
		{
			// ָ����������
			WindowAPI::SendMsg(PBM_SETMARQUEE, EnableMarquee.value);
		}
		else return false;
		return true;
	}

public:

#pragma region ����

	// ������ָ����ǰ����λ��
	_property<ProgressBar, INT32> NowPos;

	// ������ָ����Сλ��ֵ��Ĭ��Ϊ0
	_property<ProgressBar, INT32> MinPos;

	// ������ָ�����λ��ֵ��Ĭ��Ϊ100
	_property<ProgressBar, INT32> MaxPos;

	// ������ָ������������Ĭ��Ϊ10
	_property<ProgressBar, INT32> StepIncrement;

	// ������ָ����ǰ״̬
	_property<ProgressBar, ProgressBarState> State;

	// ������ָ���Ƿ�����marqueeģʽ
	// �����Խ�����֮ǰ��Ч
	_property<ProgressBar, bool> MarqueeMode;

	// ������ָ���Ƿ���Marquee��������ȡ��ֵʱ�������һ����ȷ
	_property<ProgressBar, bool> EnableMarquee;
	
#pragma endregion

};


_WGUI_END


#endif // !_WGUI_PROGRESS_BAR_
