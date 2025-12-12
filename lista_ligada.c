#include<stdio.h>
#include<stdlib.h>

typedef struct _no_{
    int elemento;
    struct _no_* proximo;
} No;

typedef struct{
    No* primeiro;
} ListaLigada;

ListaLigada* cria_lista(){
    ListaLigada* lista = (ListaLigada*)malloc(sizeof(ListaLigada));
    lista->primeiro = NULL;

    return lista;
}

void destroi_lista(ListaLigada* lista){
    No* p = lista->primeiro;
    No* tmp;

    while(p){
        tmp = p;
        p = p->proximo;
        free(tmp);
    }
    free(lista);
}

void imprime_lista(ListaLigada* lista){
    
    No* p;

    printf("Lista: \n");
    
    for(p = lista->primeiro; p; p = p->proximo){
        printf("%d ", p->elemento);
    }
}

int tamanho_lista(ListaLigada* lista){
    int tamanho = 0;
    No* p = lista->primeiro; 
    while(p){
        tamanho++;
        p = p->proximo;   
    }
    return tamanho;
}

int valor_indice(ListaLigada* lista, int index){
    No* p = lista->primeiro;
    for(int i = 0; i < index; i++) p = p->proximo;
        
    return p->elemento;
}

int busca_valor(ListaLigada* lista, int valor){
    No* p = lista->primeiro;
    int i = 0;

    while(p && p->elemento != valor){
        p = p->proximo;
        i++;
    }
    return p ? i : -1;
}

int insere_valor(ListaLigada* lista, int valor, int indice){
    int i, antecessor;

    No* p;
    No* novo = (No*)malloc(sizeof(No));

    novo->elemento = valor;

    if(indice >= 0 && indice <= tamanho_lista(lista)){
        if(indice == 0){
            novo->proximo = lista->primeiro;
            lista->primeiro = novo;
        }
        else{
            i = 0;
            antecessor = indice - 1;
            p = lista->primeiro;

            while(i < antecessor){
                i++;
                p = p->proximo;

            }
            novo->proximo = p->proximo;
            p->proximo = novo;
        }
        return 0;
    }
    return 1;
}   

int remove_valor(ListaLigada* lista, int valor){
    No* p;
    No* tmp;
    int index = busca_valor(lista, valor);
    int i, antecessor = index - 1;


    if(index >= 0){
        if(index == 0){
            tmp = lista->primeiro;
            lista->primeiro = tmp->proximo;
            free(tmp);
        }
        else{
            i = 0;
            p = lista->primeiro;

            while(i < antecessor){
                i++;
                p = p->proximo;
            }
            tmp = p->proximo;
            p->proximo = p->proximo->proximo;
            free(tmp);
        }
        return 0;
    }
    return 1;
}

int main(){
    ListaLigada* lista = cria_lista();
    insere_valor(lista, 30, 0);
    insere_valor(lista, 20, 1);
    insere_valor(lista, 40, 1);
    insere_valor(lista, 10, 0);
    insere_valor(lista, 50, 1);
    insere_valor(lista, 70, 2);
    insere_valor(lista, 60, 3);
    imprime_lista(lista);
    printf("\n");
    int valorIndice = valor_indice(lista, 1);
    printf("valor no indice 1: %d\n", valorIndice);
    
    int i;
    int soma = 0;
    int n = tamanho_lista(lista);
    for(i = 0; i < n; i++) soma += valor_indice(lista, i);
    printf("\nSoma: %d\n", soma);
    
    destroi_lista(lista);
    return 0;
}