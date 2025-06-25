//
// Created by 31305 on 25-6-18.
//
#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <queue>
#include <regex>

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

template <typename CharT,
    typename = std::enable_if_t<std::is_same_v<CharT, char>
        || std::is_same_v<CharT, wchar_t>>>
struct LyricLine
{
    std::int64_t m_start_ms;
    std::basic_string<CharT> m_text;

    LyricLine(const std::int64_t time_ms
              , std::basic_string<CharT>&& text)
        : m_start_ms(time_ms),
          m_text(std::move(text))
    {
    }

    LyricLine& operator=(const LyricLine& other) = default;
    LyricLine& operator=(LyricLine&& other) = default;
};


template <typename CharT,
    typename = std::enable_if_t<std::is_same_v<CharT, char>
        || std::is_same_v<CharT, wchar_t>>>
class LyricParser
{
public:
    LyricParser() = default;
    explicit LyricParser(std::basic_string_view<CharT> file);
    ~LyricParser();

    enum class State
    {
        Uninitialized, True, False
    };

    static void readFile(std::basic_ifstream<CharT>& stream
                         , std::basic_string_view<CharT> file);

    void parseLRC(std::basic_ifstream<CharT>& stream);

    static void trimString(std::basic_string<CharT>& str);
    [[nodiscard]] std::vector<LyricLine<CharT>> getLyricArray() const;
    [[nodiscard]] std::vector<std::basic_string<CharT>> getLyricTags() const;
    [[nodiscard]] bool isEnhanced() const;

private:
    std::vector<LyricLine<CharT>> m_lyricVector;
    std::vector<std::basic_string<CharT>> m_lyricTags;
    State m_isEnhanced{State::Uninitialized};

    static const std::basic_regex<CharT> s_regexMatchTag;
    static const std::basic_regex<CharT> s_regexSearchEnhancedText;
    static const std::basic_regex<CharT> s_regexMatchText;
    static const std::basic_regex<CharT> s_regexMatchTime;

    static std::int64_t timeStringToMS(std::basic_string_view<CharT> time_str);
    static std::int64_t timeStringToMS(std::basic_string_view<CharT> min
                                       , std::basic_string_view<CharT> sec
                                       , std::basic_string_view<CharT> ms);
};
}
