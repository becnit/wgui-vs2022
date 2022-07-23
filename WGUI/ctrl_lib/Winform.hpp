#pragma once
#ifndef _WGUI_WINFORM_
#define _WGUI_WINFORM_

#include "../core/BaseWindow.hpp"

_WGUI_BEGIN

enum class WinformBorder : WORD
{
	None,		// 无边框
	Lock,		// 锁定边框
	Adjustable	// 可调边框
};


class Winform
	: public BaseWindow
	, public Attri_Caption
{
public:
	inline Winform(WStr _ClassName = L"WGUIWinform")noexcept
		: BaseWindow()
		, Attri_Caption(BaseWindow::m_hWnd)
		, m_Wcex({ sizeof(m_Wcex) })
		, Border(PROPERTY_INIT(Winform), WinformBorder::Lock)
		, Icon(PROPERTY_INIT(Winform))
		, MinimizeBox(PROPERTY_INIT(Winform), true)
		, MaximizeBox(PROPERTY_INIT(Winform), true)
		, CloseBox(PROPERTY_INIT(Winform), true)
		, ShowInTaskbar(PROPERTY_INIT(Winform), true)
	{
		BaseWindow::m_ClassName = _ClassName;
		Rect = { CW_USEDEFAULT, CW_USEDEFAULT, 860,540 };
		WS_SIZEBOX;
	}

	inline ~Winform()noexcept
	{}
	
	// 创建窗口
	// @param _ParentWnd  父窗口对象
	// @returns 创建成功返回true，否则返回false
	virtual bool Create(const BaseWindow& _ParentWnd = Winform())noexcept
	{
		return BaseWindow::Create(WStr(Caption), _ParentWnd, nullptr, (LPVOID)this);
	}

private:
	// 创建控件前的初始化操作
	inline virtual bool _CreateBefore_()noexcept
	{
	#pragma region 注册窗口类

		m_Wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		m_Wcex.cbClsExtra = 0;
		m_Wcex.cbWndExtra = 0;
		m_Wcex.lpfnWndProc = __WndProc;
		m_Wcex.hInstance = ::GetModuleHandle(nullptr);
		m_Wcex.hIcon = Icon;
		m_Wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		m_Wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		m_Wcex.lpszMenuName = nullptr;
		m_Wcex.lpszClassName = BaseWindow::m_ClassName;
		m_Wcex.hIconSm = m_Wcex.hIcon;
		
		if (0 == ::RegisterClassExW(&m_Wcex))
			return false;
		
	#pragma endregion

	#pragma region 初始化窗口风格

		// 通用的窗口风格
		BaseWindow::m_dwStyle |= WS_OVERLAPPED;

		// 设置边框类型
		switch (Border)
		{
		#pragma warning(disable : 26819)
			case WinformBorder::Adjustable: // 可调边框
				BaseWindow::m_dwStyle |= (WS_THICKFRAME | WS_CAPTION | WS_SYSMENU);
				break;
				
			case WinformBorder::Lock:	// 锁定边框
				BaseWindow::m_dwStyle |= (WS_CAPTION | WS_SYSMENU);
				break;
				
			case WinformBorder::None:	// 无边框
				BaseWindow::m_dwStyle |= CW_USEDEFAULT;
				break;

		#pragma warning(default : 26819)
		}
				
		// 是否有最小化按钮
		BaseWindow::m_dwStyle |= MinimizeBox ? WS_MINIMIZEBOX : 0;

		// 是否有最大化按钮
		BaseWindow::m_dwStyle |= MaximizeBox ? WS_MAXIMIZEBOX : 0;
		
		// 是否在任务栏显示窗口图标
		BaseWindow::m_dwExStyle |= ShowInTaskbar ? 0 : WS_EX_TOOLWINDOW;
		
	#pragma endregion

		return true;
	}

	// 创建控件前的操作
	inline virtual bool _CreateAfter_()noexcept
	{
		return true;
	}

	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;

		if (_Symbol == Border.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			if (BaseWindow::m_dwStyle & WS_THICKFRAME)
				Border.value = WinformBorder::Adjustable;
			else if (BaseWindow::m_dwStyle & CW_USEDEFAULT)
				Border.value = WinformBorder::None;
			else
				Border.value = WinformBorder::Lock;
		}
		else if (_Symbol == Icon.symbol)
		{
			Icon.value = WindowAPI::ClassLong<HICON>(GCL_HICON);
		}
		else if (_Symbol == MinimizeBox.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			MinimizeBox.value = BaseWindow::m_dwStyle & WS_MINIMIZEBOX ? true : false;
		}
		else if (_Symbol == MaximizeBox.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			MaximizeBox.value = BaseWindow::m_dwStyle & WS_MAXIMIZEBOX ? true : false;
		}
		else if (_Symbol == CloseBox.symbol)
		{
		}
		else if (_Symbol == ShowInTaskbar.symbol)
		{
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);
			ShowInTaskbar.value = BaseWindow::m_dwExStyle & WS_EX_TOOLWINDOW ? false : true;
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		// 返回true表示设置新值成功
		
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == Border.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~(WS_THICKFRAME);
			switch (Border.value)
			{
				case WinformBorder::Adjustable: // 可调边框
					BaseWindow::m_dwStyle |= (WS_THICKFRAME | WS_CAPTION);
					break;

				case WinformBorder::Lock:	// 锁定边框
					BaseWindow::m_dwStyle |= (WS_CAPTION);
					break;

				case WinformBorder::None:	// 无边框
					BaseWindow::m_dwStyle &= ~WS_CAPTION;
					break;
			}
			WindowAPI::WndLong(GWL_STYLE, BaseWindow::m_dwStyle);
		}
		else if (_Symbol == Icon.symbol)
		{
			WindowAPI::ClassLong(GCL_HICON, Icon.value);
		}
		else if (_Symbol == MinimizeBox.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~WS_MINIMIZEBOX;
			
			HMENU hMenu = ::GetSystemMenu(BaseWindow::m_hWnd, false);
			
			if (!MinimizeBox.value)
			{
				::RemoveMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
			}
			else
			{
				::AppendMenuW(hMenu, MF_BYCOMMAND, SC_MINIMIZE, L"最小化");
			}
			WindowAPI::WndLong(GWL_STYLE, BaseWindow::m_dwStyle | (MinimizeBox.value ? WS_MINIMIZEBOX : 0));
			::DrawMenuBar(BaseWindow::m_hWnd);
			::DestroyMenu(hMenu);
		}
		else if (_Symbol == MaximizeBox.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~WS_MAXIMIZEBOX;

			HMENU hMenu = ::GetSystemMenu(BaseWindow::m_hWnd, false);

			if (!MaximizeBox.value)
			{
				::RemoveMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
			}
			else
			{
				::AppendMenuW(hMenu, MF_BYCOMMAND, SC_MAXIMIZE, L"最大化");
			}
			WindowAPI::WndLong(GWL_STYLE, BaseWindow::m_dwStyle | (MaximizeBox.value ? WS_MAXIMIZEBOX : 0));
			::DrawMenuBar(BaseWindow::m_hWnd);
			::DestroyMenu(hMenu);
		}
		else if (_Symbol == CloseBox.symbol)
		{
			HMENU hMenu = ::GetSystemMenu(BaseWindow::m_hWnd, false);
			if (!CloseBox.value)
			{
				::RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
			}
			else
			{
				::AppendMenuW(hMenu, MF_BYCOMMAND, SC_CLOSE, L"关闭");
			}
			::DrawMenuBar(BaseWindow::m_hWnd);
			::DestroyMenu(hMenu);
		}
		else if (_Symbol == ShowInTaskbar.symbol)
		{
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);
			BaseWindow::m_dwExStyle &= ~WS_EX_TOOLWINDOW;
			WindowAPI::WndLong(GWL_EXSTYLE, BaseWindow::m_dwExStyle | (ShowInTaskbar ? 0 : WS_EX_TOOLWINDOW));
		}
		
		else return false;
		return true;
	}

	inline static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		Winform* wf = nullptr;

		if (uMsg == WM_NCCREATE)
		{
			LPCREATESTRUCT lpCS = reinterpret_cast<LPCREATESTRUCT>(lParam);

			// 保存this到窗口属性表中
			wf = static_cast<Winform*>(lpCS->lpCreateParams);
			wf->BaseWindow::m_hWnd = hWnd;
			wf->SetPropItem(BaseWindow::PROP_THIS, wf);
		}
		else
		{
			wf = reinterpret_cast<Winform*>(::GetPropW(hWnd, BaseWindow::PROP_THIS));

			if (wf != nullptr)
			{
				LRESULT retn_val = 0;

				// 处理消息，返回true表示已处理该消息
				// if (wf->m_HandleMsg.HandleMessage(retn_val, uMsg, wParam, lParam))
				// 	return retn_val;
			}

			switch (uMsg)
			{
				case WM_DESTROY:
					::PostQuitMessage(wParam);
					return 0;

				case WM_SETFONT:	// 保存字体句柄到窗口属性表中
					::SetPropW(hWnd, BaseWindow::PROP_FONT, reinterpret_cast<HANDLE>(wParam));
					break;

				case WM_GETFONT:	// 从窗口属性表中获取字体句柄
					return reinterpret_cast<LRESULT>(::GetPropW(hWnd, BaseWindow::PROP_FONT));
			}
		}
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
protected:
	WNDCLASSEXW m_Wcex;

public:
	// 本属性指定窗口的边框类型，WinformBorder::Lock
	_property<Winform, WinformBorder> Border;

	// 本属性指定窗口的图标
	_property<Winform, HICON> Icon;

	// 本属性指定是否启用最小化按钮
	_property<Winform, bool> MinimizeBox;

	// 本属性指定是否启用最大化按钮
	_property<Winform, bool> MaximizeBox;
	
	// 本属性指定是否启用关闭按钮
	_property<Winform, bool> CloseBox;

	// 本属性指定是否在任务栏显示窗口图标，目前设置无效
	_property<Winform, bool> ShowInTaskbar;
	
};

// 消息获取、翻译、分发、循环
// @return 返回线程窗口的退出值
inline WPARAM MessageLoop()noexcept
{
	MSG Msg = { 0 };
	BOOL bRetn;

	while (bRetn = ::GetMessageW(&Msg, nullptr, 0, 0))
	{
		if (bRetn == -1)
		{
			// Error
			break;
		}
		else
		{
			TranslateMessage(&Msg);
			DispatchMessageW(&Msg);
		}
	}
	return Msg.wParam;
}

_WGUI_END

#endif // !_WGUI_WINFORM_
