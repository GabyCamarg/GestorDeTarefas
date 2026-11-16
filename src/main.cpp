
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <limits>
using namespace std;

// No de tarefas armazenadas na lista encadeada.
struct Tarefa {
    int id;
    string titulo;
    int prioridade;
    bool concluida;
    Tarefa* proximo;
};

// No de historico usado pela pilha de desfazer.
struct Acao {
    int id;
    bool estadoAnterior;
    Acao* proximo;
};

// No da fila de atendimento.
struct NoFila {
    int id;
    NoFila* proximo;
};

// No da arvore binaria de busca.
struct NoArvore {
    int id;
    NoArvore* esquerda;
    NoArvore* direita;
};

Tarefa* inicio = nullptr;
Acao* topo = nullptr;
NoFila* frente = nullptr;
NoFila* fim = nullptr;
NoArvore* raiz = nullptr;
int proximoId = 1;

// Procura uma tarefa na lista pelo identificador.
Tarefa* localizarTarefa(int id) {
    for (Tarefa* atual = inicio; atual != nullptr; atual = atual->proximo) {
        if (atual->id == id) return atual;
    }
    return nullptr;
}

// Insere recursivamente um identificador na arvore.
NoArvore* inserirArvore(NoArvore* no, int id) {
    if (no == nullptr) return new NoArvore{id, nullptr, nullptr};
    if (id < no->id) no->esquerda = inserirArvore(no->esquerda, id);
    else if (id > no->id) no->direita = inserirArvore(no->direita, id);
    return no;
}

// Pesquisa recursivamente um identificador na arvore.
bool buscarArvore(NoArvore* no, int id) {
    if (no == nullptr || no->id == id) return no != nullptr;
    return id < no->id ? buscarArvore(no->esquerda, id)
                       : buscarArvore(no->direita, id);
}

// Remove um identificador da arvore e reorganiza seus nos.
NoArvore* removerArvore(NoArvore* no, int id) {
    if (no == nullptr) return nullptr;

    if (id < no->id) {
        no->esquerda = removerArvore(no->esquerda, id);
    } else if (id > no->id) {
        no->direita = removerArvore(no->direita, id);
    } else {
        if (!no->esquerda || !no->direita) {
            NoArvore* filho = no->esquerda ? no->esquerda : no->direita;
            delete no;
            return filho;
        }

        NoArvore* sucessor = no->direita;
        while (sucessor->esquerda) sucessor = sucessor->esquerda;

        no->id = sucessor->id;
        no->direita = removerArvore(no->direita, sucessor->id);
    }

    return no;
}

// Percorre a arvore em ordem crescente usando recursividade.
void percorrerArvore(NoArvore* no) {
    if (!no) return;
    percorrerArvore(no->esquerda);
    cout << no->id << " ";
    percorrerArvore(no->direita);
}

// Cadastra uma tarefa e registra seu ID na arvore.
void cadastrarTarefa() {
    Tarefa* nova = new Tarefa{};
    nova->id = proximoId++;

    cout << "Titulo: ";
    getline(cin >> ws, nova->titulo);

    cout << "Prioridade (1 a 5): ";
    if (!(cin >> nova->prioridade) ||
        nova->prioridade < 1 || nova->prioridade > 5) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Prioridade invalida!\n";
        delete nova;
        return;
    }

    nova->concluida = false;

    if (!inicio) {
        inicio = nova;
    } else {
        Tarefa* atual = inicio;
        while (atual->proximo) atual = atual->proximo;
        atual->proximo = nova;
    }

    raiz = inserirArvore(raiz, nova->id);
    cout << "Tarefa cadastrada! ID: " << nova->id << "\n";
}

// Exibe todas as tarefas cadastradas na lista.
void listarTarefas() {
    if (!inicio) {
        cout << "Nenhuma tarefa cadastrada.\n";
        return;
    }

    for (Tarefa* atual = inicio; atual; atual = atual->proximo) {
        cout << "\nID: " << atual->id
             << "\nTitulo: " << atual->titulo
             << "\nPrioridade: " << atual->prioridade
             << "\nStatus: "
             << (atual->concluida ? "Concluida" : "Pendente")
             << "\n----------------------\n";
    }
}

// Remove uma tarefa da lista e seu identificador da arvore.
void removerTarefa() {
    int id;
    cout << "ID para remover: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido.\n";
        return;
    }

    Tarefa* atual = inicio;
    Tarefa* anterior = nullptr;

    while (atual && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (!atual) {
        cout << "Tarefa nao encontrada.\n";
        return;
    }

    if (anterior) anterior->proximo = atual->proximo;
    else inicio = atual->proximo;

    raiz = removerArvore(raiz, id);

    delete atual;
    atual = nullptr;

    cout << "Tarefa removida!\n";
}

// Empilha o estado anterior de uma tarefa para permitir desfazer.
void empilharAcao(int id, bool estado = false) {
    topo = new Acao{id, estado, topo};
}

// Conclui uma tarefa e guarda seu estado anterior na pilha.
void concluirTarefa() {
    int id;
    cout << "ID da tarefa: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido.\n";
        return;
    }

    Tarefa* tarefa = localizarTarefa(id);

    if (!tarefa) {
        cout << "Tarefa nao encontrada.\n";
        return;
    }

    empilharAcao(id, tarefa->concluida);
    tarefa->concluida = true;
    cout << "Tarefa concluida!\n";
}

// Desfaz a ultima conclusao registrada na pilha.
void desfazerAcao() {
    if (!topo) {
        cout << "Nenhuma acao para desfazer.\n";
        return;
    }

    Acao* acao = topo;
    Tarefa* tarefa = localizarTarefa(acao->id);

    if (tarefa) {
        tarefa->concluida = acao->estadoAnterior;
        cout << "Acao desfeita!\n";
    } else {
        cout << "A tarefa da acao nao existe mais.\n";
    }

    topo = topo->proximo;
    delete acao;
    acao = nullptr;
}

// Adiciona o identificador de uma tarefa ao final da fila.
void adicionarFila() {
    int id;
    cout << "ID da tarefa: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido.\n";
        return;
    }

    if (!localizarTarefa(id)) {
        cout << "Tarefa nao encontrada.\n";
        return;
    }

    NoFila* novo = new NoFila{id, nullptr};

    if (fim) fim->proximo = novo;
    else frente = novo;

    fim = novo;
    cout << "Tarefa adicionada a fila!\n";
}

// Remove e atende o primeiro identificador da fila (FIFO).
void atenderFila() {
    if (!frente) {
        cout << "Fila vazia.\n";
        return;
    }

    NoFila* temporario = frente;
    Tarefa* tarefa = localizarTarefa(temporario->id);

    if (tarefa) cout << "Atendendo: " << tarefa->titulo << "\n";
    else cout << "Tarefa removida anteriormente.\n";

    frente = frente->proximo;
    if (!frente) fim = nullptr;

    delete temporario;
    temporario = nullptr;
}

// Ordena uma copia dos ponteiros por prioridade com Insertion Sort.
void ordenarTarefas() {
    vector<Tarefa*> dados;

    for (Tarefa* p = inicio; p; p = p->proximo) {
        dados.push_back(p);
    }

    for (int i = 1; i < static_cast<int>(dados.size()); ++i) {
        Tarefa* chave = dados[i];
        int j = i - 1;

        while (j >= 0 && dados[j]->prioridade < chave->prioridade) {
            dados[j + 1] = dados[j];
            --j;
        }

        dados[j + 1] = chave;
    }

    cout << "\nTarefas ordenadas por prioridade:\n";

    for (Tarefa* t : dados) {
        cout << t->id << " - " << t->titulo
             << " - Prioridade " << t->prioridade << "\n";
    }

    // Complexidade: O(n^2) no pior caso e O(n) no melhor caso.
}

// Busca sequencial pelo titulo e informa operacoes e tempo.
void buscaSequencial() {
    string titulo;
    int operacoes = 0;
    bool encontrado = false;

    cout << "Titulo para buscar: ";
    getline(cin >> ws, titulo);

    auto inicioTempo = chrono::high_resolution_clock::now();

    for (Tarefa* p = inicio; p; p = p->proximo) {
        ++operacoes;

        if (p->titulo == titulo) {
            cout << "Encontrada! ID: " << p->id << "\n";
            encontrado = true;
            break;
        }
    }

    auto fimTempo = chrono::high_resolution_clock::now();
    auto tempo = chrono::duration_cast<chrono::nanoseconds>(
        fimTempo - inicioTempo).count();

    if (!encontrado) cout << "Tarefa nao encontrada.\n";

    cout << "Operacoes: " << operacoes
         << " | Tempo: " << tempo << " ns\n";
}

// Cria um vetor ordenado por ID para a busca binaria.
vector<Tarefa*> prepararBuscaBinaria() {
    vector<Tarefa*> dados;

    for (Tarefa* p = inicio; p; p = p->proximo) {
        dados.push_back(p);
    }

    for (int i = 1; i < static_cast<int>(dados.size()); ++i) {
        Tarefa* chave = dados[i];
        int j = i - 1;

        while (j >= 0 && dados[j]->id > chave->id) {
            dados[j + 1] = dados[j];
            --j;
        }

        dados[j + 1] = chave;
    }

    return dados;
}

// Executa a busca binaria no vetor ordenado por identificador.
void buscaBinaria() {
    int id;
    int operacoes = 0;

    cout << "ID para buscar: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido.\n";
        return;
    }

    vector<Tarefa*> dados = prepararBuscaBinaria();

    int esquerda = 0;
    int direita = static_cast<int>(dados.size()) - 1;
    bool encontrado = false;

    auto inicioTempo = chrono::high_resolution_clock::now();

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        ++operacoes;

        if (dados[meio]->id == id) {
            cout << "Encontrada: " << dados[meio]->titulo << "\n";
            encontrado = true;
            break;
        }

        if (dados[meio]->id < id) esquerda = meio + 1;
        else direita = meio - 1;
    }

    auto fimTempo = chrono::high_resolution_clock::now();
    auto tempo = chrono::duration_cast<chrono::nanoseconds>(
        fimTempo - inicioTempo).count();

    if (!encontrado) cout << "Tarefa nao encontrada.\n";

    cout << "Operacoes: " << operacoes
         << " | Tempo: " << tempo << " ns\n";
}

// Consulta um identificador na arvore binaria de busca.
void consultarArvore() {
    int id;
    cout << "ID para buscar na arvore: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido.\n";
        return;
    }

    cout << (buscarArvore(raiz, id)
             ? "ID encontrado na arvore.\n"
             : "ID nao encontrado na arvore.\n");
}

// Libera recursivamente todos os nos da arvore binaria.
void liberarArvore(NoArvore* no) {
    if (!no) return;

    liberarArvore(no->esquerda);
    liberarArvore(no->direita);
    delete no;
}

// Libera todos os nos da lista, pilha, fila e arvore.
void liberarMemoria() {
    while (inicio) {
        Tarefa* temp = inicio;
        inicio = inicio->proximo;
        delete temp;
    }

    while (topo) {
        Acao* temp = topo;
        topo = topo->proximo;
        delete temp;
    }

    while (frente) {
        NoFila* temp = frente;
        frente = frente->proximo;
        delete temp;
    }

    fim = nullptr;
    liberarArvore(raiz);
    raiz = nullptr;
}

// Exibe o menu principal do sistema.
void exibirMenu() {
    cout << "\n===== GESTOR DE TAREFAS =====\n"
         << "1 - Cadastrar tarefa\n"
         << "2 - Listar tarefas\n"
         << "3 - Remover tarefa\n"
         << "4 - Concluir tarefa\n"
         << "5 - Desfazer conclusao\n"
         << "6 - Adicionar tarefa a fila\n"
         << "7 - Atender proxima tarefa\n"
         << "8 - Ordenar tarefas\n"
         << "9 - Busca sequencial\n"
         << "10 - Busca binaria por ID\n"
         << "11 - Consultar arvore\n"
         << "12 - Exibir arvore em ordem\n"
         << "0 - Sair\n"
         << "Escolha: ";
}

// Executa a funcionalidade correspondente a opcao escolhida.
void executarOpcao(int opcao) {
    switch (opcao) {
        case 1: cadastrarTarefa(); break;
        case 2: listarTarefas(); break;
        case 3: removerTarefa(); break;
        case 4: concluirTarefa(); break;
        case 5: desfazerAcao(); break;
        case 6: adicionarFila(); break;
        case 7: atenderFila(); break;
        case 8: ordenarTarefas(); break;
        case 9: buscaSequencial(); break;
        case 10: buscaBinaria(); break;
        case 11: consultarArvore(); break;
        case 12:
            cout << "IDs em ordem: ";
            percorrerArvore(raiz);
            cout << "\n";
            break;
        case 0:
            cout << "Sistema encerrado!\n";
            break;
        default:
            cout << "Opcao invalida!\n";
    }
}

// Controla o ciclo principal e libera a memoria ao encerrar.
int main() {
    int opcao = -1;

    do {
        exibirMenu();

        if (!(cin >> opcao)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida!\n";
            continue;
        }

        if (opcao != 0) executarOpcao(opcao);

    } while (opcao != 0);

    liberarMemoria();
    return 0;
}
