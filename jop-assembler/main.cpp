//
// Created by Joseph on 13/09/2018.
//
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "compiler.h"

using namespace std;

namespace po = boost::program_options;
namespace fs = boost::filesystem;


int main(int ac, const char* av[]) {
    po::variables_map vm;
    po::positional_options_description p;
    p.add("input-file", 1);
    po::options_description desc;

    desc.add_options()
            ("help,h", "produce help message")
            ("input-file", po::value<std::string>()->required(), "The assembly file to assemble.")
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
                    auto input = make_shared<std::ifstream>( pt.string());
                    try {

                        auto bins = compile(input);
                        input->close();
                        std::ofstream outputf;
                        outputf.exceptions(std::ofstream::badbit);
                        try {
                            outputf.open("file.binary", std::ios::out | std::ios::binary);
                            outputf.write(reinterpret_cast<const char *>(&bins[0]), bins.size());
                            outputf.close();
                        } catch (const std::ofstream::failure& e) {

                            cout << e.what();
                        }
                    } catch (CompilerError &e) {

                        cerr << e.what();
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
