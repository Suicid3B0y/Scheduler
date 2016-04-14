#include "networkTests.h"


using namespace std;


// Fake server listener for testing the NetworkEntity class without using the NetworkServer class
class FakeServer
{
    private:
        ServerSocket server;
        ServerSocket client_socket;
        thread t;

    public:
        FakeServer(): server{ServerSocket(4242)}, client_socket{}, t{}
        {
            thread subt(&FakeServer::handle_acceptance, this);
            t = std::move(subt);
        }

        void handle_acceptance()
        {
            server.accept(client_socket);
        }

        ServerSocket get_remote()
        {
            t.join();
            return client_socket;
        }

};

void testNetworkEntityManipulations() {
    FakeServer serv{};
    Message to_send(42, "hello");

    // Simulate a socket connection
    ClientSocket client_socket("127.0.0.1", 4242);
    ServerSocket server_socket = serv.get_remote();
    NetworkEntity entity(server_socket, "127.0.0.1", make_shared<MessageHandler>(MessageHandler{}));

    // Send a message from the client to the server
    debug("[1] First attempt..." << endl);
    client_socket << to_send.encoded_message_length() << to_send;
    debug("[2] First attempt..." << endl);
    Message received(entity.get_message(42));
    debug("[3] First attempt..." << endl);
    ASSERT((string)received == "*hello");

    // Send a message from the server to the client
    entity << Message(42, "hello");
    std::string message;
    client_socket >> message;
    ASSERT(message == to_send.encoded_message_length() + "*hello");
}

void testSockets() {
    FakeServer serv{};

    // Simulate a socket connection
    ClientSocket client_socket("127.0.0.1", 4242);
    ServerSocket server_socket = serv.get_remote();
    string received;

    // Send a message from the client to the server
    client_socket << Message(42, "hello");
    server_socket >> received;
    ASSERT(received == "*hello");

    // Send a message from the server to the client
    server_socket << Message(42, "hello");
    client_socket >> received;
    ASSERT(received == "*hello");
}

void testMessageManipulations() {
    Message message(42, "hello");

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

    Message message2(42, std::string(message_content, message_content + 10793));
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

    cute::ide_listener<> lis;
    cute::makeRunner(lis)(s, "Network test suite");
}

