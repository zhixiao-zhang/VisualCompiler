objects = compiler.tab.o lex.yy.o compiler.o
midobj = tmp/compiler.tab.c tmp/lex.yy.c tmp/compiler.tab.h

zzxcc : $(objects)
	clang -g -o zzxcc $(objects)
compiler.o : include/codegen.h
	clang -g -c ./src/compiler.c
compiler.tab.o : src/compiler.y
	bison -d --output-file=./tmp/compiler.tab.c ./src/compiler.y
	clang -g -c tmp/compiler.tab.c -I include
lex.yy.o : src/compiler.l include/semantic.h
	flex --outfile=./tmp/lex.yy.c ./src/compiler.l
	clang -g -c tmp/lex.yy.c -I include
.PHONY : clean
clean : 
	-rm $(objects) $(midobj) ./output/semantic_analysis.txt ./output/IR.txt ./output/assembly.txt
