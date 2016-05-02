#include "jobTests.h"


using namespace std;


void testShortJobInstantiation() {
    Job j("ls", 100);

    ASSERT(j.getCommandLine() == "ls");
    ASSERT(j.getBurstTime() == 100);
}


void testLongJobInstantiation() {
    Job j("ls", 200, 1, 10);

    ASSERT(j.getUserPriority() == 1);
    ASSERT(j.getCPULoad() == 10);
}


void testJobPriorityQueue() {
    job_ptr p_j1 = make_shared<Job>("ls", 100, 1, 10);
    job_ptr p_j2 = make_shared<Job>("la", 200, 2, 10);
    job_ptr p_j3 = make_shared<Job>("lb", 300, 2, 10);

    JobQueue queue;
    queue.push(p_j1);
    queue.push(p_j2);
    queue.push(p_j3);

    ASSERT(queue.top().get()->getCommandLine() == p_j2.get()->getCommandLine());
    queue.pop();
    ASSERT(queue.top().get()->getCommandLine() == p_j3.get()->getCommandLine());
    queue.pop();
    ASSERT(queue.top().get()->getCommandLine() == p_j1.get()->getCommandLine());
}


void runJobSuite() {
    cute::suite s;

    s.push_back(CUTE(testShortJobInstantiation));
    s.push_back(CUTE(testLongJobInstantiation));

    cute::ide_listener<> lis;
    cute::makeRunner(lis)(s, "Job test suite");
}


void runJobQueueSuite() {
    cute::suite s;

    s.push_back(CUTE(testJobPriorityQueue));

    cute::ide_listener<> lis;
    cute::makeRunner(lis)(s, "Job test suite");
}
