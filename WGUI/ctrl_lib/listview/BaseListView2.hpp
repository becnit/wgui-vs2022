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
	// ����ǰ�ĳ�ʼ��
	virtual bool _CreateAfter_()noexcept
	{
		
		// ָ������/����ѡ��
		BaseWindow::m_dwStyle |= (AllowMultiSelect.value ? 0 : LVS_SINGLESEL);

		// ָ���ɱ༭
		BaseWindow::m_dwStyle |= (AllowEdit.value ? 0 : LVS_EDITLABELS);

		// ָ��ѡ���
		BaseWindow::m_dwStyle |= (HasCheckBox.value ? LVS_EX_CHECKBOXES : 0);

		// ָ���ȵ����
		BaseWindow::m_dwStyle |= (AllowHotTrack.value ? 0 : LVS_EX_NOHOTTRACKING);
		
		// ָ������ʽ
		switch (SortType.value)
		{
			case 1:	// ��������
				BaseWindow::m_dwStyle |= LVS_SORTASCENDING;
				break;
				
			case 2:	// ��������
				BaseWindow::m_dwStyle |= LVS_SORTDESCENDING;
				break;	
		}
		
		return true;
	}

	// ������Ĺ���
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
	// ������ָ����ǰѡ���б����λ������
	_property<BaseListView, UINT> CurSel;

	// ������ָ���Ƿ���ٲ���������ʾ��ǰ�����������
	_property<BaseListView, bool> AllowHotTrack;

	// ������ָ���Ƿ�����༭�������
	_property<BaseListView, bool> AllowEdit;

	// ������ָ���Ƿ��ڸ�����ǰ����ѡ���
	 _property<BaseListView, bool> HasCheckBox;

	// ������ָ���Ƿ�����ѡ�����
	 _property<BaseListView, bool> AllowMultiSelect;

	// ������ָ������ķ�ʽ
	// 0��������
	// 1����������
	// 2����������
	_property<BaseListView, UINT> SortType;

};

_WGUI_END

#endif // !_WGUI_BASELISTVIEW_
