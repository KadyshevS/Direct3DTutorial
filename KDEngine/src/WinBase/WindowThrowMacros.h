#pragma once

#define KD_EXCEPT(hr) KDWindow::HrException(__LINE__, __FILE__, (hr))
#define KD_EXCEPT_LAST() KDWindow::HrException(__LINE__, __FILE__, GetLastError())
#define KD_EXCEPT_NOGFX() KDWindow::NoGfxException(__LINE__, __FILE__)