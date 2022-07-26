#pragma once
#ifndef _WGUI_IMAGELISTVIEW_
#define _WGUI_IMAGELISTVIEW_

#include "BaseListView.hpp"
#include "../ImageList.hpp"

_WGUI_BEGIN

// ͼ���б���ͼ
template<class _ElemHandleMessage = DefHandleMessage>
class ImageListView
	: public BaseListView<ImageListView<_ElemHandleMessage>, _ElemHandleMessage>
{
	using BaseLV = BaseListView<ImageListView<_ElemHandleMessage>, _ElemHandleMessage>;
	using BaseCtrl = BaseControl<BaseLV, _ElemHandleMessage>;

public:
	inline ImageListView()noexcept
		: BaseLV(LVS_LIST)
	{}

	inline ~ImageListView()
	{}

public:

	ImageList Image;

	// ������ָ���������ʱ�Ƿ��Զ�����
	_property<ImageListView, bool> WarpText;

	// 
};

_WGUI_END

#endif // !_WGUI_IMAGELISTVIEW_