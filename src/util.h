#ifndef __INFINIT__UTIL_H
#define __INFINIT__UTIL_H

// Misc

void console_init(void);


// Message logging

enum loglevel
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_NOPREFIX
};

void infinit_log(enum loglevel level, const char* msg, ...);
#define panic(msg, ...)                                     \
    infinit_log(LOG_ERROR, "fatal. " msg, ## __VA_ARGS__);  \
    for(;;);

#endif /* !__INFINIT__UTIL_H */
