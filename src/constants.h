//
// Created by archangel on 25/04/16.
//

#ifndef GSI_SCHEDULER_CONSTANTS_H
#define GSI_SCHEDULER_CONSTANTS_H

#include <iostream>

/**
 * Project constants definition
 */
#define DEBUG 1
#define DEFAULT_PORT 3636

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

#endif //GSI_SCHEDULER_CONSTANTS_H
