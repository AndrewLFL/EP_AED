#include <stdio.h>
#include <stdlib.h>

typedef struct no{
    int valor;
    struct no *esquerdo, *direito;
    short altura;
}No;

No* novoNo(int x){
    No *novo = (No*)malloc(sizeof(No));

    if(novo){
        novo->valor = x;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0;
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

No* inserir(No *raiz, int x){
    if(raiz == NULL) return novoNo(x); //arvore vazia
    else {
        if(x < raiz->valor) raiz->esquerdo = inserir(raiz->esquerdo, x);
        else if(x > raiz->valor) raiz->direito = inserir(raiz->direito, x);
        else printf("\nInsrcao nao realizada, o elemento %d nao existe\n", x);
    }
    //recalcula as alturas
    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;

    //verifica se ha necessidade de balancear a arvore
    raiz = balancear(raiz);

    return raiz;
}

No* remover(No *raiz, int chave){
    if(raiz == NULL) return NULL; //valor n encontrado
    else{
        if(raiz->valor == chave) if(raiz->esquerdo == NULL && raiz->direito == NULL){//remove no sem filho
                                        free(raiz);
                                        printf("Elemento folha removido: %d", chave);
                                        return NULL;
                                    }
                                    else {
                                        //remove no que possui dois filhos
                                        if(raiz->esquerdo != NULL && raiz->direito != NULL){
                                            No* aux = raiz->esquerdo;
                                            while(aux->direito != NULL) aux = aux->direito;                                                
                                            raiz->valor = aux->valor;
                                            aux->valor = chave;
                                            printf("\nElemento trocado: %d\n", chave);
                                            raiz->esquerdo = remover(raiz->esquerdo, chave);
                                            return raiz;
                                        }
                                        else{
                                            //remove no que possui apenas um filho
                                            No* aux;
                                            if(raiz->esquerdo != NULL) aux = raiz->esquerdo;
                                            else aux = raiz->direito;
                                            free(raiz);
                                            printf("Elemento om 1 filho removido: %d", chave);
                                            return aux;
                                        }
                                    }
        else{
            if(chave < raiz->valor) raiz->esquerdo = remover(raiz->esquerdo, chave); 
            else raiz->direito = remover(raiz->direito, chave);
        }
        //recalcula a altura de todos os nos entre a raiz e o novo no inserido
        raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;

        //verifica necessidade de rebalancear a arvore
        raiz = balancear(raiz);

        return raiz;
    }
}

void imprime(No* raiz, int nivel){
    int i;
    if(raiz){
        imprime(raiz->direito, nivel + 1);
        printf("\n\n");

        for(i = 0; i < nivel; i++) printf("\t");

        printf("%d", raiz->valor);
        imprime(raiz->esquerdo, nivel + 1);
    }
}

int main(){
    int op, valor;
    No* raiz = NULL;

    do{
        printf("\n0 - sair\n1 - inserir\n2 - imprimir\n3 - remover\n");
        scanf("%d", &op);

        switch (op)
        {
        case 0:
            printf("\nSaindo...\n");
            break;
        case 1:
            printf("Digite um valor: ");
            scanf("%d", &valor);
            raiz = inserir(raiz, valor);
            break;
        case 2:
            printf("Impressao da arvore binaria:\n");
            imprime(raiz, 1);
            break;
        case 3:
            printf("Digite um valor a ser removido: ");
            scanf("%d", &valor);
            raiz = remover(raiz, valor);
            break;
        default:
        printf("\nOpcao invalida\n");
            break;
        }
    }while(op != 0);

}

