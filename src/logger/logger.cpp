#include "logger.h"

namespace turbod::logger {
    void setup(const std::string& path, const bool silent) {
        std::vector<spdlog::sink_ptr> sinks;

        if (!silent) {
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::info);

            sinks.push_back(console_sink);
        }

        auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_st>(path, 12, 0);
        file_sink->set_level(spdlog::level::trace);

        sinks.push_back(file_sink);

        auto logger = std::make_shared<spdlog::logger>("main", begin(sinks), end(sinks));

        logger->set_pattern("[%D %I] [thread %t] [%^%l%$] %v");
        logger->set_level(spdlog::level::trace);

        spdlog::register_logger(logger);
    }
}