#include "networkTests.h"


using namespace std;


void testNetworkEntityManipulations() {
    ServerSocket s(4242);
}

void testMessageManipulations() {
    Message message(42, "hello");

    ASSERT((string)message == "*hello");

    message.load_from_string("ABCDEF");
    ASSERT((string)message == "ABCDEF");
}

void runNetworkSuite() {
    cute::suite s;

    s.push_back(CUTE(testMessageManipulations));

    cute::ide_listener<> lis;
    cute::makeRunner(lis)(s, "Network test suite");
}

