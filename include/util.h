#pragma once

/**
 * A macro that implements something windows specific (first arg) and other systems (typically UNIX-like) (second arg)
 */
#ifdef _WIN32
#define CFS_IMPL_WIN_OTHER(windows_body, other_body){ { windows_body } }
#else
#define CFS_IMPL_WIN_OTHER(windows_body, other_body){ { other_body } }
#endif
