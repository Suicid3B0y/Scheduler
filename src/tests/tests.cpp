#include "tests.h"


int main() {
    runJobSuite();
    runJobQueueSuite();
    runControllerSuite();
    runSocketSuite();
    runNetworkSuite();
}
