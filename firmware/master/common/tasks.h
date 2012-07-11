/*
 * Thundercracker Firmware -- Confidential, not for redistribution.
 * Copyright <c> 2012 Sifteo, Inc. All rights reserved.
 */

#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>

class Tasks
{
public:
    enum TaskID {
        UsbOUT,
        AudioPull,
        Debugger,
        AssetLoader,
        HomeButton,
        Profiler
    };

    static void init();
    
    /// Run pending tasks until no tasks are pending
    static void work();

    /// Block an idle caller. Runs pending tasks OR waits for a hardware event
    static void idle();

    /*
     * Tasks that have been set pending get called once each time
     * Tasks::work() is run.
     *
     * Call clearPending() to unregister your task. You can pend it again
     * at any time.
     */
    static void setPending(TaskID id, void *p = 0);
    static void clearPending(TaskID id);

    #ifndef SIFTEO_SIMULATOR
    static void waitForInterrupt()
    {
        /// Block until the next hardware event
        __asm__ __volatile__ ("wfi");
    }
    #endif

private:
    typedef void (*TaskCallback)(void *);

    static uint32_t pendingMask;
    struct Task {
        TaskCallback callback;
        void *param;
    };

    static Task TaskList[];
};

#endif // TASKS_H
