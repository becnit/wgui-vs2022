#pragma once
#ifndef _WGUI_ALIGNMENT_
#define _WGUI_ALIGNMENT_

#include "Framework.hpp"

_WGUI_BEGIN

// ����ģʽ
enum class AlignMode : unsigned short
{
	Near,		// �븸�ؼ��Ľ�����루��߶��롢���߶��룩
	Center,		// �븸�ؼ������Ķ��루ˮƽ���С���ֱ���У�
	Far			// �븸�ؼ���Զ����루�ұ߶��롢�ײ����룩
};

// ���ÿؼ�����
// @param _CtrlMove     ����ؼ����ÿؼ����ƶ�
// @param _CtrlLock     ����ؼ����ÿؼ����ƶ�
// @param _HorzMode     ˮƽ����ģʽ
// @param _VertMode     ��ֱ����ģʽ
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

// �������ģʽ
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
	// ������ָ�������ı��ĺ������ģʽ
	_property<_Elem, AlignMode> HorzAlignMode;

private:
	HWND& m_hWnd;
};

// �������ģʽ
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
	// ������ָ�������ı����������ģʽ
	_property<_Elem, AlignMode> VertAlignMode;

private:
	HWND& m_hWnd;
};
	
_WGUI_END

#endif // !_WGUI_ALIGNMENT_
