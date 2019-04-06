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
- [] reconhecer os programas que fazem parte da linguagem
  - [] criar uma gramática que reconheça o programa
  - [] criar o "parser.y"
- [] imprimir mensagem de erro sintático para programas não reconhecidos
- [] os nodos armazenados na tabela hash devem distinguir entre os tipos de símbolos armazenados e o nodo deve ser associado ao token retornado através da atribuição para yylval.symbol;
