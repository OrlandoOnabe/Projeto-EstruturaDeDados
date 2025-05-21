#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 100 //Tamanho para string (RG e nome)
#define LEN 20 //Tamanho máximo da fila prioritária


typedef struct Data //struct com a data de registro com dia, mês e ano
{
    int dia;
    int mes;
    int ano;
}Data;

typedef struct Registro //struct com o registro do paciente
{
    char Nome[TAM]; //nome do paciente
    int idade; //idade do paciente
    char RG[TAM]; //RG do paciente
    Data *Entrada; //ponteiro do struct Data para a data de registro
}Registro;


typedef struct Celula { //struct para as celulas da lista de pacientes e fila de atendimento
    Registro *dados; //ponteiro para os dados do registro do paciente
    struct Celula *proximo; //ponteiro para o proximo da celula
} Celula;

typedef struct EABB{ //struct dos vertices da arvore binário de busca
	Registro *dados; //ponteiro para os dados do registro do paciente
	struct EABB* esq; //filho esquerdo
	struct EABB* dir; //filho direito
} EABB;

typedef struct { //lista de pacientes
    Celula *primeiro; //aponta para o primeiro da lista
    int qtd; 
} LDE;

typedef struct { //fila de atendimento
	Celula *head; //inicio
	Celula *tail; //fim
	int qtd; 
} Fila;

typedef struct { //heap para fila prioritária
  Registro *dados[LEN]; //ponteiro para os dados do registro do paciente com no máximo 20 para a fila prioritária
  int qtd;
} heap;

typedef struct ABB{ //arvore binária de busca para pesquisa
	EABB* raiz; //raiz da arvore
	int qtd;
} ABB;

typedef struct Log { //Log para guardas as operações feitas na fila de atendimento
  struct Log *anterior; //ponteiro para Log anterior
  struct Log *proximo; //ponteiro para proximo Log
  char operacao[TAM]; //Tipo da operação (enfileirar/desenfileirar)
  Registro *dados; //ponteiro para os dados do paciente
} Log;

typedef struct { //Pilha para guardar operações na fila de atendimento
  Log *top; //ponteiro para o topo da pilha
  int qtd;
} Pilha;


void clearBuffer(){ //função para limpara o buffer do teclado
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Celula *criar_celula(Registro *dados){ //função para criar celula
    Celula *nova = malloc(sizeof(Celula));
    nova->proximo = NULL; //ponteiro proximo começa em nulo
    nova->dados = dados; //ponteiro dados aponta para os dados do registro do paciente colocados como parametro ao chamar a função
    return nova;
}

LDE *criar_lista(){ //função para criar a lista de pacientes
    LDE *lista = malloc(sizeof(LDE));
    lista->primeiro = NULL; //ponteiro primeiro começa em nulo
    lista->qtd = 0;
    return lista;
}

Fila *cria_queue(){ //função para criar a fila de atendimento 
	Fila *queue = malloc(sizeof(Fila));
	queue->head = NULL; //inicio começa em nulo
	queue->tail = NULL; //fim começa em nulo
	queue->qtd = 0;

	return queue;
}

EABB *cria_vertice(Registro *dados){ //função para criar as vertices da arvore binária de busca
	EABB* nova = malloc(sizeof(EABB));
	nova->dir = NULL; //filho direito e esquerdo começam em nulo
	nova->esq = NULL;
	nova->dados = dados; //ponteiro dados aponta para os dados do registro do paciente colocados como parametro ao chamar a função
	
	return nova;
}

ABB *cria_arvore(){ //função para criar a arvore binária de busca para pesquisa
	ABB* arvore = malloc(sizeof(ABB));
	arvore->raiz = NULL; //raiz começa em nulo
	arvore->qtd = 0;

	return arvore;
}

Log *cria_log(Registro *dados, char *operacao) { //função para criar a log para guardar a operação feita na fila de atendimento
  Log *nova = malloc(sizeof(Log));
  nova->anterior = NULL; //ponteiro anterior e proximo começam em nulo
  nova->proximo = NULL;
  strncpy(nova->operacao, operacao, TAM); //tipo da função será o char colocado como parametro ao chamar a função (enfileirar/desenfileirar)
  nova->dados = dados; //ponteiro dados aponta para os dados do registro do paciente colocados como parametro ao chamar a função
  return nova;
}

Pilha *cria_pilha() { //função para criar a pilha ara guardar as logs de operações
  Pilha *pilha = malloc(sizeof(Pilha));
  pilha->qtd = 0;
  pilha->top = NULL; //ponteiro topo começa em nulo
  return pilha;
}

void in_ordem_idade_EABB(EABB *raiz) { //percorre os vertices da arvores de forma in_ordem, ordenada pela idade do paciente.
    if(raiz == NULL){
        return;
    }
    in_ordem_idade_EABB(raiz->esq); //faz in_ordem do filho esquerdo
    printf("Paciente: %s Idade: %d\n", raiz->dados->Nome, raiz->dados->idade); //mostra nome e idade
    in_ordem_idade_EABB(raiz->dir);//faz in_ordem do filho direito
}

void in_ordem_dia_EABB(EABB *raiz) { //percorre os vertices da arvores de forma in_ordem, ordenada pelo dia de entrada do registro.
    if(raiz == NULL){
        return;
    }
    in_ordem_dia_EABB(raiz->esq); //faz in_ordem do filho esquerdo
    printf("Paciente: %s Dia: %d\n", raiz->dados->Nome, raiz->dados->Entrada->dia); //mostra nome e dia
    in_ordem_dia_EABB(raiz->dir);//faz in_ordem do filho direito
}

void in_ordem_mes_EABB(EABB *raiz) { //percorre os vertices da arvores de forma in_ordem, ordenada pelo mes de entrada do registro.
    if(raiz == NULL){
        return;
    }
    in_ordem_mes_EABB(raiz->esq); //faz in_ordem do filho esquerdo
    printf("Paciente: %s Mês: %d\n", raiz->dados->Nome, raiz->dados->Entrada->mes); //mostra nome e mes
    in_ordem_mes_EABB(raiz->dir); //faz in_ordem do filho direito
}

void in_ordem_ano_EABB(EABB *raiz) { //percorre os vertices da arvores de forma in_ordem, ordenada pelo ano de entrada do registro.
    if(raiz == NULL){
        return;
    }
    in_ordem_ano_EABB(raiz->esq); //faz in_ordem do filho esquerdo
    printf("Paciente: %s Ano: %d\n", raiz->dados->Nome, raiz->dados->Entrada->ano); //mostra nome e ano
    in_ordem_ano_EABB(raiz->dir); //faz in_ordem do filho direito
}

void in_ordem_ano(ABB *arvore) { //função para chamar a mostragem in_ordem de cada arvore na main
    if (arvore == NULL){
        return;
    }
    in_ordem_ano_EABB(arvore->raiz);
}

void in_ordem_mes(ABB *arvore) { //função para chamar a mostragem in_ordem de cada arvore na main
    if (arvore == NULL){
        return;
    }
    in_ordem_mes_EABB(arvore->raiz);
}

void in_ordem_dia(ABB *arvore) { //função para chamar a mostragem in_ordem de cada arvore na main
    if (arvore == NULL){
        return;
    }
    in_ordem_dia_EABB(arvore->raiz);
}

void in_ordem_idade(ABB *arvore) { //função para chamar a mostragem in_ordem de cada arvore na main
    if (arvore == NULL){
        return;
    }
    in_ordem_idade_EABB(arvore->raiz);
}

int filho_esq(int pai) { //filho esquerdo no heap
    return 2 * pai + 1;
}

int filho_dir(int pai) {  //filho direito no heap
    return 2 * pai + 2;
}

int pai(int filho) { //pai no heap
    return (filho - 1) / 2;
}

int ultimo_pai(heap *h) { //ultimo pai do heap
    return (h->qtd / 2) - 1;
}

void inserirArvoreIdade(ABB* arvore, Registro *dados){ //inserir paciente na arvore de busca ordenada por idade
	EABB* nova = cria_vertice(dados); //cria o novo vertice com os dados do registro do paciente
    if(arvore->raiz == NULL){ //se a arvore estava vazia, o novo vertice vira a nova raiz
        arvore->raiz = nova;
        return;
    }
    EABB* atual = arvore->raiz; //atual começa na raiz para percorrer a arvore
    EABB* pai = NULL; //pai começa nulo
    while (atual != NULL) //percorre a arvore
    {
        pai = atual;
        if(dados->idade < atual->dados->idade){ //se a idade do novo vertice for menor que a do atual ele vai para o filho esquerdo
			atual = atual->esq;
		}else{ //senão vai para o filho direito
			atual = atual->dir;
		}
    }
    if(dados->idade < pai->dados->idade){ //se idade do novo vertice for menor que a idade do pai apos o fim do looping
        pai->esq = nova; //novo vertice vira filho esquerdo do pai
    }
    else{
        pai->dir = nova; //senão vira filho direito
    }
    arvore->qtd++;
}

void inserirArvoreDia(ABB* arvore, Registro *dados){ //inserir paciente na arvore de busca ordenada por dia
	EABB* nova = cria_vertice(dados); //cria o novo vertice com os dados do registro do paciente
    if(arvore->raiz == NULL){//se a arvore estava vazia, o novo vertice vira a nova raiz
        arvore->raiz = nova;
        return;
    }
    EABB* atual = arvore->raiz; //atual começa na raiz para percorrer a arvore
    EABB* pai = NULL; //pai começa nulo
    while (atual != NULL) //percorre a arvore
    {
        pai = atual;
        if(dados->Entrada->dia < atual->dados->Entrada->dia){ //se o dia do novo vertice for menor que o do atual ele vai para o filho esquerdo
			atual = atual->esq;
		}else{ //senão vai para o filho direito
			atual = atual->dir;
		}
    }
    if(dados->Entrada->dia < pai->dados->Entrada->dia){ //se dia do novo vertice for menor que o dia do pai apos o fim do looping
        pai->esq = nova; //novo vertice vira filho esquerdo do pai
    }
    else{
        pai->dir = nova; //senão vira filho direito
    }
    arvore->qtd++;
}

void inserirArvoreMes(ABB* arvore, Registro *dados){ //inserir paciente na arvore de busca ordenada por mes
	EABB* nova = cria_vertice(dados);//cria o novo vertice com os dados do registro do paciente
    if(arvore->raiz == NULL){//se a arvore estava vazia, o novo vertice vira a nova raiz
        arvore->raiz = nova;
        return;
    }
    EABB* atual = arvore->raiz;//atual começa na raiz para percorrer a arvore
    EABB* pai = NULL;//pai começa nulo
    while (atual != NULL)//percorre a arvore
    {
        pai = atual;
        if(dados->Entrada->mes < atual->dados->Entrada->mes){//se o mes do novo vertice for menor que o do atual ele vai para o filho esquerdo
			atual = atual->esq;
		}else{//senão vai para o filho direito
			atual = atual->dir;
		}
    }
    if(dados->Entrada->mes < pai->dados->Entrada->mes){//se mes do novo vertice for menor que o mes do pai apos o fim do looping
        pai->esq = nova;//novo vertice vira filho esquerdo do pai
    }
    else{
        pai->dir = nova;//senão vira filho direito
    }
    arvore->qtd++;
}

void inserirArvoreAno(ABB* arvore, Registro *dados){ //inserir paciente na arvore de busca ordenada por ano
	EABB* nova = cria_vertice(dados);//cria o novo vertice com os dados do registro do paciente
    if(arvore->raiz == NULL){//se a arvore estava vazia, o novo vertice vira a nova raiz
        arvore->raiz = nova;
        return;
    }
    EABB* atual = arvore->raiz;//atual começa na raiz para percorrer a arvore
    EABB* pai = NULL;//pai começa nulo
    while (atual != NULL)//percorre a arvore
    {
        pai = atual;
        if(dados->Entrada->ano < atual->dados->Entrada->ano){//se o ano do novo vertice for menor que o do atual ele vai para o filho esquerdo
			atual = atual->esq;
		}else{//senão vai para o filho direito
			atual = atual->dir;
		}
    }
    if(dados->Entrada->ano < pai->dados->Entrada->ano){//se ano do novo vertice for menor que o ano do pai apos o fim do looping
        pai->esq = nova;//novo vertice vira filho esquerdo do pai
    }
    else{
        pai->dir = nova;//senão vira filho direito
    }
    arvore->qtd++;
}

void CadastrarPaciente(LDE *lista, ABB *arvoreAno, ABB *arvoreMes, ABB *arvoreDia, ABB *arvoreIdade){ //cadastrar novos pacientes
    Registro *nova = malloc(sizeof(Registro)); //cria um novo registro para guardar os dados do novo paciente
    nova->Entrada = malloc(sizeof(Data)); //cria um novo Data para o ponteiro Entrada

    printf("Nome: ");
    clearBuffer();
    fgets(nova->Nome, sizeof(nova->Nome), stdin); //pega o nome do novo paciente
    nova->Nome[strcspn(nova->Nome, "\n")] = '\0';

    printf("Idade: ");
    scanf("%d", &nova->idade);//pega a idade do novo paciente

    printf("RG: ");
    clearBuffer();
    fgets(nova->RG, sizeof(nova->RG), stdin); //pega o RG do novo paciente
    nova->RG[strcspn(nova->RG, "\n")] = '\0';

    printf("Data do registro: ");
    scanf("%d %d %d", &nova->Entrada->dia, &nova->Entrada->mes, &nova->Entrada->ano); //pega a data de registrado entrada

    Celula *paciente = criar_celula(nova); //cria uma nova celula para a lista de pacientes que contem os dados do novo paciente
    //faz a inserção em ordem de registro, inserção no inicio da lista
    Celula *atual = lista->primeiro; 
    lista->primeiro = paciente;
    paciente->proximo = atual;
    lista->qtd++;
    printf("Paciente cadastrado com sucesso!\n");
    //insere o novo paciente nas arvores binárias de busca
    inserirArvoreAno(arvoreAno, nova);
    inserirArvoreMes(arvoreMes, nova);
    inserirArvoreDia(arvoreDia, nova);
    inserirArvoreIdade(arvoreIdade, nova);
}

void ConsultarPaciente(LDE *lista){ //consultar os dados de um paciente da lista
    char consulta[TAM]; //variavel para guardar o RG que será procurado na lista
    printf("RG do paciente a ser consultado: ");
    clearBuffer();
    fgets(consulta, sizeof(consulta), stdin); //pega o RG a ser consultado
    consulta[strcspn(consulta, "\n")] = '\0';
    Celula *atual = lista->primeiro; //atual começa no primeiro para percorrer a lista
    while(atual != NULL && strcmp(consulta, atual->dados->RG) != 0){ //percorre a lista até achar o RG a ser consultado
        atual = atual->proximo;
    }
    if(atual == NULL){ //se apos percorrer atual terminar nulo o RG não foi encontrado na lista
        printf("Paciente não encontrado.\n");
    }
    else{ //senão os dados do paciente são mostrados
        printf("Nome: %s\n", atual->dados->Nome);
        printf("RG: %s\n", atual->dados->RG);
        printf("Idade: %d\n", atual->dados->idade);
        printf("Data de registro: %d/%d/%d\n", atual->dados->Entrada->dia, atual->dados->Entrada->mes, atual->dados->Entrada->ano);
    }
    printf("\n");
}

void listaCompleta(LDE *lista){ //mostra a lista completa de pacientes
    Celula *atual = lista->primeiro;//atual começa no primeiro para percorrer a lista
    while(atual != NULL){ //percorre a lista mostrando os dados de cada paciente
        printf("Nome: %s\n", atual->dados->Nome);
        printf("RG: %s\n", atual->dados->RG);
        printf("Idade: %d\n", atual->dados->idade);
        printf("Data de registro: %d/%d/%d\n", atual->dados->Entrada->dia, atual->dados->Entrada->mes, atual->dados->Entrada->ano);
        atual = atual->proximo;
    }
    printf("\n");
}

void atualizarPaciente(LDE *lista){ //atualiza informaçoes de um paciente
    char atualizar[TAM]; //variavel para guardar o RG que será procurado na lista
    printf("RG do paciente a ser consultado: ");
    clearBuffer();
    fgets(atualizar, sizeof(atualizar), stdin); //pega o RG do paciente a ser atualizado
    atualizar[strcspn(atualizar, "\n")] = '\0';
    Celula *atual = lista->primeiro; //atual começa no primeiro para percorrer a lista
    while(atual != NULL && strcmp(atualizar, atual->dados->RG) != 0){  //percorre a lista até achar o RG do paciente a ser atualizado
        atual = atual->proximo;
    }
    if(atual == NULL){ //se apos percorrer atual terminar nulo o RG não foi encontrado na lista
        printf("Paciente não encontrado.\n");
        return;
    }
    //senão os dados do atual são atualizados
    printf("Novo nome: ");
    fgets(atual->dados->Nome, sizeof(atual->dados->Nome), stdin);
    atual->dados->Nome[strcspn(atual->dados->Nome, "\n")] = '\0';
    printf("Nova idade: ");
    scanf("%d", &atual->dados->idade);
    printf("Paciente atualizado com sucesso!\n");
    
}

void removerPaciente(LDE *lista){ //remover um paciente da lista de pacientes
    char remover[TAM]; //variavel para guardar o RG que será procurado na lista
    printf("RG do paciente a ser removido: ");
    clearBuffer();
    fgets(remover, sizeof(remover), stdin);//guarda o RG a ser removido da lista
    remover[strcspn(remover, "\n")] = '\0';
    Celula *anterior = NULL; //anterior começa em nulo
    Celula *atual = lista->primeiro; //atual começa no primeiro para percorrer a lista
    while(atual != NULL && strcmp(remover, atual->dados->RG) != 0){ //percorre a lista até encontrar o RG a ser removido
        anterior = atual;
        atual = atual->proximo;
    }
    if(atual == NULL){//se apos percorrer atual terminar nulo o RG não foi encontrado na lista
        printf("Paciente não encontrado.\n");
        return;
    }
    if(anterior == NULL){//senão verifica se o RG a ser removido e o primeiro da lista e ser for faz o ponteiro primeiro apontar para o proximo
        lista->primeiro = atual->proximo;
    } else {
        anterior->proximo = atual->proximo; //senão o anterior proximo aponta para o proximo
    }
    free(atual);
    lista->qtd--;
    printf("Paciente removido com sucesso!\n");
}

void guardarOperacao(Pilha *pilha, Registro *dados, char *operacao) { //guarda a operação realizada na fila de atendimento
  Log *nova = cria_log(dados, operacao); //cria um novo log com os dados do paciente e o tipo de operação realizada
  if (pilha->qtd != 0) { //se a pilha não estava vazia
    nova->proximo = pilha->top; //o proximo do novo log aponta para o topo e o anterior do topo aponta para o novo log
    pilha->top->anterior = nova;
  }
  pilha->top = nova; //novo log virá o novo topo da pilha
  pilha->qtd++;
}

void desfazerOperacao(Pilha *pilha, Fila *queue) { //desfaz a operação realizada na fila de atendimento
  if (pilha->qtd > 0) { //se a pilha não está vazia
    Log *remover = pilha->top; //pega o log no topo da pilha para remove-lo
    pilha->top = pilha->top->proximo; // o topo aponta para o proximo do antigo topo
    if (pilha->top != NULL){ //se o novo topo não é nulo
      pilha->top->anterior = NULL; //o anterior do novo topo aponta para nulo
    }
    pilha->qtd--;
    if(strcmp(remover->operacao, "enfileirar") == 0){ //se a operação a ser removida era um enfileirar ele desenfilera o paciente da fila de atendimento
        Celula *atual = queue->head; //atual começa no inicio da fila para percorre-la
        while(atual != NULL && strcmp(remover->dados->RG, atual->dados->RG) != 0){  //percorre a fila até achar o RG do paciente a ser desenfileirado
            atual = atual->proximo;
        }
        Celula *aux = queue->head; //auxiliar com o inicio atual da fila
        if(queue->qtd == 1){ //se só havia um paciente na fila o incio e fim apontam para nulo
            queue->head = NULL;
            queue->tail = NULL;
        }
        else{ //senão o inicio passa a apontar para o proximo do antigo inicio
            queue->head = queue->head->proximo;
        }
        queue->qtd--;
        free(aux);
    }
    else if(strcmp(remover->operacao, "desenfileirar") == 0){ //se a operação a ser removida era um desenfilerr ele enfileira o paciente da fila de atendimento
        Celula *nova = criar_celula(remover->dados); //cria a nova celula para ser enfileirada na fila de atendimento
        if(queue->qtd == 0){ //se a fila estava vazia a nova celula virá o inicio
            queue->head = nova;
        }
        else{ //senão ela é adicionada ao final da fila
            queue->tail->proximo = nova;
        }
        queue->tail = nova; //nova celula se torna o novo final da fila 
        queue->qtd++;
    }
    free(remover);
  }
  printf("Operação desfeita com sucesso!");
}

void enfilerarPaciente(LDE *lista, Fila *queue, Pilha *pilha){ //enfileirar pacientes na fila de atendimento
    char enfilerar[TAM]; //variavel para guardar o RG que sera procurado na fila de pacientes
    printf("RG do paciente a ser adicionado a fila de atendimento: ");
    clearBuffer();
    fgets(enfilerar, sizeof(enfilerar), stdin);//guarda o RG a ser enfileirado
    enfilerar[strcspn(enfilerar, "\n")] = '\0';
    Celula *atual = lista->primeiro; //atual começa no primeiro da lista para percorrer
    while(atual != NULL && strcmp(enfilerar, atual->dados->RG) != 0){ //percorre a lista até achar o RG a ser enfileirado
        atual = atual->proximo;
    }
    if(atual == NULL){ //se ao final de percorrer o atual for nulo o RG não foi encontrado na lista
        printf("Paciente não encontrado.\n");
        return;
    }
    Celula *nova = criar_celula(atual->dados); //senão cria a nova celula para a fila com os dados do paciente
    if(queue->qtd == 0){//se a fila estava vazia a nova celula se torna o inicio
        queue->head = nova;
    }
    else{//senão ela é adicionada ao final da fila
        queue->tail->proximo = nova;
    }
    queue->tail = nova; //nova celula se torna o novo final da fila 
    queue->qtd++;
    printf("Paciente adicionado a fila de atendimento.\n");
    guardarOperacao(pilha, atual->dados, "enfileirar"); //guarda a operação realizada
}

void desenfilerarPaciente(Fila *queue, Pilha *pilha){//desenfileirar pacientes na fila de atendimento
    char desenfilerar[TAM]; //variavel para guardar o RG que sera procurado na fila de pacientes
    printf("RG do paciente a ser removido da fila de atendimento: ");
    clearBuffer();
    fgets(desenfilerar, sizeof(desenfilerar), stdin); //guarda o RG a ser desenfileirado
    desenfilerar[strcspn(desenfilerar, "\n")] = '\0';
    Celula *atual = queue->head; //atual começa no inico da fila para percorrer
    while(atual != NULL && strcmp(desenfilerar, atual->dados->RG) != 0){ //percorre a fila até achar o RG a ser desenfileirado
        atual = atual->proximo;
    }
    if(atual == NULL){ //se ao final de percorrer o atual for nulo o RG não foi encontrado na lista
        printf("Paciente não encontrado.\n");
        return;
    }
    Celula *aux = queue->head;//auxiliar com o inicio atual da fila
    if(queue->qtd == 1){ //se só havia um paciente na fila o inicio e fim passam a apontar para nulo
        queue->head = NULL;
        queue->tail = NULL;
    }
    else{ //senão o inicio passa a apontar para o proximo do antigo inicio
        queue->head = queue->head->proximo;
    }
    queue->qtd--;
    free(aux);
    printf("Paciente removido da fila de atendimento.\n");
    guardarOperacao(pilha, atual->dados, "desenfileirar"); //guarda a operação realizada
}

void mostrarFila(Fila *queue){ //mostra a fila de atendimento
	Celula *atual = queue->head; //atual começa no inico da fila para percorrer
    int pos = 1; //posição do paciente na fila
    while(atual != NULL){ //percorre a fila mostrando os dados dos pacientes enfileirados e sua posição na fila
        printf("Posição: %d\n", pos);
        printf("Nome: %s\n", atual->dados->Nome);
        printf("RG: %s\n", atual->dados->RG);
        printf("Idade: %d\n", atual->dados->idade);
        printf("Data de registro: %d/%d/%d\n", atual->dados->Entrada->dia, atual->dados->Entrada->mes, atual->dados->Entrada->ano);
        pos++;
        atual = atual->proximo;
    }
}

void peneirar(heap *h, int pai) { //peneirar o heap para ser de máximo idade
    int fe = filho_esq(pai); //filho esquerdo
    int fd = filho_dir(pai); //filho direito
    int maior = pai; //inicialmente o maior é o pai

    if(fe < h->qtd && h->dados[fe]->idade > h->dados[maior]->idade){ //se a idade do filho esquerdo for maior que a do pai
        maior = fe; //filho esquerdo passa a ser o maior
    }
    if(fd < h->qtd && h->dados[fd]->idade > h->dados[maior]->idade){ //se a idade do filho direito for maior que a do pai
        maior = fd; //filho direito passa a ser o maior
    }

    if(pai != maior){ //se o pai não for mais o maior
        Registro *aux = h->dados[pai]; //auxiliar com os dados do pai atual
        h->dados[pai] = h->dados[maior]; //filho com maior idade sobe para pai
        h->dados[maior] = aux; //antigo pai vai para a antiga posição do filho que subiu
        peneirar(h, maior); //peneira com o maior
    }

    //para melhor vizualização, feito para que entre os filhos o com maior idade sempre esteja a direita, 
    //assim a função mostrar estará com as idades em forma crescente corretamente, 
    //sem acontecer de ele mostrar o filho esquerdo primeiro e ele ter uma idade maior que o filho direito
    fe = filho_esq(pai);
    fd = filho_dir(pai); 
    if (fd < h->qtd && fe < h->qtd && h->dados[fd]->idade > h->dados[fe]->idade) { //se fiho direito tem maior idade que filho esquerdo
        Registro *aux = h->dados[fe]; //aurxiliar com o filho esquerdo
        h->dados[fe] = h->dados[fd]; //move filho direito para a posição do esquerdo
        h->dados[fd] = aux; //coloca o esquerdo na posição do direito
    }
}

void construir(heap *h) { //aplica o peneiramento no heap para ser de máximo idade
    for(int i = ultimo_pai(h); i >= 0; i--){ //do ultimo até a raiz
        peneirar(h, i);
    }
}

void enfilerarPrioritario(heap *h, LDE *lista) { //enfileirar no heap de fila de prioridade
    if(h->qtd >= LEN){ //se a fila ja tem 20 paciente ela esta cheia
        printf("Fila de prioridade cheia!!!");
        return;
    }
    char enfilerarP[TAM]; //variavel para guardar o RG a ser procurado na lista
    printf("RG do paciente a ser adicionado a fila de atendimento prioritário: ");
    clearBuffer();
    fgets(enfilerarP, sizeof(enfilerarP), stdin); //guarda o RG a ser enfileira na fila de prioridade
    enfilerarP[strcspn(enfilerarP, "\n")] = '\0';
    Celula *atual = lista->primeiro; //atual começa no primeiro para percorrer a lista
    while(atual != NULL && strcmp(enfilerarP, atual->dados->RG) != 0){ //percorre a lista até achar o RG a ser enfileirado
        atual = atual->proximo;
    }
    if(atual == NULL){ //se apos percorrer o atual for nulo o RG não foi encontrado
        printf("Paciente não encontrado.\n");
        return;
    }
    Celula *nova = criar_celula(atual->dados); 
    h->dados[h->qtd] = atual->dados; // adiciona os dados do paciente na próxima posição do heap
    h->qtd++;
    printf("Paciente adicionado a fila de atendimento prioritario.\n");
    construir(h); //chama o construir para manter a ordem por máximo de idade
}

void desenfilerarPrioritario(heap *h) { //desenfileirar no heap de fila de prioridade
    if(h->qtd == 0){ //se a fila está vazia a remoção não ocorre
        printf("Fila de prioridade vazia!!!");
        return;
    }
    char desenfilerarP[TAM]; //variavel para guardar o RG a ser procurado na lista
    printf("RG do paciente a ser removido da fila de atendimento prioritário: ");
    clearBuffer();
    fgets(desenfilerarP, sizeof(desenfilerarP), stdin); //guarda o RG a ser desenfileirado
    desenfilerarP[strcspn(desenfilerarP, "\n")] = '\0';
    int deletar; //indice do paciente a ser deletado
    int controle = 0; //variavel para saber se o paciente foi encontrado
    for(int i = 0; i < h->qtd; i++){ 
        if(strcmp(h->dados[i]->RG, desenfilerarP) == 0){ //percorre o heap até encontrar o RG a ser desenfileirado
            deletar = i; //guarda o indice do paciente a ser desenfileirado
            controle = 1;
            break;
        }
    }
    if(controle == 0){ //se apos percorrer o heap o controle continuar em 0 o RG não foi encontrado
        printf("Paciente não encontrado.\n");
        return;
    }
    h->dados[deletar] = h->dados[h->qtd - 1]; //substitui o paciente no indice deletar pelo ultimo do heap
    h->qtd--;
    printf("Paciente removido da fila de atendimento prioritario.\n");
    construir(h); //chama o construir para manter a ordem por máximo de idade
}

void mostrarPrioritario(heap *h) { //mostra a fila prioritario
    int pos = 1; //posição do paciente na fila
    for(int i = 0; i < h->qtd; i++){ //percorre o heap mostrando os dados dos pacientes e sua posição na fila
        printf("Posição: %d\n", pos);
        printf("Nome: %s\n", h->dados[i]->Nome);
        printf("RG: %s\n", h->dados[i]->RG);
        printf("Idade: %d\n", h->dados[i]->idade);
        printf("Data de registro: %d/%d/%d\n", h->dados[i]->Entrada->dia, h->dados[i]->Entrada->mes, h->dados[i]->Entrada->ano);
        pos++;
    }
    printf("\n");
}

void mostrarOperacao(Pilha *pilha){ //mostra os logs de operações na fila de atendimento
    Log *atual = pilha->top; //atual começa no topo da pilha para percorrer
    while (atual != NULL) //percorre a pilha mostrando as logs com os dados do paciente e o tipo de operação
    {
        printf("Paciente: %s  Idade: %d  RG: %s  Operação: %s\n", atual->dados->Nome, atual->dados->idade, atual->dados->RG, atual->operacao);
        atual =  atual->proximo;
    }
    
}

void operacaoSendoRemovida(Pilha *pilha){ //mostra a operação que está no topo da pilha e será removida
    Log *remover = pilha->top;
    printf("Operação a ser desfeita:\n");
    printf("Paciente: %s  Idade: %d  RG: %s  Operação: %s\n", remover->dados->Nome, remover->dados->idade, remover->dados->RG, remover->operacao);
}

void Salvar_paciente(LDE *lista){ //salvar pacientes em um arquivo binario
  FILE *f = fopen("pacientes.bin", "wb"); //abre arquivo para escrita           
  if (f == NULL){ //verifica se abriu
    printf("Erro ao abrir o arquivo de paciente\n");
    return;
  }
  Celula *atual = lista->primeiro; //atual começa no primeiro para percorrer a lista de pacientes
  while (atual != NULL) //percorre a lista de pacientes 
  {
    //escreve os dados dos pacientes da lista de pacientes no arquivo
    int salvarNome = fwrite(atual->dados->Nome, sizeof(char), TAM, f);
    int salvarIdade = fwrite(&(atual->dados->idade), sizeof(int), 1, f);
    int salvarRG = fwrite(atual->dados->RG, sizeof(char), TAM, f);
    int salvarData = fwrite(atual->dados->Entrada, sizeof(Data), 1, f);
    if (salvarNome == 0 || salvarIdade == 0 || salvarRG == 0 || salvarData == 0){ //verifica se alguma escrita falhou
        printf("Erro ao salvar pacientes\n");
        return;
    }
    atual = atual->proximo;
  }
  
  if(fclose(f)){ //verifica se da erro ao fechar o arquivo
    return;
  }
  printf("Pacientes salvos!\n");
}

void Carregar_paciente(LDE *lista, ABB *arvoreAno, ABB *arvoreMes, ABB *arvoreDia, ABB *arvoreIdade){ //carrega os pacientes do arquivo binário para a lista
  FILE *f = fopen("pacientes.bin", "rb"); //abre arquivo para leitura
  if (f == NULL){ //verifica se abriu
    printf("Erro ao abrir o arquivo de pacientes\n");
    return;
  }

  Registro *nova;
    
  while (1) {
    nova = malloc(sizeof(Registro)); //cria um novo registro para carregar os dados do paciente
    nova->Entrada = malloc(sizeof(Data)); //cria um novo Data para o ponteiro Entrada
    //faz a leitura dos dados do paciente do arquivo
    int carregarNome = fread(nova->Nome, sizeof(char), TAM, f);
    int carregarIdade = fread(&(nova->idade), sizeof(int), 1, f);
    int carregarRG = fread(nova->RG, sizeof(char), TAM, f);
    int carregarData = fread(nova->Entrada, sizeof(Data), 1, f);
    if (carregarNome == 0 || carregarIdade == 0 || carregarRG == 0 || carregarData == 0) { //verifica se alguma leitura falhou
        if (feof(f)) { //se ja esta no fim do arquivo não há nenhum erro
            free(nova->Entrada);
            free(nova);
            break;
        }
        else{ //senão houve erro na leitura
            printf("Erro ao carregar pacientes\n");
            free(nova->Entrada);
            free(nova);  
            return;
        }       
    }
    
    //faz uma inversão para que a mostragem da lista continue em ordem de inserção, e não na ordem em que ele lê do arquivo que estaria ao contrário
    Celula *paciente = criar_celula(nova); //cria a celula com os dados do paciente carregados
    paciente->proximo = NULL;
    if (lista->primeiro == NULL) { //se a lista está vazia
        lista->primeiro = paciente; //a celula é o primeiro da lista
    } 
    else { 
        Celula *atual = lista->primeiro; //senão percorre até o final da lista
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = paciente; //insere a celula ao final da lista para manter a ordem de inserção
    }
    lista->qtd++;
    inserirArvoreAno(arvoreAno, nova);
    inserirArvoreMes(arvoreMes, nova);
    inserirArvoreDia(arvoreDia, nova);
    inserirArvoreIdade(arvoreIdade, nova);
  }

  if (fclose(f)){ //verifica se da erro ao fechar o arquivo
    return;
  }
  printf("Pacientes carregados!\n");
}

int main(void){
    //criação da lista, fila, heap, arvores de busca e pilha para o funcionamento do projeto
    LDE *lista = criar_lista();
    Fila *fila = cria_queue();
    heap *h = malloc(sizeof(heap));
    ABB *arvoreAno = cria_arvore();
    ABB *arvoreMes = cria_arvore();
    ABB *arvoreDia = cria_arvore();
    ABB *arvoreIdade = cria_arvore();
    Pilha *pilha = cria_pilha();
    int opcao; //variavel para armazenar a opção que o usuario deseja
    while(1){
        printf("\nMenu Principal\n"); //menu principal
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
                printf("\nMenu de Cadastro\n"); //menu de cadastro
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
                printf("\nMenu de Atendimento\n"); //menu da fila de atendimento
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
                printf("\nMenu de Atendimento Prioritario\n"); //menu da fila de atendimento prioritario
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
                printf("\nMenu de Pesquisa\n"); //menu da pesquisa por arvore binaria de busca
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
                    in_ordem_dia(arvoreDia);
                }
                else if(opcao == 3){
                    in_ordem_mes(arvoreMes);
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
                printf("\nMenu de Desfazer\n"); //menu de operações na fila de atendimento
                printf("1 - Mostrar operações realizadas na fila de atendimento\n");
                printf("2 - Defazer a ultima operação realizada na fila de atendimento\n");
                printf("0 - Retornar ao menu principal \n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);
                if(opcao == 1){
                    mostrarOperacao(pilha);
                }
                else if(opcao == 2){
                    operacaoSendoRemovida(pilha); //mostra a log a ser removida e pergunta se o usuario realmente quer desfazer a operação
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
        else if(opcao == 6){ //salva pacientes 
            Salvar_paciente(lista);
        }
        else if(opcao == 7){ //carrega pacientes
            Carregar_paciente(lista, arvoreAno, arvoreMes, arvoreDia, arvoreIdade);
        }
        else if(opcao == 8){ //desenvolvedores do projeto
            printf("Desenvolvedores do projeto:\n");
            printf("Orlando Nagrockis Bertholdo \tRA: 24.223.033-5\n");
            printf("Morgana Rodrigues Zanetti \tRA: 24.223.010-0\n");
            printf("Informações adicionais:\n");
            printf("Curso: \tCiencia da Computacao\n");
            printf("Disciplina: \tEstrutura de Dados\n");
            printf("Ciclo: \t4° Semestre\n");
            printf("Data da apresentação do projeto: 21.05.2025\n");
        }
        else if(opcao == 0){ //sair do programa
            printf("Finalizando programa...\n");
            break;
        }
        else{ 
            printf("Opção inválida\n");
        }
    }
    
    return 0;
}