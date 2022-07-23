#pragma once
#ifndef _WGUI_DATETIMEPICK_
#define _WGUI_DATETIMEPICK_

#include "BaseControl.hpp"

_WGUI_BEGIN

class DateTimePicker
	: public BaseControl<DateTimePicker>
{
public:
	inline DateTimePicker()noexcept
		: BaseControl<DateTimePicker>(DATETIMEPICK_CLASSW)
	{}
};

_WGUI_END

#endif // !_WGUI_DATETIMEPICK_
