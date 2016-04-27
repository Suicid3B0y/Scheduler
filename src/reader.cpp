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
        Job job = Reader::parseString(line);
        jobs.push_back(job);
    }

    file.close();

    return jobs;

}

Job Reader::parseString(const std::string jobStr) {

    string commande_line;
    unsigned burst_time, cpu_load;
    int user_priority;

    istringstream in(jobStr);
    in >> burst_time >> user_priority >> cpu_load;
    getline(in, commande_line);

    Job job{commande_line, burst_time, user_priority, cpu_load};
    return job;
}
