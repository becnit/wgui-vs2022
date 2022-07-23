#pragma once
#ifndef _WGUI_EDITBOX_
#define _WGUI_EDITBOX_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

enum class EditBoxType : DWORD
{
	Default		= 0x00000000L,	// 默认类型
	Password	= 0x00000020L,	// 密码类型，ES_PASSWORD
	Number		= 0x00002000L,	// 数字类型，ES_NUMBER
};

using EditBoxScrollBar = CtrlScrollBar;
using EditBoxBorder = CtrlBorder;

// 编辑框
class EditBox
	: public BaseControl<EditBox>
	, public Attri_Caption
{
public:

	inline EditBox()noexcept
		: BaseControl<EditBox>(WC_EDITW)
		, Attri_Caption(BaseWindow::m_hWnd)
		, AllowMultiLines(PROPERTY_INIT(EditBox), false)
		, Border(PROPERTY_INIT(EditBox), EditBoxBorder::Null)
		, MaxAllowLen(PROPERTY_INIT(EditBox), 0)
		, Type(PROPERTY_INIT(EditBox), EditBoxType::Default)
		, ReadOnlyMode(PROPERTY_INIT(EditBox), false)
		, ScrollBar(PROPERTY_INIT(EditBox),EditBoxScrollBar::Null)
		, PasswordChar(PROPERTY_INIT(EditBox), 0)
		, SelRange(PROPERTY_INIT(EditBox),{-1, 0})
		, SelText(PROPERTY_INIT(EditBox), L"")
		, Modify(PROPERTY_INIT(EditBox), false)
	{}

	inline ~EditBox()noexcept
	{}

	// 创建控件
	// @param _ParentWnd  父控件对象
	// @param _InitText   初始文本
	// @returns 成功返回true，失败返回false
	virtual bool Create(const BaseWindow& _ParentWnd, const WStr& _InitText = L"")noexcept
	{
		return BaseControl::Create(_ParentWnd, _InitText);
	}

#pragma region 内容操作

	// 用指定的文本替换选定的文本，若无被选中文本，则在光标位置插入文本
	// @param _Text  用于替换的文本
	// @returns 修改成功返回true，否则返回false
	inline bool ModifySelText(LPCTSTR _Text)
	{
		if (_Text == nullptr)
			return false;

		// WindowAPI::SetFocus();
		WindowAPI::SendMsg(EM_REPLACESEL, TRUE, _Text);
		return true;
	}

	/// <summary>
	/// 获取编辑框文本长度
	/// </summary>
	/// <returns></returns>
	inline UINT GetLength()
	{
		return WindowAPI::SendMsg(WM_GETTEXTLENGTH);
	}

	/// <summary>
	/// 在指定位置插入文本
	/// </summary>
	/// <param name="_Index">插入位置</param>
	/// <param name="_Text">用于插入的文本</param>
	/// <returns>插入成功返回true，失败返回false</returns>
	inline bool InsertText(UINT _Index, LPCTSTR _Text)
	{
		if (_Text == nullptr)
			return false;

		_Index = _Index > GetLength() ? -1 : _Index;
		WindowAPI::SendMsg(EM_SETSEL, _Index, _Index);
		ModifySelText(_Text);
		return true;
	}

	/// <summary>
	/// 在指定位置插入文本
	/// </summary>
	/// <param name="_Row">指定插入位置的行号</param>
	/// <param name="_Column">指定插入位置的列号</param>
	/// <param name="_Text">用于插入的文本</param>
	/// <returns>插入成功返回true，失败返回false</returns>
	inline bool InsertText(UINT _Row, UINT _Column, LPCTSTR _Text)
	{
		if (_Text == nullptr)
			return false;

		UINT pos = 0;

		_Row = _Row > GetLineCount() ? GetLineCount() : _Row;
		pos = WindowAPI::SendMsg(EM_LINEINDEX, _Row);
		_Column = _Column > (UINT)WindowAPI::SendMsg(EM_LINELENGTH, pos) ? WindowAPI::SendMsg(EM_LINELENGTH, pos) : _Column;
		pos += _Column;

		WindowAPI::SendMsg(EM_SETSEL, pos, pos);
		ModifySelText(_Text);
		return true;
	}

	/// <summary>
	/// 在尾部加入文本
	/// </summary>
	/// <param name="_Text">要加入的文本</param>
	/// <returns>添加成功返回true，失败返回false</returns>
	inline bool AddText(LPCTSTR _Text)
	{
		if (_Text == nullptr)
			return false;

		//SelStartPos = GetLength();
		//SelTextLen = -1;
		ModifySelText(_Text);
		return true;
	}

	/// <summary>
	/// 删除文本
	/// </summary>
	/// <param name="_Start">删除文本的起始位置</param>
	/// <param name="_End">删除文本的结束位置，-1表示到尾部</param>
	inline void Clear(UINT _Start = 0, UINT _End = -1)
	{
		WindowAPI::SendMsg(EM_SETSEL, _Start, _End);
		ModifySelText(TEXT(""));
	}

	/// <summary>
	/// 获取编辑框文本内容
	/// </summary>
	/// <returns>返回含有编辑框全部内容的字符串指针</returns>
	inline WStr GetText()
	{
		WStr str(::GetWindowTextLengthW(BaseWindow::m_hWnd));

		if (str)
		{
			::GetWindowTextW(BaseWindow::m_hWnd, str, str.Capacity());
		}
		return str;
	}

	/// <summary>
	/// 设置编辑框的文本内容
	/// </summary>
	/// <param name="_Str"></param>
	/// <returns>成功设置返回true，否则返回false</returns>
	inline bool SetText(LPCTSTR _Str)
	{
		return ::SetWindowTextW(BaseWindow::m_hWnd, _Str);
	}

#pragma endregion

#pragma region 快捷操作

	/// <summary>
	/// 是否可以进行撤销操作
	/// </summary>
	/// <returns>可以撤销返回true，不可以撤销返回false</returns>
	inline bool CanUndo()
	{
		return WindowAPI::SendMsg(EM_CANUNDO);
	}

	/// <summary>
	/// 清空撤销缓冲区的数据
	/// </summary>
	inline void EmptyUndo()
	{
		WindowAPI::SendMsg(EM_EMPTYUNDOBUFFER);
	}

	/// <summary>
	/// 撤销到上一步
	/// </summary>
	/// <returns>成功撤销返回true，否则返回false</returns>
	inline bool Undo()
	{
		return WindowAPI::SendMsg(EM_UNDO);
	}

	/// <summary>
	/// 复制选中的文本到剪贴板
	/// </summary>
	inline void Copy()
	{
		WindowAPI::SendMsg(WM_COPY);
	}

	/// <summary>
	/// 在当前位置粘贴剪贴板的数据
	/// </summary>
	inline void Paste()
	{
		WindowAPI::SendMsg(WM_PASTE);
	}

	/// <summary>
	/// 剪切选中的文本到剪贴板
	/// </summary>
	inline void Cut()
	{
		WindowAPI::SendMsg(WM_CUT);
	}

	/// <summary>
	/// 全选文本
	/// </summary>
	inline void AllSel()
	{
		WindowAPI::SendMsg(EM_SETSEL, 0, -1);
	}

#pragma endregion

#pragma region 杂项

	/// <summary>
	/// 获取密码明文
	/// </summary>
	/// <returns>返回含有密码明文的字符串指针</returns>
	inline WStr GetPwdText()
	{
		WCHAR chPwd = PasswordChar;
		WStr lpStr;
		
		if (chPwd != L'\0')
		{
			PasswordChar = L'\0';
			lpStr.SetCapacity(::GetWindowTextLengthW(BaseWindow::m_hWnd));
			::GetWindowTextW(BaseWindow::m_hWnd, lpStr, lpStr.Capacity());
			PasswordChar = chPwd;
			return lpStr;
		}
		return WStr();
	}

	/// <summary>
	/// 获取文本 行的总数
	/// </summary>
	/// <returns></returns>
	inline UINT GetLineCount()
	{
		return WindowAPI::SendMsg(EM_GETLINECOUNT);
	}

	/// <summary>
	/// 取光标所在的坐标位置(字符坐标)
	/// </summary>
	/// <param name="_Row">接收光标所在位置行号的变量</param>
	/// <param name="_Column">接收光标所在位置列号的变量</param>
	/// <returns></returns>
	inline UINT GetCur(PUINT _Row = nullptr, PUINT _Column = nullptr)
	{
		WindowAPI::SetFocus();

		UINT pos_now = 0;
		UINT pos_row = 0;
		UINT pos_column = 0;

		// GetSel(&pos_now);

		if (0)
		{
			pos_row = WindowAPI::SendMsg(EM_LINEFROMCHAR, pos_now);
			pos_column = pos_now - WindowAPI::SendMsg(EM_LINEINDEX, pos_row);
		}
		else pos_row = 0, pos_column = pos_now;

		if (_Row)
			*_Row = pos_row;
		if (_Column)
			*_Column = pos_column;
		return pos_now;
	}

	/// <summary>
	/// 获取格式设置矩形，格式设置矩形是控件将文本绘制到其中的限制矩形
	/// </summary>
	/// <returns></returns>
	inline RECT GetInputRect()
	{
		RECT rc;
		WindowAPI::SendMsg(EM_GETRECT, 0, &rc);
		return rc;
	}

	/// <summary>
	/// 设置格式设置矩形，格式设置矩形是控件将文本绘制到其中的限制矩形
	/// </summary>
	/// <param name = "_Rect">格式设置矩形</param>
	inline void SetInputRect(RECT _Rect)
	{
		WindowAPI::SendMsg(EM_SETRECT, 0, &_Rect);
	}

	/// <summary>
	/// 清除格式设置矩形，格式设置矩形是控件将文本绘制到其中的限制矩形
	/// </summary>
	inline void ClearInputRect()
	{
		WindowAPI::SendMsg(EM_SETRECT);
	}

#pragma endregion

protected:
	// 创建前的初始化操作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数
		if (!BaseControl::_CreateBefore_())
			return false;
		
		// 指定控件类型
		BaseWindow::m_dwStyle |= (DWORD)Type.value;

		// 指定允许多行
		BaseWindow::m_dwStyle |= (AllowMultiLines.value ? ES_MULTILINE | ES_WANTRETURN : 0);

		// 指定边框样式
		if (Border.value == EditBoxBorder::Border)
			BaseWindow::m_dwStyle |= WS_BORDER;
		else if (Border.value == EditBoxBorder::Suuken)
			BaseWindow::m_dwExStyle |= WS_EX_STATICEDGE;
		
		// 指定只读模式
		BaseWindow::m_dwStyle |= (ReadOnlyMode.value ? ES_READONLY : 0);

		// 指定滚动条
		switch (ScrollBar.value)
		{
			case CtrlScrollBar::Horz:
				BaseWindow::m_dwStyle |= ES_AUTOHSCROLL;
				break;

			case CtrlScrollBar::Vert:
				BaseWindow::m_dwStyle |= ES_AUTOVSCROLL;
				break;
			
			case CtrlScrollBar::Both:
				BaseWindow::m_dwStyle |= (ES_AUTOHSCROLL | ES_AUTOVSCROLL);
				break;
		}
	
		return true;
	}

	// 创建后的初始化操作
	virtual bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数
		if (!BaseControl::_CreateAfter_())
			return false;

		// 指定最大允许输入字符的数目
		WindowAPI::SendMsg(EM_LIMITTEXT, MaxAllowLen.value);

		// 指定密码遮盖字符
		if (Type.value == EditBoxType::Password)
			WindowAPI::SendMsg(EM_SETPASSWORDCHAR, PasswordChar.value);

		// 指定选择范围
		WindowAPI::SendMsg(EM_SETSEL, SelRange.value.Start, SelRange.value.End);
		
		// 以下属性不能在重建前赋值，清空其设定值
		SelText.value = L"";
		Modify.value = false;
		
		return true;
	}

private:
	inline bool ReCreate()noexcept
	{
		// 获取父窗口对象
		BaseWindow* ParentWnd = BaseWindow::GetParentCtrl();
		if (!ParentWnd)
			return false;
		
		WStr Text = GetText();

		// 销毁控件
		::DestroyWindow(BaseWindow::m_hWnd);

		if (Create(*ParentWnd))
		{
			SetText(Text);
			return true;
		}
		return false;
	}
	
	inline void _property_get(int _Symbol)noexcept
	{
		if (!BaseWindow::IsCreate())
			return;

		if (_Symbol == Type.symbol)
		{
			// 获取控件风格
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			// 检索类型
			if (BaseWindow::m_dwStyle & ES_NUMBER)
				Type.value = EditBoxType::Number;
			else if (BaseWindow::m_dwStyle & ES_PASSWORD)
				Type.value = EditBoxType::Password;
			else Type.value = EditBoxType::Default;
		}
		else if (_Symbol == AllowMultiLines.symbol)
		{
			// 检查是否为允许多行
			AllowMultiLines.value = (WindowAPI::WndLong(GWL_STYLE) & ES_MULTILINE);
		}
		else if (_Symbol == Border.symbol)
		{
			// 获取控件风格
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);

			// 检索控件边框类型
			if (BaseWindow::m_dwStyle & WS_BORDER)
				Border.value = EditBoxBorder::Border;
			else if (BaseWindow::m_dwExStyle & WS_EX_STATICEDGE)
				Border.value = EditBoxBorder::Suuken;
			else Border.value = EditBoxBorder::Null;
		}
		else if (_Symbol == MaxAllowLen.symbol)
		{
			// 获取最大允许长度
			MaxAllowLen.value = WindowAPI::SendMsg(EM_GETLIMITTEXT);
		}
		else if (_Symbol == ReadOnlyMode.symbol)
		{
			// 检查是否为只读模式
			ReadOnlyMode.value = (WindowAPI::WndLong(GWL_STYLE) & ES_READONLY);
		}
		else if (_Symbol == ScrollBar.symbol)
		{
			// 获取控件风格
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			// 检索滚动条类型
			if (BaseWindow::m_dwStyle & (ES_AUTOHSCROLL | ES_AUTOVSCROLL))
				ScrollBar.value = EditBoxScrollBar::Both;
			else if (BaseWindow::m_dwStyle & ES_AUTOHSCROLL)
				ScrollBar.value = EditBoxScrollBar::Horz;
			else if (BaseWindow::m_dwStyle & ES_AUTOVSCROLL)
				ScrollBar.value = EditBoxScrollBar::Vert;
			else ScrollBar.value = EditBoxScrollBar::Null;
		}
		else if (_Symbol == PasswordChar.symbol)
		{
			// 获取密码遮盖字符
			PasswordChar.value = (wchar_t)WindowAPI::SendMsg(EM_GETPASSWORDCHAR);
		}
		else if (_Symbol == SelRange.symbol)
		{
			// 获取选择范围
			WindowAPI::SendMsg(EM_GETSEL, &SelRange.value.Start, &SelRange.value.End);
		}
		else if (_Symbol == SelText.symbol)
		{
			// 更新选择范围
			SelRange.operator const WGUI::SELECT_RANGE& ();
			 
			// 检查选择范围
			if (SelRange.value.Start == SelRange.value.End)
			{
				// 选择起始位置和结束位置相同，则没有选择任何内容，当清空选中文本
				SelText.value.Clear();
			}
			else if (SelRange.value.Start < 0 || SelRange.value.End < -1)
			{
				// 若起始位置为负数，则选择范围异常
				// 若结束位置小于-1，则选中范围异常
				// 当清空选中文本
				
				SelText.value.Clear();
			}
			else
			{
				// 获取内容缓冲区指针
				HLOCAL hBuffer = (HLOCAL)WindowAPI::SendMsg(EM_GETHANDLE);
				
				// 锁定缓冲区，取得字符串指针
				wchar_t* pBuffer = (wchar_t*)::LocalLock(hBuffer);
				
				if (pBuffer + SelRange.value.Start == nullptr || pBuffer == nullptr)
					return;
				
				// 获取选择文本
				SelText.value = pBuffer + SelRange.value.Start;
				
				// 截断多余文本
				SelText.value.ResetLength(SelRange.value.End - SelRange.value.Start);

				// 解锁缓冲区
				::LocalUnlock(hBuffer);
			}
		}
		else if (_Symbol == Modify.symbol)
		{
			// 获取内容是否已修改
			Modify.value = WindowAPI::SendMsg(EM_GETMODIFY);
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!BaseWindow::IsCreate())
			return true;

		if (_Symbol == Type.symbol)
		{
			// 获取控件风格
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			// 清除控件风格
			BaseWindow::m_dwStyle &= ~ES_NUMBER;
			BaseWindow::m_dwStyle &= ~ES_PASSWORD;
			
			// 重建控件
			return ReCreate();
		}
		else if (_Symbol == AllowMultiLines.symbol)
		{
			// 获取控件风格
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			
			// 清除控件风格
			BaseWindow::m_dwStyle &= ~ES_MULTILINE;

			// 重建控件
			return ReCreate();
		}
		else if (_Symbol == Border.symbol)
		{
			// 获取控件风格
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);
			
			// 清除控件风格
			BaseWindow::m_dwStyle &= ~WS_BORDER;
			BaseWindow::m_dwStyle &= ~WS_EX_STATICEDGE;
			
			// 重建控件
			return ReCreate();
		}
		else if (_Symbol == MaxAllowLen.symbol)
		{
			// 设置最大允许长度
			WindowAPI::SendMsg(EM_LIMITTEXT, MaxAllowLen.value);
		}
		else if (_Symbol == ReadOnlyMode.symbol)
		{
			// 开关只读模式
			WindowAPI::SendMsg(EM_SETREADONLY, ReadOnlyMode.value);
		}
		else if (_Symbol == ScrollBar.symbol)
		{
			// 获取控件风格
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			// 清除控件风格
			BaseWindow::m_dwStyle &= ~ES_AUTOHSCROLL;
			BaseWindow::m_dwStyle &= ~ES_AUTOVSCROLL;

			// 重建控件
			return ReCreate();
		}
		else if (_Symbol == PasswordChar.symbol)
		{
			// 设置密码字符
			if (Type.value == EditBoxType::Password)
				WindowAPI::SendMsg(EM_SETPASSWORDCHAR, PasswordChar.value);
			else return false;
		}
		else if (_Symbol == SelRange.symbol)
		{
			// 设置选择范围
			WindowAPI::SendMsg(EM_SETSEL, SelRange.value.Start, SelRange.value.End);
		}
		else if (_Symbol == SelText.symbol)
		{
			// 修改选中的文本
			WindowAPI::SendMsg(EM_REPLACESEL, TRUE, SelText.value.CStr());
		}
		else if (_Symbol == Modify.symbol)
		{
			// 设置指示内容修改的标记
			WindowAPI::SendMsg(EM_SETMODIFY, Modify.value);
		}
		else return false;
		return true;
	}

public:
#pragma region 属性
	// 本属性指定编辑框的输入模式，改变此值会使控件丢失全部内容
	_property<EditBox, EditBoxType> Type;
	
	// 本属性指定是否允许输入多行文本，改变此值会使控件丢失全部内容
	_property<EditBox, bool> AllowMultiLines;

	// 本属性指定是否显示单线边框，改变此值会使控件丢失全部内容
	_property<EditBox, EditBoxBorder> Border;

	// 本属性指定最大允许输入字符的数目，如为0则输入字符数目不受限制。
	_property<EditBox, UINT> MaxAllowLen;

	// 本属性指定是否为只读模式
	_property<EditBox, bool> ReadOnlyMode;

	// 本属性指定滚动条的类型，仅<AllowMultiLines = true>时有效，改变此值会使控件丢失全部内容
	_property<EditBox, EditBoxScrollBar> ScrollBar;

	// 本属性指定密码遮盖字符
	_property<EditBox, WCHAR> PasswordChar;

	// 本属性指定所选择文本的起始位置和结束位置
	_property<EditBox, SELECT_RANGE> SelRange;

	// 本属性指定被选中的文本
	_property<EditBox, WStr> SelText;

	// 本属性指定编辑框的内容是否已修改
	_property<EditBox, bool> Modify;

#pragma endregion

};

_WGUI_END

#endif // !_WGUI_EDITBOX_
