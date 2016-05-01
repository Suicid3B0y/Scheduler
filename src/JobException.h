// JobException class

#ifndef JobException_class
#define JobException_class

#include <string>

class JobException
{
   public:
    JobException(std::string s) : m_s(s){};
    ~JobException(){};

    std::string description()
    {
        return m_s;
    }

   private:
    std::string m_s;
};

#endif
