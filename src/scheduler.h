#ifndef _H_SCHEDULER
#define _H_SCHEDULER


#include <iostream>


/**
 * Project constants definition
 */
#define DEBUG 1

/* Job constants */
#define DEFAULT_USER_PRIORITY 0
#define DEFAULT_CPU_LOAD 100


/**
 * Project macro definition
 */
#if DEBUG
    #define debug(text) std::cerr << text
#else
    #define debug(text)
#endif


#endif
