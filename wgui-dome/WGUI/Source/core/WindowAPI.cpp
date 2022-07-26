#include "../../Include/core/WindowAPI.hpp"

_WGUI_BEGIN

inline WindowAPI::WindowAPI(const HWND& hWnd)noexcept
	: m_hWnd(hWnd)
{
	static bool bInitCOM = false;

	if (!bInitCOM)
	{
		/*
		* ICC_WIN95_CLASSES �������¿ؼ�
		*
		* ICC_BAR_CLASSES			��������״̬����������
		* ICC_CAPEDIT_CLASS
		* ICC_COOL_CLASSES
		* ICC_DATE_CLASSES			���ں�ʱ��ѡ����
		* ICC_LISTVIEW_CLASSES		�б���ͼ
		* ICC_PROGRESS_CLASS		������
		* ICC_FE_CLASSES
		* ICC_TAB_CLASSES			ѡ�
		* ICC_TOOLTIP_CLASSES		������ʾ
		* ICC_TREEVIEW_CLASSES		����ͼ
		* ICC_UPDOWN_CLASS			���ڰ�ť
		*
		*/

		INITCOMMONCONTROLSEX ICCE = { sizeof(ICCE) };

		// �������пؼ�����
		ICCE.dwICC = ICC_WIN95_CLASSES;
		bInitCOM = InitCommonControlsEx(&ICCE);
	}

}

inline WindowAPI::~WindowAPI()noexcept
{}

#pragma region �ؼ��߽�ͳߴ�

// ��ȡ�ؼ��ı߽����
// @returns ���ص�ǰ�ؼ�����ڸ��ؼ��ı߽����
inline RECT WindowAPI::GetRect()const
{
	RECT rc, rc2;
	HWND hWndParent = ::GetParent(m_hWnd);

	::GetWindowRect(m_hWnd, &rc);
	if (hWndParent)
	{
		// ������ڸ��ؼ������������ڸ����ڵ�����
		::GetWindowRect(hWndParent, &rc2);
		rc.right -= rc.left;
		rc.bottom -= rc.top;
		rc.left -= rc2.left;
		rc.top -= rc2.top;
		rc.right += rc.left;
		rc.bottom += rc.top;
	}
	return rc;
}

// ��ȡ�ؼ��ߴ�
// @returns ���ؿؼ��ߴ磬�����ǿͻ����ĳߴ�
inline SIZE WindowAPI::GetSize()const noexcept
{
	RECT rc = GetRect();
	return { rc.right - rc.left,rc.bottom - rc.top };
}

// ��ȡ�ؼ��ͻ����ı߽����
// @returns ���ص�ǰ�ؼ��ͻ����ı߽����
inline RECT WindowAPI::GetClientRect() const
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	return rc;
}

// ��ȡ�ؼ��Ŀͻ����ߴ�
// @returns ���ؿؼ��Ŀͻ����ĳߴ�
inline SIZE WindowAPI::GetClientSize()const noexcept
{
	RECT rc = GetClientRect();
	return { rc.right, rc.bottom };
}

// �ƶ�����λ��
// @param _X  ָ�����ڵ���λ�õ�X����
// @param _Y  ָ�����ڵ���λ�õ�Y����
// @param _W  ָ�����ڵ��¿��
// @param _H  ָ�����ڵ��¸߶�
// @returns �ɹ�����true��ʧ�ܷ���false
inline bool WindowAPI::Move(int _X, int _Y, int _W, int _H)const noexcept
{
	return ::MoveWindow(m_hWnd, _X, _Y, _W, _H, FALSE);
}

#pragma endregion

#pragma region �ؼ��������

// ���ƿ�ʼ
// @param _PS ָ����ջ�ͼ��Ϣ�ı���
// @returns ����ָ���ؼ���"��ʾ�豸������"���
inline HDC WindowAPI::Paint_Begin(PAINTSTRUCT& _PS)noexcept
{
	return ::BeginPaint(m_hWnd, &_PS);
}

// ���ƽ���
// @param _PS ��ͼ�йص���Ϣ
// @returns ����ֵʼ���Ƿ�0
inline void WindowAPI::Paint_End(const PAINTSTRUCT& _PS)noexcept
{
	::EndPaint(m_hWnd, &_PS);
}

// �������¿ؼ�
// @returns �ɹ�����TURE
inline BOOL WindowAPI::Update()const noexcept
{
	return ::UpdateWindow(m_hWnd);
}

// ����ָ������������򽫱����»���
// @param _Rect ��������
// @param _Erase ָ���Ƿ����������Ĭ�ϲ���
// @returns �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE
inline BOOL WindowAPI::UpdateRect(RECT _Rect, BOOL _Erase)const noexcept
{
	return ::InvalidateRect(m_hWnd, &_Rect, _Erase);
}

// ȡ��ָ��������ػ����
// @param _Rect �ػ�����
// @returns �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE
inline BOOL WindowAPI::UpdateRect_Undo(RECT _Rect)const noexcept
{
	return ::ValidateRect(m_hWnd, &_Rect);
}

#pragma endregion

#pragma region �ؼ��������

// ��ȡ�ؼ����
// @returns ����ǰ�����Ƿ񴴽��˿ؼ����򷵻ص�ǰ�ؼ��ľ�������򷵻�nullptr
inline HWND WindowAPI::GetHWND()const noexcept
{
	return m_hWnd;
}

#pragma endregion

#pragma region ����

// �����ؼ���Z˳��ʹ֮��ָ���ؼ����Ϸ�
// @param _hWndInsertAfter ָ���ؼ��ľ�������ָ������ֵ��HWND_*
// @returns �ɹ�����true��ʧ�ܷ���false
inline bool WindowAPI::SetZOrder(HWND _hWndInsertAfter)const noexcept
{
	return ::SetWindowPos(m_hWnd, _hWndInsertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

// ������ǰ�ؼ��Ƿ��Ѵ���
// @returns �ؼ��Ѵ�������true�����򷵻�false
inline bool WindowAPI::IsCreate()const noexcept
{
	return ::IsWindow(m_hWnd);
}

// ��������
// @param _Font ������
// @returns ���óɹ�����true�����򷵻�false
inline bool WindowAPI::SetFont(HFONT _Font)noexcept
{
	return WindowAPI::SendMsg(WM_SETFONT, _Font);
}

// ȷ���Ƿ�ΪUnicode�ؼ�
// @returns ΪUnicode�ؼ�����true�����򷵻�false
inline bool WindowAPI::IsUnicode()const noexcept
{
	return ::IsWindowUnicode(m_hWnd);
}

// ��ȡ�ؼ�����
// @returns ���ؿؼ������ַ�����ʧ�ܷ��ؿ��ַ���
inline WStr WindowAPI::GetClsName()noexcept
{
	WStr ClsName(260);

	::GetClassNameW(m_hWnd, ClsName, ClsName.Capacity());
	return ClsName;
}

// ������ǰ���̽����Ƿ��ڵ�ǰ�ؼ�
// @returns ���̽����ڵ�ǰ�����򷵻�TRUE
inline BOOL WindowAPI::IsFocus()const noexcept
{
	return m_hWnd == ::GetFocus();
}

// ���ü��̽��㵽��ǰ�ؼ���
// @returns �ɹ�����TRUE
inline HWND WindowAPI::SetFocus()const noexcept
{
	::SetForegroundWindow(m_hWnd);
	return ::SetFocus(m_hWnd);
}

// ���ÿؼ�����ʾ״̬
// @param _ShowMode ָ���ؼ������ʾ
// @returns ����ؼ�֮ǰ�ɼ����򷵻�ֵΪ���㡣����ؼ�֮ǰ�����أ��򷵻�ֵΪ��
inline BOOL WindowAPI::Show(int _ShowMode)const noexcept
{
	return ::ShowWindow(m_hWnd, _ShowMode);
}

// ���ô�����ʾ������ʱ��������Ч��
// @param _Flags ������AW_*
// @param _Time ����ʱ����Ĭ��Ϊ200ms
// @returns ���óɹ�true,���򷵻�false
inline bool WindowAPI::Animate(DWORD _Flags, DWORD _Time)const noexcept
{
	return ::AnimateWindow(m_hWnd, _Time, _Flags);
}

#pragma endregion

_WGUI_END