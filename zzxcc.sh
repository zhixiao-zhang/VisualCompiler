#! /bin/zsh
if [ $# -eq 0 ]; then
    echo "Usage: $0 <-option> <source code file>"
    exit 1
fi
if [ $1 = "--help" ]; then
    echo "Usage: $0 <-option> <source code file>"
    echo "Options:"
    echo "  -S: assemble"
    echo "  -I: IR"
    echo "  -s: semantic"
    echo "  -p: lexer and parser"
    echo "  -l: lexer only"
    exit 0
fi

if [ $1 = "-l" ]; then
    ./lexer $2 >> ./output/lex.txt
    exit 0
fi

if [ $1 = "-p" || $1 = "-s"]; then
    ./zzxcc -p $2
    exit 0
fi

if [ $1 = "-I" ]; then
    ./zzxcc -I $2
    exit 0
fi

if [ $1 = "-S" ]; then
    ./zzxcc -S $2
    exit 0
fi
