objects = compiler.tab.o lex.yy.o compiler.o
midobj = compiler.tab.c lex.yy.c compiler.tab.h

zzxcc : $(objects)
	gcc -g -o zzxcc $(objects)
compiler.o : include/codegen.h
	gcc -g -c ./src/compiler.c
compiler.tab.o : src/compiler.y
	bison -d ./src/compiler.y
	gcc -g -c compiler.tab.c -I include
lex.yy.o : src/compiler.l include/semantic.h
	flex ./src/compiler.l
	gcc -g -c lex.yy.c -I include
.PHONY : clean
clean : 
	-rm zzxcc $(objects) $(midobj) semantic_analysis.txt assembly.txt IR.txt
