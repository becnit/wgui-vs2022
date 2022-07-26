#pragma once
#ifndef _WGUI_3STATEBUTTON_
#define _WGUI_3STATEBUTTON_

#include "BaseButton.hpp"

_WGUI_BEGIN

// 3̬��ѡ��ؼ�
class ThreeStateButton
	: public BaseButton<ThreeStateButton>
{
public:
	inline ThreeStateButton()noexcept
		: BaseButton<ThreeStateButton>(BS_AUTO3STATE)
		, Checked(PROPERTY_INIT(ThreeStateButton))
	{
		HorzAlignMode = AlignMode::Near;
	}

	virtual ~ThreeStateButton()noexcept
	{
	}

protected:
	// �ؼ�����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateBefore_())
			return false;
		
		return true;
	}

	// �ؼ�������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateAfter_())
			return false;
		
		// ָ����ʼѡ��״̬
		WindowAPI::SendMsg(BM_SETCHECK, Checked.value);

		return true;
	}

private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;

		if (_Symbol == Checked.symbol)
		{
			// ��ȡѡ��״̬
			Checked.value = WindowAPI::SendMsg(BM_GETCHECK);
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == Checked.symbol)
		{
			// ����ѡ��״̬
			WindowAPI::SendMsg(BM_SETCHECK, Checked.value);
		}
		else return false;
		return true;
	}

public:
	
	// ������ָ���ؼ���ѡ��״̬
	// 0: δѡ��, BST_UNCHECKED
	// 1: ѡ��, BST_CHECKED
	// 2: ��ȷ��, BST_INDETERMINATE
	_property<ThreeStateButton, int> Checked;
};

_WGUI_END

#endif // !_WGUI_3STATEBUTTON_
