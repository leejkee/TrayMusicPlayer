<p align="center">
  <img src="docs/img/MusicPlayerLogo.svg" alt="Tray Core Module Icon" width="150">
  <h3 align="center">TrayMusicPlayer</h3>
  <p align="center">
    一个实现了播放列表管理并包含系统托盘的本地音乐播放器
  </p>
</p>

---

<p align="center">
  <img src="https://img.shields.io/badge/Qt-6.5-green?logo=qt&logoColor=white" alt="Qt 6.5"/>
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B&logoColor=white" alt="C++ 17"/>
  <img src="https://img.shields.io/badge/CMake-3.20%2B-orange?logo=cmake&logoColor=white" alt="CMake 3.0+"/>
</p>


## 运行
- 部分功能演示(Windows 11)  
  ![show](docs/img/windows-show.gif)

- Linux(Arch linux with cosmic)  
  ![show_linux](docs/img/show-view-linux.png)  

## 快速使用
点击右上角`设置`图标，添加本地音乐路径，点击`Local`按钮加载Local歌单

## 项目结构文档
- [主体框架图](docs/UML/index.md)
- [后端功能文档](docs/Core.md)

## 构建本项目

### 环境需求
- `C++ 17`
- `Qt6.5` 以上
- `CMake`
- `Git`

### Windows

#### 1. 使用Qt6官方提供的`MinGW-w64的g++工具链、CMake、 Ninja生成器`

- 构建本项目, 进入项目根目录

Debug
```shell
cmake -B build --preset windows-mingw-debug -S .
```
or 

Release
```shell
cmake -B build --preset windows-mingw-release -S .
```

build
```shell
cmake --build build
```

- 编译并打包项目，对于windows平台提供了额外的launcher启动程序，以保持release目录整洁，qt的dll被存放在bin目录
```cmake
cmake --build build/window-mingw-release --config Release --target package_release
```

#### 2. 使用`MSVC`编译，链接Qt官方提供的基于MSVC2022构建的Qt6库

待测试


### Linux (待测试)

#### 使用g++编译
- 构建本项目，进入项目根目录  
release
```shell
cmake -B build --preset linux-gcc-release -S .
```
build
```
cmake --build build
```

## 资源文件
本项目logo **`TrayMusic`** 除外，其他svg资源均来自网络，商用联系阿里巴巴矢量图标社区作者

## ToDo
- [x] 可扩展的设置页面嵌入
- [x] 内置lrc歌词解析支持
- [ ] 可贴边的浮动歌词支持
- [ ] 更美观的主题切换支持
- [ ] 更用户友好的随机数生成算法

