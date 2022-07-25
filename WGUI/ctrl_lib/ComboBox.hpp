#pragma once
#ifndef _WGUI_COMBOBOX_
#define _WGUI_COMBOBOX_

#include "../core/BaseControl.hpp"
#include "EditBox.hpp"

_WGUI_BEGIN

enum class ComboBoxType : DWORD
{
	Simple		= 0x0001,	// �ɱ༭�б�ʽ
	DropDwon	= 0x0002,	// �ɱ༭����ʽ
	DropList	= 0x0003		// ���ɱ༭����ʽ
};

// ��Ͽ�
class ComboBox
	: public BaseControl<ComboBox>
{
public:
	inline ComboBox()noexcept
		: BaseControl<ComboBox>(WC_COMBOBOXW)
		, Type(PROPERTY_INIT(ComboBox), ComboBoxType::DropList)
		, CurSel(PROPERTY_INIT(ComboBox), -1)
		, TopItem(PROPERTY_INIT(ComboBox), -1)
		, CueBannerText(PROPERTY_INIT(ComboBox))
		, DropWidth(PROPERTY_INIT(ComboBox))
		, DropHeight(PROPERTY_INIT(ComboBox))
		, AutoSort(PROPERTY_INIT(ComboBox))
		, EditCtrl()
	{
		BaseWindow::m_dwStyle |= (CBS_AUTOHSCROLL | CBS_HASSTRINGS);
	}

	virtual ~ComboBox()noexcept
	{}
	
	// �����ؼ�
	// @param _ParentWnd ���ؼ�����
	// @returns �����ɹ�����true�����򷵻�false
	virtual bool Create(const BaseWindow& _ParentWnd)noexcept
	{
		return BaseControl::Create(_ParentWnd);
	}

#pragma region ��Ŀ����

// ��ȡ��Ŀ����
// @returns ��ȡ�ɹ�������Ŀ���������򷵻�-1
inline int GetItemCount()const noexcept
{
	return WindowAPI::SendMsg(CB_GETCOUNT);
}

// ��β�����һ���µ���Ŀ
// @param _Text	  ��Ŀ�ı�
// @param _Data   32λ��������
// @return ��ӳɹ����ظ���Ŀ������λ�ã�ʧ�ܷ���-1
template<typename _Ty = INT32>
inline int AddItem(LPCWSTR _Text, _Ty _Data = _Ty())noexcept
{
	int Index = WindowAPI::SendMsg(CB_ADDSTRING, 0, _Text);
	if (Index != CB_ERR && _Data != _Ty())
		WindowAPI::SendMsg(CB_SETITEMDATA, 0, _Data);
	return Index;
}

// ��β�����һ�������µ���Ŀ
// @param _Items  ��Ŀ�ı�
// @returns ������ӳɹ�����Ŀ��Ŀ
inline int AddItems(const INIT_LIST<LPCWSTR>& _Items)noexcept
{
	int Count = 0;
	for (auto& Item : _Items)
	{
		if (AddItem(Item) != CB_ERR)
			Count++;
	}
	return Count;
}

// ��β�����һ�������µ���Ŀ
// @param _Items  ��Ŀ��������
// @returns ������ӳɹ�����Ŀ��Ŀ
template<typename _Ty = INT32>
inline int AddItems(const INIT_LIST<TABLE_ITEM<_Ty>>& _Items)noexcept
{
	int Count = 0;
	for (auto& Item : _Items)
	{
		if (AddItem(Item.Text, Item.Data) != CB_ERR)
			Count++;
	}
	return Count;
}

// ��ָ��λ��ǰ����һ���µ���Ŀ
// @param _Index  ָ��λ��
// @param _Text	  ��Ŀ�ı�
// @param _Data   32λ��������
// @return ����ɹ����ظ���Ŀ������λ�ã�ʧ�ܷ���-1
template<typename _Ty = INT32>
inline int InsertItem(int _Index, LPCWSTR _Text, _Ty _Data = _Ty())
{
	_Index = WindowAPI::SendMsg(CB_INSERTSTRING, _Index, _Text);
	if (_Index != CB_ERR && _Data != _Ty())
		WindowAPI::SendMsg(CB_SETITEMDATA, _Index, _Data);
	return _Index;
}

// ��ָ��λ��ǰ����һ�������µ���Ŀ
// @param _Index  ָ��λ��
// @param _Items  ��Ŀ�ı�
// @returns ���ز���ɹ�����Ŀ��Ŀ
inline int InsertItems(int _Index, const INIT_LIST<LPCWSTR>& _Items)noexcept
{
	int Count = 0;
	for (auto& Item : _Items)
	{
		if (InsertItem(_Index, Item) != CB_ERR)
			Count++;
	}
	return Count;
}

// ��ָ��λ��ǰ����һ��������Ŀ
// @param _Items  ��Ŀ��������
// @returns ���ز���ɹ�����Ŀ��Ŀ
template<typename _Ty = INT32>
inline int InsertItems(int _Index, const INIT_LIST<TABLE_ITEM<_Ty>>& _Items)
{
	int Count = 0;
	for (auto& Item : _Items)
	{
		if (InsertItem(_Index, Item.Text, Item.Data) != CB_ERR)
			Count++;
	}
	return Count;
}

// ɾ��ָ��λ�õ���Ŀ
// @param _Index  ָ��λ�ã�-1��ʾɾ��ȫ����Ŀ
// @returns ɾ���ɹ�����true�����򷵻�false
inline bool DeleteItem(int _Index)noexcept
{
	if (_Index == -1)
		WindowAPI::SendMsg(CB_RESETCONTENT);
	else return CB_ERR != WindowAPI::SendMsg(CB_DELETESTRING, _Index);
	return true;
}

// ������ָ���ַ����е��ַ���ͷ����Ŀ
// @param _Text  �������ı�
// @param _Off   ��������ʼλ�á�Ĭ��Ϊ0
//				 ����������ײ���û���ҵ����ϵ�ƥ����ʱ��������Ӷ�����������ʼλ�ã�
// @param _End   �����Ľ���λ�á�
//				 ��������λ��ʱ�����δ�ҵ�ƥ������ʾ����ʧ�ܡ�
//				 ���Ϊ-1����ʾ�˲�����Ч����ΪĬ��ֵ
// @param _Mode  �����ı���ƥ��ģʽ��
//				 0����Ŀ�ı��а��������ı�
//				 1����Ŀ��ͷ�����ı�Ϊ�����ı�
//				 2����Ŀ�ı�Ϊ�����ı�
// 				 4�����ִ�Сд
// @returns ����ƥ�����λ������������ʧ�ܷ���-1
inline int FindItem(WStr _Text, int _Off = 0, int _End = -1, int _Mode = 0)noexcept
{
	// ��ǰ��Ŀ����
	int Count = GetItemCount();
	
	// ���������ʼλ���Ƿ���ȷ
	if (_Off < 0 || _Off >= Count)
		return -1;

	// ��_EndΪ-1ʱ����ָ��Ϊ������ʼλ��
	_End = _End == -1 ? Count - 1: _End;

	// �����������λ���Ƿ���ȷ
	if (_End < 0 || _End >= Count)
		return -1;

	int		Index = _Off;		// ��ǰ����λ��
	int		Find	= -1;		// ƥ��������
	bool	bBreak	= false;	// ����������־
	WStr	ItemText;			// ��Ŀ�ı�

	if ((_Mode & 4) == 0)
		StrToUpper(_Text);
	
	while (Index != (_End + 1))
	{
		ItemText = GetItemText(Index);

		if (_Mode & 1)
		{
			if (_Mode & 4)
			{
				Find = (::wcsncmp(ItemText, _Text, _Text.Length()) == 0) ? Index : -1;
			}
			else
			{
				Find = (::_wcsnicmp(ItemText, _Text, _Text.Length()) == 0) ? Index : -1;
			}
		}
		else if (_Mode & 2)
		{
			if (_Mode & 4)
			{
				Find = (::wcscmp(ItemText, _Text) == 0) ? Index : -1;
			}
			else
			{
				Find = (::_wcsicmp(ItemText, _Text) == 0) ? Index : -1;
			}
		}
		else
		{
			if ((_Mode & 4) == 0)
				StrToUpper(ItemText);

			Find = (::wcsstr(ItemText, _Text) != nullptr) ? Index : -1;
		}

		if (Find != -1)
			break;
		
		if (++Index == Count && Index != _End + 1)
			Index = 0;
	}

	return Find;
}

	// ��ȡָ����Ŀ���ı�����
	// @param _Index ָ����Ŀ������
	// @returns ����ָ����Ŀ���ı���ָ��λ�ò����ڣ��򷵻ؿ��ı�
	inline int GetItemTextLen(int _Index)const noexcept
	{
		return WindowAPI::SendMsg(CB_GETLBTEXTLEN, _Index);
	}

	// ��ȡָ����Ŀ�ı�
	// @param _Index  ָ����Ŀ
	// @returns ����ָ����Ŀ���ı���ָ��λ�ò����ڣ��򷵻ؿ��ı�
	inline WStr GetItemText(int _Index)const noexcept
	{
		WStr Text;
		int Len = GetItemTextLen(_Index);

		if (Len > 0)
		{
			Text.SetCapacity(Len);
			WindowAPI::SendMsg(CB_GETLBTEXT, _Index, Text.Data());
		}
		return Text;
	}
	
	// ����ָ����Ŀ�ı�
	// @param _Index ָ����Ŀ������
	// @param _Text  ����Ŀ�ı�
	// @returns �ɹ�����true��ʧ�ܷ���false
	inline bool SetItemText(int _Index, LPCWSTR _Text)noexcept
	{
		if (_Index < 0 || _Index > GetItemCount())
			return false;

		auto Data = GetItemData(_Index);

		DeleteItem(_Index);
		return CB_ERR != InsertItem(_Index, _Text, Data);
	}

	// ��ȡָ����Ŀ��32λ��������
	// @param _Index ָ����Ŀ������
	// @returns ����ָ����Ŀ��32λ��������
	template<typename _Ty=INT32>
	inline _Ty GetItemData(int _Index)const noexcept
	{
		return WindowAPI::SendMsg(CB_GETITEMDATA, _Index);
	}

	// ����ָ����Ŀ��32λ��������
	// @param _Index  ָ����Ŀ������
	// @param _Data   �µ�32λ��������
	// @returns �ɹ�����true��ʧ�ܷ���false
	template<typename _Ty = INT32>
	inline bool SetItemData(int _Index, _Ty _Data)noexcept
	{
		return WindowAPI::SendMsg(CB_SETITEMDATA, _Index, _Data);
	}

#pragma endregion

protected:
	// �ؼ�����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateBefore_())
			return false;

		// ָ���ؼ�����
		BaseWindow::m_dwStyle |= (DWORD)Type.value;
		
		// ָ���Զ�����
		BaseWindow::m_dwStyle |= AutoSort.value ? CBS_SORT : 0;

		return true;
	}

	// �ؼ�������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ��������ʹ����ĳ�ʼ�����������
		if (!BaseControl::_CreateAfter_())
			return false;

		// ��ȡ�ӿؼ���EditBox
		if (Type.value != ComboBoxType::DropList)
		{
			HWND hWndEdit = ::FindWindowEx(BaseWindow::m_hWnd, nullptr, WC_EDITW, nullptr);
			if (!::IsWindow(hWndEdit))
				return false;
			
			EditCtrl.Attach(hWndEdit);
		}
		
		// ָ����ǰѡ����Ŀ
		WindowAPI::SendMsg(CB_SETCURSEL, CurSel.value);

		// ָ���ö���Ŀ
		WindowAPI::SendMsg(CB_SETTOPINDEX, CurSel.value);
		
		// ָ����ʾ����ı�
		WindowAPI::SendMsg(CB_SETCUEBANNER, CueBannerText.value.CStr());
		
		// ָ�������б����С���
		WindowAPI::SendMsg(CB_SETDROPPEDWIDTH, DropWidth.value);

		// ָ�������б����С�߶�
		WindowAPI::SendMsg(CB_SETITEMHEIGHT, DropHeight.value);
		
		// ָ������DropDwon��DropDwonList���͵���չUI
		if (Type.value == ComboBoxType::DropDwon || Type.value == ComboBoxType::DropList)
		{
			WindowAPI::SendMsg(CB_SETEXTENDEDUI, TRUE);
		}

		return true;
	}

private:
	inline bool ReCreate()noexcept
	{
		BaseWindow* ParentWnd = BaseWindow::GetParentCtrl();
		if (!ParentWnd)
			return false;
	
		TABLE_ITEM<INT32>* pItem = nullptr;
		int Count = GetItemCount();

		if (Count > 0)
			pItem = new TABLE_ITEM<INT32>[Count];

		// ��ȡ��Ŀ����
		for (int i = 0; i < Count; i++)
		{
			pItem[i].Text = GetItemText(i);
			pItem[i].Data = GetItemData(i);
		}

		::DestroyWindow(BaseWindow::m_hWnd);

		// ���´����ؼ�
		if (!Create(*ParentWnd))
			return false;

		// �ָ���Ŀ����
		if (pItem != nullptr)
		{
			for (int i = 0; i < Count; i++)
			{
				AddItem(pItem[i].Text, pItem[i].Data);
			}
			delete[]pItem;
		}
		return true;
	}
		
	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;

		if (_Symbol == Type.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);

			if (BaseWindow::m_dwStyle & CBS_SIMPLE)
			{
				Type = ComboBoxType::Simple;
			}
			else if (BaseWindow::m_dwStyle & CBS_DROPDOWN)
			{
				Type = ComboBoxType::DropDwon;
			}
			else if (BaseWindow::m_dwStyle & CBS_DROPDOWNLIST)
			{
				Type = ComboBoxType::DropList;
			}
		}
		else if (_Symbol == CurSel.symbol)
		{
			CurSel.value = WindowAPI::SendMsg(CB_GETCURSEL);
		}
		else if (_Symbol == TopItem.symbol)
		{
			TopItem.value = WindowAPI::SendMsg(CB_SETTOPINDEX);
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == Type.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~CBS_SIMPLE;
			BaseWindow::m_dwStyle &= ~CBS_DROPDOWN;
			BaseWindow::m_dwStyle &= ~CBS_DROPDOWNLIST;

			// �ؽ��ؼ�
			return ReCreate();
		}
		else if (_Symbol == CurSel.symbol)
		{
			return CB_ERR != WindowAPI::SendMsg(CB_GETCURSEL, CurSel.value);
		}
		else if (_Symbol == TopItem.symbol)
		{
			return CB_ERR != WindowAPI::SendMsg(CB_SETTOPINDEX, TopItem.value);
		}
		else if (_Symbol == CueBannerText.symbol)
		{
			return WindowAPI::SendMsg(CB_SETCUEBANNER, CueBannerText.value.Data());
		}
		else return false;
		
		return true;
	}

public:
	// ������ָ����Ͽ������
	_property<ComboBox, ComboBoxType> Type;

	// ������ָ����ǰѡ�е���Ŀ������-1�������Ŀ��ֵ����ʾȡ��ѡ��
	_property<ComboBox, int> CurSel;

	// ������ָ���ö�����Ŀ������-1�������Ŀ��ֵ����ʾȡ���ö�
	_property<ComboBox, int> TopItem;

	// ������ָ����ʾ����ı�
	_property<ComboBox, WStr> CueBannerText;

	// ������ָ�������б����С���
	 _property<ComboBox, int> DropWidth;
	
	// ������ָ�������б���Ŀ�ĸ߶�
	_property<ComboBox, int> DropHeight;

	// ������ָ���Ƿ����ӵ���Ŀ�����Զ�����
	_property<ComboBox, bool> AutoSort;

	// Simple��DropDwon���еı༭�����
	EditBox EditCtrl;
	
private:
	// ������ָ���༭��ؼ�
};

_WGUI_END

#endif // !_WGUI_COMBOBOX_
