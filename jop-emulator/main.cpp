//
// Created by Joseph on 09/09/2018.
//


#include <jopemulation.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;


int main(int ac, char* av[]) {
    po::variables_map vm;
    po::positional_options_description p;
    p.add("input-file", 1);
    po::options_description desc;

    desc.add_options()
            ("help,h", "produce help message")
            ("input-file", po::value<std::string>()->required(), "The binary file to execute")
            ("allocated-memory,m", po::value<int>()->default_value(1024))
            ;
    try {
    po::store(po::command_line_parser(ac, av).
            options(desc).positional(p).run(), vm);
    po::notify(vm);
    if (vm.count("help")) {
        cout << desc;
        return 0;
    }

    if(vm.count("input-file")) {
        fs::path pt(vm["input-file"].as<std::string>());

        try {
            if (fs::exists(pt) && fs::is_regular_file(pt)) {
                if (fs::file_size(pt) > vm["allocated-memory"].as<int>()) {
                    cerr << "Please allocate enough memory for the program." << endl;
                    return 1;
                }
                std::ifstream input( pt.string(), std::ios::binary );
                std::vector<char> vec;
                vec.reserve(fs::file_size(pt));
                // read the data:
                std::copy(std::istreambuf_iterator<char>(input),
                           std::istreambuf_iterator<char>(),
                                   std::back_inserter(vec));
                /*cout << "Vector size: " << vec.size() << endl;
                cout << "File size: " << fs::file_size(pt) << endl; */
                // Create memory holder
                auto mem = make_shared<jop::RAMSegment>(0, vm["allocated-memory"].as<int>());
                // Have to copy to reinterpret vector<char> to vector<jop::byte>
                mem->load(static_cast<int>(vec.size()), jop::dataFromBytes(vector<jop::byte>(vec.begin(), vec.end())).data());
                auto iome = make_shared<jop::StandardIOHandler>();
                jop::Processor x;
                x.add_memory(static_pointer_cast<jop::MemoryHandler, jop::RAMSegment>(mem));
                x.set_io_handler(static_pointer_cast<jop::InputOutputHandler, jop::StandardIOHandler>(iome));
                try {
                    x.run();
                } catch(jop::ProcessorException &e) {
                    cout << e.what();
                }

            } else {
                cerr << "The file " << pt << " does not exist." << endl;
            }
        } catch (const fs::filesystem_error& err) {
            cerr << err.what() << endl;
            return 1;
        }
    } }
    catch (const po::error &x) {
        cerr << x.what() << endl;
        return 1;
    }

    return 0;
}
