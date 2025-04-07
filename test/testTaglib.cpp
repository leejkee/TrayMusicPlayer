//
// Created by cww on 25-4-7.
//

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <QString>
#include <string>
#include <QDebug>


TEST_CASE("Diff with std::string with QString", "[String]") {

    const QString qstr("此情可待成追忆");

    const std::wstring w_str = qstr.toStdWString();
    const std::string utf8_str = qstr.toStdString();
    qDebug() << w_str << w_str.size();
    qDebug() << utf8_str << utf8_str.size();

    SECTION("Wide string should be longer in character count than UTF-8 byte length") {
        REQUIRE(w_str.length() == qstr.length()); // 每个字符一个 wchar_t
        REQUIRE(utf8_str.length() > w_str.length()); // UTF-8 编码中文是 3 bytes per character
    }

    SECTION("Print and visually inspect the encoding") {
        qDebug() << "QString:" << qstr;
        qDebug() << "toStdWString length:" << w_str.length();
        qDebug() << "toStdString (UTF-8 assumed) length:" << utf8_str.length();

        // 打印每个字节 (调试用)
        for (size_t i = 0; i < utf8_str.size(); ++i) {
            printf("Byte %zu: 0x%02X\n", i, static_cast<unsigned char>(utf8_str[i]));
        }

        SUCCEED("Manual inspection passed if printed values make sense.");
    }

    SECTION("Check UTF-8 string is not equal to wide string after narrowing") {
        std::string narrow_from_wide(w_str.begin(), w_str.end()); // 会截断高字节
        REQUIRE(narrow_from_wide != utf8_str);
    }
}

