#include <iostream>


#define DEFAULT_MESSAGE_HANDLER_CLASS MessageHandler

class MessageHandler
{
    public:
        MessageHandler();
        MessageHandler(const MessageHandler &handler);
        MessageHandler& operator=(const MessageHandler &handler);
};
