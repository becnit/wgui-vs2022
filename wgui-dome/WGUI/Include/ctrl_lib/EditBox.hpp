#pragma once
#ifndef _WGUI_EDITBOX_
#define _WGUI_EDITBOX_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

enum class EditBoxType : DWORD
{
	Default		= 0x00000000L,	// Ĭ������
	Password	= 0x00000020L,	// �������ͣ�ES_PASSWORD
	Number		= 0x00002000L,	// �������ͣ�ES_NUMBER
};

using EditBoxScrollBar = CtrlScrollBar;
using EditBoxBorder = CtrlBorder;

// �༭��
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

	// �����ؼ�
	// @param _ParentWnd  ���ؼ�����
	// @param _InitText   ��ʼ�ı�
	// @returns �ɹ�����true��ʧ�ܷ���false
	virtual bool Create(const BaseWindow& _ParentWnd, const WStr& _InitText = L"")noexcept
	{
		return BaseControl::Create(_ParentWnd, _InitText);
	}

#pragma region ���ݲ���

	// ��ָ�����ı��滻ѡ�����ı������ޱ�ѡ���ı������ڹ��λ�ò����ı�
	// @param _Text  �����滻���ı�
	// @returns �޸ĳɹ�����true�����򷵻�false
	inline bool ModifySelText(LPCTSTR _Text)
	{
		if (_Text == nullptr)
			return false;

		// WindowAPI::SetFocus();
		WindowAPI::SendMsg(EM_REPLACESEL, TRUE, _Text);
		return true;
	}

	/// <summary>
	/// ��ȡ�༭���ı�����
	/// </summary>
	/// <returns></returns>
	inline UINT GetLength()
	{
		return WindowAPI::SendMsg(WM_GETTEXTLENGTH);
	}

	/// <summary>
	/// ��ָ��λ�ò����ı�
	/// </summary>
	/// <param name="_Index">����λ��</param>
	/// <param name="_Text">���ڲ�����ı�</param>
	/// <returns>����ɹ�����true��ʧ�ܷ���false</returns>
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
	/// ��ָ��λ�ò����ı�
	/// </summary>
	/// <param name="_Row">ָ������λ�õ��к�</param>
	/// <param name="_Column">ָ������λ�õ��к�</param>
	/// <param name="_Text">���ڲ�����ı�</param>
	/// <returns>����ɹ�����true��ʧ�ܷ���false</returns>
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
	/// ��β�������ı�
	/// </summary>
	/// <param name="_Text">Ҫ������ı�</param>
	/// <returns>��ӳɹ�����true��ʧ�ܷ���false</returns>
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
	/// ɾ���ı�
	/// </summary>
	/// <param name="_Start">ɾ���ı�����ʼλ��</param>
	/// <param name="_End">ɾ���ı��Ľ���λ�ã�-1��ʾ��β��</param>
	inline void Clear(UINT _Start = 0, UINT _End = -1)
	{
		WindowAPI::SendMsg(EM_SETSEL, _Start, _End);
		ModifySelText(TEXT(""));
	}

	/// <summary>
	/// ��ȡ�༭���ı�����
	/// </summary>
	/// <returns>���غ��б༭��ȫ�����ݵ��ַ���ָ��</returns>
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
	/// ���ñ༭����ı�����
	/// </summary>
	/// <param name="_Str"></param>
	/// <returns>�ɹ����÷���true�����򷵻�false</returns>
	inline bool SetText(LPCTSTR _Str)
	{
		return ::SetWindowTextW(BaseWindow::m_hWnd, _Str);
	}

#pragma endregion

#pragma region ��ݲ���

	/// <summary>
	/// �Ƿ���Խ��г�������
	/// </summary>
	/// <returns>���Գ�������true�������Գ�������false</returns>
	inline bool CanUndo()
	{
		return WindowAPI::SendMsg(EM_CANUNDO);
	}

	/// <summary>
	/// ��ճ���������������
	/// </summary>
	inline void EmptyUndo()
	{
		WindowAPI::SendMsg(EM_EMPTYUNDOBUFFER);
	}

	/// <summary>
	/// ��������һ��
	/// </summary>
	/// <returns>�ɹ���������true�����򷵻�false</returns>
	inline bool Undo()
	{
		return WindowAPI::SendMsg(EM_UNDO);
	}

	/// <summary>
	/// ����ѡ�е��ı���������
	/// </summary>
	inline void Copy()
	{
		WindowAPI::SendMsg(WM_COPY);
	}

	/// <summary>
	/// �ڵ�ǰλ��ճ�������������
	/// </summary>
	inline void Paste()
	{
		WindowAPI::SendMsg(WM_PASTE);
	}

	/// <summary>
	/// ����ѡ�е��ı���������
	/// </summary>
	inline void Cut()
	{
		WindowAPI::SendMsg(WM_CUT);
	}

	/// <summary>
	/// ȫѡ�ı�
	/// </summary>
	inline void AllSel()
	{
		WindowAPI::SendMsg(EM_SETSEL, 0, -1);
	}

#pragma endregion

#pragma region ����

	/// <summary>
	/// ��ȡ��������
	/// </summary>
	/// <returns>���غ����������ĵ��ַ���ָ��</returns>
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
	/// ��ȡ�ı� �е�����
	/// </summary>
	/// <returns></returns>
	inline UINT GetLineCount()
	{
		return WindowAPI::SendMsg(EM_GETLINECOUNT);
	}

	/// <summary>
	/// ȡ������ڵ�����λ��(�ַ�����)
	/// </summary>
	/// <param name="_Row">���չ������λ���кŵı���</param>
	/// <param name="_Column">���չ������λ���кŵı���</param>
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
	/// ��ȡ��ʽ���þ��Σ���ʽ���þ����ǿؼ����ı����Ƶ����е����ƾ���
	/// </summary>
	/// <returns></returns>
	inline RECT GetInputRect()
	{
		RECT rc;
		WindowAPI::SendMsg(EM_GETRECT, 0, &rc);
		return rc;
	}

	/// <summary>
	/// ���ø�ʽ���þ��Σ���ʽ���þ����ǿؼ����ı����Ƶ����е����ƾ���
	/// </summary>
	/// <param name = "_Rect">��ʽ���þ���</param>
	inline void SetInputRect(RECT _Rect)
	{
		WindowAPI::SendMsg(EM_SETRECT, 0, &_Rect);
	}

	/// <summary>
	/// �����ʽ���þ��Σ���ʽ���þ����ǿؼ����ı����Ƶ����е����ƾ���
	/// </summary>
	inline void ClearInputRect()
	{
		WindowAPI::SendMsg(EM_SETRECT);
	}

#pragma endregion

protected:
	// ����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ������
		if (!BaseControl::_CreateBefore_())
			return false;
		
		// ָ���ؼ�����
		BaseWindow::m_dwStyle |= (DWORD)Type.value;

		// ָ���������
		BaseWindow::m_dwStyle |= (AllowMultiLines.value ? ES_MULTILINE | ES_WANTRETURN : 0);

		// ָ���߿���ʽ
		if (Border.value == EditBoxBorder::Border)
			BaseWindow::m_dwStyle |= WS_BORDER;
		else if (Border.value == EditBoxBorder::Suuken)
			BaseWindow::m_dwExStyle |= WS_EX_STATICEDGE;
		
		// ָ��ֻ��ģʽ
		BaseWindow::m_dwStyle |= (ReadOnlyMode.value ? ES_READONLY : 0);

		// ָ��������
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

	// ������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ������
		if (!BaseControl::_CreateAfter_())
			return false;

		// ָ��������������ַ�����Ŀ
		WindowAPI::SendMsg(EM_LIMITTEXT, MaxAllowLen.value);

		// ָ�������ڸ��ַ�
		if (Type.value == EditBoxType::Password)
			WindowAPI::SendMsg(EM_SETPASSWORDCHAR, PasswordChar.value);

		// ָ��ѡ��Χ
		WindowAPI::SendMsg(EM_SETSEL, SelRange.value.Start, SelRange.value.End);
		
		// �������Բ������ؽ�ǰ��ֵ��������趨ֵ
		SelText.value = L"";
		Modify.value = false;
		
		return true;
	}

private:
	inline bool ReCreate()noexcept
	{
		// ��ȡ�����ڶ���
		BaseWindow* ParentWnd = BaseWindow::GetParentCtrl();
		if (!ParentWnd)
			return false;
		
		WStr Text = GetText();

		// ���ٿؼ�
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
			// ��ȡ�ؼ����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			// ��������
			if (BaseWindow::m_dwStyle & ES_NUMBER)
				Type.value = EditBoxType::Number;
			else if (BaseWindow::m_dwStyle & ES_PASSWORD)
				Type.value = EditBoxType::Password;
			else Type.value = EditBoxType::Default;
		}
		else if (_Symbol == AllowMultiLines.symbol)
		{
			// ����Ƿ�Ϊ�������
			AllowMultiLines.value = (WindowAPI::WndLong(GWL_STYLE) & ES_MULTILINE);
		}
		else if (_Symbol == Border.symbol)
		{
			// ��ȡ�ؼ����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);

			// �����ؼ��߿�����
			if (BaseWindow::m_dwStyle & WS_BORDER)
				Border.value = EditBoxBorder::Border;
			else if (BaseWindow::m_dwExStyle & WS_EX_STATICEDGE)
				Border.value = EditBoxBorder::Suuken;
			else Border.value = EditBoxBorder::Null;
		}
		else if (_Symbol == MaxAllowLen.symbol)
		{
			// ��ȡ���������
			MaxAllowLen.value = WindowAPI::SendMsg(EM_GETLIMITTEXT);
		}
		else if (_Symbol == ReadOnlyMode.symbol)
		{
			// ����Ƿ�Ϊֻ��ģʽ
			ReadOnlyMode.value = (WindowAPI::WndLong(GWL_STYLE) & ES_READONLY);
		}
		else if (_Symbol == ScrollBar.symbol)
		{
			// ��ȡ�ؼ����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			// ��������������
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
			// ��ȡ�����ڸ��ַ�
			PasswordChar.value = (wchar_t)WindowAPI::SendMsg(EM_GETPASSWORDCHAR);
		}
		else if (_Symbol == SelRange.symbol)
		{
			// ��ȡѡ��Χ
			WindowAPI::SendMsg(EM_GETSEL, &SelRange.value.Start, &SelRange.value.End);
		}
		else if (_Symbol == SelText.symbol)
		{
			// ����ѡ��Χ
			SelRange.operator const WGUI::SELECT_RANGE& ();
			 
			// ���ѡ��Χ
			if (SelRange.value.Start == SelRange.value.End)
			{
				// ѡ����ʼλ�úͽ���λ����ͬ����û��ѡ���κ����ݣ������ѡ���ı�
				SelText.value.Clear();
			}
			else if (SelRange.value.Start < 0 || SelRange.value.End < -1)
			{
				// ����ʼλ��Ϊ��������ѡ��Χ�쳣
				// ������λ��С��-1����ѡ�з�Χ�쳣
				// �����ѡ���ı�
				
				SelText.value.Clear();
			}
			else
			{
				// ��ȡ���ݻ�����ָ��
				HLOCAL hBuffer = (HLOCAL)WindowAPI::SendMsg(EM_GETHANDLE);
				
				// ������������ȡ���ַ���ָ��
				wchar_t* pBuffer = (wchar_t*)::LocalLock(hBuffer);
				
				if (pBuffer + SelRange.value.Start == nullptr || pBuffer == nullptr)
					return;
				
				// ��ȡѡ���ı�
				SelText.value = pBuffer + SelRange.value.Start;
				
				// �ض϶����ı�
				SelText.value.ResetLength(SelRange.value.End - SelRange.value.Start);

				// ����������
				::LocalUnlock(hBuffer);
			}
		}
		else if (_Symbol == Modify.symbol)
		{
			// ��ȡ�����Ƿ����޸�
			Modify.value = WindowAPI::SendMsg(EM_GETMODIFY);
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!BaseWindow::IsCreate())
			return true;

		if (_Symbol == Type.symbol)
		{
			// ��ȡ�ؼ����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			// ����ؼ����
			BaseWindow::m_dwStyle &= ~ES_NUMBER;
			BaseWindow::m_dwStyle &= ~ES_PASSWORD;
			
			// �ؽ��ؼ�
			return ReCreate();
		}
		else if (_Symbol == AllowMultiLines.symbol)
		{
			// ��ȡ�ؼ����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			
			// ����ؼ����
			BaseWindow::m_dwStyle &= ~ES_MULTILINE;

			// �ؽ��ؼ�
			return ReCreate();
		}
		else if (_Symbol == Border.symbol)
		{
			// ��ȡ�ؼ����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);
			
			// ����ؼ����
			BaseWindow::m_dwStyle &= ~WS_BORDER;
			BaseWindow::m_dwStyle &= ~WS_EX_STATICEDGE;
			
			// �ؽ��ؼ�
			return ReCreate();
		}
		else if (_Symbol == MaxAllowLen.symbol)
		{
			// �������������
			WindowAPI::SendMsg(EM_LIMITTEXT, MaxAllowLen.value);
		}
		else if (_Symbol == ReadOnlyMode.symbol)
		{
			// ����ֻ��ģʽ
			WindowAPI::SendMsg(EM_SETREADONLY, ReadOnlyMode.value);
		}
		else if (_Symbol == ScrollBar.symbol)
		{
			// ��ȡ�ؼ����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			// ����ؼ����
			BaseWindow::m_dwStyle &= ~ES_AUTOHSCROLL;
			BaseWindow::m_dwStyle &= ~ES_AUTOVSCROLL;

			// �ؽ��ؼ�
			return ReCreate();
		}
		else if (_Symbol == PasswordChar.symbol)
		{
			// ���������ַ�
			if (Type.value == EditBoxType::Password)
				WindowAPI::SendMsg(EM_SETPASSWORDCHAR, PasswordChar.value);
			else return false;
		}
		else if (_Symbol == SelRange.symbol)
		{
			// ����ѡ��Χ
			WindowAPI::SendMsg(EM_SETSEL, SelRange.value.Start, SelRange.value.End);
		}
		else if (_Symbol == SelText.symbol)
		{
			// �޸�ѡ�е��ı�
			WindowAPI::SendMsg(EM_REPLACESEL, TRUE, SelText.value.CStr());
		}
		else if (_Symbol == Modify.symbol)
		{
			// ����ָʾ�����޸ĵı��
			WindowAPI::SendMsg(EM_SETMODIFY, Modify.value);
		}
		else return false;
		return true;
	}

public:
#pragma region ����
	// ������ָ���༭�������ģʽ���ı��ֵ��ʹ�ؼ���ʧȫ������
	_property<EditBox, EditBoxType> Type;
	
	// ������ָ���Ƿ�������������ı����ı��ֵ��ʹ�ؼ���ʧȫ������
	_property<EditBox, bool> AllowMultiLines;

	// ������ָ���Ƿ���ʾ���߱߿򣬸ı��ֵ��ʹ�ؼ���ʧȫ������
	_property<EditBox, EditBoxBorder> Border;

	// ������ָ��������������ַ�����Ŀ����Ϊ0�������ַ���Ŀ�������ơ�
	_property<EditBox, UINT> MaxAllowLen;

	// ������ָ���Ƿ�Ϊֻ��ģʽ
	_property<EditBox, bool> ReadOnlyMode;

	// ������ָ�������������ͣ���<AllowMultiLines = true>ʱ��Ч���ı��ֵ��ʹ�ؼ���ʧȫ������
	_property<EditBox, EditBoxScrollBar> ScrollBar;

	// ������ָ�������ڸ��ַ�
	_property<EditBox, WCHAR> PasswordChar;

	// ������ָ����ѡ���ı�����ʼλ�úͽ���λ��
	_property<EditBox, SELECT_RANGE> SelRange;

	// ������ָ����ѡ�е��ı�
	_property<EditBox, WStr> SelText;

	// ������ָ���༭��������Ƿ����޸�
	_property<EditBox, bool> Modify;

#pragma endregion

};

_WGUI_END

#endif // !_WGUI_EDITBOX_
