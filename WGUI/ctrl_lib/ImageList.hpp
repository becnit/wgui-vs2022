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

// ͼ���б�
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

	// ����Ƿ��Ѵ���HIMAGELIST����
	inline bool IsCreate()const noexcept
	{
		return m_hImageList;
	}

	// ��ȡͼ������
	inline INT GetCount()const noexcept
	{
		return ::ImageList_GetImageCount(m_hImageList);
	}

	// ���ͼ��
	// @param _hIcon  ͼ����
	// @returns ����ͼ�������λ��
	inline INT Add(HICON _hIcon)noexcept
	{
		return ::ImageList_ReplaceIcon(m_hImageList, -1, _hIcon);
	}

	// ���λͼ
	// @param _hBitmap  λͼ���
	// @returns ����λͼ������λ��
	inline INT Add(HBITMAP _hBitmap, HBITMAP _hBitmapMask = nullptr)noexcept
	{
		return ::ImageList_Add(m_hImageList, _hBitmap, _hBitmapMask);
	}

	// �滻ָ��λ�õ�ͼ��
	// @param _Index  ָ��λ��
	// @param _hIcon  ͼ����
	// @returns ����ͼ�������λ��
	inline INT Replace(INT _Index, HICON _hIcon)noexcept
	{
		return ::ImageList_ReplaceIcon(m_hImageList, _Index, _hIcon);
	}

	// �滻ָ��λ�õ�λͼ
	// @param _Index  ָ��λ��
	// @param _hIcon  λͼ���
	// @returns ����λͼ������λ��
	inline INT Replace(INT _Index, HBITMAP _hBitmap, HBITMAP _hBitmapMask = nullptr)noexcept
	{
		return ::ImageList_Replace(m_hImageList, _Index, _hBitmap, _hBitmapMask);
	}

	// �Ƴ�ָ��λ�õ�ͼ��
	// @param _Index  ָ��λ�ã�-1��ʾ�Ƴ�ȫ��ͼ��
	// @returns �ɹ�����true
	inline bool Remove(INT _Index)noexcept
	{
		::ImageList_Remove(m_hImageList, _Index);
	}

	// ��ȡָ��λ�õ�ͼ��
	// @param _Index  ָ��λ��
	// @returns ����ͼ���������ָ��λ�ò����ڣ��򷵻�nullptr
	inline HICON GetIcon(INT _Index)const noexcept
	{
		return ::ImageList_GetIcon(m_hImageList, _Index, m_Flags);
	}

	// ��ȡָ��λ�õ�λͼ
	// @param _Index  ָ��λ��
	// @param _Type   ��ȡ���͡�Ϊtrue�򷵻�����λͼ��MaskBitmao��
	// ����λͼ���
	inline HBITMAP GetBitmap(INT _Index, bool _Type = false)const noexcept
	{
		IMAGEINFO Info = { 0 };
		::ImageList_GetImageInfo(m_hImageList, _Index, &Info);
		return _Type ? Info.hbmMask : Info.hbmImage;
	}

	// ����ͼ���б�����
	// @param _Buffer  ����ͼ���б����ݵĻ���������GlobalAlloc��GlobalReAlloc���
	// @param _BufSize ��������С
	// @returns ����ɹ�����ture�����򷵻�false
	inline bool Save(HGLOBAL _Buffer, SIZE_T _BufSize)noexcept
	{
		IStream* pIS = nullptr;

		if (E_OUTOFMEMORY == CreateStreamOnHGlobal(_Buffer, FALSE, &pIS))
		{
			return ::ImageList_Write(m_hImageList, pIS);
		}
		return false;
	}

	// ��ȡͼ���б���
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
#pragma region ����
	// ������ָ��������ɫ
	_property<ImageList, COLORREF> BkColor;

	// ������ָ��ͼ��ߴ�
	_property<ImageList, ICONSIZE> IconSize;

#pragma endregion


};

_WGUI_END

#endif // !_WGUI_IMAGELIST_
