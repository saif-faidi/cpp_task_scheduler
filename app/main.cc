#include <iostream>

#include <spdlog/spdlog.h>

#include "lib.h"



int main([[maybe_unused]] int argc, [[maybe_unused]]char **argv)
{
    const auto welcome_message = "--------------------- Welcome to CppTaskScheduler! ---------------------";
    const auto spdlog_version = std::format(" Using spdlog lib version {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    spdlog::info(welcome_message);
    spdlog::info(spdlog_version);
    return 0;
}
