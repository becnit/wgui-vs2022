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

// ͼ���б�
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
#pragma region ����������

	inline ImageList(INT _Width, INT _Height, UINT _Flags = ILC_MASK | ILC_COLOR32)noexcept
		: ImageList(::ImageList_Create(_Width, _Height, _Flags, 0, 0), _Flags)
	{}

	// �������ֽ�����
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

#pragma region ��������

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

	// ��һ������ͼ����ӵ�ͼ���б���ָ����λͼ��������
	// @param _hBitmap  ����һ������ͼ���λͼ�����ͼ��������Ǵ�λͼ�Ŀ���ƶϳ����ġ�
	// @param _crMask   �����������ֵ���ɫ��ָ��λͼ�и���ɫ��ÿ�����ض���Ϊ��ɫ�������ж�Ӧ��λ����Ϊ1��
	//                  ����ò���ΪCLR_DEFAULT����(0,0)�����ص���ɫ��Ϊ���롣
	// @returns ����ɹ����򷵻ص�һ����ͼ������������򷵻� -1
	inline INT AddMask(HBITMAP _hBitmap, COLORREF _crMask)noexcept
	{
		return ::ImageList_AddMasked(m_hImageList, _hBitmap, _crMask);
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

	// ��ָ��ͼ����ӵ�Ҫ���������ɰ��ͼ���б���
	// @param _Index    Ҫ���������ɰ��ͼ���λ��
	// @param _Overlay  ��������Ĵ�1��ʼ������
	// @returns �ɹ�����true��ʧ�ܷ���false
	inline bool SetOverlayImage(INT _Index, INT _Overlay)noexcept
	{
		return ::ImageList_SetOverlayImage(m_hImageList, _Index, _Overlay);
	}
	
	// ���Ƹ���ͼ���б��е�ͼ��
	// @param _IndexDst        Ҫ�������Ʋ���Ŀ���ͼ��Ĵ��㿪ʼ������
	// @param _ImageListSrc    ��Ϊ���Ʋ���Ŀ���ͼ���б�ľ��
	// @param _IndexSrc        Ҫ�������Ƶ�ͼ��Ĵ��㿪ʼ������
	// @param _Flags           ָ��Ҫ���еĸ��Ʋ������͵�λ��־ֵ��ILCF_*
	// @returns �ɹ�����true��ʧ�ܷ���false
	inline bool Copy(INT _IndexDst, ImageList& _ImageSrc, INT _IndexSrc, UINT _Flags)noexcept
	{
		return ::ImageList_Copy(_ImageSrc.m_hImageList, _IndexSrc, m_hImageList, _IndexDst, _Flags) != FALSE;
	}
	
#pragma endregion

#pragma region ���ʺ���

	// �����й�ͼ�����Ϣ��
	// @param _Index  ָ��ͼ�������λ��
	// @returns ����ͼ�����Ϣ�ṹ
	inline IMAGEINFO GetImageInfo(INT _Index)const noexcept
	{
		IMAGEINFO info;
		::ImageList_GetImageInfo(m_hImageList, _Index, &info);
		return info;
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

	// ��ȡͼ���б���
	inline ImageList Clone()const noexcept
	{
		ImageList clone(Size.value.Width, Size.value.Height, m_Flags);

		::ImageList_Destroy(clone.m_hImageList);
		clone.m_hImageList = ::ImageList_Duplicate(m_hImageList);
		return clone;
	}
	
	// ����ͼ���б�����
	// @param _Buffer  ����ͼ���б����ݵĻ���������GlobalAlloc��GlobalReAlloc���
	// @param _BufSize ��������С
	// @returns ����ɹ�����ture�����򷵻�false
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

	// ��ȡͼ���б���
	inline HIMAGELIST GetHandle()const noexcept
	{
		return m_hImageList;
	}

	// ����Ƿ��Ѵ���HIMAGELIST����
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
#pragma region ����
	// ������ָ��������ɫ
	_property<ImageList, COLORREF> BkColor;

	// ������ָ��ͼ���б�ĳߴ磬�޸�ʱ��ɾ������ͼ��
	_property<ImageList, IMAGE_SIZE> Size;

	// ������ָ��ͼ���б��ͼ������
	_property<ImageList, UINT> Count;

	// ������ָ��
	
#pragma endregion


};

_WGUI_END

#endif // !_WGUI_IMAGELIST_
