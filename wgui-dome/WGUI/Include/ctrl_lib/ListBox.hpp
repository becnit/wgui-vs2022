#pragma once
#ifndef _WGUI_LIST_BOX_
#define _WGUI_LIST_BOX_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

using ListBoxBorder = CtrlBorder;

// 单列列表框
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

#pragma region 项目操作

	// 获取项目总数
	// @returns 返回项目的总数
	inline int GetItemCount()const noexcept
	{
		return WindowAPI::SendMsg(LB_GETCOUNT);
	}

	// 获取指定项目的文本的长度
	// @param _Index 项目索引
	// @returns 返回指定项目的文本的长度
	inline int GetItemTextLen(int _Index)const noexcept
	{
		return WindowAPI::SendMsg(LB_GETTEXTLEN, _Index);
	}

	// 获取指定项目的文本
	// @param _Index 项目索引
	// @returns 返回指定项目的文本，指定项目不存在返回空文本
	inline WStr GetItemText(int _Index)const noexcept
	{
		WStr Buffer(GetItemTextLen(_Index));

		if (Buffer.Capacity() > 1)
			WindowAPI::SendMsg(LB_GETTEXT, _Index, Buffer.Data());
		return Buffer;
	}

	// 获取指定项目的附加32位数据
	// @param _Index 项目索引
	// @returns 返回指定项目的附加32位数据，指定项目不存在返回空附加32位数据
	template<typename _Ty = INT32>
	inline _Ty GetItemData(int _Index)const noexcept
	{
		return WindowAPI::SendMsg(LB_GETITEMDATA, _Index);
	}

	// 设置指定项目的文本
	// @param _Index 项目索引
	// @param _Text  新的文本
	inline bool SetItemText(int _Index, LPCWSTR _Text)noexcept
	{
		if (_Index == -1)
			return false;

		INT32 Data = GetItemData(_Index);

		DeleteItem(_Index);
		InsertItem(_Index, _Text, Data);
		return true;
	}

	// 获取指定项目的附加32位数据
	// @param _Index 项目索引
	// @param _Data 项目的附件32位数据
	// @returns 返回指定项目的附加32位数据，指定项目不存在返回空附加32位数据
	template<typename _Ty = INT32>
	inline _Ty SetItemData(int _Index, _Ty _Data)const noexcept
	{
		return WindowAPI::SendMsg(LB_SETITEMDATA, _Index, _Data);
	}

	// 在尾部添加一个新的添加项目
	// @param _Text 项目文本
	// @param _Data 项目的附件32位数据
	// @returns 添加成功返回该项目的位置索引。失败返回-1
	template<typename _Ty = INT32>
	inline int AddItem(LPCWSTR _Text, _Ty _Data = _Ty())noexcept
	{
		int Index = 0;

		Index =  WindowAPI::SendMsg(LB_ADDSTRING, 0, _Text);
		if (Index != LB_ERR && _Data != _Ty())
			SetItemData(Index, _Data);
		return Index;
	}

	// 在尾部添加一个或多个新的项目
	// @param _Items  项目文本
	// @returns 返回成功添加的项目数目
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

	// 在尾部添加一个或多个新的项目
	// @param _Items  项目数据
	// @returns 返回成功添加的项目数目
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

	// 在指定位置插入一个新的项目
	// @param _Index  插入位置索引
	// @param _Text   项目文本
	// @param _Data   项目的附件32位数据
	// @returns 插入成功返回该项目的位置索引。失败返回-1
	template<typename _Ty = INT32>
	inline int InsertItem(int _Index, LPCWSTR _Text, _Ty _Data = _Ty())noexcept
	{
		int Index = 0;

		Index = WindowAPI::SendMsg(LB_INSERTSTRING, _Text);
		if (Index != LB_ERR && _Data != _Ty())
			SetItemData(Index, _Data);
		return Index;
	}
	
	// 在指定位置插入一个或多个新的项目
	// @param _Index  插入位置索引
	// @param _Text   项目文本
	// @returns 返回成功添加的项目数目
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

	// 在指定位置插入一个或多个新的项目
	// @param _Index  插入位置索引
	// @param _Text   项目数据
	// @returns 返回成功添加的项目数目
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

	// 删除项目
	// @param _Index 项目索引，-1表示全部删除
	inline void DeleteItem(UINT _Index)noexcept
	{
		if (_Index == -1)
			WindowAPI::SendMsg(LB_RESETCONTENT);
		WindowAPI::SendMsg(LB_DELETESTRING, _Index);
	}

#pragma endregion
	
#pragma region 多选项目操作

	// 检查指定项目是否被选中
	// @param _Index 项目索引
	// @returns 被选中返回true，否则返回false
	inline bool IsSelected(UINT _Index)const noexcept
	{
		return WindowAPI::SendMsg(LB_GETSEL, _Index) != 0;
	}

	// 获取被选中的项目的数量
	// @returns 返回被选中的项目的数量
	inline int GetSelCount()const noexcept
	{
		return WindowAPI::SendMsg(LB_GETSELCOUNT);
	}
	
	// 获取被选中的项目的索引
	// @returns 返回被选中的项目的索引数组
	inline _auto_ptr<int> GetSelIndex()const noexcept
	{
		_auto_ptr<int> Indexs(GetSelCount());
		
		WindowAPI::SendMsg(LB_GETSELITEMS, Indexs.size(), Indexs.ptr());
		return Indexs;
	}

#pragma endregion

protected:
	// 创建前的初始化操作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数
		if (!BaseControl::_CreateBefore_())
			return false;

		// 指定排序
		BaseWindow::m_dwStyle |= AutoSort.value ? LBS_SORT : 0;
		
		// 指定多项选择
		BaseWindow::m_dwStyle |= AllowMultiSel.value ? LBS_MULTIPLESEL : 0;

		// 指定边框样式
		if (Border.value == ListBoxBorder::Border)
			BaseWindow::m_dwStyle |= WS_BORDER;
		else if (Border.value == ListBoxBorder::Suuken)
			BaseWindow::m_dwExStyle |= WS_EX_STATICEDGE;
		
		return true;
	}

	// 创建后的初始化操作
	virtual bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数
		if (!BaseControl::_CreateAfter_())
			return false;

		// 指定行间距
		WindowAPI::SendMsg(LB_SETITEMHEIGHT, 0, RowExtra);
		
		// 指定当前选择
		WindowAPI::SendMsg(LB_SETCURSEL, CurSel.value);

		// 指定置顶项目
		WindowAPI::SendMsg(LB_SETTOPINDEX, TopItem.value);

		// 置顶焦点项目
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
		
		// 重新创建控件
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
			// 获取控件风格
			BaseWindow::m_dwStyle = WindowAPI::WndLong(GWL_STYLE);
			BaseWindow::m_dwExStyle = WindowAPI::WndLong(GWL_EXSTYLE);

			// 清除控件风格
			BaseWindow::m_dwStyle &= ~WS_BORDER;
			BaseWindow::m_dwStyle &= ~WS_EX_STATICEDGE;

			// 重建控件
			return ReCreate();
		}
		else
			return false;

		return true;
	}
	
public:
#pragma region 属性

	// 本属性指定添加的项目是否进行排序
	_property<ListBox, bool> AutoSort;

	// 本属性指定是否允许选择多条项目
	_property<ListBox, bool> AllowMultiSel;

	// 本属性指定是项目直接的行间距
	_property<ListBox, int> RowExtra;

	// 本属性指定当前选择的项目
	_property<ListBox, int> CurSel;

	// 本属性指定顶端可见项目
	_property<ListBox, int> TopItem;

	// 本属性指定当前焦点项目
	_property<ListBox, int> CaretItem;

	// 本属性指定边框类型
	_property<ListBox, ListBoxBorder> Border;
	
#pragma endregion

};

_WGUI_END

#endif // !_WGUI_LIST_BOX_
