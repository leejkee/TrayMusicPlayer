
import argparse
import subprocess

def run_command(command):
    """执行 Git 命令，并实时显示输出"""
    process = subprocess.run(command, shell=True)

def main():
    parser = argparse.ArgumentParser(description="Git 提交脚本")
    parser.add_argument("-m", required=True, help="commit 信息")
    parser.add_argument("-b", default="main", help="提交的分支，默认为 main")

    args = parser.parse_args()
    branch = args.b
    commit_message = args.m

    print("\n🚀 开始执行 Git 提交流程:")

    print("📌 1. 添加所有更改")
    run_command("git add .")

    print("📌 2. 提交代码")
    run_command(f'git commit -m "{commit_message}"')

    print(f"📌 3. 推送到远程分支: {branch}")
    run_command(f"git push origin {branch}")

    print("\n✅ Git 提交完成！")

if __name__ == "__main__":
    main()
