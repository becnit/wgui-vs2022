#pragma once
#ifndef _WGUI_CHECKBOX_
#define _WGUI_CHECKBOX_

#include "BaseButton.hpp"

_WGUI_BEGIN


// ��ѡ��
class CheckBox
	: public BaseButton<CheckBox>
{
public:
	inline CheckBox()noexcept
		: BaseButton<CheckBox>(BS_AUTOCHECKBOX)
		, Checked(PROPERTY_INIT(CheckBox))
	{
		HorzAlignMode = AlignMode::Near;
	}

	virtual ~CheckBox()noexcept
	{}
	
protected:
	// �ؼ�����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseButton::_CreateBefore_())
			return false;

		return true;
	}

	// �ؼ�������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseButton::_CreateAfter_())
			return false;

		// ָ����ʼѡ��״̬
		WindowAPI::SendMsg(BM_SETCHECK, Checked.value ? BST_CHECKED : BST_UNCHECKED);

		// ���û���ĳ�ʼ������
		return BaseButton::_CreateAfter_();
	}

private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;

		if (_Symbol == Checked.symbol)
		{
			// ��ȡѡ��״̬
			Checked.value = WindowAPI::SendMsg(BM_GETCHECK) == BST_CHECKED;
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == Checked.symbol)
		{
			// ����ѡ��״̬
			WindowAPI::SendMsg(BM_SETCHECK, Checked.value ? BST_CHECKED : BST_UNCHECKED);
		}
		else return false;
		return true;
	}

public:
	// ������ָ���ؼ���ѡ��״̬
	_property<CheckBox, bool> Checked;
};

_WGUI_END


#endif // !_WGUI_CHECKBOX_
