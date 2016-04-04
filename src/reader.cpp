#include "reader.h"

queue<Job> Reader::parseFile(boost::filesystem::path filename) {

    // stream pour le fichier
    ifstream file;
    file.open(filename.c_str());

    if (!file.is_open()) {
        cout << "Impossible de charger le fichier : " << filename << endl;
        throw "Impossible de charger le fichier";
    }

    queue<Job> job_queue;

    string line, commande_line;
    int burst_time, user_priority, cpu_load;

    // Traitement du nuage
    while (getline(file, line)) {

        istringstream in(line);
        in >> burst_time >> user_priority >> cpu_load;
        getline(in, commande_line);

        cout << "Command line: " << commande_line << endl;
        cout << "Burst time: " << burst_time << endl;
        cout << "User priority: " << user_priority << endl;
        cout << "CPU load: " << cpu_load << endl;

        Job job = Job(commande_line, burst_time, user_priority, cpu_load);

        job_queue.push(job);

    }

    file.close();

    return job_queue;


}
