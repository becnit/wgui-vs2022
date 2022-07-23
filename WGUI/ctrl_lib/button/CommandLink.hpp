#pragma once
#ifndef _WGUI_COMMANDLINK_
#define _WGUI_COMMANDLINK_

#include "BaseButton.hpp"

_WGUI_BEGIN

// �������ӿؼ�
class CommandLink
	: public BaseControl<CommandLink>
	, public Attri_Caption
{

public:
	inline CommandLink()noexcept
		: BaseControl<CommandLink>(WC_BUTTONW)
		, Attri_Caption(BaseWindow::m_hWnd)
		, NoteText(PROPERTY_INIT(CommandLink))
	{
		BaseWindow::m_dwStyle |= BS_COMMANDLINK;
	}
	
	virtual ~CommandLink()noexcept
	{}

	// ������ť
	virtual bool Create(const BaseWindow& _ParentWnd)noexcept
	{
		return BaseControl::Create(_ParentWnd, WStr(Caption));
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

		// ָ��˵���ı�
		WindowAPI::SendMsg(BCM_SETNOTE, 0, NoteText.value.c_str());
		std::wcout << L"Note = " << NoteText.value << std::endl;
		
		return true;
	}

private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;
		
		if (_Symbol == NoteText.symbol)
		{
			// ��ȡע���ı�

			DWORD len = WindowAPI::SendMsg(BCM_GETNOTELENGTH, 0, 0) + 1;

			if (len > NoteText.value.max_size())
				NoteText.value.resize(len);

			WindowAPI::SendMsg(BCM_GETNOTE, len, NoteText.value.data());
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == NoteText.symbol)
		{
			// ������ʾ�ı�
			return WindowAPI::SendMsg(BCM_SETNOTE, 0, NoteText.value.c_str());
		}
		else return false;
		return true;
	}

public:
	// ������ָ��˵���ı�
	_property<CommandLink, std::wstring> NoteText;
};

_WGUI_END


#endif // !_WGUI_COMMANDLINK_
