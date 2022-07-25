#pragma once
#ifndef _WGUI_COMBOBOX_
#define _WGUI_COMBOBOX_

#include "../core/BaseControl.hpp"
#include "EditBox.hpp"

_WGUI_BEGIN

enum class ComboBoxType : DWORD
{
	Simple		= 0x0001,	// 可编辑列表式
	DropDwon	= 0x0002,	// 可编辑下拉式
	DropList	= 0x0003		// 不可编辑下拉式
};

// 组合框
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
	
	// 创建控件
	// @param _ParentWnd 父控件对象
	// @returns 创建成功返回true，否则返回false
	virtual bool Create(const BaseWindow& _ParentWnd)noexcept
	{
		return BaseControl::Create(_ParentWnd);
	}

#pragma region 项目操作

// 获取项目数量
// @returns 获取成功返回项目数量，否则返回-1
inline int GetItemCount()const noexcept
{
	return WindowAPI::SendMsg(CB_GETCOUNT);
}

// 在尾部添加一个新的项目
// @param _Text	  项目文本
// @param _Data   32位附件数据
// @return 添加成功返回该项目的索引位置，失败返回-1
template<typename _Ty = INT32>
inline int AddItem(LPCWSTR _Text, _Ty _Data = _Ty())noexcept
{
	int Index = WindowAPI::SendMsg(CB_ADDSTRING, 0, _Text);
	if (Index != CB_ERR && _Data != _Ty())
		WindowAPI::SendMsg(CB_SETITEMDATA, 0, _Data);
	return Index;
}

// 在尾部添加一个或多个新的项目
// @param _Items  项目文本
// @returns 返回添加成功的项目数目
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

// 在尾部添加一个或多个新的项目
// @param _Items  项目数据数组
// @returns 返回添加成功的项目数目
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

// 在指定位置前插入一个新的项目
// @param _Index  指定位置
// @param _Text	  项目文本
// @param _Data   32位附件数据
// @return 插入成功返回该项目的索引位置，失败返回-1
template<typename _Ty = INT32>
inline int InsertItem(int _Index, LPCWSTR _Text, _Ty _Data = _Ty())
{
	_Index = WindowAPI::SendMsg(CB_INSERTSTRING, _Index, _Text);
	if (_Index != CB_ERR && _Data != _Ty())
		WindowAPI::SendMsg(CB_SETITEMDATA, _Index, _Data);
	return _Index;
}

// 在指定位置前插入一个或多个新的项目
// @param _Index  指定位置
// @param _Items  项目文本
// @returns 返回插入成功的项目数目
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

// 在指定位置前插入一个或多个项目
// @param _Items  项目数据数组
// @returns 返回插入成功的项目数目
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

// 删除指定位置的项目
// @param _Index  指定位置，-1表示删除全部项目
// @returns 删除成功返回true，否则返回false
inline bool DeleteItem(int _Index)noexcept
{
	if (_Index == -1)
		WindowAPI::SendMsg(CB_RESETCONTENT);
	else return CB_ERR != WindowAPI::SendMsg(CB_DELETESTRING, _Index);
	return true;
}

// 搜索以指定字符串中的字符开头的项目
// @param _Text  搜索的文本
// @param _Off   搜索的起始位置。默认为0
//				 当搜索到达底部且没有找到符合的匹配项时，会继续从顶部搜索到起始位置；
// @param _End   搜索的结束位置。
//				 搜索到该位置时，如果未找到匹配项，则表示搜索失败。
//				 如果为-1，表示此参数无效。此为默认值
// @param _Mode  搜索文本的匹配模式。
//				 0，项目文本中包含搜索文本
//				 1，项目开头部分文本为搜索文本
//				 2，项目文本为搜索文本
// 				 4，区分大小写
// @returns 返回匹配项的位置索引，搜索失败返回-1
inline int FindItem(WStr _Text, int _Off = 0, int _End = -1, int _Mode = 0)noexcept
{
	// 当前项目数量
	int Count = GetItemCount();
	
	// 检查搜索起始位置是否正确
	if (_Off < 0 || _Off >= Count)
		return -1;

	// 若_End为-1时，则指定为搜索起始位置
	_End = _End == -1 ? Count - 1: _End;

	// 检查搜索结束位置是否正确
	if (_End < 0 || _End >= Count)
		return -1;

	int		Index = _Off;		// 当前搜索位置
	int		Find	= -1;		// 匹配项索引
	bool	bBreak	= false;	// 搜索结束标志
	WStr	ItemText;			// 项目文本

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

	// 获取指定项目的文本长度
	// @param _Index 指定项目的索引
	// @returns 返回指定项目的文本，指定位置不存在，则返回空文本
	inline int GetItemTextLen(int _Index)const noexcept
	{
		return WindowAPI::SendMsg(CB_GETLBTEXTLEN, _Index);
	}

	// 获取指定项目文本
	// @param _Index  指定项目
	// @returns 返回指定项目的文本，指定位置不存在，则返回空文本
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
	
	// 设置指定项目文本
	// @param _Index 指定项目的索引
	// @param _Text  新项目文本
	// @returns 成功返回true，失败返回false
	inline bool SetItemText(int _Index, LPCWSTR _Text)noexcept
	{
		if (_Index < 0 || _Index > GetItemCount())
			return false;

		auto Data = GetItemData(_Index);

		DeleteItem(_Index);
		return CB_ERR != InsertItem(_Index, _Text, Data);
	}

	// 获取指定项目的32位附件数据
	// @param _Index 指定项目的索引
	// @returns 返回指定项目的32位附件数据
	template<typename _Ty=INT32>
	inline _Ty GetItemData(int _Index)const noexcept
	{
		return WindowAPI::SendMsg(CB_GETITEMDATA, _Index);
	}

	// 设置指定项目的32位附件数据
	// @param _Index  指定项目的索引
	// @param _Data   新的32位附件数据
	// @returns 成功返回true，失败返回false
	template<typename _Ty = INT32>
	inline bool SetItemData(int _Index, _Ty _Data)noexcept
	{
		return WindowAPI::SendMsg(CB_SETITEMDATA, _Index, _Data);
	}

#pragma endregion

protected:
	// 控件创建前的初始化工作
	virtual bool _CreateBefore_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl::_CreateBefore_())
			return false;

		// 指定控件类型
		BaseWindow::m_dwStyle |= (DWORD)Type.value;
		
		// 指定自动排序
		BaseWindow::m_dwStyle |= AutoSort.value ? CBS_SORT : 0;

		return true;
	}

	// 控件创建后的初始化工作
	virtual bool _CreateAfter_()noexcept
	{
		// 调用基类的初始化函数，使基类的初始化工作先完成
		if (!BaseControl::_CreateAfter_())
			return false;

		// 获取子控件，EditBox
		if (Type.value != ComboBoxType::DropList)
		{
			HWND hWndEdit = ::FindWindowEx(BaseWindow::m_hWnd, nullptr, WC_EDITW, nullptr);
			if (!::IsWindow(hWndEdit))
				return false;
			
			EditCtrl.Attach(hWndEdit);
		}
		
		// 指定当前选中项目
		WindowAPI::SendMsg(CB_SETCURSEL, CurSel.value);

		// 指定置顶项目
		WindowAPI::SendMsg(CB_SETTOPINDEX, CurSel.value);
		
		// 指定提示横幅文本
		WindowAPI::SendMsg(CB_SETCUEBANNER, CueBannerText.value.CStr());
		
		// 指定下拉列表的最小宽度
		WindowAPI::SendMsg(CB_SETDROPPEDWIDTH, DropWidth.value);

		// 指定下拉列表的最小高度
		WindowAPI::SendMsg(CB_SETITEMHEIGHT, DropHeight.value);
		
		// 指定启用DropDwon、DropDwonList类型的扩展UI
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

		// 获取项目数据
		for (int i = 0; i < Count; i++)
		{
			pItem[i].Text = GetItemText(i);
			pItem[i].Data = GetItemData(i);
		}

		::DestroyWindow(BaseWindow::m_hWnd);

		// 重新创建控件
		if (!Create(*ParentWnd))
			return false;

		// 恢复项目数据
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

			// 重建控件
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
	// 本属性指定组合框的类型
	_property<ComboBox, ComboBoxType> Type;

	// 本属性指定当前选中的项目。给定-1或大于项目的值，表示取消选中
	_property<ComboBox, int> CurSel;

	// 本属性指定置顶的项目。给定-1或大于项目的值，表示取消置顶
	_property<ComboBox, int> TopItem;

	// 本属性指定提示横幅文本
	_property<ComboBox, WStr> CueBannerText;

	// 本属性指定下拉列表的最小宽度
	 _property<ComboBox, int> DropWidth;
	
	// 本属性指定下拉列表项目的高度
	_property<ComboBox, int> DropHeight;

	// 本属性指定是否对添加的项目进行自动排序
	_property<ComboBox, bool> AutoSort;

	// Simple，DropDwon特有的编辑框对象
	EditBox EditCtrl;
	
private:
	// 本属性指定编辑框控件
};

_WGUI_END

#endif // !_WGUI_COMBOBOX_
