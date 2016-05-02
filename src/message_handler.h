#ifndef _H_MESSAGE_HANDLER
#define _H_MESSAGE_HANDLER

#include <iostream>
#include "constants.h"
#include "controller.h"


#define DEFAULT_MESSAGE_HANDLER_CLASS MessageHandler


/**
 * \class BaseMessage
 * \brief Represents a simple message, as defined by the protocol describe in network.h
 */
class BaseMessage
{
    protected:
        int opcode;  /*!< Operation code associated to the message. */
        std::string payload;  /*!< Payload of the message (arguments for example). */

    public:
        /**
         * \brief Constructor.
         */
        BaseMessage();

        /**
         * \brief Copy constructor.
         */
        BaseMessage(BaseMessage &message);

        /**
         * \brief Construct the message from a string.
         */
        explicit BaseMessage(const std::string raw_message);

        /**
         * \brief Destructor
         */
        virtual ~BaseMessage();

        /**
         * \brief Construct the message from an opcode and a payload.
         */
        BaseMessage(const unsigned char opcode, const std::string payload);

        /**
         * \brief Assignment operator.
         */
        BaseMessage& operator=(BaseMessage &message);

        /**
         * \brief Load the class attributes from a string.
         */
        void load_from_string(const std::string message);

        /**
         * \brief Getter of the payload
         */
        std::string getPayload();

        /**
         * \brief Get the opcode
         */
        int getOpCode();

        /**
         * \brief Run for the scheduler.
         */
        virtual void run();

        /**
         * \brief Run for the client.
         */
        virtual void client_run();

        /**
         * \brief Get the message length.
         */
        unsigned message_length() const;

        /**
         * \brief Get the message length encoded in little endian.
         */
        std::string encoded_message_length() const;

        /**
         * \brief Convert to a string.
         */
        operator const std::string() const;
};


/**
 * \class JobAppendMessage
 * \brief Message sent when a client want to append a new job.
 */
class JobAppendMessage: public BaseMessage
{
    public:
        JobAppendMessage(BaseMessage &message);
        JobAppendMessage(int user_priority, unsigned cpu_load, unsigned burst_time, std::string command_line);
        virtual void run(Controller &controller);
};


/**
 * \class MessageHandler
 * \brief Handle the message received through network entities.
 */
class MessageHandler
{
    private:
        Controller &controller;

    public:
        MessageHandler();
        /**
         * \brief Constructor.
         */
        MessageHandler(Controller &controller);

        /**
         * \brief Copy constructor.
         */
        MessageHandler(const MessageHandler &handler);

        /**
         * \brief Assignment operator.
         */
        MessageHandler& operator=(const MessageHandler &handler);

        /**
         * \brief Handle a received message.
         */
        virtual void handle_message(BaseMessage message);
};

#endif
