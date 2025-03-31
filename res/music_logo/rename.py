import os
import shutil

# 创建 data 文件夹（如果它不存在）
if not os.path.exists("data"):
    os.makedirs("data")

# 获取当前目录中所有 PNG 文件的列表
png_files = [f for f in os.listdir() if f.endswith(".png")]

# 遍历列表中的每个文件
for i, file in enumerate(png_files):
    # 将文件重命名为 logo_x.png 格式
    new_file = f"logo_{i+1}.png"
    # 将重命名的文件移动到 data 文件夹中
    shutil.move(file, os.path.join("data", new_file))