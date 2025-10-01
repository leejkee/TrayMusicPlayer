//
// Created by 31305 on 2025/10/1.
//
#include <windows.h>
#include <string>

// 定义常量
constexpr wchar_t SUB_DIRECTORY[] = L"bin";
constexpr wchar_t CORE_EXE_NAME[] = L"MyApp_Core.exe";
constexpr wchar_t CONFIG_FILE_NAME[] = L"init.json"; // 假设您的配置文件名为 config.json

// Windows 应用程序的入口点
int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,  // 外部传入的命令行参数（如果用户双击启动器，此项通常为空）
    _In_ int nCmdShow)
{
    // ===================================================================
    // 步骤 1: 构建核心程序的命令行参数
    // ===================================================================

    // 我们需要传递给核心程序的参数： -c "根目录/config.json"
    std::wstring core_args = L"-c \"..\\";
    core_args += CONFIG_FILE_NAME;
    core_args += L"\"";

    // 如果用户双击启动器时传入了额外参数，也一并追加
    if (lpCmdLine && *lpCmdLine) {
        core_args += L" ";
        core_args += lpCmdLine;
    }

    // ===================================================================
    // 步骤 2: 切换到 DLL 所在的子目录
    // ===================================================================

    // 使用 SetCurrentDirectory 更改工作目录为 'bin'。
    // 这解决了 DLL 查找和 Qt 插件查找的问题。
    if (!SetCurrentDirectory(SUB_DIRECTORY))
    {
        MessageBox(NULL,
                   L"启动失败：无法找到 bin 目录。",
                   L"错误",
                   MB_OK | MB_ICONERROR);
        return 1;
    }

    // ===================================================================
    // 步骤 3: 启动核心程序并传递参数
    // ===================================================================

    // 现在我们位于 'bin' 目录，可以直接调用 CORE_EXE_NAME (MyApp_Core.exe)
    // 注意：我们将之前构建的 core_args 传递给核心程序
    HINSTANCE result = ShellExecuteW(
        NULL,               // 窗口句柄
        L"open",            // 操作: open
        CORE_EXE_NAME,      // 文件名 (位于当前目录 "bin" 中)
        core_args.c_str(),  // 命令行参数
        NULL,               // 工作目录 (已设置为 "bin")
        SW_SHOW             // 显示窗口
    );

    if ((intptr_t)result <= 32)
    {
        MessageBox(NULL,
                   L"启动失败：无法执行核心程序 MyApp_Core.exe。",
                   L"错误",
                   MB_OK | MB_ICONERROR);
        return 2;
    }

    return 0;
}