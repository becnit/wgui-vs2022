#pragma once
#ifndef _WGUI_REPORTLISTVIEW_
#define _WGUI_REPORTLISTVIEW_

#include "ReportColumn.hpp"
#include "ReportItem.hpp"
#include "BaseListView.hpp"
#include "../ImageList.hpp"

_WGUI_BEGIN

// ����ʽ�б���ͼ
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
		// �����Ƿ��Ƿ������϶��ı��п�
		/*if (!AllowDragColumnWidth.value)
			BaseWindow::m_dwExStyle |= LVS_EX_HEADERDRAGDROP;*/

		
		// �����Ƿ������϶��е�λ��
		/*if (AllowDragColumn.value)
			BaseWindow::m_dwExStyle |= LVS_EX_HEADERDRAGDROP;*/
		
		return true;
	}

public:
	// ������ָ���е���Ϣ
	ReportColumn Column;
	
	// ������ָ��������Ϣ
	ReportItem Item;

#pragma region  ��ͷ����

	// ������ָ����ȥ����ͷ
	// _property<ReportListView, bool> NoColumnHeader;

	// ������ָ����ͷ�Ƿ���Ա����
	// _property<ReportListView, bool> HeaderAllowClick;

	// ������ָ���Ƿ������϶��ı��п�
	//_property<ReportListView, bool> AllowDragColumnWidth;

	// ������ָ���Ƿ������϶��е�λ��
	//_property<ReportListView, bool> AllowDragColumn;
	
#pragma endregion

#pragma region ��������

	// ������ָ���Ƿ���������ѡ��
	// _property<ReportListView, bool> FullRowSelect;

	// 

	
#pragma endregion


	
	
};

_WGUI_END

#endif // !_WGUI_REPORTLISTVIEW_
