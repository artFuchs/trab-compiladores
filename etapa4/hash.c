/******************************************************
*                                                     *
* Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
*                                                     *
******************************************************/

#include "hash.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    addr = ((addr*text[i])*(i+1))%HASH_SIZE+1;
  }
  return addr - 1;
}

NODE* hashInsert(int type, int dataType, char *text){
  NODE* newNode;
  newNode = calloc(1, sizeof(NODE));
  memcpy(&newNode->type,&type,sizeof(int));
  newNode->dataType = DATATYPE_UNDEFINED;
  newNode->text = calloc(strlen(text)+1, sizeof(char));
  strcpy(newNode->text, text);

  // procurar o bucket dado pelo endereço
  int addr = hashAddress(text);
  // se o bucket estiver vazio, colocar o novo nodo
  if (Table[addr] == NULL){
    newNode->next = NULL;
    Table[addr] = newNode;
  }
  /* se o bucket estiver cheio, procurar pelo nodo nesse bucket
     se o nodo não estiver no bucket, o nodo passa a apontar para
     o nodo no inicio do bucket e entra em sua posição*/
  else{
    NODE* node = searchNode(text);
    if (node == NULL){
      newNode->next = Table[addr];
      Table[addr] = newNode;
    }else{
      newNode = node;
    }
  }
  return newNode;
}

NODE* searchNode(char*text){
  int addr = hashAddress(text);
  NODE* node = Table[addr];
  while (node!=NULL){
    if (strcmp(node->text, text)==0)
      break;
    else
      node = node->next;
  }
  return node;
}

void printTable(){
  int i=0;
  for (i=0; i<HASH_SIZE; i++){
    if (Table[i]!=NULL){
      printf("%d: {%d,%s}\n", i, Table[i]->type, Table[i]->text);
      NODE* node = Table[i]->next;
      while (node!=NULL){
        printf("-- {%d,%s}\n", node->type, node->text);
        node = node->next;
      }
    }
  }
}
