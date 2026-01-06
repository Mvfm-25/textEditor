// Primeiros passos sendo tomados, seguindo tutorial
// [mvfm]
//
// Criado : 05/01/2026  || Última modificação : 06/01/2026

/*** Includes ***/
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/*** Definições ***/
#define CTRL_KEY(k) ((k) & 0x1f)

/*** Terminal. ***/

// Função pra lidar com MORTES
// Erros. Mortes são erros. Simplesmente printa messagem de erro e sai do programa.
void morre(const char *s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

// 'Reseta' o terminal para o estado inicial após o uso do 'modoCru'
struct termios termios_velho;
void desabilitaCru() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_velho) == -1) {
        morre("tcsetattr");
    }
}

// Flags para lidar com certas 'teclas especiais'.
void modoCru() {

    if(tcgetattr(STDIN_FILENO,&termios_velho) == -1) {
        morre("tcgetattr");
    }

    atexit(desabilitaCru);
    struct termios cru = termios_velho;
    // Todos esses comandos vem diretamente de <termios.h>
    // 'IXION' Impede com que Ctrl-S & Ctrl-Q controlem o 'software flow control'.
    // 'ICRNL' Lê literalmente Ctrl-M como 13, ao invés de 10.
    cru.c_iflag &= ~(ICRNL | IXON);

    // Tava com flag errado essa DISGRACE :fire: :fire: :fire:

    // Leitura literal de '\n', criação de novas linhas é explicitamente usada após.
    cru.c_lflag &= ~(OPOST);

    // 'ICANON' Desligando modo canônico, lendo agora byte-per-byte ao invés de por linhas.
    // 'ISIG' Impedindo que Ctrl-C & Ctrl-Z fechem o programa.
    // 'IEXTEN' Lida com o Ctrl-V
    cru.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    // Timeout para futura animaçãozinha de espera.
    cru.c_cc[VMIN] = 1;
    cru.c_cc[VTIME] = 0;

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &cru) == -1) {
        morre("tcsetattr");
    }
}

// Le tecla do usuário e retorna tal tecla.
// Aqui para futuras funções de escape.
char leTecla() {
    int nLe;
    char chara;

    while ((nLe = read(STDIN_FILENO, &chara, 1)) != 1) {
        // 'EAGAIN' == recurso temporariamente indisponível.
        if (nLe == -1 && errno == EAGAIN) morre("read");
    }

    return chara;
}

/*** Output ***/
// Desenha ~ ao lado do terminal como Vim.
// altura 10 foi escolhida arbritariamente.
void desenhaBarras() {
    int altura;
    for (altura = 0; altura < 10; altura++) {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}

void limpaTela() {
    // Printando 4bytes para uma sequência de 'escape' no terminal pra limpar a tela.
    // Vai ajudar depois pra parte visual do programa.
    write(STDOUT_FILENO, "\x1b[2J", 4);
    // Re-Posiciona o cursor no canto-esquerdo superior.
    write(STDOUT_FILENO, "\x1b[H", 3);

    desenhaBarras();
    write(STDOUT_FILENO, "\x1b[H", 3);
}

/*** Input ***/

// Refatorando inteiro main aqui.
// Vai ajudar com combinações Ctrl futuramente.
void processaTecla() {
    char chara = leTecla();

    switch (chara) {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
    }

}

/*** Init ***/
int main() {
    modoCru();

    // Main ainda mais atualizado.
    // Não printando mais as teclas por enquanto.
    while (1) {
        limpaTela();
        processaTecla();
    }
    return 0;
}
