#ifndef _H_MESSAGE_HANDLER
#define _H_MESSAGE_HANDLER

#include <iostream>
#include "constants.h"


#define DEFAULT_MESSAGE_HANDLER_CLASS MessageHandler


class BaseMessage
{
    private:
        int opcode;
        std::string payload;

    public:
        BaseMessage();
        BaseMessage(BaseMessage &message);
        explicit BaseMessage(const std::string raw_message);
        BaseMessage(const unsigned char opcode, const std::string payload);
        BaseMessage& operator=(BaseMessage &message);

        void load_from_string(const std::string message);
        unsigned message_length() const;
        std::string encoded_message_length() const;
        operator const std::string() const;
};


class MessageHandler
{
    public:
        MessageHandler();
        virtual ~MessageHandler();
        MessageHandler(const MessageHandler &handler);
        MessageHandler& operator=(const MessageHandler &handler);

        virtual void handle_message(BaseMessage message);
};

#endif
