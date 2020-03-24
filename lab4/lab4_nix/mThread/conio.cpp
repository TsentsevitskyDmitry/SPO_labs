#include <termios.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>

#include "conio.h"

int _getch()
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

void enable_raw_mode()
{
	termios term;
	tcgetattr(0, &term);
	term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
	tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
	termios term;
	tcgetattr(0, &term);
	term.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &term);
}

bool _kbhit()
{
	int byteswaiting;
	ioctl(0, FIONREAD, &byteswaiting);
	return byteswaiting > 0;
}

void clearConsole()
{
    std::system ("clear");
}