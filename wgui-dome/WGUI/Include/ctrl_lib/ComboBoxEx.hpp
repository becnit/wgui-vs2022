#pragma once
#ifndef _WGUI_COMBOBOXEX_
#define _WGUI_COMBOBOXEX_

#include "../BaseControl.hpp"
#include "../ImageList.hpp"

_WGUI_BEGIN

class ComboBoxEx
	: public BaseControl<ComboBoxEx>
{
public:
	inline ComboBoxEx()
		: BaseControl<ComboBoxEx>(WC_COMBOBOXEXW)
		, Image(16, 16, ILC_MASK | ILC_COLORDDB)
	{
		BaseWindow::m_dwStyle |= (CBS_DROPDOWNLIST);
	}

	// 获取项目数量
	inline int GetItemCount()const noexcept
	{
		return WindowAPI::SendMsg(CB_GETCOUNT);
	}
	
	// 在尾部添加新的项目
	// @param _Indent    新项目的缩进
	// @param _Text      新项目的文本
	// @param _Data      新项目的32位附加数据
	// @param _Image     通常时的图标索引
	// @param _SelImage  选中时的图标索引
	// @returns 添加成功返回新项目的索引，失败则返回-1
	template<typename _Ty = INT32>
	INT32 AddItem(int _Indent, LPCWSTR _Text, _Ty _Data = 0, int _Image = -1, int _SelImage = -1)
	{
		COMBOBOXEXITEMW Item;
	
		Item.mask = CBEIF_TEXT | CBEIF_INDENT;

		if (_Image != -1)
			Item.mask |= CBEIF_IMAGE;
		
		if (_SelImage != -1)
			Item.mask |= CBEIF_SELECTEDIMAGE;

		if (_Data != _Ty())
			Item.mask |= CBEIF_LPARAM;
		
		Item.iIndent = _Indent;
		Item.pszText = (LPWSTR)_Text;
		Item.cchTextMax = wcslen(_Text) + 1;
		Item.lParam = (LPARAM)_Data;
		Item.iImage = _Image;
		Item.iSelectedImage = _SelImage;
		Item.iItem = GetItemCount();
		
		return WindowAPI::SendMsg(CBEM_INSERTITEMW, 0, &Item);
	}
	
protected:
	virtual bool _CreateAfter_()noexcept
	{
		if (!BaseControl::_CreateAfter_())
			return false;

		if (Image.IsCreate())
			WindowAPI::SendMsg(CBEM_SETIMAGELIST, 0, Image.GetHandle());
		
		HWND hWndCB = WindowAPI::SendMsg<HWND>(CBEM_GETCOMBOCONTROL);
		if (hWndCB)
			::SendMessageW(hWndCB, CB_SETITEMHEIGHT, 0, 150);
		
		return true;
	}
	
public:
	// 本属性指定图像列表
	ImageList Image;
};

_WGUI_END

#endif // !_WGUI_COMBOBOXEX_
