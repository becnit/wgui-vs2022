#pragma once
#ifndef _WGUI_WINDOW_API_
#define _WGUI_WINDOW_API_

#include "Framework.hpp"
#include "WStr.hpp"

_WGUI_BEGIN

class WindowAPI
{
protected:
	WindowAPI(const HWND& hWnd)noexcept;

	~WindowAPI()noexcept;

public:

#pragma region �ؼ��߽�ͳߴ�
	
	// ��ȡ�ؼ��ı߽����
	// @returns ���ص�ǰ�ؼ�����ڸ��ؼ��ı߽����
	RECT GetRect()const;

	// ��ȡ�ؼ��ߴ�
	// @returns ���ؿؼ��ߴ磬�����ǿͻ����ĳߴ�
	SIZE GetSize()const noexcept;

	// ��ȡ�ؼ��ͻ����ı߽����
	// @returns ���ص�ǰ�ؼ��ͻ����ı߽����
	RECT GetClientRect() const;

	// ��ȡ�ؼ��Ŀͻ����ߴ�
	// @returns ���ؿؼ��Ŀͻ����ĳߴ�
	SIZE GetClientSize()const noexcept;

	// �ƶ�����λ��
	// @param _X  ָ�����ڵ���λ�õ�X����
	// @param _Y  ָ�����ڵ���λ�õ�Y����
	// @param _W  ָ�����ڵ��¿��
	// @param _H  ָ�����ڵ��¸߶�
	// @returns �ɹ�����true��ʧ�ܷ���false
	bool Move(int _X, int _Y, int _W, int _H)const noexcept;
	
#pragma endregion

#pragma region �ؼ��������
	
	// ���ƿ�ʼ
	// @param _PS ָ����ջ�ͼ��Ϣ�ı���
	// @returns ����ָ���ؼ���"��ʾ�豸������"���
	HDC Paint_Begin(PAINTSTRUCT& _PS)noexcept;

	// ���ƽ���
	// @param _PS ��ͼ�йص���Ϣ
	// @returns ����ֵʼ���Ƿ�0
	void Paint_End(const PAINTSTRUCT& _PS)noexcept;
	
	// �������¿ؼ�
	// @returns �ɹ�����TURE
	BOOL Update()const noexcept;
	
	// ����ָ������������򽫱����»���
	// @param _Rect ��������
	// @param _Erase ָ���Ƿ����������Ĭ�ϲ���
	// @returns �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE
	BOOL UpdateRect(RECT _Rect, BOOL _Erase = TRUE)const noexcept;

	// ȡ��ָ��������ػ����
	// @param _Rect �ػ�����
	// @returns �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE
	BOOL UpdateRect_Undo(RECT _Rect)const noexcept;

#pragma endregion

#pragma region �ؼ��������

	// ��ȡ�ؼ����
	// @returns ����ǰ�����Ƿ񴴽��˿ؼ����򷵻ص�ǰ�ؼ��ľ�������򷵻�nullptr
	HWND GetHWND()const noexcept;

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
	bool SetZOrder(HWND _hWndInsertAfter)const noexcept;

	// ������ǰ�ؼ��Ƿ��Ѵ���
	// @returns �ؼ��Ѵ�������true�����򷵻�false
	bool IsCreate()const noexcept;

	// ��������
	// @param _Font ������
	// @returns ���óɹ�����true�����򷵻�false
	bool SetFont(HFONT _Font)noexcept;

	// ȷ���Ƿ�ΪUnicode�ؼ�
	// @returns ΪUnicode�ؼ�����true�����򷵻�false
	bool IsUnicode()const noexcept;

	// ��ȡ�ؼ�����
	// @returns ���ؿؼ������ַ�����ʧ�ܷ��ؿ��ַ���
	WStr GetClsName()noexcept;

	// ������ǰ���̽����Ƿ��ڵ�ǰ�ؼ�
	// @returns ���̽����ڵ�ǰ�����򷵻�TRUE
	BOOL IsFocus()const noexcept;

	// ���ü��̽��㵽��ǰ�ؼ���
	// @returns �ɹ�����TRUE
	HWND SetFocus()const noexcept;

	// ���ÿؼ�����ʾ״̬
	// @param _ShowMode ָ���ؼ������ʾ
	// @returns ����ؼ�֮ǰ�ɼ����򷵻�ֵΪ���㡣����ؼ�֮ǰ�����أ��򷵻�ֵΪ��
	BOOL Show(int _ShowMode)const noexcept;

	// ���ô�����ʾ������ʱ��������Ч��
	// @param _Flags ������AW_*
	// @param _Time ����ʱ����Ĭ��Ϊ200ms
	// @returns ���óɹ�true,���򷵻�false
	bool Animate(DWORD _Flags, DWORD _Time = 200)const noexcept;

#pragma endregion

private:
	const HWND& m_hWnd;
};

_WGUI_END

#endif // !_WGUI_WINDOW_API_
