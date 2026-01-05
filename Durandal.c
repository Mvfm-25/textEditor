// Primeiros passos sendo tomados, seguindo tutorial
// [mvfm]
//
// Criado : 05/01/2026  || Última modificação : 05/01/2026

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

// 'Reseta' o terminal para o estado inicial após o uso do 'modoCru'
struct termios termios_velho;
void desabilitaCru() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_velho);
}

// Simplesmente, não vou estar mais printando o que o usuário digita enquanto digita.
// Coisa de banco.
void modoCru() {
    tcgetattr(STDIN_FILENO, &termios_velho);
    atexit(desabilitaCru);

    struct termios cru = termios_velho;
    // Desligando modo canônico, lendo agora byte-per-byte ao invés de por linhas.
    cru.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &cru);
}

int main() {
    modoCru();

    // Lendo individualmente cada caracter do usuário. 'q' para terminar.
    char chara;
    while (read(STDIN_FILENO, &chara, 1) == 1 && chara != 'q') {
        // Printa valores ASCII de cada caracter, menos se forem 'Control Characters'
        if (iscntrl(chara)) {
            printf("%d\n", chara);
        } else {
            printf("%d ('%c')\n", chara, chara);
        }
    }

    return 0;
}
