# VisualCompiler

- [Overview](#Overview)
- [Environment](#Environment)
- [Build](#Build)
- [Usage](#Usage)
- [License](#License)

## Overview

This is an undergraduate project of the School of Software of Sichuan University, Class of 2021, entitled Visual Compiler System in the Perspective of First-Year Teaching.
This project is the sub and kernel of the undergraduate project, which is a compiler that show the process of compiling step by step.

### Introduction

This project aims to implement a C Minus compiler in C/C++ to show the step-by-step compilation process from source to assembly. By showing the compilation process, it helps the cs beginners to have a basic understanding of the compiler. If all else fails, it can always reduce the confusion of freshmen due to the lack of underlying knowledge. This project can be used as a lab's tool for freshmen to learn about compiler  on Introdution to Computer System.

### Specific implementation plan for the project

- Lexical Analysis: Using the GNU Flex and the doc given by instructor.
- Syntax Analysis: Using the GNU Bison and the Ast coded by ourselves.
- Semantic analysis: self.
- CodeGenerate: Translate the source into MIPS assembly.

## Environment

- Darwin 21.6.0

- Apple clang version 13.1.6
- bison (GNU Bison) 2.3
- flex 2.6.4 Apple(flex-34)
- GNU Make 3.81
- C (Actually I don't know my specific C version lol.)

## Build

```powershell
cd ~
git clone https://github.com/zzxdyf1314/VisualCompiler
cd VisualCompiler && make clean && make
```

## Usage

```bash
./zzxcc [-option] [source code]
-S : compile to the asm
-s : compile to semantic
-I : compile to IR
```

## License

[MIT](./LICENSE)
