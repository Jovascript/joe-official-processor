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
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;


int main(int ac, char *av[]) {
    auto console = spdlog::stdout_color_mt("console");
    spdlog::set_level(spdlog::level::info);
    po::variables_map vm;
    po::positional_options_description p;
    p.add("input-file", 1);
    po::options_description desc;

    desc.add_options()
            ("help,h", "produce help message")
            ("input-file", po::value<std::string>()->required(), "The binary file to execute")
            ("allocated-memory,m", po::value<int>()->default_value(1024))
            ("verbose,v", "log extra details");
    try {
        po::store(po::command_line_parser(ac, av).
                options(desc).positional(p).run(), vm);
        po::notify(vm);
        if (vm.count("help")) {
            cout << desc;
            return 0;
        }
        if (vm.count("verbose")) {
            spdlog::set_level(spdlog::level::debug);
        }
        if (vm.count("input-file")) {
            fs::path pt(vm["input-file"].as<std::string>());

            try {
                if (fs::exists(pt) && fs::is_regular_file(pt)) {
                    if (fs::file_size(pt) > vm["allocated-memory"].as<int>()) {
                        console->critical("Please allocate enough memory for the program.");
                        return 1;
                    }
                    std::ifstream input(pt.string(), std::ios::binary);
                    std::vector<char> vec;
                    vec.reserve(fs::file_size(pt));
                    // read the data:
                    std::copy(std::istreambuf_iterator<char>(input),
                              std::istreambuf_iterator<char>(),
                              std::back_inserter(vec));
                    console->debug("Vector size: {}", vec.size());
                    console->debug("File size: {}", fs::file_size(pt));
                    // Create memory holder
                    auto mem = make_shared<jop::RAMSegment>(0, vm["allocated-memory"].as<int>());
                    // Have to copy to reinterpret vector<char> to vector<jop::byte>
                    auto tempD = vector<jop::byte>(vec.begin(), vec.end());
                    string mstr;
                    for (auto d: tempD) {
                        mstr += fmt::format("{:02X} ", int(d));
                    }
                    console->debug(mstr);
                    auto tempL = jop::dataFromBytes(tempD);
                    string y;
                    for (auto d: tempL) {
                        y += fmt::format("{:04X} ", int(d));
                    }
                    console->debug(y);
                    mem->load(static_cast<int>(vec.size()), tempL.data());
                    auto iome = make_shared<jop::StandardIOHandler>();
                    jop::Processor x;
                    x.add_memory(static_pointer_cast<jop::MemoryHandler, jop::RAMSegment>(mem));
                    x.set_io_handler(static_pointer_cast<jop::InputOutputHandler, jop::StandardIOHandler>(iome));
                    try {
                        std::clock_t start = std::clock();
                        x.run();
                        console->info("Emulation finished in {}ms", (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000));
                    } catch (jop::ProcessorException &e) {
                        console->critical("Processor Error:");
                        console->critical(e.what());
                        return 1;
                    } catch (exception &e) {
                        console->critical(e.what());
                        return 1;
                    }

                } else {
                    console->critical("The file {} does not exist.", pt.c_str());
                    return 1;
                }
            } catch (const fs::filesystem_error &err) {
                console->critical(err.what());
                return 1;
            }
        }
    }
    catch (const po::error &x) {
        console->critical(x.what());
        return 1;
    }


    return 0;
}
