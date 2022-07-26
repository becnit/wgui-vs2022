#pragma once
#ifndef _WGUI_BASECONTROL_
#define _WGUI_BASECONTROL_

#include "BaseWindow.hpp"
#include "BaseControlProc.hpp"

_WGUI_BEGIN

// 控件基础ID
class BaseCtrlID
{
protected:
	inline BaseCtrlID();

protected:
	static DWORD m_BaseID;
};

// 控件基类
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
		// WS_CHILDWINDOW, 创建子窗口
		// WS_CLIPCHILDREN，父窗口不对子窗口区域进行绘制
		// WS_CLIPSIBLINGS，不对兄弟窗口进行绘制

		BaseWindow::m_dwStyle |= WS_CHILDWINDOW;
		BaseWindow::m_ClassName = _CtrlClass;
	}

public:
	virtual ~BaseControl()noexcept
	{}

	// 创建控件
	// @param _ParentWnd  父控件
	// @param _CtrlName   控件标题
	// @returns 成功创建返回true，否则返回false
	virtual bool Create(const BaseWindow& _ParentWnd, const WStr& _CtrlName = L"")noexcept
	{
		BaseWindow::Create(_CtrlName, _ParentWnd, (HMENU)m_BaseID);

		if (BaseWindow::IsCreate())
		{
			// 保存this指针到窗口属性表中
			WindowAPI::SetPropItem(BaseWindow::PROP_THIS, this);

			// 和父窗口同步字体
			HFONT hFont = _ParentWnd.SendMsg<HFONT>(WM_GETFONT);
			if (hFont)
				WindowAPI::SendMsg(WM_SETFONT, hFont);
		}
		return BaseWindow::IsCreate();
	}

	// 从现有控件句柄取出控件对象
	// @param _hWnd 控件句柄
	// @returns 成功取出返回控件对象，否则返回nullptr
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
	
	// 获取控件ID
	// @returns  若控件已创建，则返回控件ID，否则返回0
	inline DWORD GetCtrlID()const noexcept
	{
		if (!WindowAPI::IsCreate())
			return 0;
		return ::GetDlgCtrlID(m_hWnd);
	}

	// 设置控件消息处理类
	// @param _ProcObj 消息处理类
	// @returns 若设置成功，返回true，否则返回false
	inline bool SetHandleProc(BaseControlProc<_ElemCtrl>& _ProcObj)noexcept
	{
		if (!WindowAPI::IsCreate() || m_OldWndProc != nullptr)
			return false;
		
		m_OldWndProc = WindowAPI::WndLong(GWLP_WNDPROC, __CtrlProc);
		return m_OldWndProc != nullptr;
	}
	
protected:

	// 控件创建前的初始化工作
	virtual bool _CreateBefore_()noexcept
	{
		return true;
	}

	// 控件创建后的初始化工作
	virtual bool _CreateAfter_()noexcept
	{
		return true;
	}

	// 调用处理类
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

#pragma region 控件一些数据结构

// 控件边框类型
enum class CtrlBorder : DWORD
{
	Null	= 0x00000000L,	// 没有边框
	Border	= 0x00800000L,	// 单线边框，WS_BORDER
	Suuken	= 0x00020000L,	// 凹陷边框，WS_EX_STATICEDGE
};

// 控件滚动条类型
enum class CtrlScrollBar : DWORD
{
	Null = 0,						// 没有滚动条
	Horz = WS_HSCROLL,				// 水平滚动条
	Vert = WS_VSCROLL,				// 垂直滚动条
	Both = WS_HSCROLL | WS_VSCROLL,	// 垂直和水平滚动条
};

// 表项结构
template<typename _Ty = INT32>
struct TABLE_ITEM
{
	WStr Text = WStr();
	_Ty Data = _Ty();
};

// 表项结构-扩展
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
