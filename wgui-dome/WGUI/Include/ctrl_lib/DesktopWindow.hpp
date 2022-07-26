#pragma once
#ifndef _WGUI_DESKTOPWINDOW_
#define _WGUI_DESKTOPWINDOW_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

class DesktopWindow
	: public BaseWindow
{
public:
	DesktopWindow()noexcept;

private:
	virtual bool _CreateBefore_()noexcept;
	virtual bool _CreateAfter_()noexcept;
};

_WGUI_END

#endif // !_WGUI_DESKTOPWINDOW_
