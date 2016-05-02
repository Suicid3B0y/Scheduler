// SocketException class

#ifndef SocketException_class
#define SocketException_class

#include <string>

/**
 * \class SocketException
 * \brief Exception raised when an anormal issue happen while using the Socket class.
 */
class SocketException
{
    public:
        /**
         * \brief Constructor.
         */
        SocketException(std::string s) : m_s(s){};

        /**
         * \brief Getter for the description of the exception.
         */
        std::string description()
        {
            return m_s;
        }

    private:
        std::string m_s;  /*!< Contains the description of the exception. */
};

#endif
