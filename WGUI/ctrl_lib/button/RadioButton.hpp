#pragma once
#ifndef _WGUI_RADIOBUTTON_
#define _WGUI_RADIOBUTTON_

#include "BaseButton.hpp"

_WGUI_BEGIN

// ��ѡ��ؼ�
class RadioButton
	: public BaseButton<RadioButton>
{
public:
	inline RadioButton()noexcept
		: BaseButton<RadioButton>(BS_AUTORADIOBUTTON)
		, Checked(PROPERTY_INIT(RadioButton))
	{
		HorzAlignMode = AlignMode::Near;
	}

	virtual ~RadioButton()noexcept
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
	_property<RadioButton, bool> Checked;
	
};

_WGUI_END

#endif // !_WGUI_RADIOBUTTON_
