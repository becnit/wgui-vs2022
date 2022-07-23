#pragma once
#ifndef _WGUI_BASECONTROL_
#define _WGUI_BASECONTROL_

#include "BaseWindow.hpp"

_WGUI_BEGIN

// 控件基础ID
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
};


enum class CtrlBorder : DWORD
{
	Null	= 0x00000000L,	// 没有边框
	Border	= 0x00800000L,	// 单线边框，WS_BORDER
	Suuken	= 0x00020000L,	// 凹陷边框，WS_EX_STATICEDGE
};

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

_WGUI_END


#endif // !_WGUI_BASECONTROL_
