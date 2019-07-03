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
- [X] implementar uma rotina que percorre a árvore e gera um programa fonte funcionalmente semelhante ao programa analisado.

## Etapa 4
Fazer uma verificação semântica, testando:

 - [X] Declarações - Devem ser verificadas as declarações. Todos os identificadores devem
ter sido declarados, seja como variável, vetor ou como função. Os símbolos definidos
como identificadores na tabela de símbolos devem ter seu tipo trocado para um desses
tipos conforme a declaração, verificando-se se não houve dupla declaração ou símbolo
não declarado;
- [X] Uso correto - o uso dos identificadores deve ser compatível com sua declaração.
Variáveis somente podem ser usadas sem indexação, vetores somente podem ser
usados com indexação, funções apenas devem ser usadas com chamada, isto é,
seguidas da lista de argumentos entre parênteses;
- [X] Tipos de dados – As declarações também devem registrar os tipos de dados, em um
novo campo, dataType, na tabela de símbolos. Com o auxílio dessa informação,
quando necessário, os tipos de dados corretos devem ser verificados onde forem
usados, em expressões aritméticas, relacionais, lógicas, ou para índices de vetores;
- [X] Argumentos e parâmetros – A lista de argumentos deve ser verificada
contra a lista de parâmetros formais na declaração da função. Cada chamada de
função deve prover um argumento para cada parâmetro, e ter o tipo compatível;

## Etapa 5
Geração de código intermediário de 3 endereços

- [X] implementação da estrutura de TACs (Triple Adress Code) -  Um novo
módulo deve prover uma estrutura com instrução e três ponteiros para a tabela
de símbolos, permitindo que essas instruções sejam encadeadas em
listas de instruções. Cada instrução indica a execução de uma operação com
zero, um ou dois operandos que serão valores em memória, e cujo resultado
também está em memória, sendo os endereços de memória representados pelos
símbolos na tabela de símbolos. O módulo deve prover rotinas utilitárias para
criar, imprimir e unir listas de instruções;

- [X] Criação de símbolos temporários e *labels* - Devem ser feitas duas
rotinas auxiliares que criam novos símbolos na tabela de símbolos (*hash*),
uma para variáveis temporárias e outra para *labels*. Elas serão usadas na
geração de código para guardar sub-resultados de cada operação e para marcar os
pontos de desvio no fluxo de execução;

- [X] Geração de código - faça uma rotina que percorre a AST recursivamente,
retornando um trecho de código intermediário (lista de TACs) para cada nodo
visitado. Essa rotina primeiro processa os nodos filhos, armazena os trechos de
código gerados para cada um deles, depois testa o nodo atual e gera o código
correspondente para este nodo. A geração em geral consiste na criação de uma ou
mais novas instruções (TACs), união dos trechos das sub-árvores e dessas novas
instruções, opcionalmente com a criação de novos símbolos intermediários e
labels, retornando um trecho de código completo desse novo nodo;

## Etapa 6
A etapa 6 consiste na geração de código assembly AT&T a partir das TACs.
- [X] gerar arquivo de saída;
- [ ] fazer a geração de assembly para cada TAC, em linux e MAC;
   - [X] TAC_INT_DECL
   - [X] TAC_BYTE_DECL
   - [ ] TAC_FLOAT_DECL
   - [ ] TAC_BYTEA_DECL
   - [ ] TAC_INTA_DECL
   - [ ] TAC_FLOATA_DECL
   - [X] TAC_MOVE
   - [ ] TAC_ARRAYW
   - [ ] TAC_ARRAYR
   - [X] TAC_ADD
   - [X] TAC_SUB
   - [X] TAC_MUL
   - [X] TAC_DIV
   - [ ] TAC_AND
   - [ ] TAC_OR
   - [ ] TAC_NOT
   - [X] TAC_LT
   - [X] TAC_GT
   - [X] TAC_LE
   - [X] TAC_EQ
   - [X] TAC_NEQ
   - [X] TAC CALL
   - [X] TAC_BEGINFUN
   - [X] TAC_ENDFUN
   - [X] TAC_ARGWRITE
   - [X] TAC_IFZ
   - [X] TAC_JUMP
   - [X] TAC_LABEL
   - [X] TAC_RETURN
   - [ ] TAC_READ
      - [X] linux
      - [ ] mac
   - [X] TAC_PRINT
      - linux
        - [X] caso para literais string
        - [X] caso para variaveis
        - [ ] caso para expressões aritméticas - tem que arrumar a TAC
        - [X] caso para literais inteiros e byte
        - [ ] caso para literais float
        - [ ] caso para literais char
      - mac
        - [X] caso para literais string
        - [X] caso para variaveis
        - [ ] caso para expressões aritméticas - tem que arrumar a TAC
        - [X] caso para literais inteiros e byte
        - [ ] caso para literais float
        - [ ] caso para literais char
