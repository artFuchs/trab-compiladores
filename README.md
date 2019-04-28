# trab-compiladores
trabalho da disciplina de compiladores B, do curso de Ciência da Computação da UFRGS
alunos: Arthur Lucena Fuchs, Matheus Westhele

## Etapa 1
Fazer um analisador léxico utilizando-se da ferramenta lex. Ela deve:
- [x] reconhecer as expressões regulares que descrevem cada tipo de lexema;
- [x] classificar os lexemas reconhecidos em tokens retornando as constantes definidas no
arquivo tokens.h fornecido ou códigos ascii para caracteres simples;
- [x] incluir os identificadores e os literais (inteiros, caracteres e strings) em uma tabela de
símbolos implementada com estrutura hash;
- [x] controlar o número de linha do arquivo fonte, e fornecer uma função declarada como
int getLineNumber(void) a ser usada nos testes e pela futura análise sintática;
- [X] ignorar comentários de única linha e múltiplas linhas;
- [X] informar erro léxico ao encontrar caracteres inválidos na entrada, retornando o token
de erro;
- [x] definir e atualizar uma variável global e uma função int isRunning(void), que
mantém e retorna valor true (diferente de 0) durante a análise e muda para false (igual
a 0) ao encontrar a marca de fim de arquivo;

## Etapa 2
Fazer um analisador sintático utilizando-se das ferramentas lex e yacc. A analise sintática deve:
- [X] reconhecer os programas que fazem parte da linguagem
  - funcionando com o programa de exemplo
  - [X] criar uma gramática que reconheça o programa
  - [X] criar o "parser.y"
- [X] imprimir mensagem de erro sintático para programas não reconhecidos, informando a linha do erro e retornando o código de erro 3
- [X] os nodos armazenados na tabela hash devem distinguir entre os tipos de símbolos armazenados e o nodo deve ser associado ao token retornado através da atribuição para yylval.symbol;

## Etapa 3
- [X] Corrigir a especificação das suas expressões em relação à etapa anterior, simplificando-­as de forma que as produções utilizem a forma mais simples que é ambígua, e retirando a ambiguidade pela declaração da precedência correta dos operadores com as definições %left, %right ou %nonassoc;
- [X] implementar a estrutura da árvore de sintaxe, com funções de criação de nodo, impressão de um nodo e impressão da árvore;
- [X] colocar ações sintátivas ao lado das regras de produção descritas no arquivo para o yacc, as quais criam ou propagam os nodos da árvore, montando-a recursivamente segundo a análise. A maior parte das ações serão chamadas à rotina de criação do nodo;
- [ ] implementar uma rotina que percorre a árvore e gera um programa fonte funcionalmente semelhante ao programa analisado.
