<p align="center">
  <img src="docs/img/icon.svg" alt="Tray Core Module Icon" width="150">
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


## 运行（Windows11，Linux暂未测试）
- 部分功能演示  
  ![show](docs/img/shot_show.gif)

## 快速使用
点击右上角`设置`图标，添加本地音乐路径，点击`Local`按钮加载Local歌单

## 项目结构文档
- [主体框架图](docs/UML/index.md)
- [后端结构图](docs/UML/Core/Core.md)
- [后端功能文档](docs/Core.md)

## 构建本项目

### 环境需求
- `C++ 17`
- `Qt6.5` 以上
- `CMake`
- `Git`

### Windows
Windows 推荐使用本项目提供的taglib依赖处理方案，将taglib作为`git submodule`加入工程，在`dependencies`目录中将其作为一个CMake工程进行处理。  
因为taglib官方仅提供CMake install之后将其作为CMake子模块加入项目
- tips: [taglib官方编译教程](https://github.com/taglib/taglib/blob/master/INSTALL.md)

#### 1. 使用Qt6官方提供的`MinGW-w64的g++工具链、CMake、 Ninja生成器`

- 编译`taglib`库

```shell
git submodule update --init --recursive
```

```shell
cd dependencies
```

```shell
cmake -B build --preset windows-base -S .
```

```shell
cmake --build build
```

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

#### 2. 使用`MSVC`编译，链接Qt官方提供的基于MSVC2022构建的Qt6库

待支持


### Linux
linux可以选择使用已经安装的taglib库，也可以选择使用本项目提供的从源码编译的方式

#### 使用g++编译
- 编译taglib
```shell
cd dependencies
cmake -B build --preset linux-base -S .
cmake --build build
```

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
- [ ] 可贴边的浮动歌词支持
- [ ] 更用户友好的随机数生成算法

