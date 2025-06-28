//
// Created by 31305 on 25-6-18.
//
#pragma once

namespace BadFish::AudioToolkit
{
template <typename CharT, typename Enable>
LyricParser<CharT, Enable>::LyricParser(const std::basic_string_view<CharT> file)
{
    std::basic_ifstream<CharT> file_stream;
    readFile(file_stream, file);
    parseLRC(file_stream);
}

template <typename CharT, typename Enable>
LyricParser<CharT, Enable>::LyricParser(std::basic_ifstream<CharT> &i_f_stream)
{
    parseLRC(i_f_stream);
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
    stream.open(file.data(), std::ios::in);
    if (!stream.is_open())
    {
        std::cerr << "Error: Failed to open file\n";
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
            trimString(text);
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
                    std::basic_string<CharT> match_word = results_match[2].str();
                    trimString(match_word);
                    match_word += ' ';
                    result.append(match_word);
                    text = results_match.suffix();
                }
                if (std::isspace(static_cast<unsigned char>(result.back())))
                {
                    result.pop_back();
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

template<typename CharT, typename Enable>
void LyricParser<CharT, Enable>::clearLyricContent()
{
    m_isEnhanced = State::Uninitialized;
    m_lyricTags.clear();
    m_lyricVector.clear();
}


}
