#pragma once
#ifndef _WGUI_PUSHBUTTON_
#define _WGUI_PUSHBUTTON_

#include "BaseButton.hpp"

_WGUI_BEGIN

// 按钮控件
class PushButton
	: public BaseButton<PushButton>
{
public:
	inline PushButton()noexcept
		: BaseButton<PushButton>(BS_PUSHBUTTON)
	{
	}

	virtual ~PushButton()noexcept
	{}

protected:
	// 控件创建前的初始化工作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseButton::_CreateBefore_())
			return false;

		return true;
	}

	// 控件创建后的初始化工作
	virtual bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseButton::_CreateAfter_())
			return false;

		return true;
	}

};

_WGUI_END


#endif // !_WGUI_PUSHBUTTON_
