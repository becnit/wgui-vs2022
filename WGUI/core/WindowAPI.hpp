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

	inline ~WindowAPI()noexcept
	{}

public:

#pragma region �ؼ��߽�ͳߴ�
	
	// ��ȡ�ؼ��ı߽����
	// @returns ���ص�ǰ�ؼ�����ڸ��ؼ��ı߽����
	inline RECT GetRect()const
	{
		RECT rc, rc2;
		HWND hWndParent = ::GetParent(m_hWnd);

		::GetWindowRect(m_hWnd, &rc);
		if (hWndParent)
		{
			// ������ڸ��ؼ������������ڸ����ڵ�����
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
	
	// ��ȡ�ؼ��ߴ�
	// @returns ���ؿؼ��ߴ磬�����ǿͻ����ĳߴ�
	inline SIZE GetSize()const noexcept
	{
		RECT rc = GetRect();
		return { rc.right - rc.left,rc.bottom - rc.top };
	}
	
	// ��ȡ�ؼ��ͻ����ı߽����
	// @returns ���ص�ǰ�ؼ��ͻ����ı߽����
	inline RECT GetClientRect() const
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		return rc;
	}
	
	// ��ȡ�ؼ��Ŀͻ����ߴ�
	// @returns ���ؿؼ��Ŀͻ����ĳߴ�
	inline SIZE GetClientSize()const noexcept
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
	inline bool Move(int _X, int _Y, int _W, int _H)const noexcept
	{
		return ::MoveWindow(m_hWnd, _X, _Y, _W, _H, FALSE);
	}
	
#pragma endregion

#pragma region �ؼ��������
	
	// ���ƿ�ʼ
	// @param _PS ָ����ջ�ͼ��Ϣ�ı���
	// @returns ����ָ���ؼ���"��ʾ�豸������"���
	inline HDC Paint_Begin(PAINTSTRUCT& _PS)noexcept
	{
		return ::BeginPaint(m_hWnd, &_PS);
	}

	// ���ƽ���
	// @param _PS ��ͼ�йص���Ϣ
	// @returns ����ֵʼ���Ƿ�0
	inline void Paint_End(const PAINTSTRUCT& _PS)noexcept
	{
		::EndPaint(m_hWnd, &_PS);
	}
	
	// �������¿ؼ�
	// @returns �ɹ�����TURE
	inline BOOL Update()const noexcept
	{
		return ::UpdateWindow(m_hWnd);
	}
	
	// ����ָ������������򽫱����»���
	// @param _Rect ��������
	// @param _Erase ָ���Ƿ����������Ĭ�ϲ���
	// @returns �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE
	inline BOOL UpdateRect(RECT _Rect, BOOL _Erase = TRUE)const noexcept
	{
		return ::InvalidateRect(m_hWnd, &_Rect, _Erase);
	}

	// ȡ��ָ��������ػ����
	// @param _Rect �ػ�����
	// @returns �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE
	inline BOOL UpdateRect_Undo(RECT _Rect)const noexcept
	{
		return ::ValidateRect(m_hWnd, &_Rect);
	}

#pragma endregion

#pragma region �ؼ��������

	// ��ȡ�ؼ����
	// @returns ����ǰ�����Ƿ񴴽��˿ؼ����򷵻ص�ǰ�ؼ��ľ�������򷵻�nullptr
	inline HWND GetHWND()const noexcept
	{
		return m_hWnd;
	}

#pragma endregion

#pragma region �ؼ����Ա�

	// �ڴ��ڵ����Ա�������һ��������޸�һ��������
	// @param _KeyName �����
	// @param _Val ��ֵ
	// @returns ִ�гɹ�����TRUE�����򷵻�FALSE
	template<typename _Ty = HANDLE>
	inline bool SetPropItem(LPCWSTR _KeyName, _Ty _Val = _Ty())noexcept
	{
		return (bool)::SetPropW(m_hWnd, _KeyName, reinterpret_cast<HANDLE>(_Val));
	}

	// ɾ�����ڵ����Ա���ָ������
	// @param _KeyName �����
	// @returns ��ָ������ڣ��򷵻�ָ�����ֵ���������򷵻�nullptr
	template<typename _Ty = HANDLE>
	inline _Ty DelPropItem(LPCWSTR _KeyName)noexcept
	{
		return (_Ty)::RemovePropW(m_hWnd, _KeyName);
	}
	
	// �Ӵ��ڵ������б��м���ָ�����ֵ
	// @param _KeyName �����
	// @returns ������Ա��д�����������򷵻����ֵ�����򷵻�nullptr
	template<typename _Ty = HANDLE>
	inline _Ty GetPropItem(LPCWSTR _KeyName)const noexcept
	{
		return (_Ty)::GetPropW(m_hWnd, _KeyName);
	}

#pragma endregion

#pragma region �ؼ���Ϣ

	// �����ؼ�����Ϣ
	// @param _Index �ؼ���Ϣ������
	// @returns ����ָ�����͵Ŀؼ���Ϣ
	template <typename _Ty = LONG>
	inline _Ty WndLong(int _Index)const noexcept
	{
		return ::GetWindowLongPtrW(m_hWnd, _Index);
	}

	
	// ���Ŀؼ�����Ϣ
	// @param _Index �ؼ���Ϣ������
	// @param _NewLong �滻ֵ
	// @returns ����ָ�����͵���һ���ؼ���Ϣ
	template <typename _Ty>
	inline _Ty WndLong(int _Index, _Ty _NewLong)noexcept
	{
		return (_Ty)::SetWindowLongPtrW(m_hWnd, _Index, (LONG)_NewLong);
	}

#pragma endregion

#pragma region �ؼ�����Ϣ

	// �����ؼ������Ϣ
	// @param _Index ��������Ϣ������
	// @returns ����ָ�����͵Ĵ�������Ϣ
	template <typename _Ty = LONG>
	inline _Ty ClassLong(int _Index)const noexcept
	{
		return (_Ty)::GetClassLongPtrW(m_hWnd, _Index);
	}
	
	// ���Ŀؼ������Ϣ
	// @param _Index �ؼ�����Ϣ������
	// @param _NewLong �滻ֵ
	// @returns ����ָ�����͵���һ���ؼ�����Ϣ
	template <typename _Ty>
	inline _Ty ClassLong(int _Index, _Ty _NewLong)noexcept
	{
		return (_Ty)::SetClassLongPtrW(m_hWnd, _Index, (LONG)_NewLong);
	}

#pragma endregion

#pragma region ��Ϣ����

	// ����ָ����Ϣ����Ϣ�����У����������ء�������Ϣ������󣬵���fnCallback
	// @param _fnCallback  �ص���������Ϣ�������ô˺���
	// @param _CBParam     ���ݸ�_fnCallback�ĵ���������
	// @param _uMsg        ��Ϣ����
	// @param _wParam      ��Ϣ����
	// @param _lParam      ��Ϣ����
	// @returns ���ͳɹ�����true�����򷵻�false
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

	// ��һ����Ϣ���뵽��Ϣ������
	// @param _uMsg    ָ�������͵���Ϣ
	// @param _wParam  ָ�����ӵ���Ϣ�ض�����Ϣ
	// @param _lParam  ָ�����ӵ���Ϣ�ض�����Ϣ
	// @returns ����������óɹ������ط��㣬���������÷���ֵΪ��
	template <typename _Ty1 = WPARAM, typename _Ty2 = LPARAM>
	inline BOOL PostMsg(UINT _uMsg, _Ty1 _wParam = _Ty1(), _Ty2 _lParam = _Ty2())const noexcept
	{
		return ::PostMessageW(m_hWnd, _uMsg, (WPARAM)_wParam, (LPARAM)_lParam);
	}

	// ����һ����Ϣ���ؼ����ȴ��������
	// @param uMsg    ָ�������͵���Ϣ
	// @param wParam  ָ�����ӵ���Ϣ�ض�����Ϣ
	// @param lParam  ָ�����ӵ���Ϣ�ض�����Ϣ
	// @returns ����ָ����Ϣ����Ľ��
	template <typename _TyRetn = LRESULT, typename _Ty1 = WPARAM, typename _Ty2 = LPARAM>
	inline _TyRetn SendMsg(UINT _uMsg, _Ty1 _wParam = _Ty1(), _Ty2 _lParam = _Ty2())const noexcept
	{
		return (_TyRetn)::SendMessageW(m_hWnd, _uMsg, (WPARAM)_wParam, (LPARAM)_lParam);
	}

#pragma endregion

#pragma region ����

	// �����ؼ���Z˳��ʹ֮��ָ���ؼ����Ϸ�
	// @param _hWndInsertAfter ָ���ؼ��ľ�������ָ������ֵ��HWND_*
	// @returns �ɹ�����true��ʧ�ܷ���false
	inline bool SetZOrder(HWND _hWndInsertAfter)const noexcept
	{
		return ::SetWindowPos(m_hWnd, _hWndInsertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	
	// ������ǰ�ؼ��Ƿ��Ѵ���
	// @returns �ؼ��Ѵ�������true�����򷵻�false
	inline bool IsCreate()const noexcept
	{
		return ::IsWindow(m_hWnd);
	}

	// ��������
	// @param _Font ������
	// @returns ���óɹ�����true�����򷵻�false
	inline bool SetFont(HFONT _Font)noexcept
	{
		return WindowAPI::SendMsg(WM_SETFONT, _Font);
	}

	// ȷ���Ƿ�ΪUnicode�ؼ�
	// @returns ΪUnicode�ؼ�����true�����򷵻�false
	inline bool IsUnicode()const noexcept
	{
		return ::IsWindowUnicode(m_hWnd);
	}

	// ��ȡ�ؼ�����
	// @returns ���ؿؼ������ַ�����ʧ�ܷ��ؿ��ַ���
	inline WStr GetClsName()noexcept
	{
		WStr ClsName(260);

		::GetClassNameW(m_hWnd, ClsName, ClsName.Capacity());
		return ClsName;
	}

	// ������ǰ���̽����Ƿ��ڵ�ǰ�ؼ�
	// @returns ���̽����ڵ�ǰ�����򷵻�TRUE
	inline BOOL IsFocus()const noexcept
	{
		return m_hWnd == ::GetFocus();
	}

	// ���ü��̽��㵽��ǰ�ؼ���
	// @returns �ɹ�����TRUE
	inline HWND SetFocus()const noexcept
	{
		::SetForegroundWindow(m_hWnd);
		return ::SetFocus(m_hWnd);
	}

	// ���ÿؼ�����ʾ״̬
	// @param _ShowMode ָ���ؼ������ʾ
	// @returns ����ؼ�֮ǰ�ɼ����򷵻�ֵΪ���㡣����ؼ�֮ǰ�����أ��򷵻�ֵΪ��
	inline BOOL Show(int _ShowMode)const noexcept
	{
		return ::ShowWindow(m_hWnd, _ShowMode);
	}

	// ���ô�����ʾ������ʱ��������Ч��
	// @param _Flags ������AW_*
	// @param _Time ����ʱ����Ĭ��Ϊ200ms
	// @returns ���óɹ�true,���򷵻�false
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
