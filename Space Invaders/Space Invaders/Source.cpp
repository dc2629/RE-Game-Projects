#include"MatrixEntities.h"
#include"App.h"

using namespace std;

int main(int argc, char *argv[]){
	App SpaceInvaderApp;
	while (!SpaceInvaderApp.ProcessEvents()){
		SpaceInvaderApp.UpdateandRender();
	}

	return 0;
}