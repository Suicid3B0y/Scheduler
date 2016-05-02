// JobException class

#ifndef JobException_class
#define JobException_class

#include <string>


/**
 * \class JobException
 * \brief Exception raised when an issue is encoutered with a job.
 */
class JobException
{

    public:
        /**
         * \brief Constructor.
         */
        JobException(std::string s) : m_s(s){};

        /**
         * \brief Getter for the description.
         */
        std::string description()
        {
            return m_s;
        }

    private:
        std::string m_s;  /*!< Description container. */

};

#endif
