/*
A constante TAM define o tamanho do vetor
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#define TAM 31
//#define TAM 79
#define TAM 151

typedef struct no {
  char str[20];
  struct no *proximo;
} No;

typedef struct {
  No *inicio;
  int tam;
  int colisoes;
} Lista;

void iniciaLista(Lista *l) {
  l->inicio = NULL;
  l->tam = 0;
  l->colisoes =0;
}

void inserirNaLista(Lista *l, char str[]) {
  No *new = malloc(sizeof(No));

  if (new) {
    strcpy(new->str, str);
    new->proximo = l->inicio;
    l->inicio = new;
    l->tam++;
    if(l->tam > 1){
      l->colisoes++;
    }
  } else {
    printf("Erro ao alocar memoria\n");
  }
}

No *buscarNaLista(Lista *l, char str[]) {
  No *aux = l->inicio;
  while (aux && strcmp(aux->str, str) != 0) {
    aux = aux->proximo;
  }
  if (aux) {
    return aux;
  }
  return NULL;
}

void imprimirLista(Lista *l) {
  No *aux = l->inicio;
  printf("\nTamanho: %d \n", l->tam);
  printf("Colisoes: %d \n", l->colisoes);
  while (aux) {
    printf("\n\n%s ", aux->str);
    aux = aux->proximo;
  }
}

void inicializarTabela(Lista t[]) {
  for (int i = 0; i < TAM; i++) {
    iniciaLista(&t[i]);
  }
}

int funcaoHash(char *str) {
  int tam = strlen(str);
  unsigned int hash = 0;
  for (int i = 0; i < tam; i++) {
    hash += str[i] * (i + 1);
  }
  return hash % TAM;
}

void inserirNaTabela(Lista t[], char *str) {
  int id = funcaoHash(str);
  inserirNaLista(&t[id], str);
}

No *buscaNaTabela(Lista *t, char *str) {
  int id = funcaoHash(str);
  printf("Indice gerado: %d\n", id);
  return buscarNaLista(&t[id], str);
}

void imprimirTabela(Lista l[]) {
  for (int i = 0; i < TAM; i++) {
    printf("\n---------------\n");
    printf("Posicao %.2d ", i);
    imprimirLista(&l[i]);
    printf("\n");
  }
}

int main(void) {
  clock_t inicio, fim;

  int opcao, valor, retorno;
  Lista tabela[TAM];
  No *buscar;
  char string[20];
  char *validchars = "abcdefghijklmnopqrstuvwxyz";
  char *novastr;

  int str_len, tamanho;

  inicializarTabela(tabela);

  do {
    printf("\n\t0 - Sair\n\t1 - Inserir\n\t2 - Buscar\n\t3 - Imprimir\n");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
      inicio = time(NULL);
      fim = time(NULL);
    case 1:
      printf("\tQuantas strings deseja inserir? 100, 1000 ou 10000? ");
      do {
        scanf("%d", &tamanho);
        if (tamanho != 100 && tamanho != 1000 && tamanho != 10000) {
          printf("Digite o valor de novo!");
        }
      } while (tamanho != 100 && tamanho != 1000 && tamanho != 10000);
      inicio = clock();
      for (int i = 0; i < tamanho; i++) {
        str_len = (rand() % 20);
        str_len += str_len < 4 ? 4 : 0;
        novastr = (char *)malloc((str_len + 1) * sizeof(char));
        for (int i = 0; i < str_len; i++) {
          novastr[i] = validchars[rand() % strlen(validchars)];
          novastr[i + 1] = 0x0;
        }
        inserirNaTabela(tabela, novastr);
      }
      fim = clock();
      printf("Tempo de inserção: %lf milisegundos\n",
             (((double)(fim - inicio)) / (CLOCKS_PER_SEC / 1000)));
      int soma = 0;
      for (int i = 0; i < TAM; i++) {
        soma += tabela[i].colisoes;
      }
      printf("%d colisoes\n", soma);
      break;
    case 2:
      printf("\tQual nome deseja buscar? ");
      inicio = clock();
      scanf("%s", string);
      buscar = buscaNaTabela(tabela, string);
      if (buscar) {
        printf("Valor encontrado: %s\n", buscar->str);
      } else {
        printf("String nao encontrada nessa posicao\n");
      }
      fim = clock();
      printf("Tempo de busca: %lf milisegundos",
             (((double)(fim - inicio)) / (CLOCKS_PER_SEC / 1000)));
      break;
    case 3:
      imprimirTabela(tabela);
      break;
    }
  } while (opcao != 0);
  return 0;
}