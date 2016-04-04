#ifndef _H_SCHEDULER
#define _H_SCHEDULER


/**
 * Project constants definition
 */
#define DEBUG 1


/**
 * Project macro definition
 */
#if DEBUG
    #define debug(text) std::cerr << text
#else
    #define debug(text)
#endif


#endif
