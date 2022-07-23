#pragma once

#ifndef _WGUI_
#define _WGUI_

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include "ctrl_lib/Winform.hpp"
#include "ctrl_lib/button/PushButton.hpp"
#include "ctrl_lib/button/3StateButton.hpp"
#include "ctrl_lib/button/CheckBox.hpp"
#include "ctrl_lib/button/RadioButton.hpp"
#include "ctrl_lib/button/SplitButton.hpp"
#include "ctrl_lib/button/GroupBox.hpp"
#include "ctrl_lib/button/CommandLink.hpp"
#include "ctrl_lib/ComboBox.hpp"
//#include "ctrl_lib/combobox/ComboBoxEx.hpp" // 未完全实现
#include "ctrl_lib/EditBox.hpp"
#include "ctrl_lib/TextBox.hpp"
#include "ctrl_lib/ScrollBar.hpp"
#include "ctrl_lib/ProgressBar.hpp"
#include "ctrl_lib/ListBox.hpp"
//#include "ctrl_lib/listview/ReportListView.hpp"

#endif // !_WGUI_