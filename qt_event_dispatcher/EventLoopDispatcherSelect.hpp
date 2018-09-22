#pragma once

#if defined(UNIX_QEVENT_DISPATCHER)

#endif

#if defined(WINDOWS_QEVENT_DISPATCHER)
#include "qwindowsguieventdispatcher_p.h"
using QMainThreadEventDispatcher = QWindowsGuiEventDispatcher;
#endif

















