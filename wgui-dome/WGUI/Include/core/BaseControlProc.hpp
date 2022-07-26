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
	
	// �ؼ���Ϣ�������
	// @param uMsg      ��Ϣ����
	// @param wParam    ����1
	// @param lParam    ����2
	// @param lRetnVal  ����Ҫ���ص�ֵ��ͨ���˲�������
	// @return �Ѵ�����Ϣ�뷵��true��δ������Ϣ����false
	inline virtual bool CtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRetnVal)
	{
		return false;
	}

protected:
	_ElemCtrl& m_Ctrl;
};

_WGUI_END

#endif // !_WGUI_BASECONTROLPROC_
