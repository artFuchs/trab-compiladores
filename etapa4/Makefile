etapa3: ast.o y.tab.o lex.yy.o hash.o main.o
	gcc  -o etapa3 main.o ast.o y.tab.c lex.yy.o hash.o
main.o: main.c
	gcc  -c main.c
lex.yy.o: lex.yy.c
	gcc  -c lex.yy.c
lex.yy.c: scanner.l
	lex scanner.l
y.tab.o: y.tab.c
	gcc  -c y.tab.c
y.tab.c: parser.y
	yacc -d parser.y
hash.o: hash.c
	gcc  -c hash.c
ast.o: ast.c
	gcc -c ast.c
clean:
	rm *.o
	rm lex.* y.* etapa3
