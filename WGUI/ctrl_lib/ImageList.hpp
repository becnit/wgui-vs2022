#pragma once
#ifndef _WGUI_IMAGELIST_
#define _WGUI_IMAGELIST_

#include "BaseControl.hpp"

_WGUI_BEGIN

typedef struct ICONSIZE
{
	int nWidth;
	int nHeight;
}*PICONSIZE;

// 图像列表
class ImageList
{
public:
	inline ImageList()noexcept
		: ImageList(32, 32, ILC_MASK | ILC_COLORDDB)
	{}
	
	inline ImageList(INT _Width, INT _Height, UINT _Flags)noexcept
		: m_hImageList(::ImageList_Create(_Width, _Height, _Flags, 1, 0))
		, m_Flags(_Flags)
		, BkColor(PROPERTY_INIT(ImageList))
		, IconSize(PROPERTY_INIT(ImageList), { _Width, _Height })
	{}

	inline ImageList(const void* _Buffer, size_t _BufSize)noexcept
		: m_hImageList(nullptr)
		, m_Flags(0)
		, BkColor(PROPERTY_INIT(ImageList))
		, IconSize(PROPERTY_INIT(ImageList))
	{
		IStream* pIS = nullptr;
		
		HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, _BufSize);
		if (hGlobal == nullptr)
			return;

		if (E_OUTOFMEMORY == CreateStreamOnHGlobal(hGlobal, FALSE, &pIS))
		{
			m_hImageList = ::ImageList_Read(pIS);
		}
		::GlobalFree(hGlobal);
	}

	inline ~ImageList()noexcept
	{
		if (m_hImageList)
			::ImageList_Destroy(m_hImageList);

		m_hImageList = nullptr;
	}

	// 检查是否已创建HIMAGELIST对象
	inline bool IsCreate()const noexcept
	{
		return m_hImageList;
	}

	// 获取图像数量
	inline INT GetCount()const noexcept
	{
		return ::ImageList_GetImageCount(m_hImageList);
	}

	// 添加图标
	// @param _hIcon  图标句柄
	// @returns 返回图标的索引位置
	inline INT Add(HICON _hIcon)noexcept
	{
		return ::ImageList_ReplaceIcon(m_hImageList, -1, _hIcon);
	}

	// 添加位图
	// @param _hBitmap  位图句柄
	// @returns 返回位图的索引位置
	inline INT Add(HBITMAP _hBitmap, HBITMAP _hBitmapMask = nullptr)noexcept
	{
		return ::ImageList_Add(m_hImageList, _hBitmap, _hBitmapMask);
	}

	// 替换指定位置的图标
	// @param _Index  指定位置
	// @param _hIcon  图标句柄
	// @returns 返回图标的索引位置
	inline INT Replace(INT _Index, HICON _hIcon)noexcept
	{
		return ::ImageList_ReplaceIcon(m_hImageList, _Index, _hIcon);
	}

	// 替换指定位置的位图
	// @param _Index  指定位置
	// @param _hIcon  位图句柄
	// @returns 返回位图的索引位置
	inline INT Replace(INT _Index, HBITMAP _hBitmap, HBITMAP _hBitmapMask = nullptr)noexcept
	{
		return ::ImageList_Replace(m_hImageList, _Index, _hBitmap, _hBitmapMask);
	}

	// 移除指定位置的图像
	// @param _Index  指定位置，-1表示移除全部图像
	// @returns 成功返回true
	inline bool Remove(INT _Index)noexcept
	{
		::ImageList_Remove(m_hImageList, _Index);
	}

	// 获取指定位置的图标
	// @param _Index  指定位置
	// @returns 返回图标句柄，如果指定位置不存在，则返回nullptr
	inline HICON GetIcon(INT _Index)const noexcept
	{
		return ::ImageList_GetIcon(m_hImageList, _Index, m_Flags);
	}

	// 获取指定位置的位图
	// @param _Index  指定位置
	// @param _Type   获取类型。为true则返回掩码位图（MaskBitmao）
	// 返回位图句柄
	inline HBITMAP GetBitmap(INT _Index, bool _Type = false)const noexcept
	{
		IMAGEINFO Info = { 0 };
		::ImageList_GetImageInfo(m_hImageList, _Index, &Info);
		return _Type ? Info.hbmMask : Info.hbmImage;
	}

	// 保存图像列表数据
	// @param _Buffer  接受图像列表数据的缓冲区，有GlobalAlloc或GlobalReAlloc获得
	// @param _BufSize 缓冲区大小
	// @returns 保存成功返回ture，否则返回false
	inline bool Save(HGLOBAL _Buffer, SIZE_T _BufSize)noexcept
	{
		IStream* pIS = nullptr;

		if (E_OUTOFMEMORY == CreateStreamOnHGlobal(_Buffer, FALSE, &pIS))
		{
			return ::ImageList_Write(m_hImageList, pIS);
		}
		return false;
	}

	// 获取图像列表句柄
	inline HIMAGELIST GetHandle()const noexcept
	{
		return m_hImageList;
	}

private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (m_hImageList == nullptr)
			return;

		if (_Symbol == BkColor.symbol)
		{
			BkColor.value = ::ImageList_GetBkColor(m_hImageList);
		}
		else if (_Symbol == IconSize.symbol)
		{
			::ImageList_GetIconSize(m_hImageList, &IconSize.value.nWidth, &IconSize.value.nHeight);
		}
	}

	inline bool _property_set(int _Symbol)noexcept
	{
		if (m_hImageList == nullptr)
			return false;

		if (_Symbol == BkColor.symbol)
		{
			::ImageList_SetBkColor(m_hImageList, BkColor.value);
		}
		else if (_Symbol == IconSize.symbol)
		{
			return ::ImageList_SetIconSize(m_hImageList, IconSize.value.nWidth, IconSize.value.nHeight);
		}
		else return false;
		return true;
	}

private:
	HIMAGELIST m_hImageList;
	UINT m_Flags;

public:
#pragma region 属性
	// 本属性指定背景底色
	_property<ImageList, COLORREF> BkColor;

	// 本属性指定图标尺寸
	_property<ImageList, ICONSIZE> IconSize;

#pragma endregion


};

_WGUI_END

#endif // !_WGUI_IMAGELIST_
