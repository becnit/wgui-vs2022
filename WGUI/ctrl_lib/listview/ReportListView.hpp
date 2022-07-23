#pragma once
#ifndef _WGUI_REPORTLISTVIEW_
#define _WGUI_REPORTLISTVIEW_

#include "ReportColumn.hpp"
#include "ReportItem.hpp"
#include "BaseListView.hpp"
#include "../ImageList.hpp"

_WGUI_BEGIN

// 报表式列表视图
template<class _ElemHandleMessage = DefHandleMessage>
class ReportListView
	: public BaseListView<ReportListView<_ElemHandleMessage>, _ElemHandleMessage>
{
	using BaseLV = BaseListView<ReportListView<_ElemHandleMessage>, _ElemHandleMessage>;
	using BaseCtrl = BaseControl<BaseLV, _ElemHandleMessage>;

public:
	inline ReportListView(bool _NoHeader = false)noexcept
		: BaseLV(LVS_REPORT | (_NoHeader ? LVS_NOCOLUMNHEADER : 0))
		, Column(BaseWindow::m_hWnd)
		, Item(BaseWindow::m_hWnd)
	{
		BaseWindow::m_dwExStyle |=
			(
			   LVS_EX_FULLROWSELECT
			 /*| LVS_EX_AUTOCHECKSELECT
			 | LVS_EX_CHECKBOXES*/
			 /*| LVS_EX_BORDERSELECT*/
			 | LVS_EX_FULLROWSELECT
			 );

		BaseWindow::m_dwStyle |= 0;
	}

	inline ~ReportListView()
	{}

private:
	inline bool _CreateAfter_()noexcept
	{
		// 设置是否是否允许拖动改变列宽
		/*if (!AllowDragColumnWidth.value)
			BaseWindow::m_dwExStyle |= LVS_EX_HEADERDRAGDROP;*/

		
		// 设置是否允许拖动列的位置
		/*if (AllowDragColumn.value)
			BaseWindow::m_dwExStyle |= LVS_EX_HEADERDRAGDROP;*/
		
		return true;
	}

public:
	// 本属性指定列的信息
	ReportColumn Column;
	
	// 本属性指定表项信息
	ReportItem Item;

#pragma region  表头属性

	// 本属性指定是去掉表头
	// _property<ReportListView, bool> NoColumnHeader;

	// 本属性指定表头是否可以被点击
	// _property<ReportListView, bool> HeaderAllowClick;

	// 本属性指定是否允许拖动改变列宽
	//_property<ReportListView, bool> AllowDragColumnWidth;

	// 本属性指定是否允许拖动列的位置
	//_property<ReportListView, bool> AllowDragColumn;
	
#pragma endregion

#pragma region 表项属性

	// 本属性指定是否允许整行选中
	// _property<ReportListView, bool> FullRowSelect;

	// 

	
#pragma endregion


	
	
};

_WGUI_END

#endif // !_WGUI_REPORTLISTVIEW_
