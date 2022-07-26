#pragma once
#ifndef _WGUI_CHECKBOX_
#define _WGUI_CHECKBOX_

#include "BaseButton.hpp"

_WGUI_BEGIN


// 复选框
class CheckBox
	: public BaseButton<CheckBox>
{
public:
	inline CheckBox()noexcept
		: BaseButton<CheckBox>(BS_AUTOCHECKBOX)
		, Checked(PROPERTY_INIT(CheckBox))
	{
		HorzAlignMode = AlignMode::Near;
	}

	virtual ~CheckBox()noexcept
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

		// 指定初始选中状态
		WindowAPI::SendMsg(BM_SETCHECK, Checked.value ? BST_CHECKED : BST_UNCHECKED);

		// 调用基类的初始化函数
		return BaseButton::_CreateAfter_();
	}

private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;

		if (_Symbol == Checked.symbol)
		{
			// 获取选中状态
			Checked.value = WindowAPI::SendMsg(BM_GETCHECK) == BST_CHECKED;
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == Checked.symbol)
		{
			// 设置选中状态
			WindowAPI::SendMsg(BM_SETCHECK, Checked.value ? BST_CHECKED : BST_UNCHECKED);
		}
		else return false;
		return true;
	}

public:
	// 本属性指定控件的选中状态
	_property<CheckBox, bool> Checked;
};

_WGUI_END


#endif // !_WGUI_CHECKBOX_
