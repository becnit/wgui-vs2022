#pragma once
#ifndef _WGUI_SPLITBUTTON_
#define _WGUI_SPLITBUTTON_

#include "BaseButton.hpp"

_WGUI_BEGIN

// 拆分按钮控件
class SplitButton
	: public BaseButton<SplitButton>
{
public:
	inline SplitButton()noexcept
		: BaseButton<SplitButton>(BS_SPLITBUTTON)
	{
	}

	virtual ~SplitButton()noexcept
	{}

protected:
	// 控件创建前的初始化工作
	virtual bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl::_CreateBefore_())
			return false;

		return true;
	}

	// 控件创建后的初始化工作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl::_CreateAfter_())
			return false;

		return true;
	}
	
public:
	
	// 本属性指定子项目
	// _property<SplitButton, SPLIT_ITEM> Item;
	
};

_WGUI_END

#endif // !_WGUI_SPLITBUTTON_
