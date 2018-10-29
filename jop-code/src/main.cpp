//
// Created by Joseph on 26/09/2018.
//

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <antlr4-runtime.h>
#include "JScriptLexer.h"
#include "JScriptParser.h"

#include "ASTConstructorVisitor.h"
#include "ast.h"


using namespace antlr4;
using namespace JScript;

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int ac, char *av[]) {
    // Logger
    auto logger = spdlog::stdout_color_mt("console");
    spdlog::set_level(spdlog::level::info);

    // Program Arguments
    po::variables_map vm;
    po::positional_options_description p;
    p.add("input-file", 1);
    po::options_description desc;

    desc.add_options()
            ("help,h", "produce help message")
            ("input-file", po::value<std::string>()->required(), "The code file to compile")
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
                    ANTLRFileStream fileStream(pt.native());
                    JScriptLexer lexer(&fileStream);
                    CommonTokenStream tokens(&lexer);

                    tokens.fill();

                    JScriptParser parser(&tokens);

                    try {
                        parser.setErrorHandler(make_shared<BailErrorStrategy>());
                        tree::ParseTree* tree = parser.main();

                        std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

                        ASTConstructorVisitor visitor;
                        try {
                            auto result = unwrap<Block>(visitor.visit(tree));
                            cout << result->toString();
                        } catch (CompilerError &error) {
                            return EXIT_FAILURE;
                        }

                        return 0;
                    } catch (ParseCancellationException &e) {
                        try {
                            std::rethrow_if_nested(e);
                        } catch(std::exception &e1) {
                            logger->error(e1.what());
                        }
                        return EXIT_FAILURE;
                    }
                } else {
                    logger->critical("The file {} does not exist.", pt.c_str());
                    return 1;
                }
            } catch (const fs::filesystem_error &err) {
                logger->critical(err.what());
                return 1;
            }
        }
    }
    catch (const po::error &x) {
        logger->critical(x.what());
        return 1;
    }

}

