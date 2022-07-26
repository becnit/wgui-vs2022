#pragma once
#ifndef _WGUI_BASE_WINDOW_
#define _WGUI_BASE_WINDOW_

#include "GeneralAttributes.hpp"
#include "WindowAPI.hpp"
#include "Alignment.hpp"

_WGUI_BEGIN

// �ؼ�����
class BaseWindow
	: public WindowAPI
	, public Attri_General
{
protected:
	inline BaseWindow()noexcept
		: WindowAPI(m_hWnd)
		, Attri_General(m_hWnd)
		, m_hWnd(nullptr)
		, m_dwStyle(0)
		, m_dwExStyle(0)
		, m_ClassName()
	{}

	virtual ~BaseWindow()noexcept
	{
		Destroy();
	}

	// ����
	inline bool Create(const WStr& _WndName,
					   const BaseWindow& _ParentWnd,
					   HMENU _Menu,
					   LPVOID _Param = nullptr)noexcept
	{
		if (IsCreate())
			return true;

		// ����ǰ�ĳ�ʼ������
		if (!_CreateBefore_())
			return false;

		if (Visibled)
			m_dwStyle |= WS_VISIBLE;
		
		if (Disabled)
			m_dwStyle |= WS_DISABLED;

		m_hWnd = ::CreateWindowExW(m_dwExStyle,
								   m_ClassName,
								   _WndName,
								   m_dwStyle,
								   Rect->Left,
								   Rect->Top,
								   Rect->Width,
								   Rect->Height,
								   _ParentWnd.GetHWND(),
								   _Menu,
								   _ParentWnd.ClassLong<HINSTANCE>(GCLP_HMODULE),
								   _Param);
		if (IsCreate())
		{
			// ������ĳ�ʼ������
			if (!_CreateAfter_())
			{
				// ��ʼ��ʧ�ܣ���ʾ�����ؼ�ʧ�ܣ����ٿؼ�
				Destroy();
				return false;
			}
		}
		return IsCreate();
	}

public:
	// �ӹ�һ���ؼ����
	// @param _hWnd ��Ч�Ŀؼ�������ؼ������Ч��ΪWGUI�ؼ�����Ϊ�ӹ�ʧ�ܡ�
	// @returns �ӹܳɹ�����true��ʧ�ܷ���false������ǰ�����Ѵ�����ӹܿؼ�����ʧ�ܡ�
	inline bool Attach(HWND _hWnd)noexcept
	{
		// ����Ƿ��Ѱ��˿ؼ����
		if (WindowAPI::IsCreate())
			return false;
		
		// ����Ƿ�Ϊ��Ч�Ĵ��ھ��
		if (!::IsWindow(_hWnd))
			return false;

		// ������Ա����Ƿ����PROP_THIS���ԣ��������ʾ�þ����WGUI���ɵģ������нӹ�
		if (::GetPropW(_hWnd, PROP_THIS) != nullptr)
			return false;
		
		DWORD dwPID = 0;		// �󶨿ؼ���������ID
		DWORD dwTID = 0;		// �󶨿ؼ������߳�ID
		
		// ����Ƿ�Ϊͬһ�̺߳�ͬһ����
		dwTID = ::GetWindowThreadProcessId(_hWnd, &dwPID);
		if (dwPID != GetCurrentProcessId() || dwTID != ::GetCurrentThreadId())
			return false;

		// ��ȡ��������
		WStr ClsName(260);
		::GetClassNameW(_hWnd, ClsName, ClsName.Capacity());

		// ��������Ƿ�һ��
		if (ClsName != m_ClassName)
			return false;
		
		// ��Button�ؼ�����ϸ��
		if (m_ClassName == WC_BUTTONW)
		{
			DWORD dwStyle = ::GetWindowLongW(_hWnd, GWL_STYLE);

			if ((dwStyle & BS_PUSHBUTTON) && (m_dwStyle & BS_PUSHBUTTON))
			{}
			else if ((dwStyle & BS_AUTOCHECKBOX) && (m_dwStyle & BS_AUTOCHECKBOX))
			{}
			else if ((dwStyle & BS_AUTORADIOBUTTON) && (m_dwStyle & BS_AUTORADIOBUTTON))
			{}
			else if ((dwStyle & BS_AUTO3STATE) && (m_dwStyle & BS_AUTO3STATE))
			{}
			else if ((dwStyle & BS_GROUPBOX) && (m_dwStyle & BS_GROUPBOX))
			{}
			if ((dwStyle & BS_SPLITBUTTON) && (m_dwStyle & BS_SPLITBUTTON))
			{}
			else if ((dwStyle & BS_COMMANDLINK) && (m_dwStyle & BS_COMMANDLINK))
			{}
			else
				return false;

		}
		
		BaseWindow::m_hWnd = _hWnd;
		
		// �������Ա���PROP_THIS��ֵ
		return WindowAPI::SetPropItem(PROP_THIS, this);
	}
	
	// ����ؼ������ʹ��ǰ�����ٽӹܿؼ�
	// @returns ����ɹ����ؿؼ���������򷵻�nullptr
	inline HWND Detach()noexcept
	{
		if (!WindowAPI::IsCreate())
			return nullptr;

		HWND hWnd = m_hWnd;

		// ɾ�����Ա���PROP_THIS��ֵ
		WindowAPI::DelPropItem(PROP_THIS);
		
		// ��տؼ����
		m_hWnd = nullptr;
		
		return hWnd;
	}

	// ���ٿؼ�
	inline bool Destroy()noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (::DestroyWindow(m_hWnd))
		{
			m_hWnd = nullptr;
			Rect = { 0,0,0,0 };
			Visibled = true;
			Disabled = false;

			m_dwExStyle = 0;
			m_dwStyle = 0;
			return true;
		}
		return false;
	}
	
	// ȡָ��ID���ӿؼ�
	// @param _ID �ؼ�ID
	// @return �����ӿؼ������ָ�룬��ȡʧ�ܻ򲻴��ڷ���nullptr
	template<class _ElemCtrl = BaseWindow>
	inline _ElemCtrl* GetChildCtrl(DWORD _CtrlID)noexcept
	{
		// ��ȡ�ӿؼ��ľ��
		HWND hWndCtrl = ::GetDlgItem(BaseWindow::m_hWnd, _CtrlID);
		if (hWndCtrl == nullptr)
			return nullptr;

		// ��ȡ�ӿؼ�������
		WStr ClsName(260);
		::GetClassNameW(hWndCtrl, ClsName, ClsName.Capacity());
		
		// ��ʱ�ӿؼ�����
		_ElemCtrl TempCtrl;
		
		// ��������Ƿ�һ��
		if (TempCtrl.BaseWindow::m_ClassName != m_ClassName)
			return false;

		// ��Button�ؼ�����ϸ��
		if (ClsName == WC_BUTTONW)
		{
			DWORD dwStyle = ::GetWindowLongW(hWndCtrl, GWL_STYLE);
			
			if ((dwStyle & BS_PUSHBUTTON) && (TempCtrl.BaseWindow::m_dwStyle & BS_PUSHBUTTON))
			{}
			else if ((dwStyle & BS_AUTOCHECKBOX) && (TempCtrl.BaseWindow::m_dwStyle & BS_AUTOCHECKBOX))
			{}
			else if ((dwStyle & BS_AUTORADIOBUTTON) && (TempCtrl.BaseWindow::m_dwStyle & BS_AUTORADIOBUTTON))
			{}
			else if ((dwStyle & BS_AUTO3STATE) && (TempCtrl.BaseWindow::m_dwStyle & BS_AUTO3STATE))
			{}
			else if ((dwStyle & BS_GROUPBOX) && (TempCtrl.BaseWindow::m_dwStyle & BS_GROUPBOX))
			{}
			if ((dwStyle & BS_SPLITBUTTON) && (TempCtrl.BaseWindow::m_dwStyle & BS_SPLITBUTTON))
			{}
			else if ((dwStyle & BS_COMMANDLINK) && (TempCtrl.BaseWindow::m_dwStyle & BS_COMMANDLINK))
			{}
			else
				return nullptr;
		}
		
		// ��ȡ����ָ��
		return (_ElemCtrl*)::GetPropW(hWndCtrl, PROP_THIS);
	}

	// ��ȡ���ؼ�����
	// @return ���ظ��ؼ������ָ�룬��ȡʧ�ܻ򲻴��ڷ���nullptr
	template<class _ElemCtrl = BaseWindow>
	inline _ElemCtrl* GetParentCtrl()const noexcept
	{
		HWND hWndParent = ::GetParent(BaseWindow::m_hWnd);
		if (hWndParent == nullptr)
			return nullptr;
		
		return (_ElemCtrl*)::GetPropW(hWndParent, PROP_THIS);
	}

	// ���ĵ�ǰ�ؼ�����ĸ��ؼ�����
	// @param _ParentWnd ���ؼ�����
	// @return �����Ƿ���ĳɹ�
	template<class _ElemCtrl = BaseWindow>
	inline bool SetParentCtrl(const _ElemCtrl& _ParentCtrl)noexcept
	{
		if (!_ParentCtrl.IsCreate())
			return false;
		
		if (::SetParent(BaseWindow::m_hWnd, _ParentCtrl.GetHWND()))
			return true;
		return false;
	}
	
private:
	// �ؼ�����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept = 0;
	
	// �ؼ�������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept = 0;

protected:
	HWND m_hWnd;

	DWORD m_dwExStyle;
	DWORD m_dwStyle;
	WStr  m_ClassName;

	// ����thisָ���������
	static constexpr WCHAR PROP_THIS[8]{ L"3162JH4" };

	// ����HFONT��������
	static constexpr WCHAR PROP_FONT[8]{ L"2VBGG23" };
};

_WGUI_END

#endif // !_WGUI_BASE_WINDOW_
