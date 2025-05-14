#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 100
#define LEN 20


typedef struct Data
{
    int dia;
    int mes;
    int ano;
}Data;

typedef struct Registro
{
    char Nome[TAM];
    int idade;
    char RG[TAM];
    Data *Entrada;
}Registro;


typedef struct CelulaLista {
    Registro *dados;
    struct CelulaLista *proximo;
} CelulaLista;

typedef struct {
    CelulaLista *primeiro;
    int qtd;
} LDE;

typedef struct {
	CelulaLista *head;
	CelulaLista *tail;
	int qtd;
} Fila;

typedef struct {
  Registro *dados[LEN];
  int qtd;
} heap;

void clearBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

CelulaLista *criar_celula(Registro *dados){
    CelulaLista *nova = malloc(sizeof(CelulaLista));
    nova->proximo = NULL;
    nova->dados = dados;
    return nova;
}

LDE *criar_lista(){
    LDE *lista = malloc(sizeof(LDE));
    lista->primeiro = NULL;
    lista->qtd = 0;
    return lista;
}

Fila *cria_queue(){
	Fila *queue = malloc(sizeof(Fila));
	queue->head = NULL;
	queue->tail = NULL;
	queue->qtd = 0;

	return queue;
}

int filho_esq(int pai) { 
    return 2 * pai + 1;
}

int filho_dir(int pai) { 
    return 2 * pai + 2;
}

int pai(int filho) { 
    return (filho - 1) / 2;
}

int ultimo_pai(heap *h) { 
    return (h->qtd / 2) - 1;
}

void inserirLista(LDE *lista, Registro *dados){
    CelulaLista *nova = criar_celula(dados);
    CelulaLista *atual = lista->primeiro;
    lista->primeiro = nova;
    nova->proximo = atual;
    lista->qtd++;
}

void CadastrarPaciente(LDE *lista){
    Registro *nova = malloc(sizeof(Registro));
    nova->Entrada = malloc(sizeof(Data));

    printf("Nome: ");
    clearBuffer();
    fgets(nova->Nome, sizeof(nova->Nome), stdin);
    nova->Nome[strcspn(nova->Nome, "\n")] = '\0';

    printf("Idade: ");
    scanf("%d", &nova->idade);

    printf("RG: ");
    clearBuffer();
    fgets(nova->RG, sizeof(nova->RG), stdin);
    nova->RG[strcspn(nova->RG, "\n")] = '\0';

    printf("Data do registro: ");
    scanf("%d %d %d", &nova->Entrada->dia, &nova->Entrada->mes, &nova->Entrada->ano);

    inserirLista(lista, nova);
}

void ConsultarPaciente(LDE *lista){
    int controle = 0;
    char consulta[TAM];
    printf("RG do paciente a ser consultado: ");
    clearBuffer();
    fgets(consulta, sizeof(consulta), stdin);
    consulta[strcspn(consulta, "\n")] = '\0';
    CelulaLista *atual = lista->primeiro;
    while(atual != NULL){
        if(strcmp(consulta, atual->dados->RG) == 0){
            controle = 1;
            break;
        }
        atual = atual->proximo;
    }
    if(controle == 0){
        printf("Paciente não encontrado.\n");
    }
    else if(controle == 1){
        printf("Nome: %s\n", atual->dados->Nome);
        printf("RG: %s\n", atual->dados->RG);
        printf("Idade: %d\n", atual->dados->idade);
        printf("Data de registro: %d/%d/%d\n", atual->dados->Entrada->dia, atual->dados->Entrada->mes, atual->dados->Entrada->ano);
    }
    printf("\n");
}

void listaCompleta(LDE *lista){
    CelulaLista *atual = lista->primeiro;
    while(atual != NULL){
        printf("Nome: %s\n", atual->dados->Nome);
        printf("RG: %s\n", atual->dados->RG);
        printf("Idade: %d\n", atual->dados->idade);
        printf("Data de registro: %d/%d/%d\n", atual->dados->Entrada->dia, atual->dados->Entrada->mes, atual->dados->Entrada->ano);
        atual = atual->proximo;
    }
    printf("\n");
}

void atualizarPaciente(LDE *lista){
    char atualizar[TAM];
    printf("RG do paciente a ser consultado: ");
    clearBuffer();
    fgets(atualizar, sizeof(atualizar), stdin);
    atualizar[strcspn(atualizar, "\n")] = '\0';
    CelulaLista *atual = lista->primeiro;
    while(atual != NULL && strcmp(atualizar, atual->dados->RG) != 0){ 
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Paciente não encontrado.\n");
        return;
    }
    printf("Novo nome: ");
    fgets(atual->dados->Nome, sizeof(atual->dados->Nome), stdin);
    atual->dados->Nome[strcspn(atual->dados->Nome, "\n")] = '\0';
    printf("Nova idade: ");
    scanf("%d", &atual->dados->idade);
    printf("Data do registro atualizado: ");
    scanf("%d %d %d", &atual->dados->Entrada->dia, &atual->dados->Entrada->mes, &atual->dados->Entrada->ano);
    printf("Paciente atualizado com sucesso!\n");
}

void removerPaciente(LDE *lista){
    char remover[TAM];
    printf("RG do paciente a ser removido: ");
    clearBuffer();
    fgets(remover, sizeof(remover), stdin);
    remover[strcspn(remover, "\n")] = '\0';
    CelulaLista *anterior = NULL; 
    CelulaLista *atual = lista->primeiro; 
    while(atual != NULL && strcmp(remover, atual->dados->RG) != 0){ 
        anterior = atual;
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Paciente não encontrado.\n");
        return;
    }
    if(anterior == NULL){
        lista->primeiro = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    free(atual);
    lista->qtd--;
    printf("Paciente removido com sucesso!\n");
}

void enfilerarPaciente(LDE *lista, Fila *queue){
    char enfilerar[TAM];
    printf("RG do paciente a ser adicionado a fila de atendimento: ");
    clearBuffer();
    fgets(enfilerar, sizeof(enfilerar), stdin);
    enfilerar[strcspn(enfilerar, "\n")] = '\0';
    CelulaLista *atual = lista->primeiro;
    while(atual != NULL && strcmp(enfilerar, atual->dados->RG) != 0){ 
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Paciente não encontrado.\n");
        return;
    }
    CelulaLista *nova = criar_celula(atual->dados);
    if(queue->qtd == 0){
        queue->head = nova;
    }
    else{
        queue->tail->proximo = nova;
    }
    queue->tail = nova;
    queue->qtd++;
    printf("Paciente adicionado a fila de atendimento.\n");
}

void desenfilerarPaciente(Fila *queue){
    char desenfilerar[TAM];
    printf("RG do paciente a ser removido da fila de atendimento: ");
    clearBuffer();
    fgets(desenfilerar, sizeof(desenfilerar), stdin);
    desenfilerar[strcspn(desenfilerar, "\n")] = '\0';
    CelulaLista *atual = queue->head;
    while(atual != NULL && strcmp(desenfilerar, atual->dados->RG) != 0){ 
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Paciente não encontrado.\n");
        return;
    }
	if(queue->qtd == 0){
        printf("Paciente não encontrado na fila de atendimento.\n");
        return;
    }
    CelulaLista *aux = queue->head;
    if(queue->qtd == 1){
        queue->head = NULL;
        queue->tail = NULL;
    }
    else{
        queue->head = queue->head->proximo;
    }
    queue->qtd--;
    free(aux);
    printf("Paciente removido da fila de atendimento.\n");
}

void mostrarFila(Fila *queue){
	CelulaLista *atual = queue->head;
    int pos = 1;
    while(atual != NULL){
        printf("Posição: %d\n", pos);
        printf("Nome: %s\n", atual->dados->Nome);
        printf("RG: %s\n", atual->dados->RG);
        printf("Idade: %d\n", atual->dados->idade);
        printf("Data de registro: %d/%d/%d\n", atual->dados->Entrada->dia, atual->dados->Entrada->mes, atual->dados->Entrada->ano);
        pos++;
        atual = atual->proximo;
    }
}

void peneirar(heap *h, int pai) {
    int fe = filho_esq(pai);
    int fd = filho_dir(pai);
    int maior = pai;

    if(fe < h->qtd && h->dados[fe]->idade > h->dados[maior]->idade){
        maior = fe;
    }
    if(fd < h->qtd && h->dados[fd]->idade > h->dados[maior]->idade){
        maior = fd;
    }

    if(pai != maior){
        Registro *aux = h->dados[pai];
        h->dados[pai] = h->dados[maior];
        h->dados[maior] = aux;
        peneirar(h, maior);
    }

    fe = filho_esq(pai);
    fd = filho_dir(pai);
    if (fd < h->qtd && fe < h->qtd && h->dados[fd]->idade > h->dados[fe]->idade) {
        Registro *aux = h->dados[fe];
        h->dados[fe] = h->dados[fd];
        h->dados[fd] = aux;
    }
}

void construir(heap *h) {
    for(int i = ultimo_pai(h); i >= 0; i--){
        peneirar(h, i);
    }
}

void enfilerarPrioritario(heap *h, LDE *lista) {
    if(h->qtd >= LEN){
        printf("Fila de prioridade cheia!!!");
        return;
    }
    char enfilerarP[TAM];
    printf("RG do paciente a ser adicionado a fila de atendimento: ");
    clearBuffer();
    fgets(enfilerarP, sizeof(enfilerarP), stdin);
    enfilerarP[strcspn(enfilerarP, "\n")] = '\0';
    CelulaLista *atual = lista->primeiro;
    while(atual != NULL && strcmp(enfilerarP, atual->dados->RG) != 0){ 
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Paciente não encontrado.\n");
        return;
    }
    CelulaLista *nova = criar_celula(atual->dados);
    h->dados[h->qtd] = atual->dados;
    h->qtd++;
    printf("Paciente adicionado a fila de atendimento prioritario.\n");
    construir(h);
}

void desenfilerarPrioritario(heap *h) {
    if(h->qtd == 0){
        printf("Fila de prioridade vazia!!!");
        return;
    }
    char desenfilerarP[TAM];
    printf("RG do paciente a ser adicionado a fila de atendimento: ");
    clearBuffer();
    fgets(desenfilerarP, sizeof(desenfilerarP), stdin);
    desenfilerarP[strcspn(desenfilerarP, "\n")] = '\0';
    int deletar;
    int controle = 0;
    for(int i = 0; i < h->qtd; i++){ 
        if(strcmp(h->dados[i]->RG, desenfilerarP) == 0){
            deletar = i;
            controle = 1;
            break;
        }
    }
    if(controle == 1){
        printf("Paciente não encontrado.\n");
        return;
    }
    h->dados[deletar] = h->dados[h->qtd - 1];
    h->qtd--;
    printf("Paciente removido da fila de atendimento prioritario.\n");
    construir(h);
}

void mostrarPrioritario(heap *h) { 
    int pos = 1;
    for(int i = 0; i < h->qtd; i++){
        printf("Posição: %d\n", pos);
        printf("Nome: %s\n", h->dados[i]->Nome);
        printf("RG: %s\n", h->dados[i]->RG);
        printf("Idade: %d\n", h->dados[i]->idade);
        printf("Data de registro: %d/%d/%d\n", h->dados[i]->Entrada->dia, h->dados[i]->Entrada->mes, h->dados[i]->Entrada->ano);
        pos++;
    }
    printf("\n");
}

int main(void){
    LDE *lista = criar_lista();
    Fila *fila = cria_queue();
    heap *h = malloc(sizeof(heap));
    int opcao;
    while(1){
        printf("\nMenu Principal\n");
        printf("1 - Cadastrar\n");
        printf("2 - Atendimento\n");
        printf("3 - Atendimento Prioritário\n");
        printf("4 - Pesquisa\n");
        printf("5 - Desfazer\n");
        printf("6 - Salvar\n");
        printf("7 - Carregar\n");
        printf("8 - Sobre\n");
        printf("0 - Sair\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);
        if(opcao == 1){
            while(1){
                printf("\nMenu de Cadastro\n");
                printf("1 - Cadastrar novo paciente\n");
                printf("2 - Consultar paciente cadastrado\n");
                printf("3 - Mostrar lista completa \n");
                printf("4 - Atualizar dados do paciente \n");
                printf("5 - Remover paciente \n");
                printf("0 - Retornar ao menu principal \n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);
                if(opcao == 1){
                    CadastrarPaciente(lista);
                }
                else if(opcao == 2){
                    ConsultarPaciente(lista);
                }
                else if(opcao == 3){
                    listaCompleta(lista);
                }
                else if(opcao == 4){
                    atualizarPaciente(lista);
                }
                else if(opcao == 5){
                    removerPaciente(lista);
                }
                else if(opcao == 0){
                    break;
                }
                else{
                    printf("Opção inválida\n");
                }
            }
        }
        else if(opcao == 2){
            while (1)
            {
                printf("\nMenu de Atendimento\n");
                printf("1 - Enfileirar paciente\n");
                printf("2 - Desenfileirar paciente \n");
                printf("3 - Mostrar fila\n");
                printf("0 - Retornar ao menu principal \n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);
                if(opcao == 1){
                    enfilerarPaciente(lista, fila);
                }
                else if(opcao == 2){
                    desenfilerarPaciente(fila);
                }
                else if(opcao == 3){
                    mostrarFila(fila);
                }
                else if(opcao == 0){
                    break;
                }
                else{
                    printf("Opção inválida\n");
                }
            }
        }
        else if(opcao == 3){
            while (1)
            {
                printf("\nMenu de Atendimento Prioritario\n");
                printf("1 - Enfileirar paciente\n");
                printf("2 - Desenfileirar paciente \n");
                printf("3 - Mostrar fila\n");
                printf("0 - Retornar ao menu principal \n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);
                if(opcao == 1){
                    enfilerarPrioritario(h, lista);
                }
                else if(opcao == 2){
                    desenfilerarPrioritario(h);
                }
                else if(opcao == 3){
                    mostrarPrioritario(h);
                }
                else if(opcao == 0){
                    break;
                }
                else{
                    printf("Opção inválida\n");
                }
            }
        }
        else if(opcao == 8){
            printf("Desenvolvedores do projeto:\n");
            printf("Orlando Nagrockis Bertholdo \tRA: 24.223.033-5\n");
            printf("Morgana Rodrigues Zanetti \tRA: 24.223.010-0\n");
            printf("Informacoes adicionais:\n");
            printf("Curso: \tCiencia da Computacao\n");
            printf("Disciplina: \tEstrutura de Dados\n");
            printf("Ciclo: \t4° Semestre\n");
        }
        else if(opcao == 0){
            printf("Finalizando programa...\n");
            break;
        }
        else{
            printf("Opção inválida\n");
        }
    }
    
    return 0;
}