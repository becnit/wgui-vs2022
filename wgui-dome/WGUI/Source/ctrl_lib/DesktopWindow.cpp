#include "../../Include/ctrl_lib/DesktopWindow.hpp"

_WGUI_BEGIN


DesktopWindow::DesktopWindow()noexcept
	: BaseWindow()
{
	m_hWnd = ::GetDesktopWindow();
}

// �ؼ�����ǰ�ĳ�ʼ������
bool DesktopWindow::_CreateBefore_()noexcept
{
	return true;
}

// �ؼ�������ĳ�ʼ������
bool DesktopWindow::_CreateAfter_()noexcept
{
	return true;
}

_WGUI_END