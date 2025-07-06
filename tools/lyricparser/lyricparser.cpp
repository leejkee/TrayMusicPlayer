//
// Created by 31305 on 25-6-18.
//
#include "lyricparser.h"
#include <iostream>
#include <fstream>
#include <windows.h>

namespace BadFish::AudioToolkit
{
LyricParser::LyricParser(const std::filesystem::path& file_path)
{
    std::ifstream file_stream;
}

LyricParser::LyricParser(std::ifstream& file_stream)
{
    parse_lrc(file_stream);
}

LyricParser::~LyricParser() = default;

void LyricParser::read_file(const std::filesystem::path& file_path)
{
    // refactor
    std::ifstream lyric_stream(file_path, std::ios::binary);
    if (!lyric_stream.is_open())
    {
        std::cerr << "Error: Failed to open file\n";
        return;
    }
    std::string read_line;
    while (std::getline(lyric_stream, read_line))
    {
        if (read_line.empty())
        {
            continue;
        }
        std::string result_line;

        if (m_encoding == Encoding::UNKNOWN)
        {
            m_encoding = detect_encoding(read_line);
        }

        if (m_encoding == Encoding::UTF8)
        {
            result_line = read_line;
        }
        else
        {
            result_line = any_to_utf8(read_line, m_encoding);
        }
        m_lyric_original.emplace_back(result_line);
    }
}

LyricParser::Encoding LyricParser::detect_encoding(const std::string_view str_line)
{
    if (str_line.empty())
    {
        return Encoding::UNKNOWN;
    }

    // --- Step 1: Attempt to convert as GBK (Code Page 936) ---
    const int wide_char_len_gbk = MultiByteToWideChar(936
        , 0
        , str_line.data()
        , static_cast<int>(str_line.length())
        , nullptr
        , 0);
    if (wide_char_len_gbk > 0)
    {
        bool contains_high_bit_byte = false;
        for (const char c : str_line)
        {
            if (static_cast<unsigned char>(c) >= 0x80)
            {
                // there are non-ASCII characters, it's not just ASCII
                contains_high_bit_byte = true;
                break;
            }
        }
        if (contains_high_bit_byte)
        {
            return Encoding::GBK;
        }
    }

    // --- Step 2: Attempt to convert as UTF-8 ---
    const int wide_char_len_utf8 = MultiByteToWideChar(
         CP_UTF8
         , 0
         , str_line.data()
         , static_cast<int>(str_line.
             length())
         , nullptr
         , 0
        );

    if (wide_char_len_utf8 > 0)
    {
        return Encoding::UTF8;
    }

    // --- Step 3: Minimal UTF-16LE check (if desired, less common for "Chinese file content") ---
    if (str_line.length() % 2 == 0)
    {
        const auto* wide_str_ptr = reinterpret_cast<const wchar_t*>(str_line.data());
        const int wide_str_len = static_cast<int>(str_line.length() / sizeof(
            wchar_t));

        const int utf8_len = WideCharToMultiByte(
                                                 CP_UTF8
                                                 , 0
                                                 , wide_str_ptr
                                                 , wide_str_len
                                                 , nullptr
                                                 , 0
                                                 , nullptr
                                                 , nullptr
                                                );
        if (utf8_len > 0)
        {
            return Encoding::UTF16LE;
        }
    }

    // --- Step 4: Fallback ---
    return Encoding::UNKNOWN;
}

#if defined(_WIN32) || defined(_WIN64)
std::string LyricParser::any_to_utf8(const std::string_view any_str
                                     , const Encoding encoding)
{
    unsigned int code_page;
    switch (encoding)
    {
    case Encoding::UTF8:
        {
            code_page = CP_UTF8;
            return std::string(any_str);
        }
    case Encoding::UTF16LE:
        {
            code_page = 1200;
            break;
        }
    case Encoding::GBK:
        {
            code_page = 936;
            break;
        }
    default:
        {
            code_page = CP_UTF8;
            return {};
        }
    }
    // calculate the needed size for wide char string
    const int w_str_size = MultiByteToWideChar(code_page
                                               , 0
                                               , any_str.data()
                                               , -1
                                               , nullptr
                                               , 0);

    std::wstring w_str(w_str_size - 1, L'\0');

    MultiByteToWideChar(code_page
                        , 0
                        , any_str.data()
                        , -1
                        , w_str.data()
                        , w_str_size);

    const int size_utf8_str = WideCharToMultiByte(CP_UTF8
                                                  , 0
                                                  , w_str.c_str()
                                                  , -1
                                                  , nullptr
                                                  , 0
                                                  , nullptr
                                                  , nullptr);
    // -1 to exclude null terminator that WideCharToMultiByte adds

    std::string str_utf8(size_utf8_str, '\0');
    WideCharToMultiByte(CP_UTF8
                        , 0
                        , w_str.c_str()
                        , -1
                        , str_utf8.data()
                        , size_utf8_str
                        , nullptr
                        , nullptr);
    return str_utf8;
}

#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    std::string LyricParser::any_to_utf8(const std::string_view str
                                     , const Encoding encoding)
#endif


void LyricParser::parse_lrc(std::ifstream& file_stream)
{
    std::string read_line;
    std::smatch results_match;
    while (std::getline(file_stream, read_line))
    {
        trim_string(read_line);
        if (read_line.empty())
        {
            continue;
        }
        // 1. Tags match
        if (std::regex_match(read_line, results_match, s_regex_match_tag))
        {
            m_lyric_tags.emplace_back(results_match[1].str());
            continue;
        }
        // 1.

        // 2. Text match
        if (std::regex_match(read_line, results_match, s_regex_match_text))
        {
            std::int64_t start_ms = time_to_ms(results_match[1].str()
                                               , results_match[2].str()
                                               , results_match[3].str());
            std::string text = results_match[5].str();
            trim_string(text);
            std::string result;
            if (m_is_enhanced == State::Uninitialized)
            {
                if (std::regex_search(text
                                      , results_match
                                      , s_regex_search_enhanced_text))
                {
                    m_is_enhanced = State::True;
                }
                else
                {
                    m_is_enhanced = State::False;
                }
            }

            if (m_is_enhanced == State::True)
            {
                while (std::regex_search(text
                                         , results_match
                                         , s_regex_search_enhanced_text))
                {
                    std::string match_word = results_match[2].str();
                    trim_string(match_word);
                    match_word += ' ';
                    result.append(match_word);
                    text = results_match.suffix();
                }
                if (std::isspace(static_cast<unsigned char>(result.back())))
                {
                    result.pop_back();
                }
            }
            else if (m_is_enhanced == State::False)
            {
                result = text;
            }
            m_lyric_vector.emplace_back(start_ms, std::move(result));
        }
        // 2.
    }
}


std::vector<LyricLine> LyricParser::get_lrc_text() const
{
    return m_lyric_vector;
}

std::vector<std::string> LyricParser::get_lyric_tags() const
{
    return m_lyric_tags;
}

bool LyricParser::is_enhanced() const
{
    return m_is_enhanced == State::True;
}

std::int64_t LyricParser::time_to_ms(
    const std::string_view time_str)
{
    const std::string str{time_str};
    std::int64_t result{0};
    if (std::smatch time_match; std::regex_match(str
                                                 , time_match
                                                 , s_regex_match_time))
    {
        result = time_to_ms(time_match[1].str()
                            , time_match[2].str()
                            , time_match[3].str());
    }
    return result;
}

std::int64_t LyricParser::time_to_ms(
    const std::string_view min
    , const std::string_view sec
    , const std::string_view ms)
{
    const std::int64_t time_min{std::stoi(std::string{min})};
    const std::int64_t time_sec{std::stoi(std::string{sec})};
    const std::int64_t time_ms{std::stoi(std::string{ms})};
    return (time_min * 60 + time_sec) * 1000 + time_ms;
}

void LyricParser::clear_result()
{
    m_is_enhanced = State::Uninitialized;
    m_lyric_tags.clear();
    m_lyric_vector.clear();
}

const std::basic_regex<char> LyricParser::s_regex_match_tag{R"(\[(.*)\])"};

const std::basic_regex<char> LyricParser::s_regex_search_enhanced_text{
    R"(<([^>]+)>(.*?)(?=<|$))"
};

const std::basic_regex<char> LyricParser::s_regex_match_text{
    R"(\[(\d{1,2}):(\d{1,2})\.(\d{2,3})(?:\.(\d{2,3}))?\](.*))"
};

const std::basic_regex<char> LyricParser::s_regex_match_time{
    R"((\d{1,2}):(\d{1,2})\.(\d{2,3}))"
};
}
