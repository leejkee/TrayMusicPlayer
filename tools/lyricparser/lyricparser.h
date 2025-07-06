//
// Created by 31305 on 25-6-18.
//
#pragma once
#include <string>
#include <algorithm>
#include <regex>
#include <string_view>
#include <vector>
#include <filesystem>


namespace BadFish::AudioToolkit
{
/// Enhanced LRC supported
/// to be completed
// struct EnhancedLyricLineInfo
// {
//     std::int64_t m_end_ms{0};
//     std::queue<std::int64_t> m_timing_queue;
//
//     explicit EnhancedLyricLineInfo() = default;
//
//     explicit EnhancedLyricLineInfo(const std::int64_t end_ms
//                                    , std::queue<std::int64_t>&& timing_q)
//         : m_end_ms(end_ms),
//           m_timing_queue(std::move(timing_q))
//     {
//     }
//
//     EnhancedLyricLineInfo& operator=(const EnhancedLyricLineInfo& other)
//     = default;
//
//     EnhancedLyricLineInfo& operator=(EnhancedLyricLineInfo&& other) noexcept
//     {
//         m_end_ms = other.m_end_ms;
//         m_timing_queue = std::move(other.m_timing_queue);
//         return *this;
//     }
// };
/// Enhanced LRC supported

struct LyricLine
{
    std::int64_t m_start_ms;

    std::string m_text;

    LyricLine() : LyricLine(0, std::string()){}

    LyricLine(const std::int64_t time_ms
              , std::string&& text)
        : m_start_ms(time_ms),
          m_text(std::move(text))
    {
    }

    LyricLine(const LyricLine& other) = default;

    LyricLine(LyricLine&& other) = default;

    LyricLine& operator=(const LyricLine& other) = default;

    LyricLine& operator=(LyricLine&& other) noexcept = default;

    bool operator==(const LyricLine& other) const
    {
        return (m_start_ms == other.m_start_ms) && (m_text == other.m_text);
    }

    bool operator!=(const LyricLine& other) const
    {
        return (m_start_ms != other.m_start_ms) || (m_text != other.m_text);
    }
};


class LyricParser
{
public:
    LyricParser() = default;

    explicit LyricParser(const std::filesystem::path& file_path);

    explicit LyricParser(std::ifstream& file_stream);

    ~LyricParser();

    enum class State
    {
        Uninitialized, True, False
    };

    enum class Encoding
    {
        UTF8,
        GBK,
        UTF16LE,
        UNKNOWN
    };

    void read_file(const std::filesystem::path& file_path);

    static Encoding detect_encoding(std::string_view str_line);

    void parse_lrc(std::ifstream& file_stream);

    template <typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char>
        || std::is_same_v<CharT, wchar_t>>>
    static void trim_string(std::basic_string<CharT>& str)
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

    static std::string any_to_utf8(std::string_view any_str, Encoding encoding = Encoding::UTF8);

    static std::int64_t time_to_ms(std::string_view time_str);

    static std::int64_t time_to_ms(std::string_view min
                                       , std::string_view sec
                                       , std::string_view ms);

    [[nodiscard]] std::vector<LyricLine> get_lrc_text() const;

    [[nodiscard]] std::vector<std::string> get_lyric_tags() const;

    [[nodiscard]] bool is_enhanced() const;

    Encoding get_encoding() const;

    void clear_result();

private:
    std::filesystem::path m_file_path;

    std::vector<std::string> m_lyric_original;

    std::vector<LyricLine> m_lyric_vector;

    std::vector<std::string> m_lyric_tags;

    State m_is_enhanced{State::Uninitialized};

    Encoding m_encoding{Encoding::UTF8};

    static const std::regex s_regex_match_tag;

    static const std::regex s_regex_search_enhanced_text;

    static const std::regex s_regex_match_text;

    static const std::regex s_regex_match_time;

};
}
