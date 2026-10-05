#ifndef SNAPMON_IOCTL_H
#define SNAPMON_IOCTL_H
#include <linux/ioctl.h>
#define SNAPMON_MAGIC 'S'
enum snapmon_event { SNAPMON_WRITE = 1, SNAPMON_SNAPSHOT = 2, SNAPMON_ROLLBACK = 3 };
#define SNAPMON_IOC_EVENT _IOW(SNAPMON_MAGIC, 1, int)
#endif
