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
    //unsigned burst_time;
    //int user_priority;
    //unsigned cpu_load;

    po::options_description generic("Generic options");
    generic.add_options()
            ("file-startup,F",
             "launches the scheduler and feed Job Queue with jobs contained in a specified file with the -i option")
            //("job-startup,J", "launches the scheduler and feed Job Queue with the specified job")
            ("no-background", "launches the scheduler in foreground")
            //("version,v", "print version string")
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

    /*po::options_description job("Job options");
    config.add_options()
            ("command-line,l", po::value<string>(&command_line), "command line of the given job")
            ("burst-time,b", po::value<unsigned>(&burst_time), "time the job will use the CPU to compute a job")
            ("user-priority,p", po::value<int>(&user_priority)->default_value(0), "priority set by the user, default value is 0")
            ("cpu-load,u", po::value<unsigned>(&cpu_load)->default_value(0), "specified the CPU load of the given job");
*/
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

    if (!vm.count("no-background")) {
        // TODO : faire un daemon sysctl à l'avenir
        debug("Always launch the application with --no-background option until we made the deamon.\n");
        exit(200);
    }

    Scheduler scheduler(core_number, timeout, time_slice);

    if (vm.count("file-startup")) {
        if (vm.count("input-file")) {
            debug("Input file: " << input_filepath << endl);

            vector<Job> newJobs = Reader::parseFile(input_filepath);
            scheduler.updateJobQueue(newJobs);
            scheduler.run();

        } else {
            cerr << "No file specified, please use -i (or --input-file) option to specify a filepath" << endl;
            return 1;
        }
    }

    /*if (vm.count("job-startup")) {
        if (vm.count("command-line")) {
            if (vm.count("burst-time")) {
                debug("Job options:" << endl);
                debug("   command-line: " << command_line << endl);
                debug("   burst-time: " << burst_time << endl);
                debug("   user-priority: " << user_priority << endl);
                debug("   cpu-load: " << cpu_load << endl);
                // TODO : INITIATE JOB QUEUE WITH QUEUE OPTIONS
                // TODO : LAUNCH SCHEDULING WITH QUEUE FEED WITH THE SPECIFIED JOB
            } else {
                cout << "No burst time specified, please use -b (or --burst-time) option to specify the job's burst time" << endl;
                return 1;
            }
        } else {
            cout << "No command line specified, please use -l (or --command-line) option to specify a job command line" << endl;
            return 1;
        }
    }*/

    return 0;
}
