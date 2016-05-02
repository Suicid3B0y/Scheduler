#include "client.h"

#include "../network.h"
#include "../message_handler.h"

using namespace std;

int send_job(int user_priority, unsigned cpu_load, unsigned burst_time, string command_line) {
    cout << "Sending job" << endl;
    cout << "User prio : " << user_priority << endl;
    cout << "CPU load  : " << cpu_load << endl;
    cout << "Burst time: " << burst_time << endl;
    cout << "Command   : " << command_line << endl;

    Client client{"localhost", 3636};
    JobAppendMessage m{user_priority, cpu_load, burst_time, command_line};
    client.sendJob(m);

    return 0;
}

int main(int argc, char **argv) {
    int user_priority;
    unsigned cpu_load;
    unsigned burst_time;
    string command_line;

    po::options_description generic("Generic options");
    generic.add_options()
            ("help,h", "produce help message");

    po::options_description job_options("Job options");
    job_options.add_options()
        ("priority,p",
         po::value<int>(&user_priority)->default_value(DEFAULT_USER_PRIORITY),
         "user defined priority of the job")
        ("cpu-load,l",
         po::value<unsigned>(&cpu_load)->default_value(DEFAULT_CPU_LOAD),
         "CPU load for the job (from 1 to 100%)")
        ("burst-time,b",
         po::value<unsigned>(&burst_time),
         "Job's burst time")
        ("command-line,c",
         po::value<string>(&command_line),
         "Job's command line");

    po::options_description cmdline_options;
    cmdline_options.add(generic);
    cmdline_options.add(job_options);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << cmdline_options << std::endl;

        std::cout << "-- Usage examples --" << std::endl;
        std::cout << "  Create a basic job: -b {burst_time} -c {command_line}" << std::endl;
        std::cout << "                      -b 100 -c \"ls -larth\"" << std::endl;
        std::cout << "  Create a job with priority: -b {burst_time} -p {priority} -c {command_line}" << std::endl;
        std::cout << "                              -b 100 -p 1 -c \"ls -larth\"" << std::endl;
        std::cout << "  Create a job with cpu load: -b {burst_time} -l {cpu load} -c {command_line}" << std::endl;
        std::cout << "                              -b 100 -l 50 -l \"ls -larth\"" << std::endl;

        return 1;
    }

    if (!vm.count("burst-time")) {
        std::cout << "Burst time is required (option -b)." << std::endl;
        return 1;
    }

    if (!vm.count("command-line")) {
        std::cout << "Command line is required (option -c)." << std::endl;
        return 1;
    }

    send_job(user_priority, cpu_load, burst_time, command_line);

    /*
    Client client{"localhost", 3636};
    std::string jobStr;
    std::string formattedJobStr;
    std::string response;
    bool isAlive = true;

    while (isAlive) {
        std::cout << "Enter your job string (--help for format & example) (q for quit):";
        getline(cin, jobStr);

        if (jobStr == "q") {
            isAlive = false;
            socketClient.send("END TRANSMISSION\n");
            socketClient.close();
        } else {
            formattedJobStr = jobStr;
            formattedJobStr += '\n';

            socketClient.send(formattedJobStr);
            std::cout << "You sent : " << jobStr << std::endl;
            socketClient.recv(response);
            std::cout << "Server responded : " << response << std::endl;
        }
    }

    return 0;
    */
}
