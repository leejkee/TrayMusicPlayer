//
// Created by 31305 on 2025/6/26.
//
#include "lyricparser.h"
namespace BadFish::AudioToolkit
{

template <>
const std::basic_regex<char> LyricParser<char, void>::s_regexMatchTag{R"(\[(.*)\])"};

template <>
const std::basic_regex<char> LyricParser<char, void>::s_regexSearchEnhancedText{
    R"(<([^>]+)>(.*?)(?=<|$))"
};

template <>
const std::basic_regex<char> LyricParser<char, void>::s_regexMatchText{
    R"(\[(\d{1,2}):(\d{1,2})\.(\d{2,3})(?:\.(\d{2,3}))?\](.*))"
};

template <>
const std::basic_regex<char> LyricParser<char, void>::s_regexMatchTime{
    R"((\d{1,2}):(\d{1,2})\.(\d{2,3}))"
};

template <>
const std::basic_regex<wchar_t> LyricParser<wchar_t, void>::s_regexMatchTag{
    LR"(\[(.*)\])"
};

template <>
const std::basic_regex<wchar_t> LyricParser<wchar_t, void>::s_regexSearchEnhancedText{
    LR"(<([^>]+)>(.*?)(?=<|$))"
};

template <>
const std::basic_regex<wchar_t> LyricParser<wchar_t, void>::s_regexMatchText{
    LR"(\[(\d{1,2}):(\d{1,2})\.(\d{2,3})(?:\.(\d{2,3}))?\](.*))"
};

template <>
const std::basic_regex<wchar_t> LyricParser<wchar_t, void>::s_regexMatchTime{
    LR"((\d{1,2}):(\d{1,2})\.(\d{2,3}))"
};
}