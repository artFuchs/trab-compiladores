#include "hash.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

NODE *Table[HASH_SIZE];

void hashInit(){
  int i;
  for (i=0; i<HASH_SIZE; i++){
    Table[i] = NULL;
  }
}

int hashAddress(char* text){
  int addr = 1;
  int i;
  for (i = 0; i<strlen(text); i++){
    addr = (addr*text[i])*(i+1)%HASH_SIZE;
  }
  return addr - 1;
}

NODE* hashInsert(int type, char *text){
  NODE* newNode;
  newNode = calloc(1, sizeof(NODE));
  newNode->type = type;
  newNode->text = calloc(strlen(text)+1, sizeof(char));
  strcpy(newNode->text, text);

  // procurar o bucket dado pelo endereço
  int addr = hashAddress(text);
  // se o bucket estiver vazio, colocar o novo nodo
  if (Table[addr] == NULL){
    newNode->next = NULL;
    Table[addr] = newNode;
    printf ("\nnodo {%d,%s} inserido no endereço %d.\n", type, text, addr);
  }
  /* se o bucket estiver cheio, procurar pelo nodo nesse bucket
     se o nodo não estiver no bucket, o nodo passa a apontar para
     o nodo no inicio do bucket e entra em sua posição*/
  else{
    NODE* node = searchNode(type, text);
    if (node == NULL){
      newNode->next = Table[addr];
      Table[addr] = newNode;
      printf ("\nnodo {%d,%s} inserido no endereço %d.\n", type, text, addr);
    }else{
      newNode = NULL;
      printf ("\nnodo {%d,%s} já está na tabela, no endereço %d.\n", type, text, addr);
    }
  }
  return newNode;
}

NODE* searchNode(int type, char*text){
  int addr = hashAddress(text);
  NODE* node = Table[addr];
  while (node!=NULL){
    if (strcmp(node->text, text)==0 && node->type == type)
      break;
    else
      node = node->next;
  }
  return node;
}
