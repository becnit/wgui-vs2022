#pragma once
#ifndef _WGUI_3STATEBUTTON_
#define _WGUI_3STATEBUTTON_

#include "BaseButton.hpp"

_WGUI_BEGIN

// 3态复选框控件
class ThreeStateButton
	: public BaseButton<ThreeStateButton>
{
public:
	inline ThreeStateButton()noexcept
		: BaseButton<ThreeStateButton>(BS_AUTO3STATE)
		, Checked(PROPERTY_INIT(ThreeStateButton))
	{
		HorzAlignMode = AlignMode::Near;
	}

	virtual ~ThreeStateButton()noexcept
	{
	}

protected:
	// 控件创建前的初始化工作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl::_CreateBefore_())
			return false;
		
		return true;
	}

	// 控件创建后的初始化工作
	virtual bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl::_CreateAfter_())
			return false;
		
		// 指定初始选中状态
		WindowAPI::SendMsg(BM_SETCHECK, Checked.value);

		return true;
	}

private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;

		if (_Symbol == Checked.symbol)
		{
			// 获取选中状态
			Checked.value = WindowAPI::SendMsg(BM_GETCHECK);
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == Checked.symbol)
		{
			// 设置选中状态
			WindowAPI::SendMsg(BM_SETCHECK, Checked.value);
		}
		else return false;
		return true;
	}

public:
	
	// 本属性指定控件的选中状态
	// 0: 未选中, BST_UNCHECKED
	// 1: 选中, BST_CHECKED
	// 2: 不确定, BST_INDETERMINATE
	_property<ThreeStateButton, int> Checked;
};

_WGUI_END

#endif // !_WGUI_3STATEBUTTON_
