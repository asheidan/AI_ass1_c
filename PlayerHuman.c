#include "Board.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

void PlayerHumanInit() {
	struct termios term;
	int stdin_fileno = fileno(stdin);

	if(isatty(stdin_fileno) == 0) {
		fprintf(stderr, "stdin is not a terminal");
		exit(-1);
	}
	if(tcgetattr(stdin_fileno, &term) < 0) {
		perror("Getting terminfor");
		exit(-1);
	}

	term.c_lflag &= (~ECHO)&(~ICANON);

	tcsetattr(stdin_fileno,TCSANOW, &term);
}

ACTION PlayerHumanNextMove(Board *b) {
	fprintf(stderr, "Choose direction: ");
	fflush(stdin);
	char c = getc(stdin); 
	if(c == EOF) {
		return -1;
	}
	uint8_t i = c - '1';
	
	if(i >= 5) {
		i--;
	}

	if(i > 7) {
		return -1;
	}

	fprintf(stdout, "%s\n",ActionNames[i]);
	
	return i;
}
