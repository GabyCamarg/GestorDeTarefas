
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

// Estrutura principal
struct Tarefa {
    int id;
    string titulo;
    int prioridade;
    bool concluida;
    Tarefa* proximo;
};

// Estrutura da pilha
struct Acao {
    int id;
    bool estadoAnterior;
    Acao* proximo;
};

// Estrutura da fila
struct NoFila {
    int id;
    NoFila* proximo;
};

// Estrutura da arvore binaria
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

// Busca uma tarefa pelo ID
Tarefa* localizarTarefa(int id) {
    Tarefa* atual = inicio;

    while (atual != nullptr) {
        if (atual->id == id)
            return atual;
        atual = atual->proximo;
    }
    return nullptr;
}

// Insere um ID na arvore binaria
NoArvore* inserirArvore(NoArvore* no, int id) {
    if (no == nullptr) {
        NoArvore* novo = new NoArvore{id, nullptr, nullptr};
        return novo;
    }

    if (id < no->id)
        no->esquerda = inserirArvore(no->esquerda, id);
    else if (id > no->id)
        no->direita = inserirArvore(no->direita, id);

    return no;
}

// Busca recursivamente na arvore
bool buscarArvore(NoArvore* no, int id) {
    if (no == nullptr) return false;
    if (no->id == id) return true;

    if (id < no->id)
        return buscarArvore(no->esquerda, id);

    return buscarArvore(no->direita, id);
}

// Exibe a arvore em ordem
void percorrerArvore(NoArvore* no) {
    if (no == nullptr) return;

    percorrerArvore(no->esquerda);
    cout << no->id << " ";
    percorrerArvore(no->direita);
}

// Cadastra uma tarefa
void cadastrarTarefa() {
    Tarefa* nova = new Tarefa;

    nova->id = proximoId++;

    cout << "Titulo: ";
    getline(cin >> ws, nova->titulo);

    cout << "Prioridade (1 a 5): ";
    cin >> nova->prioridade;

    if (nova->prioridade < 1 || nova->prioridade > 5) {
        cout << "Prioridade invalida!\n";
        delete nova;
        return;
    }

    nova->concluida = false;
    nova->proximo = nullptr;

    if (inicio == nullptr) {
        inicio = nova;
    } else {
        Tarefa* atual = inicio;
        while (atual->proximo != nullptr)
            atual = atual->proximo;
        atual->proximo = nova;
    }

    raiz = inserirArvore(raiz, nova->id);

    cout << "Tarefa cadastrada! ID: " << nova->id << "\n";
}

// Lista as tarefas
void listarTarefas() {
    Tarefa* atual = inicio;

    if (!atual) {
        cout << "Nenhuma tarefa cadastrada.\n";
        return;
    }

    while (atual != nullptr) {
        cout << "\nID: " << atual->id
             << "\nTitulo: " << atual->titulo
             << "\nPrioridade: " << atual->prioridade
             << "\nStatus: "
             << (atual->concluida ? "Concluida" : "Pendente")
             << "\n----------------------\n";

        atual = atual->proximo;
    }
}

// Remove uma tarefa da lista
void removerTarefa() {
    int id;
    cout << "ID para remover: ";
    cin >> id;

    Tarefa* atual = inicio;
    Tarefa* anterior = nullptr;

    while (atual != nullptr && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (!atual) {
        cout << "Tarefa nao encontrada.\n";
        return;
    }

    if (anterior == nullptr)
        inicio = atual->proximo;
    else
        anterior->proximo = atual->proximo;

    delete atual;
    cout << "Tarefa removida!\n";
}

// Empilha uma acao
void empilharAcao(int id, bool estado) {
    Acao* nova = new Acao{id, estado, topo};
    topo = nova;
}

// Conclui uma tarefa
void concluirTarefa() {
    int id;
    cout << "ID da tarefa: ";
    cin >> id;

    Tarefa* tarefa = localizarTarefa(id);

    if (!tarefa) {
        cout << "Tarefa nao encontrada.\n";
        return;
    }

    empilharAcao(id, tarefa->concluida);
    tarefa->concluida = true;

    cout << "Tarefa concluida!\n";
}

// Desfaz a ultima conclusao
void desfazerAcao() {
    if (!topo) {
        cout << "Nenhuma acao para desfazer.\n";
        return;
    }

    Acao* acao = topo;
    Tarefa* tarefa = localizarTarefa(acao->id);

    if (tarefa != nullptr) {
        tarefa->concluida = acao->estadoAnterior;
        cout << "Acao desfeita!\n";
    } else {
        cout << "A tarefa da acao nao existe mais.\n";
    }

    topo = topo->proximo;
    delete acao;
}

// Adiciona uma tarefa na fila
void adicionarFila() {
    int id;
    cout << "ID da tarefa: ";
    cin >> id;

    if (!localizarTarefa(id)) {
        cout << "Tarefa nao encontrada.\n";
        return;
    }

    NoFila* novo = new NoFila{id, nullptr};

    if (fim == nullptr)
        frente = novo;
    else
        fim->proximo = novo;

    fim = novo;
    cout << "Tarefa adicionada a fila!\n";
}

// Atende a primeira tarefa da fila
void atenderFila() {
    if (!frente) {
        cout << "Fila vazia.\n";
        return;
    }

    NoFila* temporario = frente;
    Tarefa* tarefa = localizarTarefa(temporario->id);

    if (tarefa)
        cout << "Atendendo: " << tarefa->titulo << "\n";
    else
        cout << "Tarefa removida anteriormente.\n";

    frente = frente->proximo;

    if (!frente) fim = nullptr;

    delete temporario;
}

// Ordenacao Insertion Sort por prioridade
void ordenarTarefas() {
    vector<Tarefa*> dados;

    for (Tarefa* p = inicio; p != nullptr; p = p->proximo)
        dados.push_back(p);

    for (int i = 1; i < (int)dados.size(); i++) {
        Tarefa* chave = dados[i];
        int j = i - 1;

        while (j >= 0 &&
               dados[j]->prioridade < chave->prioridade) {
            dados[j + 1] = dados[j];
            j--;
        }

        dados[j + 1] = chave;
    }

    cout << "\nTarefas ordenadas por prioridade:\n";

    for (Tarefa* t : dados)
        cout << t->id << " - " << t->titulo
             << " - Prioridade " << t->prioridade << "\n";

    // Complexidade: O(n²) no pior caso.
}

// Busca sequencial pelo titulo
void buscaSequencial() {
    string titulo;
    int operacoes = 0;
    bool encontrado = false;

    cout << "Titulo para buscar: ";
    getline(cin >> ws, titulo);

    auto inicioTempo = chrono::high_resolution_clock::now();

    for (Tarefa* p = inicio; p != nullptr; p = p->proximo) {
        operacoes++;

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

// Busca binaria pelo ID
void buscaBinaria() {
    int id;
    cout << "ID para buscar: ";
    cin >> id;

    vector<Tarefa*> dados;

    for (Tarefa* p = inicio; p != nullptr; p = p->proximo)
        dados.push_back(p);

    // Ordena os ponteiros por ID
    for (int i = 1; i < (int)dados.size(); i++) {
        Tarefa* chave = dados[i];
        int j = i - 1;

        while (j >= 0 && dados[j]->id > chave->id) {
            dados[j + 1] = dados[j];
            j--;
        }

        dados[j + 1] = chave;
    }

    int esquerda = 0;
    int direita = (int)dados.size() - 1;
    int operacoes = 0;
    bool encontrado = false;

    auto inicioTempo = chrono::high_resolution_clock::now();

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        operacoes++;

        if (dados[meio]->id == id) {
            cout << "Encontrada: " << dados[meio]->titulo << "\n";
            encontrado = true;
            break;
        }

        if (dados[meio]->id < id)
            esquerda = meio + 1;
        else
            direita = meio - 1;
    }

    auto fimTempo = chrono::high_resolution_clock::now();
    auto tempo = chrono::duration_cast<chrono::nanoseconds>(
        fimTempo - inicioTempo).count();

    if (!encontrado) cout << "Tarefa nao encontrada.\n";

    cout << "Operacoes: " << operacoes
         << " | Tempo: " << tempo << " ns\n";
}

// Busca um ID na arvore
void consultarArvore() {
    int id;
    cout << "ID para buscar na arvore: ";
    cin >> id;

    cout << (buscarArvore(raiz, id)
             ? "ID encontrado na arvore.\n"
             : "ID nao encontrado na arvore.\n");
}

// Libera toda a memoria da arvore
void liberarArvore(NoArvore* no) {
    if (!no) return;

    liberarArvore(no->esquerda);
    liberarArvore(no->direita);
    delete no;
}

// Libera pilha, fila, lista e arvore
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

// Menu principal
int main() {
    int opcao;

    do {
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

        if (!(cin >> opcao)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada invalida!\n";
            continue;
        }

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
                liberarMemoria();
                cout << "Sistema encerrado!\n";
                break;
            default:
                cout << "Opcao invalida!\n";
        }

    } while (opcao != 0);

    return 0;
}
