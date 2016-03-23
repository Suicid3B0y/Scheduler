#include "Reader.h"

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
        in >> commande_line >> burst_time >> user_priority >> cpu_load;

        /*
        cout << commande_line << endl;
        cout << burst_time << endl;
        cout << user_priority << endl;
        cout << cpu_load << endl;
         */

        Job job = Job(commande_line, burst_time, user_priority, cpu_load);

        job_queue.push(job);

    }

    file.close();

    return job_queue;


}