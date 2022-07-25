#pragma once
#ifndef _WGUI_BASELISTVIEW_
#define _WGUI_BASELISTVIEW_

#include "../../core/BaseControl.hpp"

_WGUI_BEGIN

template<class _ElemCtrl>
class BaseListView
	: public BaseControl<_ElemCtrl>
{
protected:
	inline BaseListView()noexcept
		: BaseControl<_ElemCtrl>(WC_LISTVIEWW)
	{}

};

_WGUI_END

#endif // !_WGUI_BASELISTVIEW_
