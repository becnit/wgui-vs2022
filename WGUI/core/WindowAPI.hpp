#pragma once
#ifndef _WGUI_WINDOW_API_
#define _WGUI_WINDOW_API_

#include "Framework.hpp"
#include "WStr.hpp"

_WGUI_BEGIN

class WindowAPI
{
protected:
	inline WindowAPI(const HWND& hWnd)noexcept
		: m_hWnd(hWnd)
	{
		static bool bInitCOM = false;

		if (!bInitCOM)
		{
			/*
			* ICC_WIN95_CLASSES 包含以下控件
			*
			* ICC_BAR_CLASSES			工具栏、状态栏、命令栏
			* ICC_CAPEDIT_CLASS
			* ICC_COOL_CLASSES
			* ICC_DATE_CLASSES			日期和时间选择器
			* ICC_LISTVIEW_CLASSES		列表视图
			* ICC_PROGRESS_CLASS		进度条
			* ICC_FE_CLASSES
			* ICC_TAB_CLASSES			选项卡
			* ICC_TOOLTIP_CLASSES		工具提示
			* ICC_TREEVIEW_CLASSES		树视图
			* ICC_UPDOWN_CLASS			调节按钮
			*
			*/

			INITCOMMONCONTROLSEX ICCE = { sizeof(ICCE) };

			// 启用所有控件类型
			ICCE.dwICC = ICC_WIN95_CLASSES;
			bInitCOM = InitCommonControlsEx(&ICCE);
		}
	
	}

	inline ~WindowAPI()noexcept
	{}

public:

#pragma region 控件边界和尺寸
	
	// 获取控件的边界矩形
	// @returns 返回当前控件相对于父控件的边界矩形
	inline RECT GetRect()const
	{
		RECT rc, rc2;
		HWND hWndParent = ::GetParent(m_hWnd);

		::GetWindowRect(m_hWnd, &rc);
		if (hWndParent)
		{
			// 如果存在父控件，则计算相对于父窗口的坐标
			::GetWindowRect(hWndParent, &rc2);
			rc.right	-= rc.left;
			rc.bottom	-= rc.top;
			rc.left		-= rc2.left;
			rc.top		-= rc2.top;
			rc.right	+= rc.left;
			rc.bottom	+= rc.top;
		}
		return rc;
	}
	
	// 获取控件尺寸
	// @returns 返回控件尺寸，包含非客户区的尺寸
	inline SIZE GetSize()const noexcept
	{
		RECT rc = GetRect();
		return { rc.right - rc.left,rc.bottom - rc.top };
	}
	
	// 获取控件客户区的边界矩形
	// @returns 返回当前控件客户区的边界矩形
	inline RECT GetClientRect() const
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		return rc;
	}
	
	// 获取控件的客户区尺寸
	// @returns 返回控件的客户区的尺寸
	inline SIZE GetClientSize()const noexcept
	{
		RECT rc = GetClientRect();
		return { rc.right, rc.bottom };
	}

	// 移动窗口位置
	// @param _X  指定窗口的新位置的X坐标
	// @param _Y  指定窗口的新位置的Y坐标
	// @param _W  指定窗口的新宽度
	// @param _H  指定窗口的新高度
	// @returns 成功返回true，失败返回false
	inline bool Move(int _X, int _Y, int _W, int _H)const noexcept
	{
		return ::MoveWindow(m_hWnd, _X, _Y, _W, _H, FALSE);
	}
	
#pragma endregion

#pragma region 控件界面更新
	
	// 绘制开始
	// @param _PS 指向接收绘图信息的变量
	// @returns 返回指定控件的"显示设备描述表"句柄
	inline HDC Paint_Begin(PAINTSTRUCT& _PS)noexcept
	{
		return ::BeginPaint(m_hWnd, &_PS);
	}

	// 绘制结束
	// @param _PS 绘图有关的信息
	// @returns 返回值始终是非0
	inline void Paint_End(const PAINTSTRUCT& _PS)noexcept
	{
		::EndPaint(m_hWnd, &_PS);
	}
	
	// 立即更新控件
	// @returns 成功返回TURE
	inline BOOL Update()const noexcept
	{
		return ::UpdateWindow(m_hWnd);
	}
	
	// 更新指定区域，这个区域将被重新绘制
	// @param _Rect 更新区域
	// @param _Erase 指定是否擦除背景，默认擦除
	// @returns 成功则返回TRUE，失败返回FALSE
	inline BOOL UpdateRect(RECT _Rect, BOOL _Erase = TRUE)const noexcept
	{
		return ::InvalidateRect(m_hWnd, &_Rect, _Erase);
	}

	// 取消指定区域的重绘操作
	// @param _Rect 重绘区域
	// @returns 成功则返回TRUE，失败返回FALSE
	inline BOOL UpdateRect_Undo(RECT _Rect)const noexcept
	{
		return ::ValidateRect(m_hWnd, &_Rect);
	}

#pragma endregion

#pragma region 控件句柄操作

	// 获取控件句柄
	// @returns 若当前对象是否创建了控件，则返回当前控件的句柄，否则返回nullptr
	inline HWND GetHWND()const noexcept
	{
		return m_hWnd;
	}

#pragma endregion

#pragma region 控件属性表

	// 在窗口的属性表中增加一个新项或修改一个现有项
	// @param _KeyName 项键名
	// @param _Val 项值
	// @returns 执行成功返回TRUE，否则返回FALSE
	template<typename _Ty = HANDLE>
	inline bool SetPropItem(LPCWSTR _KeyName, _Ty _Val = _Ty())noexcept
	{
		return (bool)::SetPropW(m_hWnd, _KeyName, reinterpret_cast<HANDLE>(_Val));
	}

	// 删除窗口的属性表中指定的项
	// @param _KeyName 项键名
	// @returns 若指定项存在，则返回指定项的值，不存在则返回nullptr
	template<typename _Ty = HANDLE>
	inline _Ty DelPropItem(LPCWSTR _KeyName)noexcept
	{
		return (_Ty)::RemovePropW(m_hWnd, _KeyName);
	}
	
	// 从窗口的属性列表中检索指定项的值
	// @param _KeyName 项键名
	// @returns 如果属性表中存在项键名，则返回项的值。否则返回nullptr
	template<typename _Ty = HANDLE>
	inline _Ty GetPropItem(LPCWSTR _KeyName)const noexcept
	{
		return (_Ty)::GetPropW(m_hWnd, _KeyName);
	}

#pragma endregion

#pragma region 控件信息

	// 检索控件的信息
	// @param _Index 控件信息的类型
	// @returns 返回指定类型的控件信息
	template <typename _Ty = LONG>
	inline _Ty WndLong(int _Index)const noexcept
	{
		return ::GetWindowLongPtrW(m_hWnd, _Index);
	}

	
	// 更改控件的信息
	// @param _Index 控件信息的类型
	// @param _NewLong 替换值
	// @returns 返回指定类型的上一个控件信息
	template <typename _Ty>
	inline _Ty WndLong(int _Index, _Ty _NewLong)noexcept
	{
		return (_Ty)::SetWindowLongPtrW(m_hWnd, _Index, (LONG)_NewLong);
	}

#pragma endregion

#pragma region 控件类信息

	// 检索控件类的信息
	// @param _Index 窗口类信息的类型
	// @returns 返回指定类型的窗口类信息
	template <typename _Ty = LONG>
	inline _Ty ClassLong(int _Index)const noexcept
	{
		return (_Ty)::GetClassLongPtrW(m_hWnd, _Index);
	}
	
	// 更改控件类的信息
	// @param _Index 控件类信息的类型
	// @param _NewLong 替换值
	// @returns 返回指定类型的上一个控件类信息
	template <typename _Ty>
	inline _Ty ClassLong(int _Index, _Ty _NewLong)noexcept
	{
		return (_Ty)::SetClassLongPtrW(m_hWnd, _Index, (LONG)_NewLong);
	}

#pragma endregion

#pragma region 消息发送

	// 发送指定消息到消息队列中，并立即返回。当改消息被处理后，调用fnCallback
	// @param _fnCallback  回调函数，消息处理后掉用此函数
	// @param _CBParam     传递给_fnCallback的第三个参数
	// @param _uMsg        消息类型
	// @param _wParam      消息参数
	// @param _lParam      消息参数
	// @returns 发送成功返回true，否则返回false
	template <typename _CB_Ty, typename _TyRetn = WPARAM, typename _Ty2 = LPARAM>
	inline bool SendMsgCallback(SENDASYNCPROC _fnCallback,
								_CB_Ty _CBParam,
								UINT _uMsg,
								_TyRetn _wParam = _TyRetn(),
								_Ty2 _lParam = _Ty2())const
	{
		return ::SendMessageCallbackW(m_hWnd, _uMsg, _wParam, _lParam,
									  (SENDASYNCPROC)_fnCallback, (ULONG_PTR)_CBParam) != 0;
	}

	// 将一条消息放入到消息队列中
	// @param _uMsg    指定被寄送的消息
	// @param _wParam  指定附加的消息特定的信息
	// @param _lParam  指定附加的消息特定的信息
	// @returns 如果函数调用成功，返回非零，否则函数调用返回值为零
	template <typename _Ty1 = WPARAM, typename _Ty2 = LPARAM>
	inline BOOL PostMsg(UINT _uMsg, _Ty1 _wParam = _Ty1(), _Ty2 _lParam = _Ty2())const noexcept
	{
		return ::PostMessageW(m_hWnd, _uMsg, (WPARAM)_wParam, (LPARAM)_lParam);
	}

	// 发送一条消息到控件，等待处理完成
	// @param uMsg    指定被寄送的消息
	// @param wParam  指定附加的消息特定的信息
	// @param lParam  指定附加的消息特定的信息
	// @returns 返回指定消息处理的结果
	template <typename _TyRetn = LRESULT, typename _Ty1 = WPARAM, typename _Ty2 = LPARAM>
	inline _TyRetn SendMsg(UINT _uMsg, _Ty1 _wParam = _Ty1(), _Ty2 _lParam = _Ty2())const noexcept
	{
		return (_TyRetn)::SendMessageW(m_hWnd, _uMsg, (WPARAM)_wParam, (LPARAM)_lParam);
	}

#pragma endregion

#pragma region 杂项

	// 调整控件的Z顺序，使之在指定控件的上方
	// @param _hWndInsertAfter 指定控件的句柄，或可指定以下值，HWND_*
	// @returns 成功返回true，失败返回false
	inline bool SetZOrder(HWND _hWndInsertAfter)const noexcept
	{
		return ::SetWindowPos(m_hWnd, _hWndInsertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	
	// 检索当前控件是否已创建
	// @returns 控件已创建返回true，否则返回false
	inline bool IsCreate()const noexcept
	{
		return ::IsWindow(m_hWnd);
	}

	// 设置字体
	// @param _Font 字体句柄
	// @returns 设置成功返回true，否则返回false
	inline bool SetFont(HFONT _Font)noexcept
	{
		return WindowAPI::SendMsg(WM_SETFONT, _Font);
	}

	// 确定是否为Unicode控件
	// @returns 为Unicode控件返回true，否则返回false
	inline bool IsUnicode()const noexcept
	{
		return ::IsWindowUnicode(m_hWnd);
	}

	// 获取控件类名
	// @returns 返回控件类名字符串，失败返回空字符串
	inline WStr GetClsName()noexcept
	{
		WStr ClsName(260);

		::GetClassNameW(m_hWnd, ClsName, ClsName.Capacity());
		return ClsName;
	}

	// 检索当前键盘焦点是否在当前控件
	// @returns 键盘焦点在当前窗口则返回TRUE
	inline BOOL IsFocus()const noexcept
	{
		return m_hWnd == ::GetFocus();
	}

	// 设置键盘焦点到当前控件上
	// @returns 成功返回TRUE
	inline HWND SetFocus()const noexcept
	{
		::SetForegroundWindow(m_hWnd);
		return ::SetFocus(m_hWnd);
	}

	// 设置控件的显示状态
	// @param _ShowMode 指定控件如何显示
	// @returns 如果控件之前可见，则返回值为非零。如果控件之前被隐藏，则返回值为零
	inline BOOL Show(int _ShowMode)const noexcept
	{
		return ::ShowWindow(m_hWnd, _ShowMode);
	}

	// 设置窗口显示或隐藏时产生特殊效果
	// @param _Flags 动画，AW_*
	// @param _Time 动画时长，默认为200ms
	// @returns 设置成功true,否则返回false
	inline bool Animate(DWORD _Flags, DWORD _Time = 200)const noexcept
	{
		return ::AnimateWindow(m_hWnd, _Time, _Flags);
	}

#pragma endregion

private:
	const HWND& m_hWnd;
};

_WGUI_END

#endif // !_WGUI_WINDOW_API_
