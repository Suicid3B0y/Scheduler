#include "controllerTests.h"



int testController() {
    Job j1("a", 100, 1, 10);
    Job j2("b", 200, 2, 10);
    Job j3("c", 300, 2, 10);
    JobQueue queue;
    vector<Job> runningJobs = vector<Job>();

    queue.emplace(j1);
    queue.emplace(j2);
    queue.emplace(j3);

    Controller controller(queue, 1);
    runningJobs = controller.updateRunningJobs();  // Should start the job 2
    assert(runningJobs[0].command_line == j2.command_line);
    assert(runningJobs[0].startTime != 0);

    runningJobs = controller.updateRunningJobs();  // Should still have the job 2 started
    assert(runningJobs[0].command_line == j2.command_line);
    assert(runningJobs[0].startTime != 0);

    // Tell the job 2 is finished
    j2.isFinished = true;

    runningJobs = controller.updateRunningJobs();  // Should start the job 3
    //assert(runningJobs[0].command_line == j3.command_line);
    //assert(runningJobs[0].

    return 1;
}


