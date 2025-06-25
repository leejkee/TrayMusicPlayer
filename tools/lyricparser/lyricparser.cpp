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
template <typename CharT, typename Enable>
LyricParser<CharT, Enable>::LyricParser(
    const std::basic_string_view<CharT> file)
{
    std::basic_ifstream<CharT> file_stream;
    readFile(file_stream, file);
    parseLRC(file_stream);
    file_stream.close();
}

template <typename CharT, typename Enable>
LyricParser<CharT, Enable>::~LyricParser() = default;

template <typename CharT, typename Enable>
void LyricParser<CharT, Enable>::readFile(std::basic_ifstream<CharT>& stream
                                          , std::basic_string_view<CharT> file)
{
    if (stream.is_open())
    {
        stream.close();
    }
    stream.open(file);
    if (!stream.is_open())
    {
        std::cerr << "Error: Failed to open file: " << file << std::endl;
    }
}

template <typename CharT, typename Enable>
void LyricParser<CharT, Enable>::parseLRC(std::basic_ifstream<CharT>& stream)
{
    std::basic_string<CharT> read_line;
    std::match_results<typename std::basic_string<CharT>::const_iterator>
            results_match;
    while (std::getline(stream, read_line))
    {
        trimString(read_line);
        if (read_line.empty())
        {
            continue;
        }
        // 1. Tags match
        if (std::regex_match(read_line, results_match, s_regexMatchTag))
        {
            m_lyricTags.emplace_back(results_match[1].str());
            continue;
        }
        // 1.

        // 2. Text match
        if (std::regex_match(read_line, results_match, s_regexMatchText))
        {
            std::int64_t start_ms = timeStringToMS(results_match[1].str()
                                                   , results_match[2].str()
                                                   , results_match[3].str());
            std::basic_string<CharT> text = results_match[5].str();
            std::basic_string<CharT> result;
            if (m_isEnhanced == State::Uninitialized)
            {
                if (std::regex_search(text
                                      , results_match
                                      , s_regexSearchEnhancedText))
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
                while (std::regex_search(text
                                         , results_match
                                         , s_regexSearchEnhancedText))
                {
                    result.append(results_match[2].str());
                    text = results_match.suffix();
                }
            }
            else if (m_isEnhanced == State::False)
            {
                result = text;
            }
            m_lyricVector.emplace_back(start_ms, std::move(result));
        }
        // 2.
    }
}

template <typename CharT, typename Enable>
void LyricParser<CharT, Enable>::trimString(std::basic_string<CharT>& str)
{
    str.erase(str.begin()
              , std::find_if(str.begin()
                             , str.end()
                             , [](const unsigned char ch)
                             {
                                 return !std::isspace(ch);
                             }));

    str.erase(std::find_if(str.rbegin()
                           , str.rend()
                           , [](const unsigned char ch)
                           {
                               return !std::isspace(ch);
                           }).base()
              , str.end());
    using namespace std::literals;
}


template <typename CharT, typename Enable>
std::vector<LyricLine<CharT>> LyricParser<CharT, Enable>::getLyricArray() const
{
    return m_lyricVector;
}

template <typename CharT, typename Enable>
std::vector<std::basic_string<CharT>> LyricParser<CharT,
    Enable>::getLyricTags() const
{
    return m_lyricTags;
}

template <typename CharT, typename Enable>
bool LyricParser<CharT, Enable>::isEnhanced() const
{
    return m_isEnhanced == State::True;
}

template <typename CharT, typename Enable>
std::int64_t LyricParser<CharT, Enable>::timeStringToMS(
    const std::basic_string_view<CharT> time_str)
{
    const std::basic_string<CharT> str{time_str};
    std::basic_string<CharT> result;
    if (std::match_results<typename std::basic_string<CharT>::const_iterator>
        time_match; std::regex_match(str, time_match, s_regexMatchTime))
    {
        result = timeStringToMS(time_match[1].str()
                                , time_match[2].str()
                                , time_match[3].str());
    }
    return result;
}

template <typename CharT, typename Enable>
std::int64_t LyricParser<CharT, Enable>::timeStringToMS(
    const std::basic_string_view<CharT> min
    , const std::basic_string_view<CharT> sec
    , const std::basic_string_view<CharT> ms)
{
    std::int64_t time_min{0};
    std::int64_t time_sec{0};
    std::int64_t time_ms{0};
    if constexpr (std::is_same_v<CharT, char>)
    {
        time_min = std::stoi(std::basic_string<CharT>{min});
        time_sec = std::stoi(std::basic_string<CharT>{sec});
        time_ms = std::stoi(std::basic_string<CharT>{ms});
    }
    else if constexpr (std::is_same_v<CharT, wchar_t>)
    {
        time_min = std::stol(std::basic_string<CharT>{min});
        time_sec = std::stol(std::basic_string<CharT>{sec});
        time_ms = std::stol(std::basic_string<CharT>{ms});
    }
    return (time_min * 60 + time_sec) * 1000 + time_ms;
}

template <>
const std::basic_regex<char> LyricParser<char>::s_regexMatchTag{R"(\[(.*)\])"};

template <>
const std::basic_regex<char> LyricParser<char>::s_regexSearchEnhancedText{
    R"(<([^>]+)>(.*?)(?=<|$))"
};

template <>
const std::basic_regex<char> LyricParser<char>::s_regexMatchText{
    R"(\[(\d{1,2}):(\d{1,2})\.(\d{2,3})(?:\.(\d{2,3}))?\](.*))"
};

template <>
const std::basic_regex<char> LyricParser<char>::s_regexMatchTime{
    R"((\d{1,2}):(\d{1,2})\.(\d{2,3}))"
};

template <>
const std::basic_regex<wchar_t> LyricParser<wchar_t>::s_regexMatchTag{
    LR"(\[(.*)\])"
};

template <>
const std::basic_regex<wchar_t> LyricParser<wchar_t>::s_regexSearchEnhancedText{
    LR"(<([^>]+)>(.*?)(?=<|$))"
};

template <>
const std::basic_regex<wchar_t> LyricParser<wchar_t>::s_regexMatchText{
    LR"(\[(\d{1,2}):(\d{1,2})\.(\d{2,3})(?:\.(\d{2,3}))?\](.*))"
};

template <>
const std::basic_regex<wchar_t> LyricParser<wchar_t>::s_regexMatchTime{
    LR"((\d{1,2}):(\d{1,2})\.(\d{2,3}))"
};
}
