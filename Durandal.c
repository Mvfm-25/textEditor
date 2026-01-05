// Primeiros passos sendo tomados, seguindo tutorial
// [mvfm]
//
// Criado : 05/01/2026  || Última modificação : 05/01/2026

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

// Simplesmente, não vou estar mais printando o que o usuário digita enquanto digita.
// Coisa de banco.
void modoCru() {
    struct termios cru;

    tcgetattr(STDIN_FILENO, &cru);

    cru.c_lflag &= ~(ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &cru);
}

int main() {
    modoCru();

    // Lendo individualmente cada caracter do usuário. 'q' para terminar.
    char chara;
    while (read(STDIN_FILENO, &chara, 1) == 1 && chara != 'q');

    return 0;
}
