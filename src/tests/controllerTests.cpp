#include "controllerTests.h"


void testController() {
    job_ptr p_j1 = make_shared<Job>("a", 100, 1, 10);
    job_ptr p_j2 = make_shared<Job>("b", 200, 2, 10);
    job_ptr p_j3 = make_shared<Job>("c", 300, 2, 10);

    JobQueue queue;

    queue.emplace(p_j1);
    queue.emplace(p_j2);
    queue.emplace(p_j3);

    Controller controller(queue, 1);
    
    vector<shared_ptr<Job>> runningJobs = controller.updateRunningJobs();  // Should start the job 2
    ASSERT(runningJobs.size() == 1);
    ASSERT(runningJobs[0].get()->command_line == p_j2.get()->command_line);
    ASSERT(runningJobs[0].get()->startTime != 0);

    runningJobs = controller.updateRunningJobs();  // Should still have the job 2 started
    ASSERT(runningJobs[0].get()->command_line == p_j2.get()->command_line);
    ASSERT(runningJobs[0].get()->startTime != 0);

    // Tell the job 2 is finished
    p_j2.get()->isFinished = true;

    runningJobs = controller.updateRunningJobs();  // Should start the job 3
    ASSERT(runningJobs[0].get()->command_line == p_j3.get()->command_line);
    ASSERT(runningJobs[0].get()->startTime != 0);
}


void runControllerSuite() {
    cute::suite s;

    s.push_back(CUTE(testController));

    cute::ide_listener<> lis;
    cute::makeRunner(lis)(s, "Controller test suite");
}
