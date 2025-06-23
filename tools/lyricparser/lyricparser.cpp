//
// Created by 31305 on 25-6-18.
//

#include "lyricparser.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

namespace BadFish::AudioToolkit
{
LyricParser::LyricParser(const std::string_view file)
{
    readLycFile(file);
}

LyricParser::~LyricParser() = default;


void LyricParser::parseLRC(const std::string_view file)
{
    if (file.empty())
    {
        return;
    }
    std::ifstream file_stream{std::string{file}};
    if (!file_stream.is_open())
    {
        std::cerr << "Error: Failed to open file: " << file << std::endl;
        return;
    }
    std::string read_line;
    std::smatch line_match;
    while (std::getline(file_stream, read_line))
    {
        trimString(read_line);
        if (read_line.empty())
        {
            continue;
        }
        if (std::regex_match(read_line, line_match, m_regexMatchTag))
        {
            m_lyricTags.emplace_back(line_match[1].str());
            continue;
        }
        if (std::regex_match(read_line, line_match, m_regexMatchText))
        {
            std::int64_t start_ms = timeStringToMS(line_match[1].str(), line_match[2].str(), line_match[3].str());
            std::string text = line_match[5].str();
            std::string result;
            if (m_isEnhanced == State::Uninitialized)
            {
                if (std::smatch text_match; std::regex_search(text, text_match, m_regexSearchEnhancedText))
                {
                    m_isEnhanced = State::True;
                }
                else
                {
                    m_isEnhanced = State::False;
                }
            }

            if (m_isEnhanced == State::True)
            {
                std::smatch text_match;
                while (std::regex_search(text, text_match, m_regexSearchEnhancedText))
                {
                    result.append(text_match[2].str());
                    text = text_match.suffix();
                }
            }
            else if (m_isEnhanced == State::False)
            {
                result = text;
            }

            m_lyricVector.emplace_back(start_ms, std::move(result));
        }
    }
}

void LyricParser::trimString(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](const unsigned char ch) {
        return !std::isspace(ch);
    }));

    s.erase(std::find_if(s.rbegin(), s.rend(), [](const unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}



std::vector<LyricLine> LyricParser::getLyricArray() const
{
    return m_lyricVector;
}

std::vector<std::string> LyricParser::getLyricTags() const
{
    return m_lyricTags;
}

bool LyricParser::isEnhanced() const
{
    return m_isEnhanced == State::True;
}

std::int64_t LyricParser::timeStringToMS(const std::string_view time_str)
{
    using namespace std::literals;
    const std::string str{time_str};
    const std::regex time_regex{R"((\d{1,2}):(\d{1,2})\.(\d{2,3}))"};
    std::int64_t time_min{0};
    std::int64_t time_sec{0};
    std::int64_t time_ms{0};

    if (std::smatch time_match; std::regex_match(str, time_match, time_regex))
    {
        time_min = std::stoi(time_match[1]);
        time_sec = std::stoi(time_match[2]);
        time_ms = std::stoi(time_match[3]);
    }
    return (time_min * 60 + time_sec) * 1000 + time_ms;
}

std::int64_t LyricParser::timeStringToMS(const std::string_view min, const std::string_view sec, const std::string_view ms)
{
    const std::int64_t time_min{std::stoi(std::string{min})};
    const std::int64_t time_sec{std::stoi(std::string{sec})};
    const std::int64_t time_ms{std::stoi(std::string{ms})};
    return (time_min * 60 + time_sec) * 1000 + time_ms;
}

}