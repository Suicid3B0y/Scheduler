#include "main.h"

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char **argv) {
    fs::path input_filepath;
    string command_line;
    int time_slice;    
    int core_number;
    int timout;
    int burst_time;
    int user_priority;
    int cpu_load;

    po::options_description generic("Generic options");
    generic.add_options()
            ("file-startup,F", "launches the scheduler and feed Job Queue with jobs contained in a specified file with the -i option")
            ("job-startup,J", "launches the scheduler and feed Job Queue with the specified job")
            //("version,v", "print version string")
            ("help, h", "produce help message");

    po::options_description config("File options");
    config.add_options()
            //("output-file,o", po::value<fs::path>(&output_filepath), "output filename filepath")
            ("input-file,i", po::value<fs::path>(&input_filepath), "input filepath containing jobs");

    po::options_description queue("Queue options");
    config.add_options()
            ("time-slice,s", po::value<int>(&time_slice)->default_value(100), "time elapsed between too priority check")
            ("core-number,c", po::value<int>(&core_number)->default_value(1), "number of cores used by the scheduler")
            ("timout,t", po::value<int>(&timout)->default_value(0), "maximum execution time in milliseconds of a job, if set to 0, scheduler will be non-preemptive");
            
    po::options_description job("Job options");
    config.add_options()
            ("command-line,l", po::value<string>(&command_line), "command line of the given job")
            ("burst-time,b", po::value<int>(&burst_time), "time the job will use the CPU to compute a job")
            ("user-priority,p", po::value<int>(&user_priority)->default_value(0), "priority set by the user, default value is 0")
            ("cpu-load,u", po::value<int>(&cpu_load)->default_value(0), "specified the CPU load of the given job");

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(queue).add(job);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << cmdline_options << "\n";
        return 1;
    }
    
    if (vm.count("file-startup") || vm.count("job-startup")) {
        // FOR DEBUG ONLY, TO REMOVE
        cout << "Queue options:" << endl;
        cout << "   time-slice: " << time_slice << endl;
        cout << "   core-number: " << core_number << endl;
        cout << "   timout: " << timout << endl;
    }
    
    if (vm.count("file-startup")) {
        if (vm.count("input-file")) {
            cout << "Input file: " << input_filepath << endl;
            // TODO : INITIATE JOB QUEUE WITH QUEUE OPTIONS
            // TODO : LAUNCH SCHEDULING WITH QUEUE FEED ON INPUT FILE


            std::queue<Job> job_queue;
            job_queue = Reader::parseFile(input_filepath);

            cout << "Queue size: " << job_queue.size() << endl;

        } else {
            cout << "No file specified, please use -f (or --input-file) option to specify a filepath" << endl;
        return 1;
        }
    }
    
    if (vm.count("job-startup")) {
        if (vm.count("command-line")) {
            if (vm.count("burst-time")) {
                cout << "Job options:" << endl;
                cout << "   command-line: " << command_line << endl;
                cout << "   burst-time: " << burst_time << endl;
                cout << "   user-priority: " << user_priority << endl;
                cout << "   cpu-load: " << cpu_load << endl;         
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
    }

    return 0;
}
