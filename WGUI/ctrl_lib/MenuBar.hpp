#pragma once
#ifndef _WGUI_MENU_BAR_
#define _WGUI_MENU_BAR_

#include "../core/BaseWindow.hpp"

_WGUI_BEGIN

// 菜单项
class MenuItem
{
public:

	using ITEM_TYPE = DWORD;
	static constexpr ITEM_TYPE MIT_Normal	= 0x00;		// 普通菜单项
	static constexpr ITEM_TYPE MIT_Line		= 0x01;		// 分割线
	static constexpr ITEM_TYPE MIT_Sub		= 0x02;		// 子菜单项
	
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

#pragma region 菜单函数

	// 创建菜单项
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
					// 获取菜单项文本
					
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
	// 本属性指定菜单项类型，此值无法改变
	const ITEM_TYPE Type;

	// 本属性指定菜单项的文本
	_property<MenuItem, std::wstring> Text;
	
	// 本属性指定菜单项的图标
	_property<MenuItem, HICON> Icon;
	
	// 本属性指定菜单项的选择状态
	_property<MenuItem, bool> SelState;

};

class Menu
{};



_WGUI_END

#endif // !_WGUI_MENU_BAR_
