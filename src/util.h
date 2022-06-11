#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <utility>

namespace turbo::util {
    std::pair<std::string, int> exec(const char *cmd) {
        std::array<char, 128> buffer;
        std::string result;
        int return_code = -1;
        auto pclose_wrapper = [&return_code](FILE *cmd)
        { return_code = pclose(cmd); };
        { // scope is important, have to make sure the ptr goes out of scope first
            const std::unique_ptr<FILE, decltype(pclose_wrapper)> pipe(popen(cmd, "r"), pclose_wrapper);
            if (pipe)
            {
                while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
                {
                    result += buffer.data();
                }
            }
        }
        return make_pair(result, return_code);
    }
}