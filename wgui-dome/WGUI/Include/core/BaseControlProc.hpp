#pragma once
#ifndef _WGUI_BASECONTROLPROC_
#define _WGUI_BASECONTROLPROC_

#include "BaseControl.hpp"

_WGUI_BEGIN

template<class _ElemCtrl>
class BaseControl;

template<class _ElemCtrl>
class BaseControlProc
{
	friend class BaseControl<_ElemCtrl>;
	
public:
	inline BaseControlProc(_ElemCtrl& _Obj)noexcept
		: m_Ctrl(_Obj)
	{}
	
	inline ~BaseControlProc()noexcept
	{}

protected:
	
	// 控件消息处理过程
	// @param uMsg      消息类型
	// @param wParam    参数1
	// @param lParam    参数2
	// @param lRetnVal  接收要返回的值，通过此参数传递
	// @return 已处理消息请返回true，未处理消息返回false
	inline virtual bool CtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRetnVal)
	{
		return false;
	}

protected:
	_ElemCtrl& m_Ctrl;
};

_WGUI_END

#endif // !_WGUI_BASECONTROLPROC_
