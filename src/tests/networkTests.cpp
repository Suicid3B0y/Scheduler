#include "networkTests.h"


using namespace std;


void testNetworkServer()
{
    MessageHandler handler;
    NetworkServer serv(4243, handler);
    BaseMessage to_send(42, "hello");

    // Start the server
    serv.start();
    ASSERT(serv.get_clients().size() == 0);

    // Initialize a client connection, test if it's well handled
    ClientSocket client_socket("127.0.0.1", 4243);
    {
        int attempts = 10;
        while (attempts && !serv.get_clients().size())
        {
            this_thread::sleep_for(1s);
            --attempts;
        }
    }
    ASSERT(serv.get_clients().size() == 1);

    // We do not need to automatically handle the messages. So stop this !
    serv.get_clients()[0]->stop();

    // Try to send a short message, for testing purpose only
    client_socket << to_send.encoded_message_length() + (string)to_send;
    BaseMessage received(serv.get_clients()[0]->get_message(42));
    ASSERT((string)received == (string)to_send);


    serv.stop();
}


// Fake server listener for testing the NetworkEntity class without using the NetworkServer class
class FakeServer
{
    private:
        ServerSocket server;
        std::shared_ptr<ServerSocket> p_client_socket;
        thread t;

    public:
        FakeServer(): server{ServerSocket(4242)}, p_client_socket{std::shared_ptr<ServerSocket>{new ServerSocket()}}, t{}
        {
            thread subt(&FakeServer::handle_acceptance, this);
            t = std::move(subt);
        }

        void handle_acceptance()
        {
            server.accept(p_client_socket);
        }

        std::shared_ptr<ServerSocket> get_remote()
        {
            t.join();
            return p_client_socket;
        }

};

void testNetworkEntityManipulations() {
    FakeServer serv{};
    BaseMessage to_send(42, "hello");

    // Simulate a socket connection
    ClientSocket client_socket("127.0.0.1", 4242);
    std::shared_ptr<ServerSocket> p_server_socket = serv.get_remote();
    NetworkEntity entity(p_server_socket, "127.0.0.1", make_shared<MessageHandler>(MessageHandler{}));

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
    FakeServer serv{};

    // Simulate a socket connection
    ClientSocket client_socket("127.0.0.1", 4242);
    std::shared_ptr<ServerSocket> server_socket = serv.get_remote();
    string received;

    // Send a message from the client to the server
    client_socket << BaseMessage(42, "hello");
    (*server_socket) >> received;
    ASSERT(received == "*hello");

    // Send a message from the server to the client
    (*server_socket) << BaseMessage(42, "hello");
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

