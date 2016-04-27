#include "reader.h"

std::vector<Job> Reader::parseFile(boost::filesystem::path filename) {

    // stream pour le fichier
    ifstream file;
    file.open(filename.c_str());

    if (!file.is_open()) {
        cout << "Impossible de charger le fichier : " << filename << endl;
        throw "Impossible de charger le fichier";
    }

    std::vector<Job> jobs;

    string line, commande_line;
    unsigned burst_time;
    int user_priority, cpu_load;

    // Traitement du nuage
    while (getline(file, line)) {

        istringstream in(line);
        in >> burst_time >> user_priority >> cpu_load;
        getline(in, commande_line);

        cout << "Command line: " << commande_line << endl;
        cout << "Burst time: " << burst_time << endl;
        cout << "User priority: " << user_priority << endl;
        cout << "CPU load: " << cpu_load << endl;

        jobs.push_back(Job(commande_line, burst_time, user_priority, cpu_load));

    }

    file.close();

    return jobs;


}
