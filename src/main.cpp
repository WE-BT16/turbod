#include <filesystem>
#include <fstream>
#include <memory>

#include <args.hpp>
#include <yaml-cpp/yaml.h>

#include "logger/logger.h"

int main(int argc, char *argv[]) {
    const std::vector<std::string> args(argv + 1, argv + argc);
    args::ArgumentParser parser("Turbod", "");

    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    parser.Prog(argv[0]);

    args::Flag version(parser, "version", "Show the version of the program", {'v', "version"});
    args::Flag genOnly(parser, "gen-only", "Generate files then exit", {'g', "gen-only"});
    args::Flag silent(parser, "silent", "Dose not log anything to stdout", {'s', "silent"});
    args::Flag dryRun(parser, "dryrun", "Run without making any changes", {'n', "dry-run"});

    args::ValueFlag<std::string> workDir(parser, "work-dir", "Specify the working directory", {"work-dir"});

    try {
        auto next = parser.ParseArgs(args);

        if (bool{version}) {
            std::cout << "0.1.0" << std::endl;
            return EXIT_SUCCESS;
        }
    } catch (args::Help const &_) {
        std::cout << parser;
        return 0;
    } catch (args::Error const &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    std::string working_dir = "/var/we-bt16";

    if (bool{workDir}) {
        working_dir = args::get(workDir);
    }

    std::vector<std::string> arr;

    if (!std::filesystem::is_directory(working_dir) || !std::filesystem::exists(working_dir)) {
        arr.emplace_back("Propose creating directories: " + working_dir);
        std::filesystem::create_directories(working_dir);
    }

    if (!std::filesystem::is_directory(working_dir + "/config") || !std::filesystem::exists(working_dir)) {
        arr.emplace_back("Propose creating directory: " + working_dir + "/config");
        std::filesystem::create_directory(working_dir + "/config");
    }

    if (!std::filesystem::is_directory(working_dir + "/logs") || !std::filesystem::exists(working_dir)) {
        arr.emplace_back("Propose creating directory: " + working_dir + "/logs");
        std::filesystem::create_directory(working_dir + "/logs");
    }

    if (!std::filesystem::exists(working_dir + "/config/config.yaml")) {
        arr.emplace_back("Propose creating file: " + working_dir + "/config/config.yaml");
        std::ofstream output(working_dir + "/config/config.yaml", std::ios_base::app);
        output << "test: hello world" << std::endl;
    }

    turbod::logger::setup(working_dir + "/logs/core.log", bool{silent});
    auto logger = spdlog::get("main");

    logger->debug("using working directory: " + working_dir);

    if (bool{genOnly} || bool{dryRun}) {
        for(const std::string& i : arr) {
            logger->info(i);
        }
        // Exit here as all the files will have been generated
        exit(EXIT_SUCCESS);
    }

    YAML::Node config = YAML::LoadFile(working_dir + "/config/config.yaml");

    const std::string test = config["test"].as<std::string>();

    logger->info("test config key: " + test);

    return EXIT_SUCCESS;
}
