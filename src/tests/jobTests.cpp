#include "jobTests.h"


using namespace std;


void testShortJobInstantiation() {
    Job j("ls", 100);

    ASSERT(j.command_line == "ls");
    ASSERT(j.burst_time == 100);
}


void testLongJobInstantiation() {
    Job j("ls", 200, 1, 10);

    ASSERT(j.user_priority == 1);
    ASSERT(j.cpu_load == 10);
}


void testJobComparison() {
    Job j1("ls", 100, 1, 10);
    Job j2("ls", 200, 2, 10);
    Job j3("ls", 300, 2, 10);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    Job j4("ls", 300, 2, 10);


    ASSERT(j2 > j1);  // Lower user priority
    ASSERT(j2 > j3);  // Lower burst time
    ASSERT(j3 > j4);  // Aging
}


void testJobPriorityQueue() {
    job_ptr p_j1 = make_shared<Job>("ls", 100, 1, 10);
    job_ptr p_j2 = make_shared<Job>("la", 200, 2, 10);
    job_ptr p_j3 = make_shared<Job>("lb", 300, 2, 10);

    JobQueue queue;
    queue.push(p_j1);
    queue.push(p_j2);
    queue.push(p_j3);

    ASSERT(queue.top().get()->command_line == p_j2.get()->command_line);
    queue.pop();
    ASSERT(queue.top().get()->command_line == p_j3.get()->command_line);
    queue.pop();
    ASSERT(queue.top().get()->command_line == p_j1.get()->command_line);
}


void runJobSuite() {
    cute::suite s;

    s.push_back(CUTE(testShortJobInstantiation));
    s.push_back(CUTE(testLongJobInstantiation));
    s.push_back(CUTE(testJobComparison));

    cute::ide_listener<> lis;
    cute::makeRunner(lis)(s, "Job test suite");
}


void runJobQueueSuite() {
    cute::suite s;

    s.push_back(CUTE(testJobPriorityQueue));

    cute::ide_listener<> lis;
    cute::makeRunner(lis)(s, "Job test suite");
}
