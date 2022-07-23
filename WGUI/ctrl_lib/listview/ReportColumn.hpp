#pragma once
#ifndef _WGUI_LISTVIEW_COLUMN_
#define _WGUI_LISTVIEW_COLUMN_

#include "../BaseControl.hpp"
#include "../ImageList.hpp"

_WGUI_BEGIN

class ReportColumn;

class ReportColumnInfo
{
	friend ReportColumn;
	
public:
	inline ReportColumnInfo(HWND& hWnd)noexcept
		: m_hWnd(hWnd)
		, m_Info(LVCOLUMNW())
		, Text(PROPERTY_INIT(ReportColumnInfo))
		, Align(PROPERTY_INIT(ReportColumnInfo))
		, Width(PROPERTY_INIT(ReportColumnInfo))
		, ImageIndex(PROPERTY_INIT(ReportColumnInfo))
	{
	}

	inline ~ReportColumnInfo()noexcept
	{
	}

	struct COLUMN_INFO
	{
		std::wstring Text = L"";
		UINT Width = 100;
		UINT ImageIndex = 0;
		AlignMode Align = AlignMode::Near;
	};
	
	inline ReportColumnInfo& operator=(const COLUMN_INFO& _Info)noexcept
	{
		// m_Info.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_IMAGE | LVCF_FMT;
		m_Info.pszText = (LPWSTR)_Info.Text.c_str();
		m_Info.cx = _Info.Width;
		m_Info.iImage = _Info.ImageIndex;
		
		switch (_Info.Align)
		{
			case AlignMode::Near:
				m_Info.fmt = LVCFMT_LEFT;
				break;
			case AlignMode::Center:
				m_Info.fmt = LVCFMT_CENTER;
				break;
			case AlignMode::Far:
				m_Info.fmt = LVCFMT_RIGHT;
				break;
		}
		
		::SendMessageW(m_hWnd, LVM_SETCOLUMN, m_Info.iOrder, (LPARAM) & m_Info);
		return *this;
	}

private:
	
	inline void _property_get(int _Symbol)noexcept
	{
		if (::IsWindow(m_hWnd) == FALSE)
			return;

		if (_Symbol == Text.symbol)
		{
			m_Info.mask = LVCF_TEXT;
			m_Info.cchTextMax = 260;
			m_Info.pszText = new wchar_t[m_Info.cchTextMax]{ 0 };
			::SendMessageW(m_hWnd, LVM_GETCOLUMNW, m_Info.iOrder, (LPARAM)&m_Info);
			Text.value = m_Info.pszText;
			delete[]m_Info.pszText;
		}
		else if (_Symbol == Align.symbol)
		{
			m_Info.mask = LVCF_FMT;
			::SendMessageW(m_hWnd, LVM_GETCOLUMNW, m_Info.iOrder, (LPARAM)&m_Info);
			
			switch (m_Info.fmt)
			{
				case LVCFMT_LEFT:
					Align.value = AlignMode::Near;
					break;
					
				case LVCFMT_CENTER:
					Align.value = AlignMode::Center;
					break;
					
				case LVCFMT_RIGHT:
					Align.value = AlignMode::Far;
					break;
			}
		}
		else if (_Symbol == Width.symbol)
		{
			m_Info.mask = LVCF_WIDTH;
			::SendMessageW(m_hWnd, LVM_GETCOLUMNW, m_Info.iOrder, (LPARAM)&m_Info);
			Width.value = static_cast<UINT>(m_Info.cx);
		}
		else if (_Symbol == ImageIndex.symbol)
		{
			m_Info.mask = LVCF_IMAGE;
			::SendMessageW(m_hWnd, LVM_GETCOLUMNW, m_Info.iOrder, (LPARAM)&m_Info);
			ImageIndex.value = static_cast<UINT>(m_Info.iImage);
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (::IsWindow(m_hWnd) == FALSE)
			return true;

		if (_Symbol == Text.symbol)
		{
			m_Info.mask = LVCF_TEXT;
			m_Info.pszText = (LPWSTR)Text.value.c_str();
		}
		else if (_Symbol == Align.symbol)
		{
			m_Info.mask = LVCF_FMT;
			switch (Align.value)
			{
				case AlignMode::Near:
					m_Info.fmt = LVCFMT_LEFT;
					break;
				case AlignMode::Center:
					m_Info.fmt = LVCFMT_CENTER;
					break;
				case AlignMode::Far:
					m_Info.fmt = LVCFMT_RIGHT;
					break;
			}
		}
		else if (_Symbol == Width.symbol)
		{
			m_Info.mask = LVCF_WIDTH;
			m_Info.cx = static_cast<int>(Width.value);
		}
		else if (_Symbol == ImageIndex.symbol)
		{
			m_Info.mask = LVCF_IMAGE;
			m_Info.iImage = static_cast<int>(ImageIndex.value);
		}
		else return false;
		
		return ::SendMessageW(m_hWnd, LVM_SETCOLUMNW, m_Info.iOrder, (LPARAM)&m_Info);
	}

private:
	HWND& m_hWnd;

	LVCOLUMNW m_Info;

public:
	// 本属性指定当前列的文本
	_property<ReportColumnInfo, std::wstring> Text;

	// 本属性指定当前列的宽度
	_property<ReportColumnInfo, UINT> Width;

	// 本属性指定当前列的图标索引
	_property<ReportColumnInfo, UINT> ImageIndex;	

	// 本属性指定当前列的文本的对齐方式
	// AlginMode::Near     左侧对齐
	// AlginMode::Center   中间对齐
	// AlignMode::Far      右侧对齐
	_property<ReportColumnInfo, AlignMode> Align;
};

/*************************************************************************************/

class ReportColumn
{
public:
	inline ReportColumn(HWND& _hWnd, int _InitWidth = 100, AlignMode _InitAlign = AlignMode::Near)noexcept
		: m_hWnd(_hWnd)
		, m_ColumnInfo(_hWnd)
		, InitWidth(_InitWidth)
		, InitAlign(_InitAlign)
		, Image()
	{}

	// 获取列的数量
	// @returns 获取成功则列的数量，否则返回-1
	inline int GetCount()const noexcept
	{
		HWND hWndHeader = (HWND)::SendMessageW(m_hWnd, LVM_GETHEADER, 0, 0);
		return ::SendMessageW(hWndHeader, HDM_GETITEMCOUNT, 0, 0);
	}

	ReportColumnInfo& operator[](size_t _Index)noexcept
	{
		::memset(&m_ColumnInfo.m_Info, 0, sizeof(m_ColumnInfo.m_Info));
		m_ColumnInfo.m_Info.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | (Image.GetCount() > 0 ? LVCF_IMAGE : 0);
		m_ColumnInfo.m_Info.iOrder = _Index;
		return m_ColumnInfo;
	}

	// 添加新列
	// @returns 返回新列的引用结构
	inline ReportColumnInfo& Add()noexcept
	{
		return Insert(GetCount());
	}

	// 在指定位置插入新的列
	// @param _Index     指定位置
	// @returns 返回新列的引用结构
	inline ReportColumnInfo& Insert(UINT _Index)noexcept
	{
		::memset(&m_ColumnInfo.m_Info, 0, sizeof(m_ColumnInfo.m_Info));
		
		m_ColumnInfo.m_Info.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT;
		m_ColumnInfo.m_Info.cx = InitWidth;
		m_ColumnInfo.m_Info.pszText = L"";		// 初始化文本，必须设置，否则后面无法修改

		switch (InitAlign)
		{
			case WGUI::AlignMode::Near:
				m_ColumnInfo.m_Info.fmt = LVCFMT_LEFT;
				break;
			case WGUI::AlignMode::Center:
				m_ColumnInfo.m_Info.fmt = LVCFMT_CENTER;
				break;
			case WGUI::AlignMode::Far:
				m_ColumnInfo.m_Info.fmt = LVCFMT_RIGHT;
				break;
		}
		
		m_ColumnInfo.m_Info.iOrder = ::SendMessageW(m_hWnd, LVM_INSERTCOLUMN, _Index, (LPARAM)&m_ColumnInfo.m_Info);
		return m_ColumnInfo;
	}

	// 删除指定位置的列
	// @param _Index 指定位置
	inline bool Remove(UINT _Index)noexcept
	{
		return ::SendMessageW(m_hWnd, LVM_DELETECOLUMN, _Index, 0);
	}
	
#pragma region 批量操作

	inline ReportColumn& operator=(std::initializer_list<LPCWSTR> _List)noexcept
	{
		int index = 0;
		for (auto& item : _List)
		{
			if(index >= GetCount())
				Insert(index).Text = item;
			else
				(*this)[index].Text = item;
			++index;
		}
		return *this;
	}

	inline ReportColumn& operator=(std::initializer_list<ReportColumnInfo::COLUMN_INFO> _List)noexcept
	{
		int Index = 0;
		int CurCount = GetCount();
		LVCOLUMNW Info = { 0 };
		
		for (auto& Item : _List)
		{
			Info.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | (Image.GetCount() > 0 ? LVCF_IMAGE : 0);
			Info.cx = Item.Width;
			Info.pszText = (LPWSTR)(Item.Text.empty() ? L"" : Item.Text.c_str());
			Info.iImage = Item.ImageIndex;
			
			switch (Item.Align)
			{
				case WGUI::AlignMode::Near:
					Info.fmt = LVCFMT_LEFT;
					break;
				case WGUI::AlignMode::Center:
					Info.fmt = LVCFMT_CENTER;
					break;
				case WGUI::AlignMode::Far:
					Info.fmt = LVCFMT_RIGHT;
					break;
			}

			::SendMessageW(m_hWnd,
						   Index >= CurCount ? LVM_INSERTCOLUMN : LVM_SETCOLUMN,
						   Index,
						   (LPARAM)&Info);
			++Index;
		}
		return *this;
	}
	
	// 加载列数据
	// @param _ListData 列数据
	// @param _Length	_ListData的大小
	// @returns 加载成功返回true，否则返回false
	inline bool Load(const ReportColumnInfo::COLUMN_INFO* _ListData, size_t _Length)noexcept
	{
		if (_ListData == nullptr || _Length == 0)
			return false;

		size_t CurCount = GetCount();
		LVCOLUMNW Info = { 0 };
		
		for (size_t i = 0; i < _Length; ++i)
		{
			Info.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | (Image.GetCount() > 0 ? LVCF_IMAGE : 0);
			Info.cx = _ListData[i].Width;
			Info.pszText = (LPWSTR)(_ListData[i].Text.empty() ? L"" : _ListData[i].Text.c_str());
			Info.iImage = _ListData[i].ImageIndex;
			
			switch (_ListData[i].Align)
			{
				case WGUI::AlignMode::Near:
					Info.fmt = LVCFMT_LEFT;
					break;
				case WGUI::AlignMode::Center:
					Info.fmt = LVCFMT_CENTER;
					break;
				case WGUI::AlignMode::Far:
					Info.fmt = LVCFMT_RIGHT;
					break;
			}

			::SendMessageW(m_hWnd,
						   i >= CurCount ? LVM_INSERTCOLUMN : LVM_SETCOLUMN,
						   i,
						   (LPARAM)&Info);
		}
		return true;
	}

	// 保存列数据
	// @param _ListData 列数据
	// @param _Length	_ListData的大小
	// @returns 保存成功返回true，否则返回false
	inline bool Save(ReportColumnInfo::COLUMN_INFO* _ListData, size_t _Length)noexcept
	{
		if (_ListData == nullptr || _Length == 0)
			return false;

		int CurCount = GetCount();
		LVCOLUMNW Info = { 0 };
		
		for (size_t i = 0; i < _Length; ++i)
		{
			Info.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | LVCF_IMAGE;

			::SendMessageW(m_hWnd, LVM_GETCOLUMN, i, (LPARAM)&Info);
			
			_ListData[i].Width = Info.cx;
			_ListData[i].Text = (Info.pszText ? Info.pszText : L"");
			_ListData[i].ImageIndex = Info.iImage;
			
			switch (Info.fmt)
			{
				case LVCFMT_LEFT:
					_ListData[i].Align = WGUI::AlignMode::Near;
					break;
					
				case LVCFMT_CENTER:
					_ListData[i].Align = WGUI::AlignMode::Center;
					break;
				
				case LVCFMT_RIGHT:
					_ListData[i].Align = WGUI::AlignMode::Far;
					break;
			}

		}
		return true;
	}
	
#pragma endregion

private:
	HWND& m_hWnd;
	ReportColumnInfo m_ColumnInfo;
	
public:
#pragma region 列初始属性

	int InitWidth;
	AlignMode InitAlign;
	
#pragma endregion
	
	ImageList Image;

};

_WGUI_END

#endif // !_WGUI_LISTVIEW_COLUMN_
