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
    m_lyrics = readLycFile(file);
}

LyricParser::~LyricParser() = default;

std::vector<LyricLine> LyricParser::readLycFile(const std::string_view file)
{
    if (file.empty())
    {
        return {};
    }
    std::ifstream file_stream{std::string{file}};
    if (!file_stream.is_open())
    {
        std::cerr << "Failed to open file: " << file << std::endl;
        return {};
    }

    std::vector<LyricLine> tmp_lyric;
    std::string line;
    while (std::getline(file_stream, line))
    {
        std::smatch line_match = {};
        if (line.empty())
        {
            continue;
        }

        if (std::regex_match(line, line_match, m_normalRegex))
        {
            std::string time = line_match[1].str() + ":" + line_match[2].str() + "." + line_match[3].str();
            if (std::string text {line_match[5]}; std::regex_match(text, line_match, m_enhancedTextRegex))
            {
                std::string result;
                for (std::smatch sm; std::regex_search(text, sm, m_enhancedTextRegex);)
                {
                    result.append(sm.str());
                    text = sm.suffix();
                }
                m_lyrics.emplace_back(timeStringToMS(time), std::move(result), true);
            }
            else
            {
                m_lyrics.emplace_back(timeStringToMS(time), std::move(text), false );
            }
        }
        else if (std::regex_match(line, line_match, m_tagRegex))
        {
            if (line_match.size() == 3)
            {
                std::string tag = line_match[1].str() + ": " + line_match[2].str();
                m_lyricTags.push_back(tag);
            }
        }
    }
    return tmp_lyric;
}

std::vector<LyricLine> LyricParser::getLyricArray() const
{
    return m_lyrics;
}

std::vector<std::string> LyricParser::getLyricTags() const
{
    return m_lyricTags;
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

}