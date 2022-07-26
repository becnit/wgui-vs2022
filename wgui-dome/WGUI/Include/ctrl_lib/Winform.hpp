#pragma once
#ifndef _WGUI_WINFORM_
#define _WGUI_WINFORM_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

enum class WinformBorder : WORD
{
	None,		// 无边框
	Lock,		// 锁定边框
	Adjustable	// 可调边框
};

// 窗体控件
class Winform
	: public BaseControl<Winform>
	, public Attri_Caption
{
public:
	Winform(LPCWSTR _ClassName = L"WGUIWinform")noexcept;

	~Winform()noexcept;
	
	// 创建窗口
	// @param _ParentWnd  父窗口对象
	// @returns 创建成功返回true，否则返回false
	virtual bool Create(const BaseWindow& _ParentWnd = Winform())noexcept;

protected:
	// 创建控件前的初始化操作
	virtual bool _CreateBefore_()noexcept;

	// 创建控件前的操作
	virtual bool _CreateAfter_()noexcept;

private:
	void _property_get(int _Symbol)noexcept;

	bool _property_set(int _Symbol)noexcept;

	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	WNDCLASSEXW m_Wcex;

public:
	// 本属性指定窗口的边框类型，WinformBorder::Lock
	_property<Winform, WinformBorder> Border;

	// 本属性指定窗口的图标
	_property<Winform, HICON> Icon;

	// 本属性指定是否启用最小化按钮
	_property<Winform, bool> MinimizeBox;

	// 本属性指定是否启用最大化按钮
	_property<Winform, bool> MaximizeBox;
	
	// 本属性指定是否启用关闭按钮
	_property<Winform, bool> CloseBox;

	// 本属性指定是否在任务栏显示窗口图标，目前设置无效
	_property<Winform, bool> ShowInTaskbar;
	
};

// 消息获取、翻译、分发、循环
// @return 返回线程窗口的退出值
WPARAM MessageLoop()noexcept;

_WGUI_END

#endif // !_WGUI_WINFORM_
