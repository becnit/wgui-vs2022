#include "../../Include/core/BaseControl.hpp"

_WGUI_BEGIN

DWORD BaseCtrlID::m_BaseID = 100;

BaseCtrlID::BaseCtrlID()
{
	++m_BaseID;
}

_WGUI_END
