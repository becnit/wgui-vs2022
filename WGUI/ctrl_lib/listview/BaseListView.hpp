#pragma once
#ifndef _WGUI_BASELISTVIEW_
#define _WGUI_BASELISTVIEW_

#include "../BaseControl.hpp"

_WGUI_BEGIN

template<class _ElemCtrl, class _ElemHandleMessage>
class BaseListView
	: public BaseControl<_ElemCtrl, _ElemHandleMessage>
{
	using BaseCtrl = BaseControl<_ElemCtrl, _ElemHandleMessage>;

public:
	inline BaseListView(DWORD _Style)noexcept
		: BaseCtrl(WC_LISTVIEW)
		, CurSel(PROPERTY_INIT(BaseListView))
		, AllowHotTrack(PROPERTY_INIT(BaseListView))
		, AllowEdit(PROPERTY_INIT(BaseListView))
		, HasCheckBox(PROPERTY_INIT(BaseListView))
		, AllowMultiSelect(PROPERTY_INIT(BaseListView))
	{
		BaseWindow::m_dwStyle |= _Style | WS_BORDER;
	}

	inline ~BaseListView()noexcept
	{}

private:
	// 创建前的初始化
	virtual bool _CreateAfter_()noexcept
	{
		
		// 指定单项/多项选择
		BaseWindow::m_dwStyle |= (AllowMultiSelect.value ? 0 : LVS_SINGLESEL);

		// 指定可编辑
		BaseWindow::m_dwStyle |= (AllowEdit.value ? 0 : LVS_EDITLABELS);

		// 指定选择框
		BaseWindow::m_dwStyle |= (HasCheckBox.value ? LVS_EX_CHECKBOXES : 0);

		// 指定热点跟踪
		BaseWindow::m_dwStyle |= (AllowHotTrack.value ? 0 : LVS_EX_NOHOTTRACKING);
		
		// 指定排序方式
		switch (SortType.value)
		{
			case 1:	// 正向排序
				BaseWindow::m_dwStyle |= LVS_SORTASCENDING;
				break;
				
			case 2:	// 反向排序
				BaseWindow::m_dwStyle |= LVS_SORTDESCENDING;
				break;	
		}
		
		return true;
	}

	// 创建后的工作
	virtual bool _CreateBefore_()noexcept
	{
		// WindowAPI::SendMsg(LVM_SETCUSEL, CurSel.value);

		return true;
	}

	inline void _property_get(int _Symbol)noexcept
	{
		
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		return true;
	}
	
public:
	// 本属性指定当前选中行表项的位置索引
	_property<BaseListView, UINT> CurSel;

	// 本属性指定是否跟踪并高亮度显示当前鼠标所处表项
	_property<BaseListView, bool> AllowHotTrack;

	// 本属性指定是否允许编辑表项标题
	_property<BaseListView, bool> AllowEdit;

	// 本属性指定是否在各表项前加上选择框
	 _property<BaseListView, bool> HasCheckBox;

	// 本属性指定是否允许选择多项
	 _property<BaseListView, bool> AllowMultiSelect;

	// 本属性指定排序的方式
	// 0，不排序
	// 1，正向排序
	// 2，逆向排序
	_property<BaseListView, UINT> SortType;

};

_WGUI_END

#endif // !_WGUI_BASELISTVIEW_
