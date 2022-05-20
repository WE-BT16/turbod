#ifndef TURBOD_LOGGER_H
#define TURBOD_LOGGER_H

#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

namespace turbod::logger {
    /**
     * Sets up the logger
     * @param path file to log to
     * @param silent enables silent mode
     */
    void setup(const std::string& path, bool silent);
}

#endif //TURBOD_LOGGER_H