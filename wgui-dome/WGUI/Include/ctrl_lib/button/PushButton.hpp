#pragma once
#ifndef _WGUI_PUSHBUTTON_
#define _WGUI_PUSHBUTTON_

#include "BaseButton.hpp"

_WGUI_BEGIN

// ��ť�ؼ�
class PushButton
	: public BaseButton<PushButton>
{
public:
	inline PushButton()noexcept
		: BaseButton<PushButton>(BS_PUSHBUTTON)
	{
	}

	virtual ~PushButton()noexcept
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

		return true;
	}

};

_WGUI_END


#endif // !_WGUI_PUSHBUTTON_
