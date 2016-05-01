#include "server.h"

Server::Server(Scheduler &scheduler) : scheduler{scheduler}, alive{true} { }

void Server::run() {
    std::string line;
    std::string jobStr;
    unsigned port = 3636;
    try {
        Socket serverSocket;
        Socket clientSocket;

        debug("[ ] Sockets created." << std::endl);

        serverSocket.bind_to(port);
        serverSocket.accept(clientSocket);
        // TODO : gérer plusieurs client ou la déco reco d'un client
        debug("[ ] Client accepted." << std::endl);
        clientSocket.set_non_blocking(true);

        while (isAlive()) {
            try {
                /* Receiving line */
                if (clientSocket.recv(line)) {
                    jobStr = "";
                    /* Parsing line */
                    if (parseSocketString(line, jobStr)) {
                            debug("[ ] " << jobStr << std::endl);

                        /* If client wanted to disconnect */
                            if (jobStr == "END TRANSMISSION") {
                                clientSocket.close();
                                alive = false;
                                // TODO : gérer une fin plus propre pour la socket
                                break;
                            }

                        try {
                            /* Adding new job to queue */
                            Job newJob = Reader::parseString(jobStr);
                            vector<Job> newJob2 = {newJob};
                            scheduler.controller.updateJobQueue(newJob2);

                            debug("[ ] Jobs updated by socket" << std::endl);
                            clientSocket.send("[+] Job added to queue");

                        } catch (JobException &e) {
                            debug("[-] " << e.description() << std::endl);
                            clientSocket.send(e.description());
                        }
                    } else {
                        std::string error = "[-] String wasn't well received";
                        debug(error << std::endl);
                        clientSocket.send(error);
                    }
                }
            } catch (SocketException& e) {
                //debug("[-] " << e.description() << std::endl);
            }
            this_thread::sleep_for(std::chrono::duration<unsigned, std::milli>(2000));
        }

    } catch (SocketException &e) {
        std::cout << e.description() << std::endl;
    }
}

bool Server::parseSocketString(const std::string &line, std::string &result) {
    bool isComplete = false;
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] == '\n') {
            isComplete = true;
            break;
        } else {
            result += line[i];
        }
    }
    return isComplete;
}

void Server::start() {
    std::thread tmp(&Server::run, this);
    listening_thread = std::move(tmp);
    is_started = true;
}

bool Server::isAlive() {
    return alive;
}
