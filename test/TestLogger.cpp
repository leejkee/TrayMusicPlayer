
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <Logger.h>
#include <regex>

TEST_CASE("Logger::log return string format", "[logger]") {
    const std::string testModule = "TestModule";
    Core::Service::Logger logger(testModule);
    const std::string testMessage = "test message";

    SECTION("Correct format for INFO level") {
        std::string result = logger.log(Core::Service::Logger::LogLevel::Info, testMessage);
        
        // Expected pattern: [INFO] [YYYY-MM-DD HH:MM:SS] TestModule: test message\n
        std::regex expectedPattern(
            R"(\[INFO\] \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\] TestModule: test message\n)"
        );
        
        REQUIRE(std::regex_match(result, expectedPattern));
    }

    SECTION("Correct format for DEBUG level") {
        std::string result = logger.log(Core::Service::Logger::LogLevel::Debug, testMessage);
        
        std::regex expectedPattern(
            R"(\[DEBUG\] \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\] TestModule: test message\n)"
        );
        
        REQUIRE(std::regex_match(result, expectedPattern));
    }



    SECTION("Handles empty message") {
        std::string result = logger.log(Core::Service::Logger::LogLevel::Error, "");
        
        std::regex expectedPattern(
            R"(\[ERROR\] \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\] TestModule: \n)"
        );
        
        REQUIRE(std::regex_match(result, expectedPattern));
    }

    SECTION("Contains all required components") {
        std::string result = logger.log(Core::Service::Logger::LogLevel::Warning, testMessage);
        
        REQUIRE(result.find("[WARNING]") != std::string::npos);
        REQUIRE(result.find("TestModule:") != std::string::npos);
        REQUIRE(result.find(testMessage) != std::string::npos);
        REQUIRE(result.back() == '\n');
        
        size_t time_bracket_begin = result.find(']') + 1;
        // Verify timestamp appears between two spaces
        size_t first_bracket = result.find('[', time_bracket_begin);
        size_t second_bracket = result.find(']', first_bracket + 1);
        std::string timestamp = result.substr(first_bracket + 1, second_bracket - first_bracket - 1);
        
        REQUIRE(timestamp.size() == 19);  // YYYY-MM-DD HH:MM:SS
    }
}