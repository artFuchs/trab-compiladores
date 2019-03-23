# trab-compiladores
trabalho da disciplina de compiladores B, do curso de Ciência da Computação da UFRGS
alunos: Arthur Lucena Fuchs, Matheus Westhele

## Etapa 1
Fazer um analisador léxico utilizando-se da ferramenta lex. Ela deve:
- [x] reconhecer as expressões regulares que descrevem cada tipo de lexema;
- [x] classificar os lexemas reconhecidos em tokens retornando as constantes definidas no
arquivo tokens.h fornecido ou códigos ascii para caracteres simples;
- [ ] incluir os identificadores e os literais (inteiros, caracteres e strings) em uma tabela de
símbolos implementada com estrutura hash;
- [ ] controlar o número de linha do arquivo fonte, e fornecer uma função declarada como
int getLineNumber(void) a ser usada nos testes e pela futura análise sintática;
- [ ] ignorar comentários de única linha e múltiplas linhas;
- [ ] informar erro léxico ao encontrar caracteres inválidos na entrada, retornando o token
de erro;
- [x] definir e atualizar uma variável global e uma função int isRunning(void), que
mantém e retorna valor true (diferente de 0) durante a análise e muda para false (igual
a 0) ao encontrar a marca de fim de arquivo;
