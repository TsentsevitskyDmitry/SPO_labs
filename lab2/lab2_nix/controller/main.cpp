#include <iostream>
#include <string>
#include <vector>
#include <signal.h>
#include <sys/wait.h>
#include "extprocess.h"

using namespace std;

void pushWriter(vector<pid_t> &writers, string unicString);
void popWriter(vector<pid_t> &writers);
void disposeWriters(vector<pid_t> &writers);

const char* corePath = "./parent_e";

int synchronize = 1, ready = 0;
struct sigaction writeSigaction, readySigaction;

void setSynchronize(int sign)
{
  synchronize = 1;
}

void setReady(int sign)
{
   ready = 1;
}

int main() 
{
	vector<pid_t> writers; 
	int currentWriter = 0;

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
		"Pellentesque maximus lectus ultrices, rutrum velit vel, mollis est.",
		"Proin non fermentum ex. Curabitur vitae neque nibh.",
		"Donec fringilla porta diam tincidunt tempus. Phasellus lobortis hendrerit ligula.",
		"Curabitur lorem justo, bibendum vel tellus a, venenatis aliquet lorem.",
		"Donec ultrices scelerisque lacus eget pulvinar.",
		"Sed elementum diam velit, eu efficitur nisl hendrerit ac.",
		"Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.",
		"Phasellus lectus felis, maximus vitae eros ornare, faucibus posuere turpis.",
		"In hac habitasse platea dictumst. Nullam ut nisi interdum, fringilla est vitae, mattis eros.",
		"Suspendisse ut imperdiet orci. Aliquam sed libero venenatis, lobortis dolor eget, auctor nibh.",
		"Sed viverra blandit ligula porttitor scelerisque.",
		"Morbi blandit, felis vitae lacinia lobortis, dolor arcu feugiat risus, vitae lobortis purus lectus eu lectus.",
		"Cras convallis tempor fringilla. Maecenas mollis erat euismod dui pretium euismod.",
		"Donec at vestibulum justo, euismod tincidunt dolor. In luctus non erat ac egestas. Etiam at accumsan erat.",
		"Donec faucibus aliquam porttitor. Duis vitae sagittis enim. Nullam fermentum nunc ante, ac tempus odio viverra sed.",
		"Ut sollicitudin, est at rutrum semper, neque arcu vulputate odio, eu porta tellus nisl id sapien.",
		"Nunc pretium sapien est. Cras eleifend porta lorem, nec luctus eros. Integer in vulputate augue.",
		"Etiam fringilla tellus varius tristique bibendum. Phasellus nec tempor nisl, ut fermentum nunc.",
		"Ut pellentesque euismod tincidunt. Proin id magna orci.",
		"Nam ante felis, iaculis eu sollicitudin sed, dignissim et est.",
		"Sed semper metus vitae bibendum egestas. Proin commodo ligula nulla, vel dapibus justo faucibus vel.",
		"Nullam auctor pellentesque ex. Curabitur sed dui cursus, iaculis purus eu, feugiat risus.",
		"Ut sed odio enim. Ut dapibus velit sit amet magna pharetra, a imperdiet elit imperdiet.",
		"Quisque faucibus lorem eget justo rhoncus sodales. Etiam auctor odio eget tempus ullamcorper."
	};
	const int unicCount = sizeof(unicStrings) / sizeof(string);

	writeSigaction.sa_handler = setSynchronize;
    sigaction(SIGUSR1, &writeSigaction, NULL);

	readySigaction.sa_handler = setReady;
    sigaction(SIGUSR2, &readySigaction, NULL);

    enable_raw_mode();
	bool run = true;
	while (run) 
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case '+':
				pushWriter(writers, unicStrings[writers.size() % unicCount]);
				if(writers.size() == 1)
					if(synchronize)
						kill(writers[0], SIGUSR1);
				break;
			case '-':
				popWriter(writers);
				break;
			case 'q':
				disposeWriters(writers);
				run = false;
				break;
			}
		}

		if(synchronize)
		{
			if(writers.size() < 1)
				continue;
			
			synchronize = 0;
			currentWriter + 1 < writers.size() ? ++currentWriter : currentWriter = 0;
			kill(writers[currentWriter], SIGUSR1);
		}
		
	}
	disable_raw_mode();
	return 0;
}

void pushWriter(vector<pid_t> &writers, string unicString)
{
	static int id;
	ExtProcess ext;
	string args = to_string(getpid()) + "_" +  unicString; 
	ext.createProcess(corePath, args, false);
	writers.push_back(ext.getPid());
	while(!ready) usleep(100000);;
	ready = 0;
}

void popWriter(vector<pid_t> &writers)
{
	if(writers.empty())
		return;
	kill(writers.back(), SIGUSR2);
	writers.pop_back();
}

void disposeWriters(vector<pid_t> &writers)
{
	for(int i = 0; i < writers.size(); ++i)
	{
		kill(writers.back(), SIGUSR2);
		writers.pop_back();
	}
}