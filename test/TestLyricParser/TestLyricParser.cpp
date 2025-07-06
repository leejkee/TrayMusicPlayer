//
// Created by 31305 on 2025/6/24.
//
#define CATCH_CONFIG_MAIN
#include <filehelper.h>
#include <lyricparser.h>
#include <catch.hpp>

using namespace BadFish::AudioToolkit;

TEST_CASE("LyricParserTest", "char-test")
{
    const std::string filename{"test.lyc"};
    const std::wstring w_filename{L"wtest.lyc"};

    SECTION("Normal Style Test")
    {
        const std::vector<std::string> normal_content_toT{
            "[t1: Test tag]"
            , " [t2: Test l space]"
            , "[t3: Test r space] "
            , " [t4: Test l-r space] "
            , "[00:00.001] Test lyric"
            , " [00:01.000] Test lyric"
            , "[01:00.000] Test lyric "
            , " [00:00.000] Test lyric "
            ,
        };

        const std::vector<std::string> expected_tags{
            "t1: Test tag"
            , "t2: Test l space"
            , "t3: Test r space"
            , "t4: Test l-r space"
        };

        const std::vector<LyricLine<char>> expected_content_lines{
            {1, "Test lyric"}
            , {1000, "Test lyric"}
            , {60000, "Test lyric"}
            , {0, "Test lyric"}
        };
        TestFileManager<char> fm(filename, normal_content_toT);

        const LyricParser<char> lyricParser{filename};
        REQUIRE(lyricParser.isEnhanced() == false);
        REQUIRE(lyricParser.getLyricTags() == expected_tags);
        REQUIRE(lyricParser.getLyricArray() == expected_content_lines);
    }

    SECTION("Enhanced Style Test")
    {
        const std::vector<std::string> normal_content_toT{
            "[t1: Test tag]"
            , " [t2: Test l space]"
            , "[t3: Test r space] "
            , " [t4: Test l-r space] "
            , "[00:00.001] <00:00.001> Test <00:00.014> lyric"
            , " [00:01.000] <00:01.001> Test <00:00.014> lyric"
            , "[01:00.000] <00:02.001> Test <00:00.014> lyric "
            , " [00:00.000] <00:01.001> Test <00:00.014> lyric "
            ,
        };

        const std::vector<std::string> expected_tags{
            "t1: Test tag"
            , "t2: Test l space"
            , "t3: Test r space"
            , "t4: Test l-r space"
        };

        const std::vector<LyricLine<char>> expected_content_lines{
                {1, "Test lyric"}
            , {1000, "Test lyric"}
            , {60000, "Test lyric"}
            , {0, "Test lyric"}
        };
        TestFileManager<char> fm(filename, normal_content_toT);

        const LyricParser<char> lyricParser{filename};
        REQUIRE(lyricParser.isEnhanced() == true);
        REQUIRE(lyricParser.getLyricTags() == expected_tags);
        REQUIRE(lyricParser.getLyricArray() == expected_content_lines);
    }

    SECTION("Normal Style Test (wchar_t)")
    {
        const std::vector<std::wstring> normal_content_toW{
            L"[t1: Test tag \u4E2D\u6587]"
            , L" [t2: Test l space \u6D4B\u8BD5]"
            , L"[t3: Test r space \u53F3] "
            , L" [t4: Test l-r space \u5DE6\u53F3] "
            , L"[00:00.001] Test lyric \u6B4C\u8BCD"
            , L" [00:01.000] Test lyric \u6B4C\u8BCD"
            , L"[01:00.000] Test lyric \u6B4C\u8BCD "
            , L" [00:00.000] Test lyric \u6B4C\u8BCD "
        };

        const std::vector<std::wstring> expected_tags{
            L"t1: Test tag \u4E2D\u6587"
            , L"t2: Test l space \u6D4B\u8BD5"
            , L"t3: Test r space \u53F3"
            , L"t4: Test l-r space \u5DE6\u53F3"
        };

        const std::vector<LyricLine<wchar_t>> expected_content_lines{
            {1, L"Test lyric \u6B4C\u8BCD"}
            , {1000, L"Test lyric \u6B4C\u8BCD"}
            , {60000, L"Test lyric \u6B4C\u8BCD"}
            , {0, L"Test lyric \u6B4C\u8BCD"}
        };

        TestFileManager<wchar_t> fm(w_filename, normal_content_toW);

        const LyricParser<wchar_t> lyricParser{w_filename};
        REQUIRE(lyricParser.isEnhanced() == false);
        REQUIRE(lyricParser.getLyricTags() == expected_tags);
        REQUIRE(lyricParser.getLyricArray() == expected_content_lines);
    }

    SECTION("Enhanced Style Test (wchar_t)")
    {
        const std::vector<std::wstring> normal_content_toW{
            L"[t1: Test tag \u4E2D\u6587]"
            , L" [t2: Test l space \u6D4B\u8BD5]"
            , L"[t3: Test r space \u53F3] "
            , L" [t4: Test l-r space \u5DE6\u53F3] "
            , L"[00:00.001] <00:00.001> Test <00:00.014> lyric \u6B4C\u8BCD"
            , L" [00:01.000] <00:01.001> Test <00:00.014> lyric \u6B4C\u8BCD"
            , L"[01:00.000] <00:02.001> Test <00:00.014> lyric \u6B4C\u8BCD "
            , L" [00:00.000] <00:01.001> Test <00:00.014> lyric \u6B4C\u8BCD "
        };

        const std::vector<std::wstring> expected_tags{
            L"t1: Test tag \u4E2D\u6587"
            , L"t2: Test l space \u6D4B\u8BD5"
            , L"t3: Test r space \u53F3"
            , L"t4: Test l-r space \u5DE6\u53F3"
        };

        const std::vector<LyricLine<wchar_t>> expected_content_lines{
            {1, L"Test lyric \u6B4C\u8BCD"}
            , {1000, L"Test lyric \u6B4C\u8BCD"}
            , {60000, L"Test lyric \u6B4C\u8BCD"}
            , {0, L"Test lyric \u6B4C\u8BCD"}
        };
        TestFileManager<wchar_t> fm(w_filename, normal_content_toW);
        const LyricParser<wchar_t> lyricParser{w_filename};
        REQUIRE(lyricParser.isEnhanced() == true);
        REQUIRE(lyricParser.getLyricTags() == expected_tags);
        REQUIRE(lyricParser.getLyricArray() == expected_content_lines);
    }
}
