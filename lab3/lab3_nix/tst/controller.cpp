#include <iostream>
#include <string>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>
#include <vector>
#include "esemaphore.h"
#include "extprocess.h"

using namespace std;

void pushWriter(vector<int> &writeOrder, vector<int> &closeOrder, string unicString);
void popWriter(vector<int> &writeOrder, vector<int> &closeOrder);
void disposeWriters(vector<int> &writeOrder, vector<int> &closeOrder);

const char* corePath = "./child_e";
// string pipe_write;

int main()
{
	// enable_raw_mode();
	// string unicStrings[] = {
	// 	"\"Lorem ipsum dolor sit amet, consectetur adipiscing elit.\"",
	// 	"\"In quam nisl, tempus et rutrum id, sagittis et ligula.\"",
	// 	"\"Vivamus fringilla enim elementum nisi malesuada, in rutrum justo elementum.\"",
	// 	"\"Donec aliquet fringilla lorem, quis venenatis enim porttitor eget.\"",
	// 	"\"Aenean id orci non risus hendrerit rhoncus nec ac mi.\"",
	// 	"\"Duis in aliquam nisl. Mauris lobortis ante mauris, nec rhoncus erat dapibus ac.\"",
	// 	"\"Vivamus nulla justo, finibus id sollicitudin imperdiet, dictum eu elit. Duis quis neque enim.\"",
	// 	"\"In ipsum quam, venenatis non tincidunt ut, consectetur in lectus. Donec semper neque at interdum pharetra.\"",
	// 	"\"Praesent vel sodales sem, ac pretium magna. Nunc blandit augue lacus, nec mollis nibh eleifend ut.\"",
	// 	"\"Quisque mollis lectus lorem, vitae iaculis neque rutrum eu.\"",
	// 	"\"Donec quis nisl vitae justo interdum lobortis ac vel nisl.\"",
	// 	"\"Pellentesque maximus lectus ultrices, rutrum velit vel, mollis est.\"",
	// };
	// const int unicCount = sizeof(unicStrings) / sizeof(string);
	vector<int> writeOrder;
	// vector<int> closeOrder;
	// int print = CreateSemaphore('p');
	const int print_key = 666;
	int reset = CreateSemaphore('r');
	int currentWriter = 0;

	cout << "sem reset = " << reset << endl;

	ExtProcess ext1;
	ext1.createProcess(corePath, "1", true);
	writeOrder.push_back(CreateSemaphore(print_key + 1));
	cout << "sem 1 = " << writeOrder.back() << endl;

	ExtProcess ext2;
	ext2.createProcess(corePath, "2", true);
	writeOrder.push_back(CreateSemaphore(print_key + 2));
	cout << "sem 2 = " << writeOrder.back() << endl;

	ReleaseSemaphore(writeOrder[0], 0);

	while (1) 
	{
		if (WaitSemaphore(reset, 0) == 0)
		{
			currentWriter + 1 < writeOrder.size() ? ++currentWriter : currentWriter = 0;
			ReleaseSemaphore(writeOrder[currentWriter], 0);
			static int sdd;
			cout << sdd++ <<"starts " << currentWriter + 1<< endl;
		}
		usleep(100000);
	}
	disable_raw_mode();
	return 0;
}

// void pushWriter(vector<int> &writeOrder, vector<int> &closeOrder, string unicString)
// {
// 	static int id;
// 	ExtProcess ext;
// 	string eventName = to_string(id);
// 	//ext.createProcess(corePath, eventName + " " + pipe_write + " " + unicString, true);
// 	writeOrder.push_back(CreateSemaphore('p' + id));
// 	closeOrder.push_back(CreateSemaphore('c' + id));
// 	++id;
// }

// void popWriter(vector<int> &writeOrder, vector<int> &closeOrder)
// {
// 	if (writeOrder.size() < 1)
// 		return;

// 	writeOrder.pop_back();

// 	ReleaseSemaphore(closeOrder.back(), 0);
// 	closeOrder.pop_back();
// }

// void disposeWriters(vector<int> &writeOrder, vector<int> &closeOrder)
// {
// 	for (int i = 0; i < writeOrder.size(); ++i)
// 	{
// 		ReleaseSemaphore(closeOrder[i], 0);
// 	}
// }