#include "client.h"

int main(int argc, char **argv) {

    po::options_description generic("Generic options");
    generic.add_options()
            ("help,h", "produce help message");

    po::options_description cmdline_options;
    cmdline_options.add(generic);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << cmdline_options << std::endl;
        std::cout << "-- Basic informations --" << std::endl;
        std::cout << "Input variables : burst_time user_priority cpu_load command_line" << std::endl;
        std::cout << "Variable types  : unsigned   int           int      string" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "--Examples--" << std::endl;
        std::cout << "120 1 10 toto -i myfile --core 3";
        std::cout << "---";
        std::cout << "For a program having this commandline 'toto -i myfile --core 3' " << std::endl;
        std::cout << "with a burst time of 120ms, a user priority of 1 and a cpu load of 10%" << std::endl;
        return 1;
    }

    Socket socketClient{"localhost", 3636};
    std::string jobStr;

    std::cout << "Enter your job string (--help for format & example) :";
    // TODO :  envoi de la taille
    std::cin >> jobStr;

    std::string formattedJobStr = jobStr.length() + " ";
    formattedJobStr += jobStr;

    std::cout << formattedJobStr << std::endl;

    socketClient.send(formattedJobStr);

    return 0;
}