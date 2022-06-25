# VisualCompiler

- [Overview](#Overview)
- [Environment](#Environment)
- [Build](#Build)
- [License](#License)

## Overview

This is an undergraduate project of the School of Software of Sichuan University, Class of 2021, entitled Visual Compiler System in the Perspective of First-Year Teaching.
The project team consists of three members.

- Leader: Zhixiao Zhang
- Members: Yijia Guo, Chen Yang

### Introduction

This project aims to implement a visual C Minus compiler in C/C++ and C# to show the step-by-step compilation process from source to execute. By showing the compilation process, it helps the cs beginners to have a basic understanding of the compiler. If all else fails, it can always reduce the confusion of freshmen due to the lack of underlying knowledge. This project can be used as a lab's tool for freshmen to learn about compiler  on Introdution to Computer System.

### Specific implementation plan for the project

- Lexical Analysis: Using the GNU Flex and the doc given by instructor.

- Syntax Analysis: Using the GNU Bison and the Ast coded by ourselves.
- Semantic analysis: ongoing
- CodeGenerate: Translate the source into x86_64 intel assembly.
- Linker: No decision
- Visual: Compiling compiler parts into DLL dynamic link libraries for C# to call during development. Developing the visual using C#, WPF, Visual Studio on Windows.

## Environment

- Ubuntu 20.04.3 Desktop
- Windows terminal with ssh to the VMware
- GNU GCC 9.4.0
- GNU Bison 3.5.1
- GNU FLex 2.6.4
- GNU Make 4.2.1
- C (Actually I don't know my specific C version lol.)

## Build

```powershell
cd ~
git clone https://github.com/zzxdyf1314/VisualCompiler
cd VisualCompiler && make clean && make
```

## License

[MIT](./LICENSE)
