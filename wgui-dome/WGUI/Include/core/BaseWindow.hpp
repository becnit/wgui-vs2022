#pragma once
#ifndef _WGUI_BASE_WINDOW_
#define _WGUI_BASE_WINDOW_

#include "GeneralAttributes.hpp"
#include "WindowAPI.hpp"
#include "Alignment.hpp"

_WGUI_BEGIN

// 控件基类
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

	// 创建
	inline bool Create(const WStr& _WndName,
					   const BaseWindow& _ParentWnd,
					   HMENU _Menu,
					   LPVOID _Param = nullptr)noexcept
	{
		if (IsCreate())
			return true;

		// 创建前的初始化操作
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
			// 创建后的初始化操作
			if (!_CreateAfter_())
			{
				// 初始化失败，表示创建控件失败，销毁控件
				Destroy();
				return false;
			}
		}
		return IsCreate();
	}

public:
	// 接管一个控件句柄
	// @param _hWnd 有效的控件句柄。控件句柄无效或为WGUI控件，视为接管失败。
	// @returns 接管成功返回true，失败返回false。若当前对象已创建或接管控件，则失败。
	inline bool Attach(HWND _hWnd)noexcept
	{
		// 检查是否已绑定了控件句柄
		if (WindowAPI::IsCreate())
			return false;
		
		// 检查是否为有效的窗口句柄
		if (!::IsWindow(_hWnd))
			return false;

		// 检查属性表中是否存在PROP_THIS属性，存在则表示该句柄由WGUI生成的，不进行接管
		if (::GetPropW(_hWnd, PROP_THIS) != nullptr)
			return false;
		
		DWORD dwPID = 0;		// 绑定控件所属进程ID
		DWORD dwTID = 0;		// 绑定控件所属线程ID
		
		// 检查是否为同一线程和同一进程
		dwTID = ::GetWindowThreadProcessId(_hWnd, &dwPID);
		if (dwPID != GetCurrentProcessId() || dwTID != ::GetCurrentThreadId())
			return false;

		// 获取窗口类名
		WStr ClsName(260);
		::GetClassNameW(_hWnd, ClsName, ClsName.Capacity());

		// 检查类名是否一致
		if (ClsName != m_ClassName)
			return false;
		
		// 对Button控件进行细分
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
		
		// 设置属性表中PROP_THIS的值
		return WindowAPI::SetPropItem(PROP_THIS, this);
	}
	
	// 分离控件句柄，使当前对象不再接管控件
	// @returns 分离成功返回控件句柄，否则返回nullptr
	inline HWND Detach()noexcept
	{
		if (!WindowAPI::IsCreate())
			return nullptr;

		HWND hWnd = m_hWnd;

		// 删除属性表中PROP_THIS的值
		WindowAPI::DelPropItem(PROP_THIS);
		
		// 清空控件句柄
		m_hWnd = nullptr;
		
		return hWnd;
	}

	// 销毁控件
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
	
	// 取指定ID的子控件
	// @param _ID 控件ID
	// @return 返回子控件对象的指针，获取失败或不存在返回nullptr
	template<class _ElemCtrl = BaseWindow>
	inline _ElemCtrl* GetChildCtrl(DWORD _CtrlID)noexcept
	{
		// 获取子控件的句柄
		HWND hWndCtrl = ::GetDlgItem(BaseWindow::m_hWnd, _CtrlID);
		if (hWndCtrl == nullptr)
			return nullptr;

		// 获取子控件的类名
		WStr ClsName(260);
		::GetClassNameW(hWndCtrl, ClsName, ClsName.Capacity());
		
		// 临时子控件对象
		_ElemCtrl TempCtrl;
		
		// 检查类型是否一致
		if (TempCtrl.BaseWindow::m_ClassName != m_ClassName)
			return false;

		// 对Button控件进行细分
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
		
		// 获取对象指针
		return (_ElemCtrl*)::GetPropW(hWndCtrl, PROP_THIS);
	}

	// 获取父控件对象
	// @return 返回父控件对象的指针，获取失败或不存在返回nullptr
	template<class _ElemCtrl = BaseWindow>
	inline _ElemCtrl* GetParentCtrl()const noexcept
	{
		HWND hWndParent = ::GetParent(BaseWindow::m_hWnd);
		if (hWndParent == nullptr)
			return nullptr;
		
		return (_ElemCtrl*)::GetPropW(hWndParent, PROP_THIS);
	}

	// 更改当前控件对象的父控件对象
	// @param _ParentWnd 父控件对象
	// @return 返回是否更改成功
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
	// 控件创建前的初始化工作
	virtual bool _CreateBefore_()noexcept = 0;
	
	// 控件创建后的初始化工作
	virtual bool _CreateAfter_()noexcept = 0;

protected:
	HWND m_hWnd;

	DWORD m_dwExStyle;
	DWORD m_dwStyle;
	WStr  m_ClassName;

	// 保存this指针的属性名
	static constexpr WCHAR PROP_THIS[8]{ L"3162JH4" };

	// 保存HFONT的属性名
	static constexpr WCHAR PROP_FONT[8]{ L"2VBGG23" };
};

_WGUI_END

#endif // !_WGUI_BASE_WINDOW_
