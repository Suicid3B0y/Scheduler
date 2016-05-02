#include "server.h"

ClientHandler::ClientHandler(const Socket socket, Scheduler &scheduler)
    : is_alive{false}, socket{socket}, scheduler{scheduler}
{
}

void ClientHandler::start()
{
    is_alive = true;
    std::thread tmp(&ClientHandler::handle_client, this);
    listening_thread = std::move(tmp);
}

void ClientHandler::stop()
{
    is_alive = false;
    listening_thread.join();
}

void ClientHandler::handle_client()
{
    try {
        while (is_alive) {
            if (socket.has_data()) {
                handle_reception();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    } catch (SocketException &e) {
    }

    is_alive = false;
}

void ClientHandler::handle_reception()
{
    std::string line, jobStr = "";
    socket.recv(line);

    if (parseSocketString(line, jobStr)) {
        debug("[ ] " << jobStr << std::endl);

        /* If client wanted to disconnect */
        if (jobStr != "END TRANSMISSION") {
            try {
                Job newJob = Reader::parseString(jobStr);
                vector<Job> newJob2{newJob};
                scheduler.updateJobQueue(newJob2);

                debug("[+] Jobs updated by a client." << std::endl);
                socket.send("[+] Job added to queue.");
            } catch (JobException &e) {
                debug("[-] " << e.description() << std::endl);
                socket.send(e.description());
            }
        } else {
            socket.close();
            is_alive = false;
        }
    } else {
        std::string error = "[-] String wasn't well received";
        debug(error << std::endl);
        socket.send(error);
        is_alive = false;
    }
}

bool ClientHandler::parseSocketString(const std::string &line, std::string &result) {
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

Server::Server(Scheduler &scheduler) : is_alive{false}, scheduler{scheduler}
{
}

void Server::handle_accept()
{
    socket.bind_to(3636);

    try {
        while (is_alive) {
            Socket s;
            if (socket.accept(s)) {
                debug("Connection !" << std::endl);
                std::unique_ptr<ClientHandler> pClient = std::make_unique<ClientHandler>(s, scheduler);
                clients.push_back(std::move(pClient));
                clients.back()->start();
            } else {
                debug("Waiting..." << std::endl);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    } catch (SocketException &e) {
        debug("[!] Error : the server cannot accept any other clients." << std::endl);
    }
}

void Server::start() {
    is_alive = true;
    std::thread tmp(&Server::handle_accept, this);
    listening_thread = std::move(tmp);
}

void Server::stop()
{
    is_alive = false;
    for (unsigned i = 0; i < clients.size(); ++i) {
        clients[i]->stop();
    }

    listening_thread.join();
}
