#ifndef GSI_PROJECT_READER_H
#define GSI_PROJECT_READER_H


#include <string>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include "job.h"

using namespace std;

class Reader {

public :
    static std::queue<Job> parseFile(boost::filesystem::path filename);

};


#endif //GSI_PROJECT_READER_H
