//
// Created by 31305 on 25-6-18.
//
#pragma once
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <cstdint>
#include <queue>
#include <regex>

namespace BadFish::AudioToolkit
{

/// Enhanced LRC supported
/// to be completed
struct EnhancedLyricLineInfo
{
    std::int64_t m_end_ms{0};
    std::queue<std::int64_t> m_timing_queue;

    explicit EnhancedLyricLineInfo() = default;

    explicit EnhancedLyricLineInfo(const std::int64_t end_ms
                                   , std::queue<std::int64_t>&& timing_q)
        : m_end_ms(end_ms),
          m_timing_queue(std::move(timing_q))
    {
    }

    EnhancedLyricLineInfo& operator=(const EnhancedLyricLineInfo& other)
    = default;

    EnhancedLyricLineInfo& operator=(EnhancedLyricLineInfo&& other) noexcept
    {
        m_end_ms = other.m_end_ms;
        m_timing_queue = std::move(other.m_timing_queue);
        return *this;
    }
};
/// Enhanced LRC supported

struct LyricLine
{
    std::int64_t m_start_ms;
    std::string m_text;
    bool m_isEnhanced;

    LyricLine(const std::int64_t time_ms
              , std::string&& text
              , const bool isEnhanced)
        : m_start_ms(time_ms),
          m_text(std::move(text)),
          m_isEnhanced(isEnhanced)
    {
    }

    LyricLine& operator=(const LyricLine& other) = default;
    LyricLine& operator=(LyricLine&& other) = default;
};



class LyricParser
{
public:
    LyricParser() = default;
    explicit LyricParser(std::string_view file);
    ~LyricParser();
    std::vector<LyricLine> readLycFile(std::string_view file);

    [[nodiscard]] std::vector<LyricLine> getLyricArray() const;
    [[nodiscard]] std::vector<std::string> getLyricTags() const;

private:
    std::vector<LyricLine> m_lyrics;
    std::vector<std::string> m_lyricTags;

    inline static const std::regex m_normalRegex{
        R"(\[(\d{1,2}):(\d{1,2})\.(\d{2,3})(?:\.(\d{2,3}))?\](.*))"
    };
    inline static const std::regex m_enhancedTextRegex{R"(^<(.*)>)"};
    inline static const std::regex m_tagRegex{R"(\[(.*):(.*)\])"};
    static std::int64_t timeStringToMS(std::string_view time_str);
};
}
