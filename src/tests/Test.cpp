#include <chrono>  // For thread sleeping
#include <thread>  // For thread sleeping
#include <assert.h>
#include "../Job/Job.h"

/** Unit tests **/

int testJobInstantiation() {
    Job j("ls", 100);

    assert(j.command_line == "ls");
    assert(j.burst_time == 100);
    
    Job j2("ls", 200, 1, 10);

    assert(j2.user_priority == 1);
    assert(j2.cpu_load == 10);
    assert(j2.timestamp >= j.timestamp);

    return 0;
}

int testJobComparison() {
    Job j1("ls", 100, 1, 10);
    Job j2("ls", 200, 2, 10);
    Job j3("ls", 300, 2, 10);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    Job j4("ls", 300, 2, 10);

    assert(j2 > j1);  // lower user priority
    assert(j2 > j3);  // lower burst time
    assert(j3 > j4);  // Aging
    
    return 0;
}

int testJobQueue() {
    Job j1("a", 100, 1, 10);
    Job j2("b", 200, 2, 10);
    Job j3("c", 300, 2, 10);

    JobQueue queue;
    queue.push(j1);
    queue.push(j2);
    queue.push(j3);

    assert(queue.top().command_line == j2.command_line);
    queue.pop();
    assert(queue.top().command_line == j3.command_line);
    queue.pop();
    assert(queue.top().command_line == j1.command_line);
    queue.pop();

    return 0;
}


int main(int argc, char** argv) {
    if (testJobInstantiation()) return 1;
    if (testJobComparison()) return 1;
    if (testJobQueue()) return 1;

    return 0;
}


