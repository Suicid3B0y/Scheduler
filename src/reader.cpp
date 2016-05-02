#include "reader.h"

std::vector<Job> Reader::parseFile(boost::filesystem::path filename) {

    std::vector<Job> jobs;
    string line;
    // stream pour le fichier
    ifstream file;

    file.open(filename.c_str());

    if (!file.is_open()) {
        cout << "Impossible de charger le fichier : " << filename << endl;
        throw "Impossible de charger le fichier";
    }

    // Traitement du nuage
    while (getline(file, line)) {
        try {
            Job job = Reader::parseString(line);
            jobs.push_back(job);
        } catch (JobException &e) {
            debug("[-] " << e.description());
        }
    }

    file.close();

    return jobs;

}

Job Reader::parseString(const std::string jobStr) {

    if (!is_valid(jobStr)) {
        throw JobException("Incorrect Job string, please respect our format.");
    }

    string commande_line;
    unsigned burst_time, cpu_load;
    int user_priority;

    istringstream in(jobStr);
    in >> burst_time >> user_priority >> cpu_load;
    getline(in, commande_line);

    Job job{commande_line, burst_time, user_priority, cpu_load};
    return job;
}



bool Reader::is_valid(std::string job_literal) {
    std::regex VALID_JOB_REGEX("^[0-9]+ [0-9]+ [0-9]+ .*$");
    return std::regex_match(job_literal, VALID_JOB_REGEX);
}