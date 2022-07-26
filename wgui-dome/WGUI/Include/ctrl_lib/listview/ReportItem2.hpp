#pragma once
#ifndef _WGUI_LISTVIEW_REPORTITEM_
#define _WGUI_LISTVIEW_REPORTITEM_

#include "../BaseControl.hpp"
#include "../ImageList.hpp"

_WGUI_BEGIN

class ReportItem
{
public:
	inline ReportItem(HWND& _hWnd)noexcept
		: m_hWnd(_hWnd)
		, m_Info(LVITEMW())
		, Image()
		, ImageIndex(PROPERTY_INIT(ReportItem))
	{
	}


	// ��ȡ�������Ŀ
	// @returns ��ȡ�ɹ����ر������Ŀ�����򷵻�-1
	inline int GetCount()const noexcept
	{
		return ::SendMessageW(m_hWnd, LVM_GETITEMCOUNT, 0, 0);
	}

	// �������
	// @param _Index  ����λ��
	// @returns ����ɹ�����true�����򷵻�false
	inline ReportItem& Insert(int _Index = -1)noexcept
	{
		m_Info = { 0 };		// �����Ϣ
		
		m_Info.mask = LVIF_TEXT;
		m_Info.iItem = _Index == -1 ? GetCount() : _Index;

		m_Info.iItem = ::SendMessageW(m_hWnd, LVM_INSERTITEMW, 0, (LPARAM)&m_Info);
		return *this;
	}

	// ɾ������
	// @param _RemoveAll  ָ���Ƿ�ɾ��ȫ������
	// @returns ɾ���ɹ�����true�����򷵻�false
	inline bool Remove(bool _RemoveAll = false)noexcept
	{
		if(_RemoveAll)
			return ::SendMessageW(m_hWnd, LVM_DELETEALLITEMS, 0, 0) != 0;
		
		if (m_Info.iItem == -1 || m_Info.iItem >= GetCount())
			return false;

		return ::SendMessageW(m_hWnd, LVM_DELETEITEM, m_Info.iItem, 0);
	}
	
	inline ReportItem& operator[](int _Index)noexcept
	{
		m_Info.iItem = _Index;
		return *this;
	}
	
#pragma region ��������

	using ITEM_TEXT_LIST = std::initializer_list<LPWSTR>;
	using ITEM_LIST = std::initializer_list<ITEM_TEXT_LIST>;

	inline ReportItem& operator=(ITEM_LIST _ItemList)noexcept
	{
		size_t NowCount = GetCount();
		size_t Index = 0;

		m_Info = { 0 };
		m_Info.mask = LVIF_TEXT;

		for (auto& Item : _ItemList)
		{
			m_Info.iItem = Index++;
			m_Info.iSubItem = 0;
			
			if(Index > NowCount)	// ��������
				::SendMessageW(m_hWnd, LVM_INSERTITEMW, 0, (LPARAM)&m_Info);
			
			for(auto& Text : Item)
			{
				m_Info.pszText = Text;
				::SendMessageW(m_hWnd, LVM_SETITEMW, 0, (LPARAM)&m_Info);
				++m_Info.iSubItem;
			}
		}
		m_Info.iItem = -1;	// ���ñ��λ��˵��û���ȵ���operator[]����Ҫ�����ı���
		return *this;
	}

	inline ReportItem& operator=(ITEM_TEXT_LIST _ItemList)noexcept
	{
		int Index = m_Info.iItem;
		
		if (Index > GetCount() || Index == -1)  // ����Ƿ��в����ı���
			return *this;

		m_Info = { 0 };				// �����Ϣ
		m_Info.iItem = Index;		// ���ñ�������
		m_Info.mask = LVIF_TEXT;	// ��������
		
		for (auto& Text : _ItemList)
		{
			m_Info.pszText = Text;
			::SendMessageW(m_hWnd, LVM_SETITEMW, 0, (LPARAM)&m_Info);
			++m_Info.iSubItem;
		}
		m_Info.iItem = -1;	// ���ñ��λ��˵��û���ȵ���operator[]����Ҫ�����ı���
		return *this;
	}
	
#pragma endregion

private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (!::IsWindow(m_hWnd))
			return;

		if (_Symbol == ImageIndex.symbol)
		{
			if (Image.GetCount() == 0)
				return;
			
			m_Info.mask = LVIF_IMAGE;
			::SendMessageW(m_hWnd, LVM_GETITEMW, 0, (LPARAM)&m_Info);
			ImageIndex.value = m_Info.iImage;
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (!::IsWindow(m_hWnd))
			return false;


		if (_Symbol == ImageIndex.symbol)
		{
			if (Image.GetCount() == 0)
				return false;

			m_Info.mask = LVIF_IMAGE;
			m_Info.iImage = ImageIndex.value;
			return ::SendMessageW(m_hWnd, LVM_SETITEMW, 0, (LPARAM)&m_Info) != 0;
		}
		else return false;
		return true;
	}
	
private:
	HWND& m_hWnd;
	LVITEMW m_Info;
	
public:
	// �����ͼ���б�
	ImageList Image;

	// ������ָ�������ͼ������
	_property<ReportItem, INT> ImageIndex;

	
};

_WGUI_END

#endif // !_WGUI_LISTVIEW_REPORTITEM_
