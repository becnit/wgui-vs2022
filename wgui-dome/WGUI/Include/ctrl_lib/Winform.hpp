#pragma once
#ifndef _WGUI_WINFORM_
#define _WGUI_WINFORM_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

enum class WinformBorder : WORD
{
	None,		// �ޱ߿�
	Lock,		// �����߿�
	Adjustable	// �ɵ��߿�
};

// ����ؼ�
class Winform
	: public BaseControl<Winform>
	, public Attri_Caption
{
public:
	Winform(LPCWSTR _ClassName = L"WGUIWinform")noexcept;

	~Winform()noexcept;
	
	// ��������
	// @param _ParentWnd  �����ڶ���
	// @returns �����ɹ�����true�����򷵻�false
	virtual bool Create(const BaseWindow& _ParentWnd = Winform())noexcept;

protected:
	// �����ؼ�ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept;

	// �����ؼ�ǰ�Ĳ���
	virtual bool _CreateAfter_()noexcept;

private:
	void _property_get(int _Symbol)noexcept;

	bool _property_set(int _Symbol)noexcept;

	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	WNDCLASSEXW m_Wcex;

public:
	// ������ָ�����ڵı߿����ͣ�WinformBorder::Lock
	_property<Winform, WinformBorder> Border;

	// ������ָ�����ڵ�ͼ��
	_property<Winform, HICON> Icon;

	// ������ָ���Ƿ�������С����ť
	_property<Winform, bool> MinimizeBox;

	// ������ָ���Ƿ�������󻯰�ť
	_property<Winform, bool> MaximizeBox;
	
	// ������ָ���Ƿ����ùرհ�ť
	_property<Winform, bool> CloseBox;

	// ������ָ���Ƿ�����������ʾ����ͼ�꣬Ŀǰ������Ч
	_property<Winform, bool> ShowInTaskbar;
	
};

// ��Ϣ��ȡ�����롢�ַ���ѭ��
// @return �����̴߳��ڵ��˳�ֵ
WPARAM MessageLoop()noexcept;

_WGUI_END

#endif // !_WGUI_WINFORM_
