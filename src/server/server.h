//
// Created by archangel on 30/04/16.
//

#ifndef GSI_SCHEDULER_SERVER_H
#define GSI_SCHEDULER_SERVER_H

#include <thread>
#include "../job.h"
#include "../reader.h"
#include "../socket/Socket.h"

class Server {
private:
    bool is_started;
    bool alive;
    void run();
    Scheduler &scheduler;

    bool isAlive();
public:
    Server(Scheduler &scheduler);
    std::thread listening_thread;
    void start();
};


#endif //GSI_SCHEDULER_SERVER_H
