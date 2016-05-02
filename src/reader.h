#ifndef GSI_PROJECT_READER_H
#define GSI_PROJECT_READER_H


#include <string>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <regex>
#include "job.h"
#include "JobException.h"

using namespace std;

/**
 * \class Reader
 * \brief Class used to parse a file.
 */
class Reader {

    private:

        /**
         * \brief Check if the job string is valid.
         */
        static bool is_valid(std::string job_literal);

    public:

        /**
         * \brief Returns a list of job from a file.
         */
        static std::vector<Job> parseFile(boost::filesystem::path filename);

        /**
         * \brief Parse a string representing a job, and instantiate a job from it.
         */
        static Job parseString(const std::string string);

};


#endif //GSI_PROJECT_READER_H
