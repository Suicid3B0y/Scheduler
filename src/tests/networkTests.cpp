#include "networkTests.h"


using namespace std;


void testNetworkServer()
{
    MessageHandler handler;
    NetworkServer serv(4251, handler);
    BaseMessage to_send(42, "hello");

    // Start the server
    serv.start();
    ASSERT(serv.get_clients().size() == 0);

    // Initialize a client connection, test if it's well handled
    Socket client_socket("127.0.0.1", 4251);
    {
        int attempts = 10;
        while (attempts && !serv.get_clients().size())
        {
            this_thread::sleep_for(0.1s);
            --attempts;
        }
    }
    ASSERT(serv.get_clients().size() == 1);

    serv.stop();
}


// Fake server listener for testing the NetworkEntity class without using the NetworkServer class
class FakeServer
{
    private:
        Socket server;
        Socket client_socket;
        unsigned port;
        thread t;

    public:
        FakeServer(unsigned port): server{Socket{}}, client_socket{Socket{}}, t{}, port{port}
        {
            server.bind_to(port);
            thread subt(&FakeServer::handle_acceptance, this);
            t = std::move(subt);
        }

        void handle_acceptance()
        {
            server.accept(client_socket);
        }

        Socket get_remote()
        {
            t.join();
            return client_socket;
        }

};

void testNetworkEntityManipulations() {
    FakeServer serv{4250};
    BaseMessage to_send(42, "hello");

    // Simulate a socket connection
    Socket client_socket("127.0.0.1", 4250);
    Socket server_socket = serv.get_remote();
    NetworkEntity entity(server_socket, MessageHandler{}, "127.0.0.1", 0);

    // Send a message from the client to the server
    client_socket << to_send.encoded_message_length() << to_send;
    BaseMessage received(entity.get_message(42));
    ASSERT((string)received == "*hello");

    // Send a message from the server to the client
    entity << BaseMessage(42, "hello");
    std::string message;
    client_socket >> message;
    ASSERT(message == to_send.encoded_message_length() + "*hello");
}

void testSockets() {
    FakeServer serv{4249};

    // Simulate a socket connection
    Socket client_socket("127.0.0.1", 4249);
    Socket server_socket = serv.get_remote();
    string received;

    // Send a message from the client to the server
    client_socket << BaseMessage(42, "hello");
    server_socket >> received;
    ASSERT(received == "*hello");

    // Send a message from the server to the client
    server_socket << BaseMessage(42, "hello");
    client_socket >> received;
    ASSERT(received == "*hello");
}

void testMessageManipulations() {
    BaseMessage message(42, "hello");

    ASSERT((string)message == "*hello");

    message.load_from_string("ABCDEF");
    ASSERT((string)message == "ABCDEF");

    // Testing the message length encoding
    // Note: a message of length 10794 characters has its length normally encoded
    //       by the string "\0\0**". This is a big endian integer encoding, based
    //       on the following equivalent Python code :
    //              import struct; struct.pack(">I", length)

    char message_content[10793];
    for (unsigned i = 0; i < 10793; ++i)
        message_content[i] = 'i';

    BaseMessage message2(42, std::string(message_content, message_content + 10793));
    char encoded_length[4] = {'\0', '\0', '*', '*'};
    std::string expected(encoded_length, encoded_length + 4);

    ASSERT(message2.encoded_message_length() == expected);
    // It's just a little bit longer than Python code :)
}

void runNetworkSuite() {
    cute::suite s;

    s.push_back(CUTE(testMessageManipulations));
    s.push_back(CUTE(testSockets));
    s.push_back(CUTE(testNetworkEntityManipulations));
    s.push_back(CUTE(testNetworkServer));

    cute::ide_listener<> lis;
    cute::makeRunner(lis)(s, "Network test suite");
}

