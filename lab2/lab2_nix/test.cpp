#include <iostream> 
#include <string> 
#include <unistd.h>

using namespace std;
string unicString = "Hello world!";
int main ()
{
	for (int i = 0; i < unicString.length(); ++i)
	{
		cout << unicString[i] << flush;	
		usleep(50000);
	}
	cout << endl;
	return 0;
}