#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

typedef struct palavra_no{
    char elemento[100];
    int linha;
    struct palavra_no *proximo;
} Palavra;

int converte_ascii(char *palavra); //implementar essa funcao

typedef struct no{
    int ascii;
    struct no *esquerdo, *direito;
    short altura;
    Palavra* palavra;
}No;

No* novoNo(Palavra *palavra){
    No *novo = (No*)malloc(sizeof(No));
    int x = converte_ascii(palavra->elemento);
    if(novo){
        novo->ascii = x;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0;
        novo->palavra = palavra;
    }
    else printf("\nErro ao aloca um No em novoNo\n");
    return novo;
}

short maior(short a, short b){//faz comparacoes
    return (a>b)? a: b;
}

short alturaNo(No *no){
    if(no == NULL) return -1;
    else return no->altura;
}

short fatorDeBalanceamento(No *no){
    if(no) return (alturaNo(no->esquerdo) - alturaNo(no->direito));
    else return 0;
}


No* rotacaoEsquerda(No* r){
    No *y, *f;
    
    y = r->direito;
    f = y->esquerdo;

    y->esquerdo = r;
    r->direito = f;

    r->altura = maior(alturaNo(r->esquerdo), alturaNo(r->direito)) + 1;
    y->altura = maior(alturaNo(y->esquerdo), alturaNo(y->direito)) + 1;

    return y;
}

No* rotacaoDireita(No* r){
    No *y, *f;

    y = r->esquerdo;
    f = y->direito;

    y->direito = r;
    r->esquerdo = f;

    r->altura = maior(alturaNo(r->esquerdo), alturaNo(r->direito)) + 1;
    y->altura = maior(alturaNo(y->esquerdo), alturaNo(y->direito)) + 1;

    return y;
}

No* rotacaoDireitaEsquerda(No* r){
    r->direito = rotacaoDireita(r->direito);
    return rotacaoEsquerda(r);
}

No* rotacaoEsquerdaDireita(No* r){
    r->esquerdo = rotacaoEsquerda(r->esquerdo);
    return rotacaoDireita(r); 
}

No* balancear(No* raiz){
    short fator_balanceamento = fatorDeBalanceamento(raiz);

    //rotacao a esquerda
    if(fator_balanceamento < -1 && fatorDeBalanceamento(raiz->direito) <= 0) raiz = rotacaoEsquerda(raiz);
    //rotacao a direita
    else if(fator_balanceamento > 1 && fatorDeBalanceamento(raiz->esquerdo) >= 0) raiz = rotacaoDireita(raiz);
    //rotacao a esquerda->direita
    else if(fator_balanceamento > 1 && fatorDeBalanceamento(raiz->esquerdo) < 0) raiz = rotacaoEsquerdaDireita(raiz);
    //rotacao a direita->esquerda
    else if(fator_balanceamento < -1 && fatorDeBalanceamento(raiz->direito) > 0) raiz = rotacaoDireitaEsquerda(raiz);

    return raiz;
}

No* inserir(No *raiz, Palavra *palavra){
    if(raiz == NULL) return novoNo(palavra); //arvore vazia
    else {
        int x = converte_ascii(palavra->elemento);
        if(x < raiz->ascii) raiz->esquerdo = inserir(raiz->esquerdo, palavra);
        else if(x > raiz->ascii) raiz->direito = inserir(raiz->direito, palavra);
        else{
            palavra->proximo = raiz->palavra;
            raiz->palavra = palavra;

            printf("\n Palavra adicionada ao no existente: %s (ASCII: %d)\n", palavra->elemento, x);
        } 
    }
    //recalcula as alturas
    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;

    //verifica se ha necessidade de balancear a arvore
    raiz = balancear(raiz);

    return raiz;
}

No* remover(No *raiz, int chave_ascii){
    if(raiz == NULL) return NULL; //valor n encontrado
    else{
        if(raiz->ascii == chave_ascii) if(raiz->esquerdo == NULL && raiz->direito == NULL){//remove no sem filho
                                        Palavra *atual = raiz->palavra;
                                        Palavra *prox;
                                        while(atual != NULL){
                                            prox = atual->proximo;
                                            free(atual);
                                            atual = prox;
                                        }
                                        free(raiz);
                                        printf("Elemento folha removido: %d", chave_ascii);
                                        return NULL;
                                    }
                                    else {
                                        //remove no que possui dois filhos
                                        if(raiz->esquerdo != NULL && raiz->direito != NULL){
                                            No* aux = raiz->esquerdo;
                                            while(aux->direito != NULL) aux = aux->direito;
                            
                                            Palavra *temp_palavra = raiz->palavra;
                                            raiz->palavra = aux->palavra;
                                            aux->palavra = temp_palavra;
                                            
                                            raiz->ascii = aux->ascii;
                                            raiz->esquerdo = remover(raiz->esquerdo, aux->ascii);

                                            raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;
                                            raiz = balancear(raiz);

                                            return raiz;
                                        }
                                        else{
                                            //remove no que possui apenas um filho
                                            No* aux;
                                            if(raiz->esquerdo != NULL) aux = raiz->esquerdo;
                                            else aux = raiz->direito;
                                            
                                            Palavra *atual = raiz->palavra;
                                            Palavra *prox;
                                            while(atual != NULL){
                                                prox = atual->proximo;
                                                free(atual);
                                                atual = prox;
                                            }
                                            free(raiz);
                                            printf("Elemento com 1 filho removido: %d", chave_ascii);
                                            return aux;
                                        }
                                    }
        else{
            if(chave_ascii < raiz->ascii) raiz->esquerdo = remover(raiz->esquerdo, chave_ascii); 
            else raiz->direito = remover(raiz->direito, chave_ascii);
        }


        if(raiz->ascii != chave_ascii){
            //recalcula a altura de todos os nos entre a raiz e o novo no inserido
            raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;

            //verifica necessidade de rebalancear a arvore
            raiz = balancear(raiz);
        }

        return raiz;
    }
}

void imprime(No* raiz, int nivel){
    int i;
    if(raiz){
        imprime(raiz->direito, nivel + 1);
        printf("\n\n");

        for(i = 0; i < nivel; i++) printf("\t");

        printf("%d", raiz->ascii);
        Palavra *p = raiz->palavra;
        while(p != NULL){
            printf(" [%s - linha %d]", p->elemento, p->linha);
            p = p->proximo;
        }
        
        imprime(raiz->esquerdo, nivel + 1);
    }
}

Palavra* criarPalavra(char *elemento, int linha){
    Palavra *nova = (Palavra*)malloc(sizeof(Palavra));
    if(nova){
        strcpy(nova->elemento, elemento);
        nova->linha = linha;
        nova->proximo = NULL;
    }
    return nova;
}