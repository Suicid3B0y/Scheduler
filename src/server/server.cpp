#include "server.h"

Server::Server(Scheduler &scheduler) : is_started{false}, alive{true}, scheduler{scheduler}
{
}

void Server::run() {
    std::string line;
    std::string jobStr;
    unsigned port = 3636;
    try {
        Socket serverSocket;
        vector<Socket> clientSockets;

        debug("[ ] Sockets created." << std::endl);

        serverSocket.bind_to(port);

        bool deadClient = true;

        while (isAlive()) {
            if (deadClient) {
                Socket newClientServer;
                clientSockets.push_back(newClientServer);
                serverSocket.accept(clientSockets[clientSockets.size() - 1]);

                debug("[ ] Client accepted." << std::endl);
                newClientServer.set_non_blocking(true);
                deadClient = false;
            }
            try {
                /* Receiving line */
                if (clientSockets[clientSockets.size() - 1].recv(line)) {
                    jobStr = "";
                    /* Parsing line */
                    if (parseSocketString(line, jobStr)) {
                        debug("[ ] " << jobStr << std::endl);

                        /* If client wanted to disconnect */
                        if (jobStr != "END TRANSMISSION") {
                            try {
                                /* Adding new job to queue */
                                Job newJob = Reader::parseString(jobStr);
                                vector<Job> newJob2 = {newJob};
                                scheduler.updateJobQueue(newJob2);

                                debug("[ ] Jobs updated by socket" << std::endl);
                                clientSockets[clientSockets.size() - 1].send("[+] Job added to queue");

                            } catch (JobException &e) {
                                debug("[-] " << e.description() << std::endl);
                                clientSockets[clientSockets.size() - 1].send(e.description());
                            }
                        } else {
                            clientSockets[clientSockets.size() - 1].close();
                            deadClient = true;
                            // TODO : gérer une fin plus propre pour la socket
                            // FIXME: ne rentre pas ici si le client crash, et donc le serveur devient inutilisable
                        }
                    } else {
                        std::string error = "[-] String wasn't well received";
                        debug(error << std::endl);
                        clientSockets[clientSockets.size() - 1].send(error);
                    }
                }
            } catch (SocketException &e) {
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

void Server::join()
{
    listening_thread.join();
}
