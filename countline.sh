#!/bin/bash

# 检查是否在正确的目录下运行
if [ ! -d "src" ]; then
    echo "错误：当前目录下未找到 src 文件夹！"
    exit 1
fi

# 统计函数
count_lines() {
    local total=0
    local files=0

    echo "正在统计 .cpp 文件行数..."
    echo "----------------------------"

    # 查找所有 .cpp 文件并逐文件统计
    while IFS= read -r file; do
        lines=$(wc -l < "$file")
        printf "%-40s %6d 行\n" "$(basename "$file")" "$lines"
        total=$((total + lines))
        files=$((files + 1))
    done < <(find src -name "*.cpp" -type f)

    echo "----------------------------"
    printf "总文件数: %d\n总代码行数: %d\n" "$files" "$total"
}

# 执行统计
count_lines