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


	// 获取表项的数目
	// @returns 获取成功返回表项的数目，否则返回-1
	inline int GetCount()const noexcept
	{
		return ::SendMessageW(m_hWnd, LVM_GETITEMCOUNT, 0, 0);
	}

	// 插入表项
	// @param _Index  插入位置
	// @returns 插入成功返回true，否则返回false
	inline ReportItem& Insert(int _Index = -1)noexcept
	{
		m_Info = { 0 };		// 清空信息
		
		m_Info.mask = LVIF_TEXT;
		m_Info.iItem = _Index == -1 ? GetCount() : _Index;

		m_Info.iItem = ::SendMessageW(m_hWnd, LVM_INSERTITEMW, 0, (LPARAM)&m_Info);
		return *this;
	}

	// 删除表项
	// @param _RemoveAll  指定是否删除全部表项
	// @returns 删除成功返回true，否则返回false
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
	
#pragma region 批量操作

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
			
			if(Index > NowCount)	// 新增表项
				::SendMessageW(m_hWnd, LVM_INSERTITEMW, 0, (LPARAM)&m_Info);
			
			for(auto& Text : Item)
			{
				m_Info.pszText = Text;
				::SendMessageW(m_hWnd, LVM_SETITEMW, 0, (LPARAM)&m_Info);
				++m_Info.iSubItem;
			}
		}
		m_Info.iItem = -1;	// 设置标记位，说明没有先调用operator[]设置要操作的表项
		return *this;
	}

	inline ReportItem& operator=(ITEM_TEXT_LIST _ItemList)noexcept
	{
		int Index = m_Info.iItem;
		
		if (Index > GetCount() || Index == -1)  // 检查是否有操作的表项
			return *this;

		m_Info = { 0 };				// 清空信息
		m_Info.iItem = Index;		// 设置表项索引
		m_Info.mask = LVIF_TEXT;	// 设置掩码
		
		for (auto& Text : _ItemList)
		{
			m_Info.pszText = Text;
			::SendMessageW(m_hWnd, LVM_SETITEMW, 0, (LPARAM)&m_Info);
			++m_Info.iSubItem;
		}
		m_Info.iItem = -1;	// 设置标记位，说明没有先调用operator[]设置要操作的表项
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
	// 表项的图标列表
	ImageList Image;

	// 本属性指定表项的图标索引
	_property<ReportItem, INT> ImageIndex;

	
};

_WGUI_END

#endif // !_WGUI_LISTVIEW_REPORTITEM_
