#define HASH_SIZE 1024

typedef struct node{
  int type;
  char *text;
  struct node *next;
} NODE;

void hashInit(); // inicializa a tabela hash
int hashAddress(char *text); // cria um endereço para uma entrada na tabela
NODE* hashInsert(int type, char *text);
NODE* searchNode(int type, char* text); // procura uma entrada na tabela
void printTable();
