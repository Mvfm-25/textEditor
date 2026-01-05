// Primeiros passos sendo tomados, seguindo tutorial
// [mvfm]
//
// Criado : 05/01/2026  || Última modificação : 05/01/2026

/*** Includes ***/
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/*** Terminal. ***/
// Função pra lidar com MORTES
void morre(const char *s) {
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

// Simplesmente, não vou estar mais printando o que o usuário digita enquanto digita.
// Coisa de banco.
void modoCru() {

    if(tcgetattr(STDIN_FILENO,&termios_velho) == -1) {
        morre("tcgetattr");
    }

    atexit(desabilitaCru);
    struct termios cru = termios_velho;
    // Todos esses comandos vem diretamente de <termios.h>
    // 'IXION' Impede com que Ctrl-S & Ctrl-Q controlem o 'software flow control'.
    // 'ICRNL' Lê literalmente Ctrl-M como 13, ao invés de 10.
    cru.c_lflag &= ~(ICRNL | IXON);

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

/*** Init ***/
int main() {
    modoCru();

    // Main atualizado.
    while (1) {
        char chara = '\0';

        if (read(STDIN_FILENO, &chara, 1) == -1 && errno != EAGAIN){
            morre("read");
        }
        read(STDIN_FILENO, &chara, 1);
        if (iscntrl(chara)) {
            printf("%d\r\n", chara);
        } else {
            printf("%d ('%c')\r\n", chara, chara);
        }

        if (chara == 'q') {break;}
    }
    return 0;
}
