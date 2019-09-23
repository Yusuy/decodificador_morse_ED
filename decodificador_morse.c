/*****************************************
Universidade de Brasília - UnB
Estrutura de Dados - Trabalho 2
Victor Gabriel Rodrigues de Almeida - 14/0052399
******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct no{
	char *dado;
	struct no *esq;
	struct no *dir;
}t_no;

typedef struct elemento{
    char c;
    struct elemento *proximo;
} t_elemento;

typedef struct elemento_morse{
    char simbolo;
    char cod[10];
    struct elemento_morse *proximo;
} t_elem_morse;

typedef struct morse{
    t_elem_morse *primeiro;
    t_elem_morse *ultimo;
} t_morse;

typedef struct lista{
    t_elemento *primeiro;
    t_elemento *ultimo;
} t_lista;

t_lista *criarLista(){
   t_lista *l = (t_lista *)malloc(sizeof(t_lista));
   l->primeiro = NULL;
   l->ultimo = NULL;
   return l;
}

t_morse *criaLMorse(){
    t_morse *l = (t_morse *)malloc(sizeof(t_morse));
    l->primeiro = NULL;
    l->ultimo = NULL;
    return l;
}

t_no *criarArvore(){
   t_no *raiz =  (t_no *)malloc(sizeof(t_no));
   raiz->esq = NULL;
   raiz->dir = NULL;
   raiz->dado = NULL;
   return raiz;
}

void insereFinal(char valor, t_lista *l){
   t_elemento *novoultimo = (t_elemento *)malloc(sizeof(t_elemento));
   novoultimo->c = valor;
   novoultimo->proximo = NULL;
   if(l->primeiro == NULL){
      l->primeiro = novoultimo;
   }else{
     l->ultimo->proximo = novoultimo;
   }
   l->ultimo = novoultimo;
}

void insereFinalM(char valor, char codg[], t_morse *l){
   t_elem_morse *novoultimo = (t_elem_morse *)malloc(sizeof(t_elem_morse));
   novoultimo->simbolo = valor;
   strcpy(novoultimo->cod, codg);
   novoultimo->proximo = NULL;
   if(l->primeiro == NULL){
      l->primeiro = novoultimo;
   }else{
     l->ultimo->proximo = novoultimo;
   }
   l->ultimo = novoultimo;
}

//Lista que contém os caracteres e seus códigos morse do arquivo
t_lista *listaCod(){
    t_lista *listaCod = criarLista();
    FILE *code;
    char c;

    code = fopen("codigo.txt", "r");
    while(!feof(code)){
        fscanf(code,"%c", &c);
        insereFinal(c, listaCod);
    }
    fclose(code);
    return listaCod;
}

void imprimirListaChar(t_lista *l){
    t_elemento *atual = l->primeiro;

    printf("\n");
    while(atual->proximo != NULL){
          printf("%c",atual->c);
          atual = atual->proximo;
    }
}

/*void imprimirListaCharM(t_morse *l){
    t_elem_morse *atual = l->primeiro;

    printf("\n");
    while(atual != NULL){
          printf("Simbolo:%c --- Codigo: %s \n",atual->simbolo, atual->cod);
          atual = atual->proximo;
    }
}*/

//Função que insere nós na árvore que guarda os caracteres
void insereNo(t_elemento *l, t_no *no){
    t_no *raiz = no;
    char var;
    if(l != NULL){
        var = l->c;
        //printf("Nada null, ta beleza - var = %c\n\n", var);
        l = l->proximo->proximo;
    }

    while(l->proximo!=NULL){
        //printf("Entrou\n\n");
         if(l->c == '.'){
            //printf("esq->");
            if(no->esq != NULL){
                no = no->esq;
            }
            else{
                no->esq = criarArvore();
                no = no->esq;
            }
         }

         else if(l->c == '-'){
            //printf("dir->");
            if(no->dir != NULL){
                no = no->dir;
            }
            else{
                no->dir = criarArvore();
                no = no->dir;
            }
         }
         else{
            no->dado = var;
            //printf("dado - %c\n", var);
            l = l->proximo;
            return insereNo(l, raiz);
         }
         l = l->proximo;
    }

    no->dado = var;
    //printf("dado - %c", no->dado);
}

//Cria a árvore que contém os caracteres
t_no *arvoreCod(){
    t_no *arvoreMorse = criarArvore();
    //printf("Criou arvore\n\n");
    t_lista *lista = listaCod();
    //printf("Pegou a lista\n\n");
    //imprimirListaChar(lista);
    //system("pause");
    t_elemento *atual = lista->primeiro;
    //printf("Pegou primeiro elemento da lista %c\n\n", atual->c);
    insereNo(atual, arvoreMorse);
    return arvoreMorse;
}

//Cria a lista que guarda a mensagem codificada do arquivo
t_lista *listaMsgC(){
    t_lista *l = criarLista();
    char c;

    FILE *msg;
    msg = fopen("mensagem.txt", "r");
    while(!feof(msg)){
        fscanf(msg,"%c",&c);
        insereFinal(c, l);
    }
    fclose(msg);

    return l;
}

//Função que decodifica a mensagem por árvore
void decArvore(){
    t_no *arvoreMorse = arvoreCod();
    t_no *raiz = arvoreMorse;
    t_lista *msg = listaMsgC();
    t_elemento *atual = msg->primeiro;

    while(atual->proximo!=NULL){
        if(atual->c == '.'){
            //printf("esq->");
            arvoreMorse = arvoreMorse->esq;
        }
        else if(atual->c == '-'){
            //printf("dir->");
            arvoreMorse = arvoreMorse->dir;
        }
        else if(atual->c == '/'){
            printf(" ");
            atual = atual->proximo;
        }
        else if(atual->c == ' '){
            if(atual->proximo->c == '/' || atual->proximo->c == '.' || atual->proximo->c == '-'){
                printf("%c",arvoreMorse->dado);
                arvoreMorse = raiz;
            }
            else{
                printf(" ");
            }
        }
        atual = atual->proximo;
    }
    printf("%c",arvoreMorse->dado);

}

//Função que insere caractere e código na lista que guardará os caracteres
void insereListC(t_elemento *atual, t_morse *m){
    //printf("\nEntrou\n");

    if(atual==NULL){
        return 0;
    }
    char n;
    char cod[10];
    n = atual->c;
    //printf("\nValor de N = %c -> ", n);
    int i = 0;
    atual = atual->proximo->proximo;
    //printf("\nFoi pra %c", atual->c);
    while(atual->c != '\n'){
        cod[i] = atual->c;
        //printf("%c", cod[i]);
        atual = atual->proximo;
        i++;
        if(atual->proximo == NULL){
            break;
        }
    }
    cod[i]='\0';
    atual = atual->proximo;
    insereFinalM(n, cod, m);
    return insereListC(atual, m);

}

//Cria a lista que contém os caracteres e seus respectivos códigos
t_morse *listaMor(){
    t_morse *listaMorse = criaLMorse();
    t_lista *listaC = listaCod();
    t_elemento *atual = listaC->primeiro;
    //imprimirListaChar(listaC);
    insereListC(atual, listaMorse);
    //printf("\nCheguei aqui");
    //imprimirListaCharM(listaMorse);
    //printf("\n\nSim: %c -> %s", listaMorse->ultimo->simbolo, listaMorse->ultimo->cod);
    return listaMorse;
}

//Função que decodifica a mensagem utilizando lista
void decLista(){
    t_lista *msg = listaMsgC();
    t_morse *codigos = listaMor();
    char caractere[10];
    int i = 0;
    t_elemento *atual = msg->primeiro;
    t_elem_morse *atualM = codigos->primeiro;

    while(atual->proximo!=NULL){
        if(atual->c == ' ' || atual->c == '/'){
            printf(" ");
            while(atual->c == ' ' || atual->c == '/'){
                //printf("\nNao e codigo:%c.", atual->c);
                atual = atual->proximo;
            }
        }
        if(atual->c == '-' || atual->c == '.'){
            while((atual->c == '-' || atual->c == '.') && atual->proximo != NULL){
                caractere[i] = atual->c;
                atual=atual->proximo;
                i++;
                //printf("%d", i);
            }
            caractere[i] = '\0';


            i = 0;
            //printf("\n Codigo lido: %s\n", caractere);
        }
        //printf("\nOk\n");
        while(1){
            if(strcmp(caractere, atualM->cod) == 0){
                printf("%c", atualM->simbolo);
                break;
            }
            atualM = atualM->proximo;
        }
        if(atual->proximo == NULL){
            break;
        }
        atualM = codigos->primeiro;
        atual = atual->proximo;
    }

}

int main(){
    clock_t Arv[2], Lis[2];
    double t_Arv, t_Lis;
    printf("____________________\nMENSAGEM CODIFICADA:\n____________________");
    t_lista *l = listaMsgC();
    imprimirListaChar(l);
    printf("\n\n");
    printf("_________________________________\nMENSAGEM DECODIFICADA POR ARVORE:\n_________________________________\n");
    Arv[0] = clock();
    decArvore();
    Arv[1] = clock();
    t_Arv = (((double)Arv[1] - (double)Arv[0])*1000)/CLOCKS_PER_SEC;
    printf("\n ------------> TEMPO NA EXECUCAO: %.2lf ms.", t_Arv);
    printf("\n\n");
    printf("_________________________________\nMENSAGEM DECODIFICADA POR LISTA:\n_________________________________\n");
    Lis[0] = clock();
    decLista();
    Lis[1] = clock();
    t_Lis = (((double)Lis[1] - (double)Lis[0])*1000)/CLOCKS_PER_SEC;
    printf("\n ------------> TEMPO NA EXECUCAO: %.2lf ms.", t_Lis);
    printf("\n\n");
    return 0;
}
