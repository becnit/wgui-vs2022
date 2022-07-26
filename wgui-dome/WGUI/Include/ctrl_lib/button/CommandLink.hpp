#pragma once
#ifndef _WGUI_COMMANDLINK_
#define _WGUI_COMMANDLINK_

#include "BaseButton.hpp"

_WGUI_BEGIN

// 命令链接控件
class CommandLink
	: public BaseControl<CommandLink>
	, public Attri_Caption
{

public:
	inline CommandLink()noexcept
		: BaseControl<CommandLink>(WC_BUTTONW)
		, Attri_Caption(BaseWindow::m_hWnd)
		, NoteText(PROPERTY_INIT(CommandLink))
	{
		BaseWindow::m_dwStyle |= BS_COMMANDLINK;
	}
	
	virtual ~CommandLink()noexcept
	{}

	// 创建按钮
	virtual bool Create(const BaseWindow& _ParentWnd)noexcept
	{
		return BaseControl::Create(_ParentWnd, WStr(Caption));
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

		// 指定说明文本
		WindowAPI::SendMsg(BCM_SETNOTE, 0, NoteText.value.CStr());
		return true;
	}

private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;
		
		if (_Symbol == NoteText.symbol)
		{
			// 获取注释文本

			DWORD len = WindowAPI::SendMsg(BCM_GETNOTELENGTH, 0, 0) + 1;

			NoteText.value.SetCapacity(len);
			WindowAPI::SendMsg(BCM_GETNOTE, len, NoteText.value.Data());
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == NoteText.symbol)
		{
			// 设置提示文本
			return WindowAPI::SendMsg(BCM_SETNOTE, 0, NoteText.value.CStr());
		}
		else return false;
		return true;
	}

public:
	// 本属性指定说明文本
	_property<CommandLink, WStr> NoteText;
};

_WGUI_END


#endif // !_WGUI_COMMANDLINK_
