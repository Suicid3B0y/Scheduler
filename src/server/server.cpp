#include "server.h"

Server::Server(Scheduler &scheduler) : scheduler{scheduler}, alive{true} { }

void Server::run() {
    std::string jobStr;
    unsigned port = 3636;
    try {
        Socket serverSocket;
        Socket clientSocket;

        std::cout << "Sockets created." << std::endl;

        serverSocket.bind_to(port);
        serverSocket.accept(clientSocket);

        clientSocket.set_non_blocking(true);

        while (isAlive()) {
            std::cout << "Receiving..." << std::endl;
            clientSocket.recv(jobStr);
            Job newJob = Reader::parseString(jobStr);
            vector<Job> newJob2 = {newJob};
            scheduler.controller.updateJobQueue(newJob2);
            std::cout << "Jobs updated by socket" << std::endl;
        }

    } catch (SocketException& e) {
        std::cout << e.description() << std::endl;
    }
}

void Server::start() {
    std::thread tmp(&Server::run, this);
    listening_thread = std::move(tmp);
    is_started = true;
}

bool Server::isAlive() {
    return alive;
}
