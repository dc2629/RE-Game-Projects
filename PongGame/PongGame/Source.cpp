#include"Entity.h"
#include"App.h"

using namespace std;

int main(int argc, char *argv[]){
	App hi;
	while (!hi.ProcessEvents()){
		hi.Update();
		hi.Render();
	}

	return 0;
}