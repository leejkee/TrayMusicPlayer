//
// Created by 31305 on 2025/6/26.
//

#pragma once
#include <lyricparser.h>
#include <filesystem>
#include <catch.hpp>
#include <utility>
using namespace BadFish::AudioToolkit;

template <typename CharT,
    typename = std::enable_if_t<std::is_same_v<CharT, char>
        || std::is_same_v<CharT, wchar_t>>>
class TestFileManager
{
public:
    explicit TestFileManager(std::filesystem::path filePath
                             , const std::vector<std::basic_string<CharT>>&
                             contentLines)
        : m_filePath(std::move(filePath))
    {
        std::basic_ofstream<CharT> outFile(m_filePath);
        if (!outFile.is_open())
        {
            std::cerr << "Failed to create test lyric file";
            return;
        }

        for (const auto& line : contentLines)
        {
            outFile << line << static_cast<CharT>('\n');
        }
        std::cout << "Test file created.";
        outFile.close();
    }

    ~TestFileManager()
    {
        if (!m_filePath.empty() && std::filesystem::exists(m_filePath))
        {
            std::filesystem::remove(m_filePath);
            std::cout << "Test file removed: " << m_filePath.string() << std::endl;
        }
    }

    TestFileManager(const TestFileManager&) = delete;

    TestFileManager& operator=(const TestFileManager&) = delete;

private:
    std::filesystem::path m_filePath;
};
