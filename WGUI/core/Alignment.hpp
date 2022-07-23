#pragma once
#ifndef _WGUI_ALIGNMENT_
#define _WGUI_ALIGNMENT_

#include "Framework.hpp"

_WGUI_BEGIN

// 对齐模式
enum class AlignMode : unsigned short
{
	Near,		// 与父控件的近侧对齐（左边对齐、顶边对齐）
	Center,		// 与父控件的中心对齐（水平居中、垂直居中）
	Far			// 与父控件的远侧对齐（右边对齐、底部对齐）
};

// 设置控件对齐
// @param _CtrlMove     对齐控件，该控件被移动
// @param _CtrlLock     对齐控件，该控件不移动
// @param _HorzMode     水平对齐模式
// @param _VertMode     垂直对齐模式
template<class _ElemMove, class _ElemLock>
inline void SetAlign(_ElemMove& _CtrlMove, const _ElemLock& _CtrlLock, AlignMode _HorzMode, AlignMode _VertMode)
{
	RECT rcMove = _CtrlMove.GetRect();
	RECT rcLock = _CtrlLock.GetRect();
	RECT rc = rcMove;
	
	rc.left = (rcLock.right - rcMove.right) / 2;
	rc.top = (rcLock.bottom - rcMove.bottom) / 2;

	_CtrlMove.Rect = { rc.left, rc.top, rc.right, rc.bottom };
}

// 横向对齐模式
template<class _Elem>
class BaseHAlignMode
{
	friend _Elem;
protected:
	inline BaseHAlignMode(HWND& hWnd,
						  _Elem* pThis,
						  void(_Elem::* _Get)(int),
						  bool(_Elem::* _Set)(int))
		: m_hWnd(hWnd)
		, HorzAlignMode(pThis, _Get, _Set)
	{}

public:
	// 本属性指定标题文本的横向对齐模式
	_property<_Elem, AlignMode> HorzAlignMode;

private:
	HWND& m_hWnd;
};

// 纵向对齐模式
template<class _Elem>
class BaseVAlignMode
{
	friend _Elem;
protected:
	inline BaseVAlignMode(HWND& hWnd,
						  _Elem* pThis,
						  void(_Elem::* _Get)(int),
						  bool(_Elem::* _Set)(int))
		: m_hWnd(hWnd)
		, VertAlignMode(pThis, _Get, _Set)
	{}

public:
	// 本属性指定标题文本的纵向对齐模式
	_property<_Elem, AlignMode> VertAlignMode;

private:
	HWND& m_hWnd;
};
	
_WGUI_END

#endif // !_WGUI_ALIGNMENT_
