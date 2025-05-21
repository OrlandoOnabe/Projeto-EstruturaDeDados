# Estrutura-de-Dados (CC4652) - Projeto  
## Gerenciador de atendimento médico


<p align="justify">
Este repositório serve como template para o desenvolvimento do projeto da disciplina de Estrutura-de-Dados (CC4652) do Centro Universitário FEI.
Aplicativo para gerenciamento de serviços de saúde

**Projeto realizado por:**  

 - Orlando Nagrockis Bertholdo RA: 23.223.003-5  
 - Morgana Rodrigues Zanetti RA: 24.223.010-0

---

**Introdução:**   
O projeto visa simular um aplicativo para gerenciamento de serviços de saúde. A proposta tem como objetivo implementar os conhecimentos adquiridos ao longo das aulas.

**Descrição do projeto:**   
O programa conta com um menu principal contendo 8 opções, sendo elas:

| Opção | Função |
| --- | --- |
| 0 | Encerrar |
| 1 | Cadastrar | 
| 2 | Atendimento |
| 4 | Atendimento prioritário |
| 4 | Pesquisa |
| 5 | Desfazer |
| 6 | Salvar |
| 7 | Carregar |
| 8 | Sobre |

**Operações:**  

  CADASTRO: Cadastrar novo paciente em uma lista dinâmica encadeada, mantendo a ordem de registro (inserção no início)
  
    - Cadastrar novo paciente
    
    - Consultar paciente cadastrado
    
    - Mostrar lista completa
    
    - Atualizar dados de paciente
    
    - Remover paciente

   ATENDIMENTO: Inserir um paciente, que já possua cadastro, em uma fila para atendimento
  
    - Enfileirar paciente
    
    - Desenfileirar paciente
    
    - Mostrar fila

  ATENDIMENTO PRIORITÁRIO: Inserir um paciente, que já possua cadastro, em uma fila para atendimento prioritário (considerar um heap de máximo pela idade), máximo de 20 atendimentos diários
  
    - Enfileirar paciente;
    
    - Desenfileirar paciente;
    
    - Mostrar fila.
    
  PESQUISA: Inserir um paciente, que já possua cadastro, em uma árvore binária de busca
  
    - Mostrar registros ordenados por ano de registro
    
    - Mostrar registros ordenados por mês de registro
    
    - Mostrar registros ordenados por dia de registro
    
    - Mostrar registros ordenados por idade do paciente

  DESFAZER: Utilize uma pilha para armazenar as operações realizadas para a montagem da fila de atendimento
  
    - Mostra as operações que foram realizadas sobre a fila de atendimento (uma espécie de log)
    
    - Desfaz a última operação realizada para a montagem da fila de atendimento
    
    - Informe ao usuário a operação a ser desfeita e solicite confirmação

    
  SALVAR:
    
    - Salvar um arquivo com os dados dos pacientes
    
    - A ordem de inserção dos registros é mantida

  CARREGAR:

    - Ler um arquivo com os dados dos pacientes e carregar a lista
    
  SOBRE: Mostrar os dados dos desenvolvedores
  
    - Nome do aluno;
    
    - Ciclo;
    
    - Curso;
    
    - Disciplina;
    
    - Data

**Funcionamento:**  
Ao iniciar o código, o menu principal será apresentado e o usuário poderá optar por uma das opções disponíveis dentro do programa. Caso o usuário insira uma opção inválida, o sistema retornará o menu e solicitará uma nova entrada até que uma opção válida seja fornecida. Dependendo da opção selecionada, um menu secundário será exibido, oferecendo outras funcionalidades para todo tipo de operação.
</p>
