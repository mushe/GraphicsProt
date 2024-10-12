#include "Debug.h"

float Debug::MemoryUsage()
{
  float memoryUsage = 0.0f;

  #ifdef __APPLE__
  struct task_basic_info info;
  mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
      return 0.0f;
  }

  memoryUsage = info.resident_size / (float)(1024 * 1024);
  #endif

  return memoryUsage;
}