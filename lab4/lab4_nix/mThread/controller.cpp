#include "controller.h"

using namespace std;

void pushWriter(vector<pthread_t> &writeOrder, vector<int> &closeOrder, string unicString);
void popWriter(vector<pthread_t> &writeOrder, vector<int> &closeOrder);
void disposeWriters(vector<int> &closeOrder);

ChildArgs args;
int pipe_write;
pthread_mutex_t* saccess;

void* controller_main(void* lpParam)
{
	enable_raw_mode();
	string unicStrings[] = {
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
		"In quam nisl, tempus et rutrum id, sagittis et ligula.",
		"Vivamus fringilla enim elementum nisi malesuada, in rutrum justo elementum.",
		"Donec aliquet fringilla lorem, quis venenatis enim porttitor eget.",
		"Aenean id orci non risus hendrerit rhoncus nec ac mi.",
		"Duis in aliquam nisl. Mauris lobortis ante mauris, nec rhoncus erat dapibus ac.",
		"Vivamus nulla justo, finibus id sollicitudin imperdiet, dictum eu elit. Duis quis neque enim.",
		"In ipsum quam, venenatis non tincidunt ut, consectetur in lectus. Donec semper neque at interdum pharetra.",
		"Praesent vel sodales sem, ac pretium magna. Nunc blandit augue lacus, nec mollis nibh eleifend ut.",
		"Quisque mollis lectus lorem, vitae iaculis neque rutrum eu.",
		"Donec quis nisl vitae justo interdum lobortis ac vel nisl.",
		"Pellentesque maximus lectus ultrices, rutrum velit vel, mollis est."
	};
	const int unicCount = sizeof(unicStrings) / sizeof(string);

	vector<pthread_t> writeOrder;
	vector<int> closeOrder;
	int print = CreateSemaphore("print");
	int close = CreateSemaphore("close");

	ControllerArgs* args = (ControllerArgs*)lpParam;
	pipe_write = args->pipe_write;
	saccess = args->saccess;

	int currentWriter = 0;

	bool run = true;
	while (run) 
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case '+':
				pushWriter(writeOrder, closeOrder, unicStrings[writeOrder.size() % unicCount]);
				break;

			case '-':
				popWriter(writeOrder, closeOrder);
				break;

			case 'q':
				run = false;
				disposeWriters(closeOrder);
				ReleaseSemaphore(close, 0);
				continue;
			}
		}

		// if (WaitSemaphore(print, 0) == 0)
		// {
		// 	if (writeOrder.size() < 1)
		// 		ReleaseSemaphore(print, 0);
			
		// 	else 
		// 	{
		// 		currentWriter + 1 < writeOrder.size() ? ++currentWriter : currentWriter = 0;
		// 		ReleaseSemaphore(writeOrder[currentWriter], 0);
		// 		static int sdd;
		// 	}
		// }
		usleep(10);
	}
	disable_raw_mode();
	return 0;
}

void pushWriter(vector<pthread_t> &writeOrder, vector<int> &closeOrder, string unicString)
{
	static int id;
	args.saccess = saccess;
	args.pipe_write = pipe_write;
	args.syncName = to_string(id);
	args.unicString = unicString;

	pthread_t controller;
	void* data = &args;
	pthread_create(&controller, NULL, child_main, data);
	
	writeOrder.push_back(controller);
	closeOrder.push_back(CreateSemaphore(string("close").append(to_string(id))));
	++id;
}

void popWriter(vector<pthread_t> &writeOrder, vector<int> &closeOrder)
{
	if (writeOrder.size() < 1)
		return;

	ReleaseSemaphore(closeOrder.back(), 0);
	closeOrder.pop_back();

	pthread_join(writeOrder.back(), NULL);
	writeOrder.pop_back();
}

void disposeWriters(vector<int> &closeOrder)
{
	for (int i = 0; i < closeOrder.size(); ++i)
	{
		ReleaseSemaphore(closeOrder[i], 0);
	}
}