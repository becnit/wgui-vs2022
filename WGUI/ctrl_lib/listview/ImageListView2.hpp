#pragma once
#ifndef _WGUI_IMAGELISTVIEW_
#define _WGUI_IMAGELISTVIEW_

#include "BaseListView.hpp"
#include "../ImageList.hpp"

_WGUI_BEGIN

// 图像列表视图
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

	// 本属性指定标题过长时是否自动换行
	_property<ImageListView, bool> WarpText;

	// 
};

_WGUI_END

#endif // !_WGUI_IMAGELISTVIEW_