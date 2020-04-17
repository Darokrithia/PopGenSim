#ifndef ARR_DPRINTF_H
#  define ARR_DPRINTF_H
#  include <pthread.h>
#  include <stdio.h>
#  include <string.h>
/// If DPRINTF_ON is defined, DPRINTF((arg)) will acquire a mutex lock,
/// hand arg to printf, flush stdout, and release the lock. On error,
/// it prints an message and exits.
///
/// If DPRINTF_ON is not defined, DPRINTF is a noop.
///
/// The mutex "outputLock" is defined in dprintf.c, which must be linked
/// into executables that use DPRINTF.
#  ifdef DPRINTF_ON
#    define DPRINTF(arg) do{                                    \
        int  dprintf_status;                                    \
        dprintf_status=pthread_mutex_lock(&outputLock);         \
        if(dprintf_status) {                                    \
            fprintf(stderr,"%s:%s:%d: lock %d (%s)\n",          \
                    __FILE__,__func__,__LINE__,                 \
                    dprintf_status, strerror(dprintf_status));  \
            exit(1);                                            \
        }                                                       \
        printf arg ;                                            \
        fflush(stdout);                                         \
        dprintf_status = pthread_mutex_unlock(&outputLock);     \
        if(dprintf_status) {                                    \
            fprintf(stderr,"%s:%s:%d: unlock %d (%s)\n",        \
                    __FILE__,__func__,__LINE__,                 \
                    dprintf_status, strerror(dprintf_status));  \
            exit(1);                                            \
        }                                                       \
    }while(0)
#  else
#    define DPRINTF(arg)
#  endif
#endif
