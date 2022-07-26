#include "../../Include/ctrl_lib/Winform.hpp"

_WGUI_BEGIN

Winform::Winform(LPCWSTR _ClassName)noexcept
	: BaseControl<Winform>(_ClassName)
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
}

Winform::~Winform()noexcept
{}

// ��������
// @param _ParentWnd  �����ڶ���
// @returns �����ɹ�����true�����򷵻�false
bool Winform::Create(const BaseWindow& _ParentWnd)noexcept
{
	return BaseWindow::Create(WStr(Caption), _ParentWnd, (HMENU)BaseCtrlID::m_BaseID, (LPVOID)this);
}

// �����ؼ�ǰ�ĳ�ʼ������
bool Winform::_CreateBefore_()noexcept
{
#pragma region ע�ᴰ����

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

#pragma region ��ʼ�����ڷ��

	// ͨ�õĴ��ڷ��
	BaseWindow::m_dwStyle |= WS_OVERLAPPED;

	// ���ñ߿�����
	switch (Border)
	{
#pragma warning(disable : 26819)
		case WinformBorder::Adjustable: // �ɵ��߿�
			BaseWindow::m_dwStyle |= (WS_THICKFRAME | WS_CAPTION | WS_SYSMENU);
			break;

		case WinformBorder::Lock:	// �����߿�
			BaseWindow::m_dwStyle |= (WS_CAPTION | WS_SYSMENU);
			break;

		case WinformBorder::None:	// �ޱ߿�
			BaseWindow::m_dwStyle |= CW_USEDEFAULT;
			break;

#pragma warning(default : 26819)
	}

	// �Ƿ�����С����ť
	BaseWindow::m_dwStyle |= MinimizeBox ? WS_MINIMIZEBOX : 0;

	// �Ƿ�����󻯰�ť
	BaseWindow::m_dwStyle |= MaximizeBox ? WS_MAXIMIZEBOX : 0;

	// �Ƿ�����������ʾ����ͼ��
	BaseWindow::m_dwExStyle |= ShowInTaskbar ? 0 : WS_EX_TOOLWINDOW;

#pragma endregion

	return true;
}

// �����ؼ�ǰ�Ĳ���
bool Winform::_CreateAfter_()noexcept
{
	return true;
}

void Winform::_property_get(int _Symbol)noexcept
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

bool Winform::_property_set(int _Symbol)noexcept
{
	// ����true��ʾ������ֵ�ɹ�

	if (!WindowAPI::IsCreate())
		return true;

	if (_Symbol == Border.symbol)
	{
		BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
		BaseWindow::m_dwStyle &= ~(WS_THICKFRAME);
		switch (Border.value)
		{
			case WinformBorder::Adjustable: // �ɵ��߿�
				BaseWindow::m_dwStyle |= (WS_THICKFRAME | WS_CAPTION);
				break;

			case WinformBorder::Lock:	// �����߿�
				BaseWindow::m_dwStyle |= (WS_CAPTION);
				break;

			case WinformBorder::None:	// �ޱ߿�
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
			::AppendMenuW(hMenu, MF_BYCOMMAND, SC_MINIMIZE, L"��С��");
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
			::AppendMenuW(hMenu, MF_BYCOMMAND, SC_MAXIMIZE, L"���");
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
			::AppendMenuW(hMenu, MF_BYCOMMAND, SC_CLOSE, L"�ر�");
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

LRESULT CALLBACK Winform::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	Winform* wf = nullptr;

	if (uMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT lpCS = reinterpret_cast<LPCREATESTRUCT>(lParam);

		// ����this���������Ա���
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

			// ������Ϣ������true��ʾ�Ѵ������Ϣ
			 if (wf->BaseControl::CallHandleProc(uMsg, wParam, lParam, retn_val))
			 	return retn_val;
		}

		switch (uMsg)
		{
			case WM_DESTROY:
				::PostQuitMessage(wParam);
				return 0;

			case WM_SETFONT:	// �������������������Ա���
				::SetPropW(hWnd, BaseWindow::PROP_FONT, reinterpret_cast<HANDLE>(wParam));
				break;

			case WM_GETFONT:	// �Ӵ������Ա��л�ȡ������
				return reinterpret_cast<LRESULT>(::GetPropW(hWnd, BaseWindow::PROP_FONT));
		}
	}
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}



// ��Ϣ��ȡ�����롢�ַ���ѭ��
// @return �����̴߳��ڵ��˳�ֵ
WPARAM MessageLoop()noexcept
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