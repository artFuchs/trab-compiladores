etap4: ast.o y.tab.o lex.yy.o semantic.o hash.o tac.o assembly.o main.o
	gcc -g -o etapa6 main.o ast.o y.tab.c lex.yy.o semantic.o hash.o tac.o assembly.o
main.o: main.c
	gcc -g -c main.c
lex.yy.o: lex.yy.c
	gcc -g -c lex.yy.c
lex.yy.c: scanner.l
	lex scanner.l
y.tab.o: y.tab.c
	gcc -g -c y.tab.c
y.tab.c: parser.y
	yacc -d parser.y
hash.o: hash.c
	gcc -g  -c hash.c
ast.o: ast.c
	gcc -g -c ast.c
semantic.o: semantic.c
	gcc -g -c semantic.c
tac.o: tac.c
	gcc -g -c tac.c
assembly.o: assembly.h assembly.c
	gcc -g -c assembly.c
clean:
	rm *.o
	rm lex.* y.* etapa6
