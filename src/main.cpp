#include <filesystem>
#include <fstream>
#include <memory>
#include <sys/types.h>
#include <unistd.h>

#include <args.hpp>
#include <INIReader.h>

#include "util.h"
#include "logger/logger.h"

int main(int argc, char *argv[]) {
    const std::vector<std::string> args(argv + 1, argv + argc);
    args::ArgumentParser parser("Turbod", "");

    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    parser.Prog(argv[0]);

    args::Flag version(parser, "version", "Show the version of the program", {'v', "version"});
    args::Flag genOnly(parser, "gen-only", "Generate files then exit", {'g', "gen-only"});
    args::Flag silent(parser, "silent", "Dose not log anything to stdout", {'s', "silent"});

    args::ValueFlag<std::string> workDir(parser, "work-dir", "Specify the working directory", {"work-dir"});
    args::ValueFlag<std::string> webt16Path(parser, "we-bt16-path", "Specify the WE-BT16 path", {"we-bt16-path"});

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

    std::string webt16_path = "/usr/bin/we-bt16";

    if (bool{webt16Path}) {
        webt16_path = args::get(webt16Path);
    }

    if (!std::filesystem::is_directory(working_dir) || !std::filesystem::exists(working_dir)) {
        std::filesystem::create_directories(working_dir);
    }

    if (!std::filesystem::is_directory(working_dir + "/config") || !std::filesystem::exists(working_dir)) {
        std::filesystem::create_directory(working_dir + "/config");
    }

    if (!std::filesystem::is_directory(working_dir + "/logs") || !std::filesystem::exists(working_dir)) {
        std::filesystem::create_directory(working_dir + "/logs");
    }

    if (!std::filesystem::is_directory(working_dir + "/data") || !std::filesystem::exists(working_dir)) {
        std::filesystem::create_directory(working_dir + "/data");
    }

    if (!std::filesystem::exists(working_dir + "/config/config.ini")) {
        std::ofstream output(working_dir + "/config/config.ini", std::ios_base::app);
        output << "test: hello world" << std::endl;
    }

    if (std::filesystem::exists(working_dir + "/data/pid")) {
        std::filesystem::remove(working_dir + "/data/pid");
    }

    if (!std::filesystem::exists(working_dir + "/data/pid")) {
        std::ofstream output(working_dir + "/data/pid", std::ios_base::app);

        int __pid = getpid();
        output << "current=" << std::endl;
        output << "launched=NULL" << std::endl; 
    }

    turbod::logger::setup(working_dir + "/logs/core.log", bool{silent});
    auto logger = spdlog::get("main");

    logger->debug("using working directory: " + working_dir);
    int pid = getpid();
    char* _current_pid = (char*) malloc(6);
    sprintf(_current_pid, "%d", pid);

    const std::string current_pid(_current_pid);

    logger->debug("current pid: " + current_pid);

    if (bool{genOnly}) {
        // Exit here as all the files will have been generated
        exit(EXIT_SUCCESS);
    }

    if (!std::filesystem::exists(webt16_path)) {
        logger->error("Failed to locate WE-BT16 installion at " + webt16_path + "!");
        logger->debug("Stoping...");
        exit(EXIT_FAILURE);
    }

    std::string is_webt16_valid = webt16_path + " --is-valid";
    auto out = turbo::util::exec(is_webt16_valid.c_str());

    if (out.second != 0 && out.first != "signed-valid-2022") {
        logger->error("Invalid WE-BT16 installion at " + webt16_path + "!");
        logger->debug("Stoping...");
        exit(EXIT_FAILURE);
    }

    INIReader reader(working_dir + "/config/config.ini");

    if (reader.ParseError() != 0) {
        logger->error("Failed to load config file");
        return 1;
    }

    const std::string postgres_url = reader.Get("database", "postgres_url", "");
    const std::string redis_url = reader.Get("database", "redis_url", "");

    logger->info(postgres_url);
    logger->info(redis_url);

    // TODO: Start WE-BT16

    return EXIT_SUCCESS;
}
