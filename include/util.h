#pragma once

/**
 * A macro that implements something windows specific (first arg) and other systems (typically UNIX-like) (second arg)
 */
#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)
    #define CFS_IMPL_WIN_OTHER(windows_body, other_body){ { windows_body } }
#else
    #define CFS_IMPL_WIN_OTHER(windows_body, other_body){ { other_body } }
#endif

/* snippet from stack overflow: https://stackoverflow.com/a/62371749 */
// Windows does not define the S_ISREG and S_ISDIR macros in stat.h, so we do.
// We have to define _CRT_INTERNAL_NONSTDC_NAMES 1 before #including sys/stat.h
// in order for Microsoft's stat.h to define names like S_IFMT, S_IFREG, and S_IFDIR,
// rather than just defining  _S_IFMT, _S_IFREG, and _S_IFDIR as it normally does.
#define _CRT_INTERNAL_NONSTDC_NAMES 1
#include <sys/stat.h>
#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
  #define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif
#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
  #define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif
