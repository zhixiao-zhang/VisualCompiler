# 项目文档

## 推荐工具

### 推荐网站

StcakOverflow：外国csdn，更好https://stackoverflow.com/

微软官方文档：https://docs.microsoft.com/zh-cn/

### 推荐编译器、编辑器、集成开发环境

推荐编译器：MinGW64

推荐编辑器：Visual Studio  Code

推荐集成开发环境：Visual Studio、Clion

推荐终端：Windows Terminal（只是因为好看）

### 版本控制工具

Git：https://git-scm.com/

Github：[GitHub](https://github.com/)

### CMake

[CMake](https://cmake.org/)

## 开发平台

### Windows平台

#### 添加环境变量

**MinGW**

下载地址：

[Download MinGW-w64 - for 32 and 64 bit Windows from SourceForge.net](https://sourceforge.net/projects/mingw-w64/files/Toolchains targetting Win32/Personal Builds/mingw-builds/installer/mingw-w64-install.exe/download)

下载之后找到安装路径，然后打开设置-关于-高级系统设置-环境变量

![image-20220112105104104](https://s2.loli.net/2022/01/12/Oxfo3NPldebBH1r.png)

点击进入后，点击添加，将安装的MinGW中的bin文件夹的路径加入到path中（新建，复制进去）

<img src="https://s2.loli.net/2022/01/12/LyAbrfOTJosvq6Z.png" alt="image-20220112110627603" style="zoom:50%;" />

之后就可以了。

**CMake**

和上方一个道理：

<img src="https://s2.loli.net/2022/01/12/yKaDSUXrEeFZ9MJ.png" alt="image-20220112124339461" style="zoom:50%;" />

之后可以再Windows Terminal里输入cmake --version查看是否安装成功。

### Mac平台

下载VMware Fusion：[Fusion - 在 Mac 上运行 Windows | 适用于 Mac 的虚拟机 | VMware | CN](https://www.vmware.com/cn/products/fusion.html)

### Linux平台

**本次工程编译器部分采用Linux的Ubuntu发行版编写。**

#### Ubuntu安装

当我们配置好虚拟机之后（互联网自行搜索教程），之后选择语言简体中文，键盘模式选择Chinese，选择最小安装模式，下面的什么软件更新都不要管（因为这时候的软件源是在Linux自己的服务器上，下载速度非常慢，换源之后再更新）。

进入Linux界面后弹出的软件更新器也不要管，这时候我们还没有换源，之后从应用列表一样可以打开。

清晰度可以从右上角电源键处调整。

#### 换国内源

进入Linux系统后，点击右上角下箭头，找到设置-关于-软件更新-下载自，选择其他站点-找到中科大的镜像下载站（http://mirrors.ustc.edu.cn/ubuntu）之后跟随引导操作即可。

#### 基本配置

首先我们注意到Ubuntu的商店不能使用，这是由于Ubuntu的商店已经停用了，转而变成了软件。我们需要打开命令行（Windows下是Ctrl+Alt+T、Mac下应该类似或自行查找）。

首先我们要卸载原来的商店：

```bash
sudo apt autoremove --purge snapd
```

之后安装gnome-software

```bash
sudo apt install gnome-software
```

之后进行几个操作：

```bash
sudo apt-get install gcc
sudo apt-get install cmake
sudo apt-get install g++
sudo apt-get install git
```

要查看是否安装成功，可以输入（gcc为例，其余同理）：

```bash
gcc --version
```

科大的镜像站下载的是gcc-9，现在的最新版本是gcc-11.2.0，不用管，我们统一用最新的。

#### GCC-11.2.0下载与编译方法

登录中国科学技术大学镜像站（[USTC Open Source Software Mirror](https://mirrors.ustc.edu.cn/)），文件列表找到gnu，点进去之后找到gcc，选择gcc-11.2.0，下载gcc.11.2.0.tar.gz，之后打开终端，找到下载目录（可以用cd命令，或者直接图形界面找到右键选择在终端打开），之后输入如下命令：

```bash
 sudo tar -xf gcc-10.1.0.tar.gz -C /usr/local/
```

将源代码文件解压到local文件夹，紧接着执行如下指令安装gcc所需要的包：

```bash
cd /usr/local/gcc-10.1.0
sudo  ./contrib/download_prerequisites
```

一定要看到这里是确实把gmp等包下载成功之后才进行下一步。

完成这些之后退回到local文件夹：

```bash
cd /usr/local
```

建立一个用于存放编译后代码的文件夹：

```bash
sudo mkdir gcc-build-11.2.0
```

之后进入该文件夹，输入如下指令：

```bash
sudo ../gcc-10.1.0/configure --enable-checking=release --enable-languages=c,c++ --disable-multilib
```

这时候就可以编译了：

```bash
sudo make	//如果这里提示没有安装的话就先安装再make
#安装命令为：
sudo apt-get install make
```

编译过程会持续数个小时（我的电脑实际用时3小时左右）。

之后如果看到done一类的词就可以输入如下指令进行安装：

```bash
sudo make install
```

安装成功之后重启虚拟机，在终端输入：

```bash
gcc --version
```

就可以看到已经有11.2.0版本的gcc了。

#### Ubuntu上Visual Studio Code的安装与配置：

在这之前我们先将系统自带的火狐卸载，安装chrome。

在火狐上搜索chrome，安装deb包之后右键选择打开方式，点击软件安装。（这一步一定要在解决商店问题后做）

终端输入（在确定安装了chrome之后）：

```bash
sudo apt autoremove firefox
```

到vscode官网下载vscode的deb包

![image-20220115140909627](https://s2.loli.net/2022/01/15/63zPuaCJXH7LBnR.png)

下载之后安装即可，正常安装插件，按照下面vscode部分的配置方法即可。

**在调试时可能会出现提示错误（找不到文件），可以参考下面这篇文章的内容，但记得把里面的参数换成你操作系统里缺的那部分！**

[(2条消息) Ubuntu下Vscode调试出现[无法打开"libc-start.c"：无法读取文件...错误解决办法_Hubery彭于晏-CSDN博客_无法打开“libc-start.c”](https://blog.csdn.net/weixin_39758398/article/details/101912759?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~default-2.no_search_link&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~default-2.no_search_link&utm_relevant_index=3)

## Git与Github的使用

下载git：[Git - Downloading Package (git-scm.com)](https://git-scm.com/download/win)

安装与配置：[(2条消息) 关于Git这一篇就够了_17岁boy的博客-CSDN博客](https://blog.csdn.net/bjbz_cxy/article/details/116703787?ops_request_misc=%7B%22request%5Fid%22%3A%22163893895916780274168634%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=163893895916780274168634&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-116703787.pc_search_mgc_flag&utm_term=git&spm=1018.2226.3001.4187)

[(2条消息) Git基本使用教程（一）：入门及第一次基本完整操作_jerryzhou的博客-CSDN博客_git基础教程](https://jerryzhou.blog.csdn.net/article/details/97698815?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~default-1.pc_relevant_paycolumn_v2&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~default-1.pc_relevant_paycolumn_v2&utm_relevant_index=2)

[(2条消息) 如何正确使用git和github__ACME_的博客-CSDN博客_git怎么用](https://blog.csdn.net/qq_18297675/article/details/79633950)

[(2条消息) GitHub怎样fork别人代码到自己仓库并进行贡献_前端小狮的博客-CSDN博客_git怎么fork](https://blog.csdn.net/m0_37885651/article/details/94734499)

去Github网站注册一个账号：[GitHub](https://github.com/)

将项目clone下来：

在你想要提交时：

```bash
git add ..//这里写你要上传的一个文件
git commit -m "注释"i//注释里写你对提交的注释
git checkout master
git merge dev
git push
```

之后从Pull request处向源项目提交。

git push不稳定，可以下个github desktop，在里面提交很方便。



## 开发技巧

### CMake + 终端的使用

#### 前言

- CMake是一个**跨平台**的安装编译工具，可以用简单的语句来描述**所有平台**的编译过程。
- 大多数C++开源项目都使用了CMake。

#### CMake能做什么

当我们不使用CMake去构建工程时，我们一般会使用Visual Studio（Windows）、Xcode（Mac）和Makefile（Linux）平台进行构建工程。

那么如果我突然多了一个文件，传统来说如果需要每个平台各做一遍添加到工程文件并构建工程的操作，但是用CMake就可以之改动CMakelist就可以构建工程了，相对来说简化了操作。

#### 基本语法

- 基本语法格式：指令（参数1 参数2）
  - 参数用括弧括起
  - 参数之间用**空格**分开
- **指令是大小无关的，参数和变量是大小有关的**
- 变量使用${}方式取值，但在if控制语句中是直接使用变量名

#### 重要指令

- **cmake_minimum_required -指定CMake的最小版本要求**

  - 语法：cmake_minimum_required(VERSION versionNumber [FATAL_ERROR])

  ```cmake
  #CMake最小版本要求为2.8.3
  cmake_minimum_required(Version 2.8.3) //后面那个一般不用
  ```

- **project -定义工程名称，并可指定工程支持的语言**

  - 语法：project(projectname [][][cxx]\[C]\[Java])

  ```cmake
  #指定工程名为helloworld
  project(helloworld)	//后面那个语言一般不指定
  ```

- **add_compile_options -添加编译参数**

  - 语法：add_compile_options(\<option>..)

  ```cmake
  #添加编译参数 -Wall -std=c++11
  add_compile_options(-Wall -std=c++11)
  ```

- **add_executable -生成可执行文件**

  - 语法：add_executable(exexname source1 source2)

  ```cmake
  #编译main.cpp生成可执行文件main
  add_executable(main main.cpp)
  ```

#### 常用变量

- **CMAKE_C_FLAGS  gcc编译选项**

  **CMAKE_CXX_FLAGS  g++编译选项**

  ```cmake
  1# 在CMAKE_CXX_FLAGS编译选项后追加-std=c++11
  2set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  ```

  **CMAKE_BUILD_TYPE  编译类型(Debug, Release)**

  ```cmake
  1# 设定编译类型为debug，调试时需要选择debug
  2set(CMAKE_BUILD_TYPE Debug) 
  3# 设定编译类型为release，发布时需要选择release
  4set(CMAKE_BUILD_TYPE Release) 
  ```

  **CMAKE_BINARY_DIR**

  **PROJECT_BINARY_DIR**

  **\<projectname>__BINARY_DIR**

- 1. 这三个变量指代的内容是一致的。
  2. 如果是 in source build，指的就是工程顶层目录。
  3. 如果是 out-of-source 编译,指的是工程编译发生的目录。
  4. PROJECT_BINARY_DIR 跟其他指令稍有区别，不过现在，你可以理解为他们是一致的。

- **CMAKE_SOURCE_DIR**

  **PROJECT_SOURCE_DIR**
  **\<projectname>__SOURCE_DIR**

- 1. 这三个变量指代的内容是一致的,不论采用何种编译方式,都是工程顶层目录。
  2. 也就是在 in source build时,他跟 CMAKE_BINARY_DIR 等变量一致。
  3. PROJECT_SOURCE_DIR 跟其他指令稍有区别,现在,你可以理解为他们是一致的。

------

- **CMAKE_C_COMPILER：指定C编译器**
- **CMAKE_CXX_COMPILER：指定C++编译器**
- **EXECUTABLE_OUTPUT_PATH：可执行文件输出的存放路径**
- **LIBRARY_OUTPUT_PATH：库文件输出的存放路径**

#### 编译流程

CMake目录结构：项目主目录存在一个CMakeLists.txt文件

**两种方式设置编译规则**：

1. 包含源文件的子文件夹包含CMakeLists.txt文件，主目录的CMakeLists.txt通过add_subdirectory添加子目录即可；
2. 包含源文件的子文件夹未包含CMakeLists.txt文件，子目录编译规则体现在主目录的CMakeLists.txt中；

**在 Windows平台下使用 CMake 构建C/C++工程的流程如下:**

- 手动编写 CmakeLists.txt。
- 执行命令 `cmake PATH`生成 Makefile ( PATH 是顶层CMakeLists.txt 所在的目录 )。
- 执行命令`make` 进行编译。

------

```cmake
1# important tips
2.          # 表示当前目录
3./         # 表示当前目录
4
5..      # 表示上级目录
6../     # 表示上级目录
```

#### **两种构建方式**

- **内部构建(in-source build)**：不推荐使用

  内部构建会在同级目录下产生一大堆中间文件，这些中间文件并不是我们最终所需要的，和工程源文件放在一起会显得杂乱无章。

  ```cmake
  1## 内部构建
  2
  3# 在当前目录下，编译本目录的CMakeLists.txt，生成Makefile和其他文件
  4cmake .
  5# 执行make命令，生成target
  6make
  ```

- **外部构建(out-of-source build)**：==推荐使用==

  将编译输出文件与源文件放到不同目录中

  ```cmake
   1## 外部构建
   2
   3# 1. 在当前目录下，创建build文件夹
   4mkdir build 
   5# 2. 进入到build文件夹
   6cd build
   7# 3. 编译上级目录的CMakeLists.txt，生成Makefile和其他文件
   8cmake ..
   9# 4. 执行make命令，生成target
  10make
  ```

### GCC/G++参数简介

菜鸟教程：https://www.runoob.com/w3cnote/gcc-parameter-detail.html

### Visual Studio Code中配置文件的介绍

**插件安装**：C/C++、C/C++ Extension Pack、C++ Themes、Chinese、Error Lens（错误提示）

写好一个C代码后，点击右侧这个虫子图标：![image-20220111200436464](https://s2.loli.net/2022/01/12/RjYnb5psw6oBDXH.png)

之后点击创建launch.json，会生成一个launch.json和一个task.json。

launch.json中的program指的是可执行文件的位置；preLaunchTask指的是在调试之前要做什么。

**注意preLaunchTask后面的参数和task.json中的label一定是要相同的，因为preLaunchTask执行的就是task.json中的内容。**

如果我们的工程是这样的：![image-20220111201128717](https://s2.loli.net/2022/01/12/AHehmlQYnwJNV3S.png)

我们就需要自己对launch.json和task.json进行修改，因为默认的这两个配置文件是针对单文件编译的。

我们可以将这里的参数进行改动：

<img src="https://s2.loli.net/2022/01/12/7cPCeQnWxiVds9y.png" alt="image-20220111201853579" style="zoom:50%;" />

这里大括号里的内容改成我们要编译的源文件：

```json
"{main.cpp}",
"{swap.cpp}",
```

**注意要把vscode生成的所有${fileDirname}换成${workspaceFolder}!**

前者是顶层目录，后者是当前工作目录。

如果我们要使用cmake的话，要先编写一个CMakelists.txt：

![](https://s2.loli.net/2022/01/12/P8XrT17x52iJseA.png)

之后要将这个文件夹配置成一个cmake工程文件夹。

有两种方法：

- 一种按CTRL+shift+P，输入cmake：configure配置。

- 另一种是建立一个build文件夹，然后在终端进入build文件夹并输入`cmake ..`

  - 第一次要输入：

    ```bash
    cmake -G "MinGW Makefiles" ..
    ```

    之后可以使用cmake ..

之后会生成这样的一个文件内容：

![image-20220111211337222](https://s2.loli.net/2022/01/12/MVgn47EzewtijIR.png)

这时候我们就创建了一个CMake工程的文件夹。

但是如果我们修改了代码想再次调试就要重新编译一下，下面我们将通过修改task.json和launch.json中的内容来让我们可以做到一键编译调试：

首先我们要将launch.json里的program里的可执行文件路径改为build里的路径，再将preLaunchTask里的名字改成自己喜欢的名字：

![image-20220111212059168](https://s2.loli.net/2022/01/12/roX4QaqIjLFSlkm.png)

![image-20220111212032533](https://s2.loli.net/2022/01/12/6A2CVtmMlgY7saU.png)

之后再将task.json改为如下内容：

![image-20220111212655180](https://s2.loli.net/2022/01/12/PFmBVcNpGb94tda.png)

![image-20220111212801011](https://s2.loli.net/2022/01/12/yDIT7wGK18kW2Zn.png)

之后就可以F5一键调试了。

## 编码规范

### 文件规范

对于采用CMake工程的C/C++项目，统一使用Visual Studio Code，且在上传时用.gitignore将.vscode忽略，主目录下只能放置源代码，build文件夹和.vscode文件夹。

对于C#工程，采用dotnet new console命令行构建项目。

### 编码格式

### 注释

## 至少需要学习的内容及使用书籍

YC：

C#、Windows平台软件开发、C#调用C/C++、数据结构、Git、Github、WPF、软件工程基本知识

- 《Windows程序设计》、微软的C#/.NET文档

GYJ：

C/C++、编译原理、数据结构与算法、CMake、Git、Github、软件工程基本知识、Windows

- C++：《C++ Primer》、《Effective C++》（可能用）
- 编译原理：《编译原理》（龙书）、Stanford的课程
- 数据结构与算法：浙大陈越的课、《挑战程序设计竞赛》、《数据结构与算法分析》（可选）
- CMake：CMake官方文档
- Git、Github：自行搜索教程
- Windows：安个虚拟机

ZZX：

C/C++、编译原理、数据结构与算法、CMake、Git、Github、软件工程基本知识、C#、C++与C#的对接、汇编语言

- C#：微软的C#/.NET文档、C#图解教程
- C++：《C++ Primer》、《Effective C++》（可能用）
- 编译原理：《编译原理》（龙书）、Stanford的课程
- 汇编语言：汇编语言（第4版）
- 数据结构与算法：浙大陈越的课、《挑战程序设计竞赛》、《数据结构与算法分析》
- CMake：CMake官方文档
- Git、Github：自行搜索教程
- C#与C/C++的对接：自行搜索
