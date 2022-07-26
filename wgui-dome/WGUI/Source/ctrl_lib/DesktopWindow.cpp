#include "../../Include/ctrl_lib/DesktopWindow.hpp"

_WGUI_BEGIN


DesktopWindow::DesktopWindow()noexcept
	: BaseWindow()
{
	m_hWnd = ::GetDesktopWindow();
}

// 控件创建前的初始化工作
bool DesktopWindow::_CreateBefore_()noexcept
{
	return true;
}

// 控件创建后的初始化工作
bool DesktopWindow::_CreateAfter_()noexcept
{
	return true;
}

_WGUI_END