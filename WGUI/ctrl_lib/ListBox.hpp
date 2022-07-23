#pragma once
#ifndef _WGUI_LIST_BOX_
#define _WGUI_LIST_BOX_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

using ListBoxBorder = CtrlBorder;

// �����б��
class ListBox
	: public BaseControl<ListBox>
{
public:
	inline ListBox()noexcept
		: BaseControl<ListBox>(WC_LISTBOXW)
		, AutoSort(PROPERTY_INIT(ListBox))
		, AllowMultiSel(PROPERTY_INIT(ListBox))
		, RowExtra(PROPERTY_INIT(ListBox), 20)
		, CurSel(PROPERTY_INIT(ListBox))
		, TopItem(PROPERTY_INIT(ListBox))
		, CaretItem(PROPERTY_INIT(ListBox))
		, Border(PROPERTY_INIT(ListBox))
	{
		BaseWindow::m_dwStyle |= (LBS_HASSTRINGS | LBS_DISABLENOSCROLL);
	}

	inline ~ListBox()noexcept
	{}

#pragma region ��Ŀ����

	// ��ȡ��Ŀ����
	// @returns ������Ŀ������
	inline int GetItemCount()const noexcept
	{
		return WindowAPI::SendMsg(LB_GETCOUNT);
	}

	// ��ȡָ����Ŀ���ı��ĳ���
	// @param _Index ��Ŀ����
	// @returns ����ָ����Ŀ���ı��ĳ���
	inline int GetItemTextLen(int _Index)const noexcept
	{
		return WindowAPI::SendMsg(LB_GETTEXTLEN, _Index);
	}

	// ��ȡָ����Ŀ���ı�
	// @param _Index ��Ŀ����
	// @returns ����ָ����Ŀ���ı���ָ����Ŀ�����ڷ��ؿ��ı�
	inline WStr GetItemText(int _Index)const noexcept
	{
		WStr Buffer(GetItemTextLen(_Index));

		if (Buffer.Capacity() > 1)
			WindowAPI::SendMsg(LB_GETTEXT, _Index, Buffer.Data());
		return Buffer;
	}

	// ��ȡָ����Ŀ�ĸ���32λ����
	// @param _Index ��Ŀ����
	// @returns ����ָ����Ŀ�ĸ���32λ���ݣ�ָ����Ŀ�����ڷ��ؿո���32λ����
	template<typename _Ty = INT32>
	inline _Ty GetItemData(int _Index)const noexcept
	{
		return WindowAPI::SendMsg(LB_GETITEMDATA, _Index);
	}

	// ����ָ����Ŀ���ı�
	// @param _Index ��Ŀ����
	// @param _Text  �µ��ı�
	inline bool SetItemText(int _Index, LPCWSTR _Text)noexcept
	{
		if (_Index == -1)
			return false;

		INT32 Data = GetItemData(_Index);

		DeleteItem(_Index);
		InsertItem(_Index, _Text, Data);
		return true;
	}

	// ��ȡָ����Ŀ�ĸ���32λ����
	// @param _Index ��Ŀ����
	// @param _Data ��Ŀ�ĸ���32λ����
	// @returns ����ָ����Ŀ�ĸ���32λ���ݣ�ָ����Ŀ�����ڷ��ؿո���32λ����
	template<typename _Ty = INT32>
	inline _Ty SetItemData(int _Index, _Ty _Data)const noexcept
	{
		return WindowAPI::SendMsg(LB_SETITEMDATA, _Index, _Data);
	}

	// ��β�����һ���µ������Ŀ
	// @param _Text ��Ŀ�ı�
	// @param _Data ��Ŀ�ĸ���32λ����
	// @returns ��ӳɹ����ظ���Ŀ��λ��������ʧ�ܷ���-1
	template<typename _Ty = INT32>
	inline int AddItem(LPCWSTR _Text, _Ty _Data = _Ty())noexcept
	{
		int Index = 0;

		Index =  WindowAPI::SendMsg(LB_ADDSTRING, 0, _Text);
		if (Index != LB_ERR && _Data != _Ty())
			SetItemData(Index, _Data);
		return Index;
	}

	// ��β�����һ�������µ���Ŀ
	// @param _Items  ��Ŀ�ı�
	// @returns ���سɹ���ӵ���Ŀ��Ŀ
	inline int AddItems(const std::initializer_list<LPCWSTR>& _Items)noexcept
	{
		int Count = 0;

		for (auto& Item : _Items)
		{
			if (AddItem(Item) != LB_ERR)
				Count++;
		}
		return Count;
	}

	// ��β�����һ�������µ���Ŀ
	// @param _Items  ��Ŀ����
	// @returns ���سɹ���ӵ���Ŀ��Ŀ
	template<typename _Ty = INT32>
	inline int AddItems(const std::initializer_list<TABLE_ITEM<_Ty>>& _Items)noexcept
	{
		int Count = 0;

		for (auto& Item : _Items)
		{
			if (AddItem(Item.Text, Item.Data) != LB_ERR)
				Count++;
		}
		return Count;
	}

	// ��ָ��λ�ò���һ���µ���Ŀ
	// @param _Index  ����λ������
	// @param _Text   ��Ŀ�ı�
	// @param _Data   ��Ŀ�ĸ���32λ����
	// @returns ����ɹ����ظ���Ŀ��λ��������ʧ�ܷ���-1
	template<typename _Ty = INT32>
	inline int InsertItem(int _Index, LPCWSTR _Text, _Ty _Data = _Ty())noexcept
	{
		int Index = 0;

		Index = WindowAPI::SendMsg(LB_INSERTSTRING, _Text);
		if (Index != LB_ERR && _Data != _Ty())
			SetItemData(Index, _Data);
		return Index;
	}
	
	// ��ָ��λ�ò���һ�������µ���Ŀ
	// @param _Index  ����λ������
	// @param _Text   ��Ŀ�ı�
	// @returns ���سɹ���ӵ���Ŀ��Ŀ
	inline int InsertItems(int _Index, const std::initializer_list<LPCWSTR>& _Items)noexcept
	{
		int Count = 0;

		for (auto& Item : _Items)
		{
			if (InsertItem(_Index, Item) != LB_ERR)
			{
				++Count;
				++_Index;
			}
		}
		return Count;
	}

	// ��ָ��λ�ò���һ�������µ���Ŀ
	// @param _Index  ����λ������
	// @param _Text   ��Ŀ����
	// @returns ���سɹ���ӵ���Ŀ��Ŀ
	template<typename _Ty = INT32>
	inline int InsertItems(const std::initializer_list<TABLE_ITEM<_Ty>>& _Items)noexcept
	{
		int Count = 0;

		for (auto& Item : _Items)
		{
			if (InsertItem(Item.Text, Item.Data) != LB_ERR)
			{
				++Count;
			}
		}
		return Count;
	}

	// ɾ����Ŀ
	// @param _Index ��Ŀ������-1��ʾȫ��ɾ��
	inline void DeleteItem(UINT _Index)noexcept
	{
		if (_Index == -1)
			WindowAPI::SendMsg(LB_RESETCONTENT);
		WindowAPI::SendMsg(LB_DELETESTRING, _Index);
	}

#pragma endregion
	
#pragma region ��ѡ��Ŀ����

	// ���ָ����Ŀ�Ƿ�ѡ��
	// @param _Index ��Ŀ����
	// @returns ��ѡ�з���true�����򷵻�false
	inline bool IsSelected(UINT _Index)const noexcept
	{
		return WindowAPI::SendMsg(LB_GETSEL, _Index) != 0;
	}

	// ��ȡ��ѡ�е���Ŀ������
	// @returns ���ر�ѡ�е���Ŀ������
	inline int GetSelCount()const noexcept
	{
		return WindowAPI::SendMsg(LB_GETSELCOUNT);
	}
	
	// ��ȡ��ѡ�е���Ŀ������
	// @returns ���ر�ѡ�е���Ŀ����������
	inline _auto_ptr<int> GetSelIndex()const noexcept
	{
		_auto_ptr<int> Indexs(GetSelCount());
		
		WindowAPI::SendMsg(LB_GETSELITEMS, Indexs.size(), Indexs.ptr());
		return Indexs;
	}

#pragma endregion

protected:
	// ����ǰ�ĳ�ʼ������
	virtual bool _CreateBefore_()noexcept
	{
		// ���û���ĳ�ʼ������
		if (!BaseControl::_CreateBefore_())
			return false;

		// ָ������
		BaseWindow::m_dwStyle |= AutoSort.value ? LBS_SORT : 0;
		
		// ָ������ѡ��
		BaseWindow::m_dwStyle |= AllowMultiSel.value ? LBS_MULTIPLESEL : 0;

		// ָ���߿���ʽ
		if (Border.value == ListBoxBorder::Border)
			BaseWindow::m_dwStyle |= WS_BORDER;
		else if (Border.value == ListBoxBorder::Suuken)
			BaseWindow::m_dwExStyle |= WS_EX_STATICEDGE;
		
		return true;
	}

	// ������ĳ�ʼ������
	virtual bool _CreateAfter_()noexcept
	{
		// ���û���ĳ�ʼ������
		if (!BaseControl::_CreateAfter_())
			return false;

		// ָ���м��
		WindowAPI::SendMsg(LB_SETITEMHEIGHT, 0, RowExtra);
		
		// ָ����ǰѡ��
		WindowAPI::SendMsg(LB_SETCURSEL, CurSel.value);

		// ָ���ö���Ŀ
		WindowAPI::SendMsg(LB_SETTOPINDEX, TopItem.value);

		// �ö�������Ŀ
		if (AllowMultiSel.value)
			WindowAPI::SendMsg(LB_SETCARETINDEX, CurSel.value);
		
		return true;
	}

private:
	inline bool ReCreate()noexcept
	{
		BaseWindow* ParentWnd = BaseWindow::GetParentCtrl();
		if (ParentWnd == nullptr)
			return false;

		using ITEM = TABLE_ITEM<INT32>;

		int ItemCount = GetItemCount();
		ITEM* pItem = new ITEM[ItemCount];

		for (int i = 0; i < ItemCount; i++)
		{
			pItem[i].Text = GetItemText(i);
			pItem[i].Data = GetItemData(i);
		}

		::DestroyWindow(BaseWindow::m_hWnd);
		
		// ���´����ؼ�
		if (!Create(*ParentWnd))
			return false;

		for (int i = 0; i < ItemCount; i++)
		{
			AddItem(pItem[i].Text, pItem[i].Data);
		}
		delete[]pItem;
		return true;
	}

	inline void _property_get(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return;

		if (_Symbol == AutoSort.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			AutoSort.value = (BaseWindow::m_dwStyle & LBS_SORT);
		}
		else if (_Symbol == AllowMultiSel.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			AllowMultiSel.value = (BaseWindow::m_dwStyle & LBS_MULTIPLESEL);
		}
		else if (_Symbol == CurSel.symbol)
		{
			CurSel.value = WindowAPI::SendMsg(LB_GETCURSEL);
		}
		else if (_Symbol == TopItem.symbol)
		{
			TopItem.value = WindowAPI::SendMsg(LB_GETTOPINDEX);
		}
		else if (_Symbol == CaretItem.symbol)
		{
			CaretItem.value = WindowAPI::SendMsg(LB_GETCARETINDEX);
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!WindowAPI::IsCreate())
			return true;

		if (_Symbol == AutoSort.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~LBS_SORT;
			return ReCreate();
		}
		else if (_Symbol == AllowMultiSel.symbol)
		{
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwStyle &= ~LBS_MULTIPLESEL;
			return ReCreate();
		}
		else if (_Symbol == CurSel.symbol)
		{
			WindowAPI::SendMsg(LB_SETCURSEL, CurSel.value);
		}
		else if (_Symbol == TopItem.symbol)
		{
			TopItem.value = WindowAPI::SendMsg(LB_GETTOPINDEX);
		}
		else if (_Symbol == CaretItem.symbol)
		{
			CaretItem.value = WindowAPI::SendMsg(LB_GETCARETINDEX);
		}
		else if (_Symbol == Border.symbol)
		{
			// ��ȡ�ؼ����
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);

			// ����ؼ����
			BaseWindow::m_dwStyle &= ~WS_BORDER;
			BaseWindow::m_dwStyle &= ~WS_EX_STATICEDGE;

			// �ؽ��ؼ�
			return ReCreate();
		}
		else
			return false;

		return true;
	}
	
public:
#pragma region ����

	// ������ָ����ӵ���Ŀ�Ƿ��������
	_property<ListBox, bool> AutoSort;

	// ������ָ���Ƿ�����ѡ�������Ŀ
	_property<ListBox, bool> AllowMultiSel;

	// ������ָ������Ŀֱ�ӵ��м��
	_property<ListBox, int> RowExtra;

	// ������ָ����ǰѡ�����Ŀ
	_property<ListBox, int> CurSel;

	// ������ָ�����˿ɼ���Ŀ
	_property<ListBox, int> TopItem;

	// ������ָ����ǰ������Ŀ
	_property<ListBox, int> CaretItem;

	// ������ָ���߿�����
	_property<ListBox, ListBoxBorder> Border;
	
#pragma endregion

};

_WGUI_END

#endif // !_WGUI_LIST_BOX_
