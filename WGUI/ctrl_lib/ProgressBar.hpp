#pragma once
#ifndef _WGUI_PROGRESS_BAR_
#define _WGUI_PROGRESS_BAR_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

enum class ProgressBarType : DWORD
{
	Horz = 0x00,		// 水平进度条
	Vert = 0x04,		// 垂直进度条
};

enum class ProgressBarState : DWORD
{
	Normal	= 0x01,		// 正常状态
	Erroe	= 0x02,		// 错误状态
	Paused	= 0x03,		// 暂停状态
};

// 进度条
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
	/// 使当前位置前进一步，超出最大位置时，则位置将设置为最近的边界
	/// </summary>
	/// <returns>返回上一个位置</returns>
	inline UINT Stepit()
	{
		return WindowAPI::SendMsg(PBM_STEPIT);
	}

protected:
	// 创建之前的工作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数
		if (!BaseControl::_CreateBefore_())
			return false;
		
		// 指定启用marquee模式
		BaseWindow::m_dwStyle |= (MarqueeMode.value ? PBS_MARQUEE : PBS_SMOOTH);
		
		return true;
	}

	// 创建之后的工作
	bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数
		if (!BaseControl::_CreateAfter_())
			return false;
		
		// 指定单步进量
		WindowAPI::SendMsg(PBM_SETSTEP, StepIncrement.value);

		// 指定当前值
		WindowAPI::SendMsg(PBM_SETPOS, NowPos.value);

		// 指定状态
		WindowAPI::SendMsg(PBM_SETSTATE, State.value);
		
		// 指定最大值和最小值
		WindowAPI::SendMsg(PBM_SETRANGE32, MinPos.value, MaxPos.value);
		
		// 若启用marquee模式，则指定开启动画
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
			// 获取当前位置
			NowPos.value = WindowAPI::SendMsg(PBM_GETPOS);
		}
		else if (_or(_Symbol, { MinPos.symbol, MaxPos.symbol }))
		{
			// 获取最大值和最小值
			PBRANGE Range;

			WindowAPI::SendMsg(PBM_GETRANGE, FALSE, &Range);
			MinPos.value = Range.iLow;
			MaxPos.value = Range.iHigh;
		}
		else if (_Symbol == StepIncrement.symbol)
		{
			// 获取单步进量
			StepIncrement.value = WindowAPI::SendMsg(PBM_GETSTEP);
		}
		else if (_Symbol == State.symbol)
		{
			// 获取状态
			State.value = WindowAPI::SendMsg<ProgressBarState>(PBM_GETSTATE);
		}
		else if (_Symbol == MarqueeMode.symbol)
		{
			// 获取是否启用marquee模式
			MarqueeMode.value = (WindowAPI::WndLong(GWL_STYLE) & PBS_MARQUEE);
		}
		else if(_Symbol == EnableMarquee.symbol)
		{
			// 获取是否开启动画
			// WindowAPI::SendMsg(PBM_SETMARQUEE, EnableMarquee.value);	// 无法获取实时值
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!BaseWindow::IsCreate())
			return true;

		if (_Symbol == NowPos.symbol)
		{
			// 指定当前值
			WindowAPI::SendMsg(PBM_SETPOS, NowPos.value);
			return true;
		}
		else if (_or(_Symbol, { MinPos.symbol, MaxPos.symbol }))
		{
			// 指定最大值和最小值
			WindowAPI::SendMsg(PBM_SETRANGE32, MinPos.value, MaxPos.value);
		}
		else if (_Symbol == StepIncrement.symbol)
		{
			// 指定单步进量
			WindowAPI::SendMsg(PBM_SETSTEP, StepIncrement.value);
		}
		else if (_Symbol == State.symbol)
		{
			// 指定状态
			WindowAPI::SendMsg(PBM_SETSTATE, (DWORD)State.value);
		}
		else if (_Symbol == MarqueeMode.symbol)
		{
			// 指定启用marquee模式
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~PBS_MARQUEE;
			
			return ReCreate();
		}
		else if (_Symbol == EnableMarquee.symbol)
		{
			// 指定开启动画
			WindowAPI::SendMsg(PBM_SETMARQUEE, EnableMarquee.value);
		}
		else return false;
		return true;
	}

public:

#pragma region 属性

	// 本属性指定当前进度位置
	_property<ProgressBar, INT32> NowPos;

	// 本属性指定最小位置值，默认为0
	_property<ProgressBar, INT32> MinPos;

	// 本属性指定最大位置值，默认为100
	_property<ProgressBar, INT32> MaxPos;

	// 本属性指定步进增量，默认为10
	_property<ProgressBar, INT32> StepIncrement;

	// 本属性指定当前状态
	_property<ProgressBar, ProgressBarState> State;

	// 本属性指定是否启用marquee模式
	// 本属性仅创建之前有效
	_property<ProgressBar, bool> MarqueeMode;

	// 本属性指定是否开启Marquee动画，获取此值时，结果不一定正确
	_property<ProgressBar, bool> EnableMarquee;
	
#pragma endregion

};


_WGUI_END


#endif // !_WGUI_PROGRESS_BAR_
