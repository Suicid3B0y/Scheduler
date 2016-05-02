#include "main.h"


using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;


int main(int argc, char **argv) {
    fs::path input_filepath;
    string command_line;
    unsigned time_slice;
    unsigned core_number;
    unsigned timeout;

    po::options_description generic("Generic options");
    generic.add_options()
            ("help,h", "produce help message");

    po::options_description config("File options");
    config.add_options()
            ("input-file,i", po::value<fs::path>(&input_filepath), "input filepath containing jobs");

    po::options_description queue("Queue options");
    config.add_options()
            ("time-slice,s", po::value<unsigned>(&time_slice)->default_value(100),
             "time elapsed between too priority check in milliseconds")
            ("core-number,c", po::value<unsigned>(&core_number)->default_value(1),
             "number of cores used by the scheduler")
            ("timeout,t", po::value<unsigned>(&timeout)->default_value(0),
             "maximum execution time in milliseconds of a job, if set to 0, scheduler will be non-preemptive");

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(queue);
    //cmdline_options.add(generic).add(config).add(queue).add(job);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << cmdline_options << "\n";
        return 1;
    }

    Scheduler scheduler(core_number, timeout, time_slice);

    if (vm.count("input-file")) {
        debug("Input file: " << input_filepath << endl);
        vector<Job> newJobs = Reader::parseFile(input_filepath);
        scheduler.updateJobQueue(newJobs);
    }

    scheduler.start();

    //Server server{scheduler};
    //server.start();

    scheduler.join();
    //server.join();

    return 0;
}
