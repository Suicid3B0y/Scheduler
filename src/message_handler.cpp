#include "message_handler.h"


BaseMessage::BaseMessage(): BaseMessage(0, "")
{
}

BaseMessage::BaseMessage(BaseMessage& message): BaseMessage(message.opcode, message.payload)
{
}

BaseMessage::BaseMessage(const std::string raw_message): BaseMessage(raw_message[0], raw_message.substr(1))
{
}

BaseMessage::BaseMessage(const unsigned char opcode, const std::string payload): opcode{opcode}, payload{payload}
{
}

BaseMessage& BaseMessage::operator=(BaseMessage &message)
{
    opcode = message.opcode;
    payload = message.payload;

    return (*this);
}

void BaseMessage::load_from_string(const std::string message)
{
    opcode = message[0];
    payload = message.substr(1);
}

unsigned BaseMessage::message_length() const
{
    return payload.size() + 1;
}

std::string BaseMessage::encoded_message_length() const
{
    unsigned char packed[4];
    unsigned lgt = message_length();

    packed[0] = lgt >> 24;
    packed[1] = lgt >> 16 & 0xff;
    packed[2] = lgt >> 8 & 0xff;
    packed[3] = lgt & 0xff;

    return std::string(packed, packed + 4);
}

BaseMessage::operator const std::string() const {
    return std::string(1, opcode) + payload;
}

// ----------------------------------------------------------------------------

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

MessageHandler::MessageHandler(const MessageHandler &handler)
{
}

MessageHandler& MessageHandler::operator=(const MessageHandler &handler)
{
    return (*this);
}

void MessageHandler::handle_message(BaseMessage message)
{
    debug("Handling message !" << std::endl);
}
