#pragma once
#ifndef _WGUI_BASECONTROL_
#define _WGUI_BASECONTROL_

#include "BaseWindow.hpp"

_WGUI_BEGIN

// �ؼ�����ID
class BaseCtrlID
{
protected:
	inline BaseCtrlID()
	{
		++m_BaseID;
	}

protected:
	static DWORD m_BaseID;
};
DWORD BaseCtrlID::m_BaseID = 100;

// �ؼ�����
template<class _ElemCtrl>
class BaseControl
	: public BaseWindow
	, protected BaseCtrlID
{
protected:
	inline BaseControl(LPCWSTR _CtrlClass)noexcept
		: BaseWindow()
		, BaseCtrlID()
	{
		// WS_CHILDWINDOW, �����Ӵ���
		// WS_CLIPCHILDREN�������ڲ����Ӵ���������л���
		// WS_CLIPSIBLINGS�������ֵܴ��ڽ��л���

		BaseWindow::m_dwStyle |= WS_CHILDWINDOW;
		BaseWindow::m_ClassName = _CtrlClass;
	}

public:
	virtual ~BaseControl()noexcept
	{}

	// �����ؼ�
	// @param _ParentWnd  ���ؼ�
	// @param _CtrlName   �ؼ�����
	// @returns �ɹ���������true�����򷵻�false
	virtual bool Create(const BaseWindow& _ParentWnd, const WStr& _CtrlName = L"")noexcept
	{
		BaseWindow::Create(_CtrlName, _ParentWnd, (HMENU)m_BaseID);

		if (BaseWindow::IsCreate())
		{
			// ����thisָ�뵽�������Ա���
			WindowAPI::SetPropItem(BaseWindow::PROP_THIS, this);

			// �͸�����ͬ������
			HFONT hFont = _ParentWnd.SendMsg<HFONT>(WM_GETFONT);
			if (hFont)
				WindowAPI::SendMsg(WM_SETFONT, hFont);
		}
		return BaseWindow::IsCreate();
	}

	// �����пؼ����ȡ���ؼ�����
	// @param _hWnd �ؼ����
	// @returns �ɹ�ȡ�����ؿؼ����󣬷��򷵻�nullptr
	static _ElemCtrl* CreateFormHWND(HWND _hWnd)noexcept
	{
		if (!::IsWindow(_hWnd))
			return nullptr;
		
		WStr ClassName(260);
		_ElemCtrl TempCtrl;
		
		::GetClassNameW(_hWnd, ClassName, ClassName.Capacity());
		if (TempCtrl.BaseWindow::m_ClassName != ClassName)
			return nullptr;
		
		return reinterpret_cast<_ElemCtrl*>(::GetPropW(_hWnd, BaseWindow::PROP_THIS));
	}
	
	// ��ȡ�ؼ�ID
	// @returns  ���ؼ��Ѵ������򷵻ؿؼ�ID�����򷵻�0
	inline DWORD GetCtrlID()const noexcept
	{
		if (!WindowAPI::IsCreate())
			return 0;
		return ::GetDlgCtrlID(m_hWnd);
	}

protected:

	// �ؼ�����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		return true;
	}

	// �ؼ�������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		return true;
	}
};


enum class CtrlBorder : DWORD
{
	Null	= 0x00000000L,	// û�б߿�
	Border	= 0x00800000L,	// ���߱߿�WS_BORDER
	Suuken	= 0x00020000L,	// ���ݱ߿�WS_EX_STATICEDGE
};

enum class CtrlScrollBar : DWORD
{
	Null = 0,						// û�й�����
	Horz = WS_HSCROLL,				// ˮƽ������
	Vert = WS_VSCROLL,				// ��ֱ������
	Both = WS_HSCROLL | WS_VSCROLL,	// ��ֱ��ˮƽ������
};

// ����ṹ
template<typename _Ty = INT32>
struct TABLE_ITEM
{
	WStr Text = WStr();
	_Ty Data = _Ty();
};

_WGUI_END


#endif // !_WGUI_BASECONTROL_
