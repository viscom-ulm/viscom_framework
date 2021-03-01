/**
 * @file   main.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.24
 *
 * @brief  Program entry point.
 */

#include "app/CoordinatorNode.h"
#include "app/WorkerNode.h"

#include <core/main.h>

#include <core/spdlog/sinks/filesink.h>

#include <core/FrameworkInternal.h>
#include <core/app_internal/ApplicationNodeInternal.h>
#include <core/initialize.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>
#include <string_view>


int main(int argc, char** argv)
{
    try {
        constexpr std::string_view directory;
        constexpr std::string_view name = "viscomlabfw.log";
        constexpr std::string_view logTag = "viscom";

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);
        console_sink->set_pattern(fmt::format("[{}] [%^%l%$] %v", logTag));

#ifdef _MSC_VER
        auto devenv_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        devenv_sink->set_level(spdlog::level::err);
        devenv_sink->set_pattern(fmt::format("[{}] [%^%l%$] %v", logTag));
#endif

        std::shared_ptr<spdlog::sinks::base_sink<std::mutex>> file_sink;
        if constexpr (viscom::DEBUG_MODE) {
            file_sink = std::make_shared<viscom::spdlog::sinks::rotating_open_file_sink_mt>(
                directory.empty() ? std::string(name) : std::string().append(directory).append("/").append(name), 5);
            file_sink->set_level(spdlog::level::trace);
        }
        else {
            file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                directory.empty() ? std::string(name) : std::string().append(directory).append("/").append(name), 5);
            file_sink->set_level(spdlog::level::trace);
        }

#ifdef _MSC_VER
        spdlog::sinks_init_list sink_list = { file_sink, console_sink, devenv_sink };
#else
        spdlog::sinks_init_list sink_list = { file_sink, console_sink };
#endif
        auto logger = std::make_shared<spdlog::logger>(std::string(logTag), sink_list.begin(), sink_list.end());

        spdlog::set_default_logger(logger);
        spdlog::flush_on(spdlog::level::err);

        if constexpr (viscom::DEBUG_MODE) {
            spdlog::set_level(spdlog::level::trace);
        }
        else {
            spdlog::set_level(spdlog::level::err);
        }

        spdlog::info("Log created.");

    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        return 0;
    }

    viscom::FWConfiguration config;
    if (argc > 1) config = viscom::LoadConfiguration(argv[1]);
    else config = viscom::LoadConfiguration("framework.cfg");

    auto appNode = Application_Init(config, [](viscom::ApplicationNodeInternal* node) { return std::make_unique<viscom::CoordinatorNode>(node); },
        [](viscom::ApplicationNodeInternal* node) { return std::make_unique<viscom::WorkerNode>(node); });

    // Main loop
    spdlog::info("Started Rendering.");
    appNode->Render();

    return 0;
}
