#include <errno.h>
#include <mntent.h>
#include <stdio.h>
#include <string.h>
#include <sys/mount.h>

#include "fs.h"
#include "util.h"

// Check if a directory is an active mountpoint. This will work only when /proc is mounted
static int ismnt(const char* dir, const char* mode)
{
    FILE* f = setmntent("/proc/mounts", "r");
    if(!f) return 0; // The file doesn't exist, let's assume it's not

    struct mntent* entry;
    int found = 0;
    while((entry = getmntent(f)))
    {
        if(!strcmp(entry->mnt_dir, dir))
        {
            if(mode)
            {
                if(hasmntopt(entry, mode))
                    found = 1;
            }
            else
            {
                found = 1;
            }

            break;
        }
    }

    endmntent(f);

    return found;
}

static void mount_safe(const char* sp, const char* dir, const char* fstype, unsigned long flag, const void* data)
{
    int status = mount(sp, dir, fstype, flag, data);
    if(status != 0 && errno != EBUSY) // errno will tell us if it was already mounted if ismnt() fails
    {
        panic("Couldn't mount %s on %s (%s): %s.", sp, dir, fstype, strerror(errno));
    }
}

static void mount_if_notmount(const char* sp, const char* dir, const char* fstype, unsigned long flag, const void* data)
{
    if(!ismnt(dir, NULL))
        mount_safe(sp, dir, fstype, flag, data);
}

void fs_mount_early(void)
{
    mount_if_notmount("proc", "/proc", "proc", 0, NULL);
	mount_if_notmount("devtmpfs", "/dev", "devtmpfs", 0, NULL);
	mount_if_notmount("sysfs", "/sys", "sysfs", 0, NULL);
}
