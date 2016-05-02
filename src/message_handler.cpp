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

BaseMessage::~BaseMessage()
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

std::string BaseMessage::getPayload()
{
    return payload;
}

int BaseMessage::getOpCode()
{
    return opcode;
}

BaseMessage::operator const std::string() const {
    return std::string(1, opcode) + payload;
}

void BaseMessage::run()
{
}

void BaseMessage::client_run()
{
}

// ----------------------------------------------------------------------------

JobAppendMessage::JobAppendMessage(BaseMessage &message)
{
    opcode = message.getOpCode();
    payload = message.getPayload();
}

JobAppendMessage::JobAppendMessage(int user_priority, unsigned cpu_load, unsigned burst_time, std::string command_line)
{
    opcode = 1;
    payload = std::to_string(user_priority) + "|" + std::to_string(cpu_load) + "|" + std::to_string(burst_time) + "|" + command_line;
}

void JobAppendMessage::run(Controller &controller)
{
    debug("A new job has been received. Appending to the queue." << std::endl);
    Job j{payload};
    std::vector<Job> vjob{j};
    controller.updateJobQueue(vjob);
}

// ----------------------------------------------------------------------------

MessageHandler::MessageHandler(): controller{*(new Controller{})}
{
    debug("Message handler instantiation" << std::endl);;
}

MessageHandler::MessageHandler(Controller &controller): controller{controller}
{
    debug("Constructor by controller assign" << std::endl);
}

MessageHandler::MessageHandler(const MessageHandler &handler): controller{handler.controller}
{
    debug("Test ?" << std::endl);
}

MessageHandler& MessageHandler::operator=(const MessageHandler &handler)
{
    return (*this);
}

void MessageHandler::handle_message(BaseMessage message)
{
    switch (message.getOpCode()) {
        case 1:
            ((JobAppendMessage)message).run(controller);
            break;
        default:
            debug("Invalid message !" << std::endl);
            break;
    }
}
