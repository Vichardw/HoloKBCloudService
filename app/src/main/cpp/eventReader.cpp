//
// Created by weijj3 on 2017/7/14.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "eventReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <poll.h>

//#define INPUT_DEVICE_NAME "lenovo-smartpad_ts"
#define INPUT_DEVICE_NAME "lenovo-keyboard"


static volatile int g_bStopService = false;
static int gDeviceFd = 0;

int openHoloKB() {
    LOG_FUNC_ENTRY;
    int version;
    int fd;
    char name[80];
    char location[80];
    char idstr[80];
    struct input_id id;

    const char *dirname = "/dev/input";
    const char *slash = "/";
    char devname[25];
    char filename[100];
    DIR *dir;
    struct dirent *de;
    dir = opendir(dirname);

    if (dir == NULL) {
        LOGE("could not open dir: %s, %d\n", dirname, errno);
        return -1;
    }

    LOGD("open dir,devname: %s \n", dirname);

    strcpy(devname, dirname);
    strcat(devname, slash);

    while ((de = readdir(dir))) {
        if (de->d_name[0] == '.'
            && (de->d_name[1] == '\0'
                || (de->d_name[1] == '.' && de->d_name[2] == '\0')))
            continue;

        strcpy(filename, devname);
        strcat(filename, de->d_name);
        LOGD("device name,de->d_name: %s \n", de->d_name);
        LOGD("device full path,filename: %s \n", filename);

        fd = open(filename, O_RDWR);

        if (fd >= 0) {
            if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), &name) < 1) {
                name[0] = '\0';
            }

            LOGD("open device,name: %s \n", name);

            if (!strcmp(name, INPUT_DEVICE_NAME)) {
                strcpy(name, filename);
                LOGD("open target device,filename: %s \n", filename);
                break;
            } else {
                close(fd);
                fd = -1;
            }
        }
    }
    closedir(dir);

    LOGD("Vichard Debug: find correct input device name: %s, open fd %d ", filename, fd );

    if (fd < 0) {
        LOGE("could not open device, %d\n", errno);
        return -1;
    }
    fd = open(filename, O_RDWR);

    if (ioctl(fd, EVIOCGVERSION, &version)) {
        LOGE("could not get driver version. %d\n", errno);
        return -1;
    }
    if (ioctl(fd, EVIOCGID, &id)) {
        LOGE("could not get driver id for %s, %s\n", filename, strerror(errno));
        return -1;
    }
    name[sizeof(name) - 1] = '\0';

    location[sizeof(location) - 1] = '\0';
    idstr[sizeof(idstr) - 1] = '\0';
    if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), &name) < 1) {
        LOGE("could not get device name for %s,\n", filename);
        name[0] = '\0';
    }
    if (ioctl(fd, EVIOCGPHYS(sizeof(location) - 1), &location) < 1) {
        location[0] = '\0';
    }
    if (ioctl(fd, EVIOCGUNIQ(sizeof(idstr) - 1), &idstr) < 1) {
        idstr[0] = '\0';
    }

    //LOGI("add device %d: %s\n", poll_fd, device);
    LOGD("  bus:      %04x\n"
                 "  vendor    %04x\n"
                 "  product   %04x\n"
                 "  version   %04x\n", id.bustype, id.vendor, id.product,
         id.version);
    LOGD("  name:     \"%s\"\n", name);
    LOGD("  location: \"%s\"\n"
            "  id:       \"%s\"\n", location, idstr);
    LOGD("  version:  %d.%d.%d\n", version >> 16, (version >> 8) & 0xff,
         version & 0xff);
    LOGD("device fd: %d", fd);
    LOG_FUNC_EXIT;
    return fd;
}

static void *readThread(void* arg) {
    LOG_FUNC_ENTRY;
    int res = 0;
    struct input_event event;

    int deviceFd =  *(int *)arg;
    if (deviceFd < 0) {
        LOGE("Unable to find input device\n");
    }
    LOGE("Vichard debug: the fd (%d) to poll", deviceFd);

    struct pollfd poll_fd;
    poll_fd.events = POLLIN;
    poll_fd.fd = deviceFd;
    poll_fd.revents = 0;

    while (!g_bStopService) {
        poll(&poll_fd, 1, -1);

        if (poll_fd.revents & (POLLIN)) {
            res = read(poll_fd.fd, &event, sizeof(event));

            LOGD("Vichard Debug: event size(%d), getsize (%d)",sizeof(event), res);
            if (res < (int)sizeof(event)) {
                LOGE("Could not get event\n");
                break;
            }

            switch(event.code) {
                case ABS_MT_SLOT:
                    LOGD("Vichard Debug: slot value(%d)", event.value);
                    break;
                case SYN_REPORT:
                    LOGD("Vichard Debug: slot SYNC");
                    break;
                case ABS_MT_TRACKING_ID:
                    LOGD("Vichard Debug: track id(%d)", event.value);
                    break;
                case ABS_MT_POSITION_X:
                    LOGD("Vichard Debug: X(%d)", event.value);
                    break;
                case ABS_MT_POSITION_Y:
                    LOGD("Vichard Debug: Y(%d)", event.value);
                    break;
                default:
                    LOGD("Vichard Debug: Other event");
                    break;
            }
            //Todo: handle input event.
        }
    }

    LOG_FUNC_EXIT;
    return NULL;
}

static void * dispatcherThread(void* arg) {
    LOG_FUNC_ENTRY;

    while (!g_bStopService) {
        //Todo, Handle
    }

    LOG_FUNC_EXIT;
    return 0;
}

int stopCollection(int fd_device){
    g_bStopService = true;
    sleep(2);
    close(fd_device);
}

int startCollection(int fd_device) {
    LOG_FUNC_ENTRY;
    int err;
    int iReadPid = 0, iHandlePid = 0;
    pthread_t sReadThread,sHandleThread;
    gDeviceFd = fd_device;

    LOGD("Vichard Debug: the opened fd(%d)", gDeviceFd);
    iReadPid = pthread_create(&sReadThread, NULL, &readThread,&gDeviceFd);
    if (iReadPid < 0) {
        LOGE("pthred_create error(%d) \n", iReadPid);
        return -1;
    }
    int status = pthread_detach(sReadThread);
    if (status != 0) {
        LOGE("failed to detach");
    }

    iHandlePid = pthread_create(&sHandleThread, NULL, &dispatcherThread, NULL);

    if (iHandlePid < 0) {
        LOGE("pthred_create error(%d) \n", iHandlePid);
        goto error;
    }

    status = pthread_detach(sHandleThread);
    LOGD("Dispatcher thread status: %d\n", status);

    if (status != 0) {
        LOGE("failed detach");
    }

    LOG_FUNC_EXIT;
    return 0;

error:
    g_bStopService = true;
    return -1;
}
