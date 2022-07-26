#pragma once
#ifndef _WGUI_IMAGELIST_
#define _WGUI_IMAGELIST_

#include "../core/BaseControl.hpp"

_WGUI_BEGIN

typedef struct IMAGE_SIZE
{
	int Width;
	int Height;
}*PIMAGE_SIZE;

// 图像列表
class ImageList
{
private:
	inline ImageList(HIMAGELIST _hImageList = nullptr, UINT _Flags = 0)
		: m_hImageList(_hImageList)
		, m_Flags(_Flags)
		, BkColor(PROPERTY_INIT(ImageList))
		, Size(PROPERTY_INIT(ImageList))
		, Count(PROPERTY_INIT(ImageList))
	{
		// ImageList_GetOverlayImage();
	}
	
public:
#pragma region 构造与析构

	inline ImageList(INT _Width, INT _Height, UINT _Flags = ILC_MASK | ILC_COLOR32)noexcept
		: ImageList(::ImageList_Create(_Width, _Height, _Flags, 0, 0), _Flags)
	{}

	// 创建自字节数据
	inline ImageList(const void* _Buffer, size_t _BufSize)noexcept
		: ImageList()
	{
		if (_Buffer == nullptr || _BufSize == 0)
			return;
		
		IStream* pIS = nullptr;
		HGLOBAL hGlobal = ::GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, _BufSize);
		
		if (hGlobal == nullptr)
			return;

		if (E_OUTOFMEMORY == ::CreateStreamOnHGlobal(hGlobal, TRUE, &pIS))
		{
			void* pData = ::GlobalLock(hGlobal);
			
			if (pData == nullptr)
				return;
			::memcpy_s(pData, _BufSize, _Buffer, _BufSize);
			::GlobalUnlock(hGlobal);
			
			m_hImageList = ::ImageList_Read(pIS);
		}
		::GlobalFree(hGlobal);
	}

	inline ImageList(ImageList& _Right)noexcept
		: ImageList(_Right.m_hImageList, _Right.m_Flags)
	{
		_Right.m_hImageList = nullptr;
		_Right.m_Flags = 0;
	}
	
	inline ~ImageList()noexcept
	{
		if (m_hImageList)
			::ImageList_Destroy(m_hImageList);

		m_hImageList = nullptr;
		m_Flags = 0;
		BkColor.value = 0;
		Size.value = { 0, 0 };
	}

#pragma endregion

#pragma region 操作函数

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

	// 将一个或多个图像添加到图像列表，从指定的位图生成掩码
	// @param _hBitmap  包含一个或多个图像的位图句柄。图像的数量是从位图的宽度推断出来的。
	// @param _crMask   用于生成遮罩的颜色。指定位图中该颜色的每个像素都变为黑色，掩码中对应的位设置为1。
	//                  如果该参数为CLR_DEFAULT，则将(0,0)处像素的颜色作为掩码。
	// @returns 如果成功，则返回第一个新图像的索引，否则返回 -1
	inline INT AddMask(HBITMAP _hBitmap, COLORREF _crMask)noexcept
	{
		return ::ImageList_AddMasked(m_hImageList, _hBitmap, _crMask);
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

	// 将指定图像添加到要用作覆盖蒙版的图像列表中
	// @param _Index    要用作覆盖蒙版的图像的位置
	// @param _Overlay  覆盖掩码的从1开始的索引
	// @returns 成功返回true，失败返回false
	inline bool SetOverlayImage(INT _Index, INT _Overlay)noexcept
	{
		return ::ImageList_SetOverlayImage(m_hImageList, _Index, _Overlay);
	}
	
	// 复制给定图像列表中的图像
	// @param _IndexDst        要用作复制操作目标的图像的从零开始的索引
	// @param _ImageListSrc    作为复制操作目标的图像列表的句柄
	// @param _IndexSrc        要用作复制的图像的从零开始的索引
	// @param _Flags           指定要进行的复制操作类型的位标志值，ILCF_*
	// @returns 成功返回true，失败返回false
	inline bool Copy(INT _IndexDst, ImageList& _ImageSrc, INT _IndexSrc, UINT _Flags)noexcept
	{
		return ::ImageList_Copy(_ImageSrc.m_hImageList, _IndexSrc, m_hImageList, _IndexDst, _Flags) != FALSE;
	}
	
#pragma endregion

#pragma region 访问函数

	// 检索有关图像的信息。
	// @param _Index  指定图像的索引位置
	// @returns 返回图像的信息结构
	inline IMAGEINFO GetImageInfo(INT _Index)const noexcept
	{
		IMAGEINFO info;
		::ImageList_GetImageInfo(m_hImageList, _Index, &info);
		return info;
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

	// 获取图像列表副本
	inline ImageList Clone()const noexcept
	{
		ImageList clone(Size.value.Width, Size.value.Height, m_Flags);

		::ImageList_Destroy(clone.m_hImageList);
		clone.m_hImageList = ::ImageList_Duplicate(m_hImageList);
		return clone;
	}
	
	// 保存图像列表数据
	// @param _Buffer  接受图像列表数据的缓冲区，有GlobalAlloc或GlobalReAlloc获得
	// @param _BufSize 缓冲区大小
	// @returns 保存成功返回ture，否则返回false
	inline bool Save(HGLOBAL _Buffer, SIZE_T _BufSize)noexcept
	{
		if (_Buffer == nullptr || _BufSize == 0)
			return false;
		
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

	// 检查是否已创建HIMAGELIST对象
	inline bool IsCreate()const noexcept
	{
		return m_hImageList;
	}

#pragma endregion
	
private:
	inline void _property_get(int _Symbol)noexcept
	{
		if (m_hImageList == nullptr)
			return;

		if (_Symbol == BkColor.symbol)
		{
			BkColor.value = ::ImageList_GetBkColor(m_hImageList);
		}
		else if (_Symbol == Size.symbol)
		{
			::ImageList_GetIconSize(m_hImageList, &Size.value.Width, &Size.value.Height);
		}
		else if (_Symbol == Count.symbol)
		{
			Count.value = ::ImageList_GetImageCount(m_hImageList);
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
		else  if (_Symbol == Size.symbol)
		{
			return ::ImageList_SetIconSize(m_hImageList, Size.value.Width, Size.value.Height);
		}
		else if (_Symbol == Count.symbol)
		{
			return ::ImageList_SetImageCount(m_hImageList, Count.value);
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

	// 本属性指定图像列表的尺寸，修改时会删除所有图像
	_property<ImageList, IMAGE_SIZE> Size;

	// 本属性指定图像列表的图像数量
	_property<ImageList, UINT> Count;

	// 本属性指定
	
#pragma endregion


};

_WGUI_END

#endif // !_WGUI_IMAGELIST_
