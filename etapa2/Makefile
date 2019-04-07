etapa2: y.tab.o lex.yy.o hash.o
	gcc -o etapa2 y.tab.c lex.yy.o hash.o
main.o: main.c
	gcc -c main.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
lex.yy.c: scanner.l
	lex scanner.l
y.tab.o: y.tab.c
	gcc -c y.tab.c
y.tab.c: parser.y
	yacc -d parser.y
hash.o: hash.c
	gcc -c hash.c
clean:
	rm *.o
	rm lex.* y.* etapa2
