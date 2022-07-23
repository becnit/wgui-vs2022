#pragma once
#ifndef _WGUI_MENU_BAR_
#define _WGUI_MENU_BAR_

#include "../core/BaseWindow.hpp"

_WGUI_BEGIN

// �˵���
class MenuItem
{
public:

	using ITEM_TYPE = DWORD;
	static constexpr ITEM_TYPE MIT_Normal	= 0x00;		// ��ͨ�˵���
	static constexpr ITEM_TYPE MIT_Line		= 0x01;		// �ָ���
	static constexpr ITEM_TYPE MIT_Sub		= 0x02;		// �Ӳ˵���
	
public:
	inline MenuItem(ITEM_TYPE _Type)noexcept
		: m_hMenu(nullptr)
		, Type(_Type)
		, Text(PROPERTY_INIT(MenuItem))
		, Icon(PROPERTY_INIT(MenuItem))
		, SelState(PROPERTY_INIT(MenuItem))
	{}

	inline ~MenuItem()noexcept
	{}

#pragma region �˵�����

	// �����˵���
	inline bool Create()noexcept
	{
		switch (Type)
		{
			case MIT_Normal:
				m_hMenu = CreateMenu();
				break;

			case MIT_Line:
				m_hMenu = CreateMenu();
				break;

			case MIT_Sub:
				m_hMenu = CreatePopupMenu();
				break;

		}
	}

	
#pragma endregion

private:
	inline void _property_get(int _Symbol)noexcept
	{
		switch (Type)
		{
			case MIT_Normal:
			{
				if (_Symbol == Text.symbol)
				{
					// ��ȡ�˵����ı�
					
				}

				break;
			}
			case MIT_Line:
				break;
			case MIT_Sub:
				break;
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{}
	

private:
	HMENU m_hMenu;
	
public:
	// ������ָ���˵������ͣ���ֵ�޷��ı�
	const ITEM_TYPE Type;

	// ������ָ���˵�����ı�
	_property<MenuItem, std::wstring> Text;
	
	// ������ָ���˵����ͼ��
	_property<MenuItem, HICON> Icon;
	
	// ������ָ���˵����ѡ��״̬
	_property<MenuItem, bool> SelState;

};

class Menu
{};



_WGUI_END

#endif // !_WGUI_MENU_BAR_
