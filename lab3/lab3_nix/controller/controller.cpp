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
const int print_key = 666;
const int close_key = 6666;
string pipe_write;

int main(int argc, char* argv[])
{
	if (argc != 2) return -1;
	pipe_write  = string(argv[1]);

	enable_raw_mode();
	string unicStrings[] = {
		"Lorem_ipsum_dolor_sit_amet,_consectetur_adipiscing_elit.",
		"In_quam_nisl,_tempus_et_rutrum_id,_sagittis_et_ligula.",
		"Vivamus_fringilla_enim_elementum_nisi_malesuada,_in_rutrum_justo_elementum.",
		"Donec_aliquet_fringilla_lorem,_quis_venenatis_enim_porttitor_eget.",
		"Aenean_id_orci_non_risus_hendrerit_rhoncus_nec_ac_mi.",
		"Duis_in_aliquam_nisl._Mauris_lobortis_ante_mauris,_nec_rhoncus_erat_dapibus_ac.",
		"Vivamus_nulla_justo,_finibus_id_sollicitudin_imperdiet,_dictum_eu_elit._Duis_quis_neque_enim.",
		"In_ipsum_quam,_venenatis_non_tincidunt_ut,_consectetur_in_lectus._Donec_semper_neque_at_interdum_pharetra.",
		"Praesent_vel_sodales_sem,_ac_pretium_magna._Nunc_blandit_augue_lacus,_nec_mollis_nibh_eleifend_ut.",
		"Quisque_mollis_lectus_lorem,_vitae_iaculis_neque_rutrum_eu.",
		"Donec_quis_nisl_vitae_justo_interdum_lobortis_ac_vel_nisl.",
		"Pellentesque_maximus_lectus_ultrices,_rutrum_velit_vel,_mollis_est."
	};
	const int unicCount = sizeof(unicStrings) / sizeof(string);

	vector<int> writeOrder;
	vector<int> closeOrder;
	int print = CreateSemaphore('p');
	int close = CreateSemaphore('c');

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
				disposeWriters(writeOrder, closeOrder);
				ReleaseSemaphore(close, 0);
				continue;
			}
		}

		if (WaitSemaphore(print, 0) == 0)
		{
			if (writeOrder.size() < 1)
				ReleaseSemaphore(print, 0);
			
			else 
			{
				currentWriter + 1 < writeOrder.size() ? ++currentWriter : currentWriter = 0;
				ReleaseSemaphore(writeOrder[currentWriter], 0);
				static int sdd;
			}
		}
		usleep(100000);
	}
	disable_raw_mode();
	return 0;
}

void pushWriter(vector<int> &writeOrder, vector<int> &closeOrder, string unicString)
{
	static int id;
	ExtProcess ext;
	string eventName = to_string(id);
	ext.createProcess(corePath, eventName + " " + pipe_write + " " + unicString, false);
	writeOrder.push_back(CreateSemaphore(print_key + id));
	closeOrder.push_back(CreateSemaphore(close_key + id));
	++id;
}

void popWriter(vector<int> &writeOrder, vector<int> &closeOrder)
{
	if (writeOrder.size() < 1)
		return;

	writeOrder.pop_back();

	ReleaseSemaphore(closeOrder.back(), 0);
	closeOrder.pop_back();
}

void disposeWriters(vector<int> &writeOrder, vector<int> &closeOrder)
{
	for (int i = 0; i < writeOrder.size(); ++i)
	{
		ReleaseSemaphore(closeOrder[i], 0);
	}
}