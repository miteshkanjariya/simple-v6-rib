/*
 * Author: Mitesh Kanjariya
 * Description: header file defining debug/error macros
 */
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#define enable_debug 0

#define SHOW(format_str, ...) do {                 \
      printf(format_str "\n", ##__VA_ARGS__);      \
  } while(0)

#define DEBUG(format_str, ...) do {                \
    if (enable_debug)                              \
      printf(format_str "\n", ##__VA_ARGS__);      \
  } while(0)

#define ERROR(format_str, ...) do {                 \
      printf(format_str "\n", ##__VA_ARGS__);       \
  } while(0)


#endif /* __DEBUG_H__ */
