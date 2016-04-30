#include "socketTests.h"


using namespace std;

class SimpleEchoServer
{
    private:
        thread listening_thread;
        Socket socket;
        bool has_client;

    public:
        SimpleEchoServer(unsigned port): socket{}, has_client{false} {
            socket.bind_to(port);

            thread t(&SimpleEchoServer::unique_handling, this);
            listening_thread = move(t);
        }

        bool is_connected() {
            return has_client;
        }

        void unique_handling() {
            Socket client;
            socket.accept(client);
            has_client = true;

            while (1) {
                string message;
                client.recv(message);

                if (message == "\0")
                    break;

                client.send(message);
            }

            client.close();
            socket.close();
        }

        void join()
        {
            listening_thread.join();
        }
};

void testTransmission()
{
    // Internal testing function, not exposed to external scopes
    SimpleEchoServer server(8000);

    this_thread::sleep_for(1s);

    Socket client("127.0.0.1", 8000);

    while (!server.is_connected()) {
        this_thread::sleep_for(2s);
    }
    client.send("test");

    string data;
    client.recv(data);
    ASSERT(data == "test");

    client.send("\0");
    client.close();

    server.join();
}

void runSocketSuite() {
    cute::suite s;

    s.push_back(CUTE(testTransmission));

    cute::ide_listener<> lis;
    cute::makeRunner(lis)(s, "Socket test suite");
}
