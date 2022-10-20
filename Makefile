objects = compiler.tab.o lex.yy.o compiler.o
midobj = compiler.tab.c lex.yy.c compiler.tab.h

zzxcc : $(objects)
	clang -O2 -o zzxcc $(objects)
	rm $(midobj)
	rm $(objects)
lexer : compiler.l
	clang -O2 -o lex ./lexer/lex.yy.c
compiler.o : include/codegen.h
	clang -O2 -c ./src/compiler.c
compiler.tab.o : src/compiler.y
	bison -d ./src/compiler.y
	clang -O2 -c compiler.tab.c -I include
lex.yy.o : src/compiler.l include/semantic.h
	flex ./src/compiler.l
	clang -O2 -c lex.yy.c -I include
.PHONY : clean
clean : 
	-rm zzxcc lex ./output/semantic_analysis.txt ./output/IR.txt ./output/assembly.txt
