#pragma once
#ifndef _WGUI_SPLITBUTTON_
#define _WGUI_SPLITBUTTON_

#include "BaseButton.hpp"

_WGUI_BEGIN

// ��ְ�ť�ؼ�
class SplitButton
	: public BaseButton<SplitButton>
{
public:
	inline SplitButton()noexcept
		: BaseButton<SplitButton>(BS_SPLITBUTTON)
	{
	}

	virtual ~SplitButton()noexcept
	{}

protected:
	// �ؼ�����ǰ�ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateBefore_())
			return false;

		return true;
	}

	// �ؼ�������ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateAfter_())
			return false;

		return true;
	}
	
public:
	
	// ������ָ������Ŀ
	// _property<SplitButton, SPLIT_ITEM> Item;
	
};

_WGUI_END

#endif // !_WGUI_SPLITBUTTON_
