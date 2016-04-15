#ifndef _H_TESTS_NETWORKTESTS
#define _H_TESTS_NETWORKTESTS


#include <thread>
#include <chrono>

#include "common.h"
#include "../network.h"
#include "../message_handler.h"
#include "../socket/ServerSocket.h"
#include "../socket/ClientSocket.h"


void runNetworkSuite();

#endif
