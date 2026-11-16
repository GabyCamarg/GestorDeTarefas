# Gestor de Tarefas

## 1. Descrição do Sistema

O Gestor de Tarefas é um sistema desenvolvido em C++ que permite cadastrar, listar, remover, concluir e organizar tarefas, utilizando estruturas de dados estudadas na disciplina de Programação e Sistemas Computacionais II.

## 2. Tema Escolhido

Gestor de Tarefas.

## 3. Funcionalidades

- Cadastro de tarefas.
- Listagem de tarefas.
- Remoção de tarefas.
- Conclusão e desfazer ações.
- Fila de atendimento.
- Ordenação por prioridade.
- Busca sequencial e binária.
- Consulta e percurso de árvore binária.

## 4. Estruturas de Dados Utilizadas

- **Lista encadeada:** armazena as tarefas dinamicamente.
- **Pilha:** armazena ações para permitir desfazer conclusões (LIFO).
- **Fila:** organiza o atendimento das tarefas (FIFO).
- **Árvore binária de busca:** organiza os identificadores das tarefas.

## 5. Diagrama das Estruturas

```text
             GESTOR DE TAREFAS
                     |
       +-------------+-------------+
       |             |             |
     LISTA         PILHA          FILA
       |             |             |
    Tarefas       Histórico     Atendimento
                     |
              ÁRVORE BINÁRIA
                     |
              IDs organizados
```

## 6. Algoritmos

- **Insertion Sort:** ordenação das tarefas por prioridade. Complexidade O(n²) no pior caso.
- **Busca sequencial:** percorre os elementos até encontrar o título desejado. Complexidade O(n).
- **Busca binária:** procura um identificador em dados ordenados. Complexidade O(log n), desconsiderando o custo de preparação dos dados.

## 7. Gerenciamento de Memória

O sistema utiliza alocação dinâmica de memória com `new` para criar nós das estruturas e `delete` para liberar a memória utilizada.

## 8. Integrante e Responsabilidades

**Gabryele Camargo Oliveira**

Desenvolvimento individual do projeto, implementação das estruturas de dados, algoritmos, testes e documentação.

## 9. Compilação e Execução

```bash
clang++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o gestor
./gestor
```
