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

class Reader {
private:
    static bool is_valid(std::string job_literal);

public:
    static std::vector<Job> parseFile(boost::filesystem::path filename);

    static Job parseString(const std::string string);

};


#endif //GSI_PROJECT_READER_H
