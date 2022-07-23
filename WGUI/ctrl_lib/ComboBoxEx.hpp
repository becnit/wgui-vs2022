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

	// ��ȡ��Ŀ����
	inline int GetItemCount()const noexcept
	{
		return WindowAPI::SendMsg(CB_GETCOUNT);
	}
	
	// ��β������µ���Ŀ
	// @param _Indent    ����Ŀ������
	// @param _Text      ����Ŀ���ı�
	// @param _Data      ����Ŀ��32λ��������
	// @param _Image     ͨ��ʱ��ͼ������
	// @param _SelImage  ѡ��ʱ��ͼ������
	// @returns ��ӳɹ���������Ŀ��������ʧ���򷵻�-1
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
	// ������ָ��ͼ���б�
	ImageList Image;
};

_WGUI_END

#endif // !_WGUI_COMBOBOXEX_
