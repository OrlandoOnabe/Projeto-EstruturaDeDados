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


typedef struct Celula {
    Registro *dados;
    struct Celula *proximo;
} Celula;

typedef struct EABB{
	Registro *dados;
	struct EABB* esq;
	struct EABB* dir;
} EABB;

typedef struct {
    Celula *primeiro;
    int qtd;
} LDE;

typedef struct {
	Celula *head;
	Celula *tail;
	int qtd;
} Fila;

typedef struct {
  Registro *dados[LEN];
  int qtd;
} heap;

typedef struct ABB{
	EABB* raiz;
	int qtd;
} ABB;

typedef struct Log {
  struct Log *anterior;
  struct Log *proximo;
  char operacao[TAM];
  Registro *dados;
} Log;

typedef struct {
  Log *top;
  int qtd;
} Pilha;


void clearBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Celula *criar_celula(Registro *dados){
    Celula *nova = malloc(sizeof(Celula));
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

EABB *cria_vertice(Registro *dados){
	EABB* nova = malloc(sizeof(EABB));
	nova->dir = NULL;
	nova->esq = NULL;
	nova->dados = dados;
	
	return nova;
}

ABB *cria_arvore(){
	ABB* arvore = malloc(sizeof(ABB));
	arvore->raiz = NULL;
	arvore->qtd = 0;

	return arvore;
}

Log *cria_log(Registro *dados, char *operacao) {
  Log *nova = malloc(sizeof(Log));
  nova->anterior = NULL;
  nova->proximo = NULL;
  strncpy(nova->operacao, operacao, TAM);
  nova->dados = dados;
  return nova;
}

Pilha *cria_pilha() {
  Pilha *pilha = malloc(sizeof(Pilha));
  pilha->qtd = 0;
  pilha->top = NULL;
  return pilha;
}

void in_ordem_idade_EABB(EABB *raiz) {
    if(raiz == NULL){
        return;
    }
    in_ordem_idade_EABB(raiz->esq);
    printf("Paciente: %s Idade: %d ", raiz->dados->Nome, raiz->dados->idade);
    in_ordem_idade_EABB(raiz->dir);
}

void in_ordem_dia_EABB(EABB *raiz) {
    if(raiz == NULL){
        return;
    }
    in_ordem_dia_EABB(raiz->esq);
    printf("Paciente: %s Dia: %d ", raiz->dados->Nome, raiz->dados->Entrada->dia);
    in_ordem_dia_EABB(raiz->dir);
}

void in_ordem_mes_EABB(EABB *raiz) {
    if(raiz == NULL){
        return;
    }
    in_ordem_mes_EABB(raiz->esq);
    printf("Paciente: %s Mês: %d ", raiz->dados->Nome, raiz->dados->Entrada->mes);
    in_ordem_mes_EABB(raiz->dir);
}

void in_ordem_ano_EABB(EABB *raiz) {
    if(raiz == NULL){
        return;
    }
    in_ordem_ano_EABB(raiz->esq);
    printf("Paciente: %s Ano: %d ", raiz->dados->Nome, raiz->dados->Entrada->ano);
    in_ordem_ano_EABB(raiz->dir);
}

void in_ordem_ano(ABB *arvore) {
    if (arvore == NULL){
        return;
    }
    in_ordem_ano_EABB(arvore->raiz);
}

void in_ordem_mes(ABB *arvore) {
    if (arvore == NULL){
        return;
    }
    in_ordem_mes_EABB(arvore->raiz);
}

void in_ordem_dia(ABB *arvore) {
    if (arvore == NULL){
        return;
    }
    in_ordem_dia_EABB(arvore->raiz);
}

void in_ordem_idade(ABB *arvore) {
    if (arvore == NULL){
        return;
    }
    in_ordem_idade_EABB(arvore->raiz);
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

void inserirArvoreIdade(ABB* arvore, Registro *dados){
	EABB* nova = cria_vertice(dados);
    if(arvore->raiz == NULL){
        arvore->raiz = nova;
        return;
    }
    EABB* atual = arvore->raiz;
    EABB* pai = NULL;
    while (atual != NULL)
    {
        pai = atual;
        if(dados->idade < atual->dados->idade){
			atual = atual->esq;
		}else if(dados->idade > atual->dados->idade){
			atual = atual->dir;
		}
    }
    if(dados->idade < pai->dados->idade){
        pai->esq = nova;
    }
    else{
        pai->dir = nova;
    }
    arvore->qtd++;
}

void inserirArvoreDia(ABB* arvore, Registro *dados){
	EABB* nova = cria_vertice(dados);
    if(arvore->raiz == NULL){
        arvore->raiz = nova;
        return;
    }
    EABB* atual = arvore->raiz;
    EABB* pai = NULL;
    while (atual != NULL)
    {
        pai = atual;
        if(dados->Entrada->dia < atual->dados->Entrada->dia){
			atual = atual->esq;
		}else if(dados->Entrada->dia > atual->dados->Entrada->dia){
			atual = atual->dir;
		}
    }
    if(dados->Entrada->dia < pai->dados->Entrada->dia){
        pai->esq = nova;
    }
    else{
        pai->dir = nova;
    }
    arvore->qtd++;
}

void inserirArvoreMes(ABB* arvore, Registro *dados){
	EABB* nova = cria_vertice(dados);
    if(arvore->raiz == NULL){
        arvore->raiz = nova;
        return;
    }
    EABB* atual = arvore->raiz;
    EABB* pai = NULL;
    while (atual != NULL)
    {
        pai = atual;
        if(dados->Entrada->mes < atual->dados->Entrada->mes){
			atual = atual->esq;
		}else if(dados->Entrada->mes > atual->dados->Entrada->mes){
			atual = atual->dir;
		}
    }
    if(dados->Entrada->mes < pai->dados->Entrada->mes){
        pai->esq = nova;
    }
    else{
        pai->dir = nova;
    }
    arvore->qtd++;
}

void inserirArvoreAno(ABB* arvore, Registro *dados){
	EABB* nova = cria_vertice(dados);
    if(arvore->raiz == NULL){
        arvore->raiz = nova;
        return;
    }
    EABB* atual = arvore->raiz;
    EABB* pai = NULL;
    while (atual != NULL)
    {
        pai = atual;
        if(dados->Entrada->ano < atual->dados->Entrada->ano){
			atual = atual->esq;
		}else if(dados->Entrada->ano > atual->dados->Entrada->ano){
			atual = atual->dir;
		}
    }
    if(dados->Entrada->ano < pai->dados->Entrada->ano){
        pai->esq = nova;
    }
    else{
        pai->dir = nova;
    }
    arvore->qtd++;
}

void CadastrarPaciente(LDE *lista, ABB *arvoreAno, ABB *arvoreMes, ABB *arvoreDia, ABB *arvoreIdade){
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

    Celula *paciente = criar_celula(nova);
    Celula *atual = lista->primeiro;
    lista->primeiro = paciente;
    paciente->proximo = atual;
    lista->qtd++;
    printf("Paciente cadastrado com sucesso!\n");
    
    inserirArvoreAno(arvoreAno, nova);
    inserirArvoreMes(arvoreMes, nova);
    inserirArvoreDia(arvoreDia, nova);
    inserirArvoreIdade(arvoreIdade, nova);
}

void ConsultarPaciente(LDE *lista){
    char consulta[TAM];
    printf("RG do paciente a ser consultado: ");
    clearBuffer();
    fgets(consulta, sizeof(consulta), stdin);
    consulta[strcspn(consulta, "\n")] = '\0';
    Celula *atual = lista->primeiro;
    while(atual != NULL && strcmp(consulta, atual->dados->RG) != 0){
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Paciente não encontrado.\n");
    }
    else{
        printf("Nome: %s\n", atual->dados->Nome);
        printf("RG: %s\n", atual->dados->RG);
        printf("Idade: %d\n", atual->dados->idade);
        printf("Data de registro: %d/%d/%d\n", atual->dados->Entrada->dia, atual->dados->Entrada->mes, atual->dados->Entrada->ano);
    }
    printf("\n");
}

void listaCompleta(LDE *lista){
    Celula *atual = lista->primeiro;
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
    Celula *atual = lista->primeiro;
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
    Celula *anterior = NULL; 
    Celula *atual = lista->primeiro; 
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

void guardarOperacao(Pilha *pilha, Registro *dados, char *operacao) {
  Log *nova = cria_log(dados, operacao);
  if (pilha->qtd != 0) {
    nova->proximo = pilha->top;
    pilha->top->anterior = nova;
  }
  pilha->top = nova;
  pilha->qtd++;
}

void desfazerOperacao(Pilha *pilha, Fila *queue) {
  if (pilha->qtd > 0) {
    Log *remover = pilha->top;
    pilha->top = pilha->top->proximo;
    if (pilha->top != NULL){
      pilha->top->anterior = NULL;
    }
    pilha->qtd--;
    if(strcmp(remover->operacao, "enfileirar") == 0){
        Celula *atual = queue->head;
        while(atual != NULL && strcmp(remover->dados->RG, atual->dados->RG) != 0){ 
            atual = atual->proximo;
        }
        Celula *aux = queue->head;
        if(queue->qtd == 1){
            queue->head = NULL;
            queue->tail = NULL;
        }
        else{
            queue->head = queue->head->proximo;
        }
        queue->qtd--;
        free(aux);
    }
    else if(strcmp(remover->operacao, "desenfileirar") == 0){
        Celula *nova = criar_celula(remover->dados);
        if(queue->qtd == 0){
            queue->head = nova;
        }
        else{
            queue->tail->proximo = nova;
        }
        queue->tail = nova;
        queue->qtd++;
    }
    free(remover);
  }
  printf("Operação desfeita com sucesso!");
}

void enfilerarPaciente(LDE *lista, Fila *queue, Pilha *pilha){
    char enfilerar[TAM];
    printf("RG do paciente a ser adicionado a fila de atendimento: ");
    clearBuffer();
    fgets(enfilerar, sizeof(enfilerar), stdin);
    enfilerar[strcspn(enfilerar, "\n")] = '\0';
    Celula *atual = lista->primeiro;
    while(atual != NULL && strcmp(enfilerar, atual->dados->RG) != 0){ 
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Paciente não encontrado.\n");
        return;
    }
    Celula *nova = criar_celula(atual->dados);
    if(queue->qtd == 0){
        queue->head = nova;
    }
    else{
        queue->tail->proximo = nova;
    }
    queue->tail = nova;
    queue->qtd++;
    printf("Paciente adicionado a fila de atendimento.\n");
    guardarOperacao(pilha, atual->dados, "enfileirar");
}

void desenfilerarPaciente(Fila *queue, Pilha *pilha){
    char desenfilerar[TAM];
    printf("RG do paciente a ser removido da fila de atendimento: ");
    clearBuffer();
    fgets(desenfilerar, sizeof(desenfilerar), stdin);
    desenfilerar[strcspn(desenfilerar, "\n")] = '\0';
    Celula *atual = queue->head;
    while(atual != NULL && strcmp(desenfilerar, atual->dados->RG) != 0){ 
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Paciente não encontrado.\n");
        return;
    }
    Celula *aux = queue->head;
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
    guardarOperacao(pilha, atual->dados, "desenfileirar");
}

void mostrarFila(Fila *queue){
	Celula *atual = queue->head;
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
    Celula *atual = lista->primeiro;
    while(atual != NULL && strcmp(enfilerarP, atual->dados->RG) != 0){ 
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Paciente não encontrado.\n");
        return;
    }
    Celula *nova = criar_celula(atual->dados);
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
    if(controle == 0){
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

void mostrarOperacao(Pilha *pilha){
    Log *atual = pilha->top;
    while (atual != NULL)
    {
        printf("Paciente: %s  Idade: %d  RG: %s  Operação: %s\n", atual->dados->Nome, atual->dados->idade, atual->dados->RG, atual->operacao);
        atual =  atual->proximo;
    }
    
}

void operacaoSendoRemovida(Pilha *pilha){
    Log *remover = pilha->top;
    printf("Operação a ser desfeita:\n");
    printf("Paciente: %s  Idade: %d  RG: %s  Operação: %s\n", remover->dados->Nome, remover->dados->idade, remover->dados->RG, remover->operacao);
}

void Salvar_paciente(LDE *lista){
  FILE *f = fopen("pacientes.bin", "wb");           
  if (f == NULL){
    printf("Erro ao abrir o arquivo de paciente\n");
    return;
  }
  Celula *atual = lista->primeiro;
  while (atual != NULL)
  {
    int salvarNome = fwrite(atual->dados->Nome, sizeof(char), TAM, f);
    int salvarIdade = fwrite(&(atual->dados->idade), sizeof(int), 1, f);
    int salvarRG = fwrite(atual->dados->RG, sizeof(char), TAM, f);
    int salvarData = fwrite(atual->dados->Entrada, sizeof(Data), 1, f);
    if (salvarNome == 0 || salvarIdade == 0 || salvarRG == 0 || salvarData == 0){
        printf("Erro ao salvar pacientes\n");
        return;
    }
    atual = atual->proximo;
  }
  
  if(fclose(f)){
    return;
  }
  printf("Pacientes salvos!\n");
}

void Carregar_paciente(LDE *lista){
  FILE *f = fopen("pacientes.bin", "rb");
  if (f == NULL){
    printf("Erro ao abrir o arquivo de pacientes\n");
    return;
  }

  Registro *nova;
  
  while (1) {
    nova = malloc(sizeof(Registro));
    nova->Entrada = malloc(sizeof(Data));
    int carregarNome = fread(nova->Nome, sizeof(char), TAM, f);
    int carregarIdade = fread(&(nova->idade), sizeof(int), 1, f);
    int carregarRG = fread(nova->RG, sizeof(char), TAM, f);
    int carregarData = fread(nova->Entrada, sizeof(Data), 1, f);
    if (carregarNome == 0 || carregarIdade == 0 || carregarRG == 0 || carregarData == 0) {
        if (feof(f)) {
            free(nova->Entrada);
            free(nova);
            break;
        }
        else{
            printf("Erro ao carregar pacientes\n");
            free(nova->Entrada);
            free(nova);  
            return;
        }       
    }
        
    Celula *paciente = criar_celula(nova);
    if (lista->primeiro == NULL) {
        lista->primeiro = paciente;
    } 
    else {
        Celula *atual = lista->primeiro;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = paciente;
    }
    lista->qtd++;
  }

  if (fclose(f)){
    return;
  }
  printf("Pacientes carregados!\n");
}

int main(void){
    LDE *lista = criar_lista();
    Fila *fila = cria_queue();
    heap *h = malloc(sizeof(heap));
    ABB *arvoreAno = cria_arvore();
    ABB *arvoreMes = cria_arvore();
    ABB *arvoreDia = cria_arvore();
    ABB *arvoreIdade = cria_arvore();
    Pilha *pilha = cria_pilha();
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
                    CadastrarPaciente(lista, arvoreAno, arvoreMes, arvoreDia, arvoreIdade);
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
                    enfilerarPaciente(lista, fila, pilha);
                }
                else if(opcao == 2){
                    desenfilerarPaciente(fila, pilha);
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
        else if(opcao == 4){
            while (1)
            {
                printf("\nMenu de Pesquisa\n");
                printf("1 - Mostrar registros ordenados por ano de registro\n");
                printf("2 - Mostrar registros ordenados por dia de registro \n");
                printf("3 - Mostrar registros ordenados por mês de registro\n");
                printf("4 - Mostrar registros ordenados por idade do paciente \n");
                printf("0 - Retornar ao menu principal \n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);
                if(opcao == 1){
                    in_ordem_ano(arvoreAno);
                }
                else if(opcao == 2){
                    in_ordem_mes(arvoreDia);
                }
                else if(opcao == 3){
                    in_ordem_dia(arvoreMes);
                }
                else if(opcao == 4){
                    in_ordem_idade(arvoreIdade);
                }
                else if(opcao == 0){
                    break;
                }
                else{
                    printf("Opção inválida\n");
                }
            }
            
        }
        else if(opcao == 5){
            while(1){
                printf("\nMenu de Desfazer\n");
                printf("1 - Mostrar operações realizadas na fila de atendimento\n");
                printf("2 - Defazer a ultima operação realizada na fila de atendimento\n");
                printf("0 - Retornar ao menu principal \n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);
                if(opcao == 1){
                    mostrarOperacao(pilha);
                }
                else if(opcao == 2){
                    operacaoSendoRemovida(pilha);
                    printf("Deseja realmente desfazer a operação?\n");
                    printf("1 - Sim\n");
                    printf("2 - Não\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcao);
                    if(opcao == 1){
                        desfazerOperacao(pilha, fila);
                    }
                    else if(opcao == 2){
                        break;
                    }
                    else{
                        printf("Opção inválida\n");
                    }
                }
                else if(opcao == 0){
                    break;
                }
                else{
                    printf("Opção inválida\n");
                }
            }
        }
        else if(opcao == 6){
            Salvar_paciente(lista);
        }
        else if(opcao == 7){
            Carregar_paciente(lista);
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