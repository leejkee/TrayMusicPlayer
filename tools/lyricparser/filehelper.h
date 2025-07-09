//
// Created by 31305 on 2025/6/28.
//

#pragma once
#include <filesystem>
#include <vector>

class FileHelper
{
public:
    enum class Encoding
    {
        UTF8, GBK, UTF16LE
    };

    explicit FileHelper(std::filesystem::path filePath);

    ~FileHelper();

    FileHelper(const FileHelper&) = delete;
    FileHelper(FileHelper&&) = delete;
    FileHelper& operator=(const FileHelper&) = delete;
    FileHelper& operator=(FileHelper&&) = delete;

    bool write_to_file(const std::vector<std::string>& content_lines
                       , Encoding encoding = Encoding::UTF8);

    bool write_to_file_winapi(const std::string& content, Encoding target_encoding);

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    bool write_to_file_iconv(const std::string& content, Encoding encoding);
#endif

private:
    std::filesystem::path m_filePath;
    static constexpr char NEWLINE_CHAR{'\n'};
};
