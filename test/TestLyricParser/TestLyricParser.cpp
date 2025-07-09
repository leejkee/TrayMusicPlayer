//
// Created by 31305 on 2025/6/24.
//
#define CATCH_CONFIG_MAIN
#include <filehelper.h>
#include <lyricparser.h>
#include <catch.hpp>

using namespace BadFish::AudioToolkit;

TEST_CASE("LyricParserNormalTest", "Normal-LRC Test")
{
    const std::string filename{"test.lyc"};
    const std::vector<std::string> normal_lrc_toT{
        " [t1: Test tag] "
        , " [标签: 测试标签] "
        , " [00:00.001] Test lyric"
        , " [00:01.010] 歌词测试"
    };

    const std::vector<std::string> expected_tags{
        "t1: Test tag"
        , "标签: 测试标签"
    };

    const std::vector<LyricLine> expected_content_lines{
        {1, "Test lyric"}
        , {1010, "歌词测试"}
    };

    SECTION("Normal-LRC Test, Encode: UTF8")
    {
        FileHelper fileHelper(filename);// de-constructor auto delete file
        fileHelper.write_to_file(normal_lrc_toT, FileHelper::Encoding::UTF8);
        const LyricParser lyricParser{filename};
        REQUIRE(lyricParser.get_encoding() == LyricParser::Encoding::UTF8);
        REQUIRE(lyricParser.is_enhanced() == false);
        REQUIRE(lyricParser.get_lyric_tags() == expected_tags);
        REQUIRE(lyricParser.get_lrc_text() == expected_content_lines);
    }

    // SECTION("Normal-LRC Test, Encode: UTF16LE")
    // {
    //     FileHelper fileHelper(filename);// de-constructor auto delete file
    //     fileHelper.write_to_file(normal_lrc_toT, FileHelper::Encoding::UTF16LE);
    //     const LyricParser lyricParser{filename};
    //     REQUIRE(lyricParser.get_encoding() == LyricParser::Encoding::UTF16LE);
    //     REQUIRE(lyricParser.is_enhanced() == false);
    //     REQUIRE(lyricParser.get_lyric_tags() == expected_tags);
    //     REQUIRE(lyricParser.get_lrc_text() == expected_content_lines);
    // }

    SECTION("Normal-LRC Test, Encode: GBK")
    {
        FileHelper fileHelper(filename);// de-constructor auto delete file
        fileHelper.write_to_file(normal_lrc_toT, FileHelper::Encoding::GBK);
        const LyricParser lyricParser{filename};
        REQUIRE(lyricParser.get_encoding() == LyricParser::Encoding::GBK);
        REQUIRE(lyricParser.is_enhanced() == false);
        REQUIRE(lyricParser.get_lyric_tags() == expected_tags);
        REQUIRE(lyricParser.get_lrc_text() == expected_content_lines);
    }
}

TEST_CASE("LyricParserEnhancedTest", "Enhanced-LRC Test")
{
    const std::string filename{"test_enhanced_complex.lyc"}; // Use a distinct filename
    const std::vector<std::string> enhanced_lrc_toT{
        " [t1: Tags Test] "
        , " [标签: 诗意歌词测试] "
        , "[00:05.123] <00:05.123> And <00:05.300> I <00:05.450> remember <00:06.000> all <00:06.200> my <00:06.500> childhood <00:07.000> dreams"
        , "[00:08.500] <00:08.500> I <00:08.650> find <00:08.800> it <00:09.000> hard <00:09.250> to <00:09.400> get <00:09.600> them <00:09.800> out <00:10.000> of <00:10.150> my <00:10.300> mind"
        , "[00:15.000] <00:15.000> 窗 <00:15.200> 透 <00:15.400> 初 <00:15.600> 晓 <00:16.000> 日 <00:16.200> 照 <00:16.400> 西 <00:16.600> 桥 <00:17.000> 云 <00:17.200> 自 <00:17.400> 摇"
        , "[00:20.000] <00:20.000> 想 <00:20.200> 你 <00:20.400> 当 <00:20.600> 年 <00:21.000> 荷 <00:21.200> 风 <00:21.400> 微 <00:21.600> 摆 <00:22.000> 的 <00:22.200> 衣 <00:22.400> 角"
        , "[00:25.000] <00:25.000> 木 <00:25.200> 雕 <00:25.400> 流 <00:25.600> 金 <00:26.000> 岁 <00:26.200> 月 <00:26.400> 涟 <00:26.600> 漪 <00:27.000> 七 <00:27.200> 年 <00:27.400> 前 <00:27.600> 封 <00:27.800> 笔"
        , "[00:30.000] <00:30.000> 因 <00:30.200> 为 <00:30.400> 我 <00:30.600> 今 <00:31.000> 生 <00:31.200> 挥 <00:31.400> 毫 <00:31.600> 只 <00:31.800> 为 <00:32.000> 你"
    };

    // Expected tags are trimmed of leading/trailing spaces
    const std::vector<std::string> expected_tags{
        "t1: Tags Test",
        "标签: 诗意歌词测试"
    };

    // Expected content lines will have timestamps from [MM:SS.ms]
    // and combined text from <MM:SS.ms>Word parts
    const std::vector<LyricLine> expected_content_lines{
        {5123, "And I remember all my childhood dreams"},
        {8500, "I find it hard to get them out of my mind"},
        {15000, "窗 透 初 晓 日 照 西 桥 云 自 摇"},
        {20000, "想 你 当 年 荷 风 微 摆 的 衣 角"},
        {25000, "木 雕 流 金 岁 月 涟 漪 七 年 前 封 笔"},
        {30000, "因 为 我 今 生 挥 毫 只 为 你"}
    };

    SECTION("Enhanced-LRC Test, Encode: UTF8")
    {
        FileHelper fileHelper(filename); // Destructor automatically deletes the file
        fileHelper.write_to_file(enhanced_lrc_toT, FileHelper::Encoding::UTF8);
        const LyricParser lyricParser{filename};

        REQUIRE(lyricParser.get_encoding() == LyricParser::Encoding::UTF8);
        REQUIRE(lyricParser.is_enhanced() == true); // Should be true for enhanced files
        REQUIRE(lyricParser.get_lyric_tags() == expected_tags);
        REQUIRE(lyricParser.get_lrc_text() == expected_content_lines);
    }

    // SECTION("Enhanced-LRC Test, Encode: UTF16LE")
    // {
    //     FileHelper fileHelper(filename);
    //     fileHelper.write_to_file(enhanced_lrc_toT, FileHelper::Encoding::UTF16LE);
    //     const LyricParser lyricParser{filename};
    //
    //     REQUIRE(lyricParser.get_encoding() == LyricParser::Encoding::UTF16LE);
    //     REQUIRE(lyricParser.is_enhanced() == true);
    //     REQUIRE(lyricParser.get_lyric_tags() == expected_tags);
    //     REQUIRE(lyricParser.get_lrc_text() == expected_content_lines);
    // }

    // SECTION("Enhanced-LRC Test, Encode: GBK")
    // {
    //     // Note: For GBK, ensure your 'FileHelper::write_to_file' and 'LyricParser'
    //     // can correctly handle the Chinese characters when converting from/to std::string.
    //     // This often requires proper locale settings or explicit encoding conversion logic.
    //     FileHelper fileHelper(filename);
    //     fileHelper.write_to_file(enhanced_lrc_toT, FileHelper::Encoding::GBK);
    //     const LyricParser lyricParser{filename};
    //
    //     REQUIRE(lyricParser.get_encoding() == LyricParser::Encoding::GBK);
    //     REQUIRE(lyricParser.is_enhanced() == true);
    //     REQUIRE(lyricParser.get_lyric_tags() == expected_tags);
    //     REQUIRE(lyricParser.get_lrc_text() == expected_content_lines);
    // }
}
