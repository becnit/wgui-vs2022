#pragma once
#ifndef _WAS_TOOLBAR_
#define _WAS_TOOLBAR_

#include "BaseControl.hpp"

_WGUI_BEGIN


template<class _ElemMessage = DefHandleMessage>
class ToolBar
	: public BaseControl<ToolBar<_ElemMessage>, _ElemMessage>
{
	using BaseCtrl = BaseControl<ToolBar<_ElemMessage>, _ElemMessage>;

public:

	inline ToolBar()noexcept
		: BaseCtrl()
	{

	}

	inline ~ToolBar()noexcept
	{

	}

public:
#pragma region ÏîÄ¿²Ù×÷

	inline UINT AddItem(HBITMAP _hBitmap)noexcept
	{}

	inline UINT AddItem(HINSTANCE _hResourcesInst, UINT _uResourcesID)noexcept
	{}


#pragma endregion

};



_WGUI_END


#endif // !_WAS_TOOLBAR_
