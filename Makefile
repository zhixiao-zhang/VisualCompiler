objects = compiler.tab.o lex.yy.o compiler.o
midobj = tmp/compiler.tab.c tmp/lex.yy.c tmp/compiler.tab.h

zzxcc : $(objects)
	gcc -g -o zzxcc $(objects)
compiler.o : include/codegen.h
	gcc -g -c ./src/compiler.c
compiler.tab.o : src/compiler.y
	bison -d --output-file=./tmp/compiler.tab.c ./src/compiler.y
	gcc -g -c tmp/compiler.tab.c -I include
lex.yy.o : src/compiler.l include/semantic.h
	flex --outfile=./tmp/lex.yy.c ./src/compiler.l
	gcc -g -c tmp/lex.yy.c -I include
.PHONY : clean
clean : 
	-rm zzxcc $(objects) $(midobj) ./output/semantic_analysis.txt ./output/IR.txt ./output/assembly.txt
