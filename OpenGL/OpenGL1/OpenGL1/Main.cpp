#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "Display.h"
#include "GameObject.h"
#include "GOManager.h"

using namespace std;

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);

	GameObject go1(0.5, 0.5);
	GameObject go2(-0.5, 0.5);
	GameObject go3(0.5, -0.5);
	GameObject go4(-0.5, -0.5);

	std::cout << game_objects.size() << std::endl;

	Display display_view(800, 800, "view");

	return 0;
}