objects = compiler.tab.o lex.yy.o compiler.o
midobj = compiler.tab.c lex.yy.c compiler.tab.h

parser : $(objects)
	cc -g -o parser $(objects)
compiler.o : include/semantic.h
	cc -g -c ./src/compiler.c
compiler.tab.o : src/compiler.y
	bison -d ./src/compiler.y
	cc -g -c compiler.tab.c -I include
lex.yy.o : src/compiler.l include/semantic.h
	flex ./src/compiler.l
	cc -g -c lex.yy.c -I include

.PHONY : clean
clean : 
	rm parser $(objects) $(midobj)
