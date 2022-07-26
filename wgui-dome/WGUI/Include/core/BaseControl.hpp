#pragma once
#ifndef _WGUI_BASECONTROL_
#define _WGUI_BASECONTROL_

#include "BaseWindow.hpp"
#include "BaseControlProc.hpp"

_WGUI_BEGIN

// �ؼ�����ID
class BaseCtrlID
{
protected:
	inline BaseCtrlID();

protected:
	static DWORD m_BaseID;
};

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
		, m_pHandleProc(nullptr)
		, m_OldWndProc(nullptr)
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

	// ���ÿؼ���Ϣ������
	// @param _ProcObj ��Ϣ������
	// @returns �����óɹ�������true�����򷵻�false
	inline bool SetHandleProc(BaseControlProc<_ElemCtrl>& _ProcObj)noexcept
	{
		if (!WindowAPI::IsCreate() || m_OldWndProc != nullptr)
			return false;
		
		m_OldWndProc = WindowAPI::WndLong(GWLP_WNDPROC, __CtrlProc);
		return m_OldWndProc != nullptr;
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

	// ���ô�����
	inline bool CallHandleProc(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRetnVal)noexcept
	{
		if (m_pHandleProc)
			return m_pHandleProc->CtrlProc(uMsg, wParam, lParam, lRetnVal);
		return false;
	}

private:
	inline static LRESULT CALLBACK __CtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)noexcept
	{
		_ElemCtrl* pCtrl = reinterpret_cast<_ElemCtrl*>(::GetPropW(hWnd, BaseWindow::PROP_THIS));
		if (!pCtrl)
			return 0;

		LRESULT lResult = 0;
		
		if (pCtrl.CallHandleProc(uMsg, wParam, lParam, lResult))
			return lResult;
		return ::CallWindowProcW(pCtrl.m_OldWndProc, hWnd, uMsg, wParam, lParam);
	}
	
private:
	BaseControlProc<_ElemCtrl>* m_pHandleProc;
	WNDPROC m_OldWndProc;
};

#pragma region �ؼ�һЩ���ݽṹ

// �ؼ��߿�����
enum class CtrlBorder : DWORD
{
	Null	= 0x00000000L,	// û�б߿�
	Border	= 0x00800000L,	// ���߱߿�WS_BORDER
	Suuken	= 0x00020000L,	// ���ݱ߿�WS_EX_STATICEDGE
};

// �ؼ�����������
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

// ����ṹ-��չ
template<typename _Ty = INT32>
struct TABLE_ITEM_EX
{
	WStr Text = WStr();
	_Ty Data = _Ty();
	int Image = -1;
};

#pragma endregion

_WGUI_END


#endif // !_WGUI_BASECONTROL_
