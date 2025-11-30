#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Estrutura de Dados ---

typedef struct {
    char tipo; // 'I', 'J', 'L', 'O', 'S', 'T', 'Z'
    int id;    // Identificador sequencial da peça
} Peca;

// --- Configurações das Estruturas ---

#define TAM_FILA 5
#define TAM_PILHA 3

// Fila Circular (Future Pieces Queue)
Peca fila[TAM_FILA];
int frente = 0; 
int tras = -1;  
int cont_fila = 0;

// Pilha Linear (Reserve Stack)
Peca pilha[TAM_PILHA];
int topo = -1; 

// Variável para gerar IDs sequenciais
int proximo_id = 1;

// --- Protótipos de Funções ---
// Necessários para que o compilador conheça as funções antes de serem usadas.

Peca gerarPeca();
void exibirPeca(Peca p);

// Fila Circular (Queue)
void inicializarFila();
int filaCheia();
int filaVazia();
void enqueue(Peca p);
Peca dequeue();
void mostrarFila();

// Pilha Linear (Stack)
void inicializarPilha();
int pilhaCheia();
int pilhaVazia();
void push(Peca p);
Peca pop();
void mostrarPilha();

// Trocas Estratégicas
void trocarFrenteFilaComTopoPilha();
void trocar3PrimeirosFilaComPilha();

// ---------------------------------------------------

// --- Funções Auxiliares ---

/**
 * @brief Gera uma nova peça com tipo aleatório e ID sequencial.
 */
Peca gerarPeca() {
    Peca nova_peca;
    // Tipos de peças de Tetris (Tetrominoes)
    char tipos[] = {'I', 'J', 'L', 'O', 'S', 'T', 'Z'}; 
    int indice_tipo = rand() % 7; 

    nova_peca.tipo = tipos[indice_tipo];
    nova_peca.id = proximo_id++;

    return nova_peca;
}

/**
 * @brief Exibe uma única peça no formato [Tipo|ID].
 */
void exibirPeca(Peca p) {
    printf("[%c|%d]", p.tipo, p.id);
}

// --- Funções da Fila Circular (Nível Novato) ---

void inicializarFila() {
    frente = 0;
    tras = -1;
    cont_fila = 0;
    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(gerarPeca()); 
    }
}

int filaCheia() {
    return cont_fila == TAM_FILA;
}

int filaVazia() {
    return cont_fila == 0;
}

void enqueue(Peca p) {
    if (filaCheia()) {
        return; // Não faz nada, a chamada principal já avisou
    }
    // Lógica circular
    tras = (tras + 1) % TAM_FILA; 
    fila[tras] = p;
    cont_fila++;
}

Peca dequeue() {
    Peca p = {'\0', 0};
    if (filaVazia()) {
        return p; 
    }
    p = fila[frente];
    // Lógica circular
    frente = (frente + 1) % TAM_FILA; 
    cont_fila--;
    return p;
}

void mostrarFila() {
    printf("   Queue (Fila de Peças Futuras, Cont: %d/%d): ", cont_fila, TAM_FILA);
    if (filaVazia()) {
        printf("Vazia.\n");
        return;
    }
    
    // 

    int i;
    int indice_atual = frente;
    for (i = 0; i < cont_fila; i++) {
        exibirPeca(fila[indice_atual]);
        if (i < cont_fila - 1) {
            printf(" -> ");
        }
        indice_atual = (indice_atual + 1) % TAM_FILA;
    }
    printf("\n");
}

// --- Funções da Pilha Linear (Nível Aventureiro) ---

void inicializarPilha() {
    topo = -1;
}

int pilhaCheia() {
    return topo == TAM_PILHA - 1;
}

int pilhaVazia() {
    return topo == -1;
}

void push(Peca p) {
    if (pilhaCheia()) {
        return; // Não faz nada
    }
    pilha[++topo] = p;
}

Peca pop() {
    Peca p = {'\0', 0};
    if (pilhaVazia()) {
        return p;
    }
    return pilha[topo--];
}

void mostrarPilha() {
    printf("   Stack (Pilha de Reserva, Cont: %d/%d): ", topo + 1, TAM_PILHA);
    if (pilhaVazia()) {
        printf("Vazia.\n");
        return;
    }
    
    // 

    // Exibe a pilha, do topo para a base (LIFO)
    printf("TOPO <- ");
    for (int i = topo; i >= 0; i--) {
        exibirPeca(pilha[i]);
        if (i > 0) {
            printf(" | ");
        }
    }
    printf("\n");
}

// --- Funções de Interação Estratégica (Nível Mestre) ---

void trocarFrenteFilaComTopoPilha() {
    if (filaVazia() || pilhaVazia()) {
        printf("\n❌ Falha na troca 4: Fila vazia ou Pilha vazia. (Fila: %d, Pilha: %d)\n", cont_fila, topo + 1);
        return;
    }

    Peca temp_fila = fila[frente];
    Peca temp_pilha = pilha[topo];

    // Troca
    fila[frente] = temp_pilha;
    pilha[topo] = temp_fila;

    printf("\n✅ TROCA 4 REALIZADA: A peça da FRENTE da Fila ");
    exibirPeca(temp_fila);
    printf(" foi trocada com a peça do TOPO da Pilha ");
    exibirPeca(temp_pilha);
    printf(".\n");
}

void trocar3PrimeirosFilaComPilha() {
    if (cont_fila < 3) {
        printf("\n❌ Falha na troca 5: A Fila deve ter no mínimo 3 peças. (Cont: %d)\n", cont_fila);
        return;
    }
    if (topo != TAM_PILHA - 1) {
        printf("\n❌ Falha na troca 5: A Pilha deve ter **exatamente** %d peças para a troca. (Cont: %d)\n", TAM_PILHA, topo + 1);
        return;
    }

    printf("\n✅ TROCA 5 INICIADA: Trocando os 3 primeiros da Fila com as 3 peças da Pilha.\n");

    for (int i = 0; i < TAM_PILHA; i++) {
        // Cálculo do índice circular dos primeiros 3 elementos da fila
        int indice_fila = (frente + i) % TAM_FILA; 

        Peca temp_fila = fila[indice_fila];
        Peca temp_pilha = pilha[i]; // Acesso da base (i=0) ao topo (i=2) da Pilha

        // Troca
        fila[indice_fila] = temp_pilha;
        pilha[i] = temp_fila;
    }

    printf("   Sucesso! As estruturas foram trocadas.\n");
}

// --- Função Principal ---

int main() {
    // Inicializa o gerador de números aleatórios (importante para gerarPeca())
    srand(time(NULL)); 

    // Inicialização
    inicializarPilha();
    inicializarFila(); // Chama enqueue() que agora é conhecida pelo protótipo.
    
    printf("--- 🧱 Desafio Tetris Stack Inicializado! 🧱 ---\n");

    int opcao;
    Peca peca_movida;

    do {
        printf("\n" "--------------------------------------------------\n");
        mostrarFila();
        mostrarPilha();
        printf("--------------------------------------------------\n");
        printf("\n### ⚙️ Menu de Ações ###\n");
        printf("1 - Jogar peça (remover da frente da Fila)\n"); 
        printf("2 - Enviar peça da Fila para a Reserva (Pilhar)\n"); 
        printf("3 - Usar peça da Reserva (Remover do topo da Pilha)\n"); 
        printf("4 - Trocar peça da frente da Fila com o topo da Pilha\n"); 
        printf("5 - Trocar os 3 primeiros da Fila com as 3 da Pilha\n"); 
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\n❌ Entrada inválida. Tente novamente.\n");
            // Limpa o buffer de entrada
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1: // Jogar peça (Dequeue e Enqueue nova)
                if (filaVazia()) {
                    printf("\n❌ Fila Vazia! Não é possível jogar.\n");
                    break;
                }
                peca_movida = dequeue();
                printf("\n🎉 Peça jogada/removida: ");
                exibirPeca(peca_movida);
                printf("\n");
                // Repõe imediatamente a fila para manter 5 peças
                enqueue(gerarPeca());
                printf("   Nova peça gerada e adicionada ao final da Fila.\n");
                break;

            case 2: // Enviar Fila -> Pilha (Dequeue e Push)
                if (filaVazia()) {
                    printf("\n❌ Fila Vazia! Não há peça para reservar.\n");
                    break;
                }
                if (pilhaCheia()) {
                    printf("\n❌ Pilha de Reserva Cheia! Não é possível mover.\n");
                    break;
                }
                peca_movida = dequeue();
                push(peca_movida);
                printf("\n➡️ Peça da Fila movida para a Reserva: ");
                exibirPeca(peca_movida);
                printf("\n");
                // Repõe a fila para manter 5 peças
                enqueue(gerarPeca());
                printf("   Nova peça gerada e adicionada ao final da Fila.\n");
                break;

            case 3: // Usar Pilha (Pop)
                peca_movida = pop();
                if (peca_movida.id != 0) {
                    printf("\n⬅️ Peça da Reserva utilizada: ");
                    exibirPeca(peca_movida);
                    printf("\n");
                } else {
                     printf("\n❌ Pilha Vazia! Nenhuma peça na reserva.\n");
                }
                break;

            case 4: // Trocar frente da fila com topo da pilha
                trocarFrenteFilaComTopoPilha();
                break;

            case 5: // Trocar 3 primeiros da fila com as 3 da pilha
                trocar3PrimeirosFilaComPilha();
                break;

            case 0:
                printf("\n👋 Saindo do sistema de controle de peças.\n");
                break;

            default:
                printf("\n❌ Opção inválida. Escolha uma opção entre 0 e 5.\n");
                break;
        }
        
    } while (opcao != 0);

    return 0;
}