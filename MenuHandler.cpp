

#include "MenuHandler.h"
#include "GL\freeglut.h"
#include "OffModel.h"

OffModel *off_obj;
GLuint program;
MenuHandler *menu;


void MenuHandler::setGlobalMenuHandler(MenuHandler* m) {
	menu = m;
}

void MenuHandler::setProgram(GLuint p) {
	program = p;
}


void MenuHandler::setOffObj(OffModel*off_object) {
	off_obj = off_object;
}

void mainMenu(int opt) {

}



void dm_solid() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glutPostRedisplay();
}
void dm_wireframes() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutPostRedisplay();
}

void drawCube() {
	//off_obj->load("cube.off");
	off_obj->control = 0;
	off_obj->reinit = 1;
	glutPostRedisplay();
}

void drawSphere() {
	off_obj->load("sphere.off");
	off_obj->control = 1;
	off_obj->reinit = 1;
	glutPostRedisplay();
}


void drawCar() {
	off_obj->load("car.off"); 
	off_obj->control = 1;
	off_obj->reinit = 1;
	glutPostRedisplay();
}

void drawTriangle() {
	off_obj->load("triangle.off");
	off_obj->control = 1;
	off_obj->reinit = 1;
	glutPostRedisplay();
}


void drawCone() {
	off_obj->load("cone.off");
	off_obj->control = 1;
	off_obj->reinit = 1;
	glutPostRedisplay();
}


void subMenu_obj(int opt) {

	switch (opt) {

	case 0:
		drawCube();
		break;
	case 1:
		drawSphere();
		break;
	case 2:
		drawCar();
		break;
	case 3:
		drawTriangle();
		break;
	case 4:
		drawCone(); 
		break;
	}

}

void subMenu_drawing(int opt) {

	switch (opt) {
	case 0:
		dm_wireframes();
		break;
	case 1:
		dm_solid();
		break;
	}

}
void subMenu_color(int opt) {
	GLuint mcontrol;GLuint myColor;
	switch (opt) {
	case 0:
		
		myColor = glGetUniformLocation(program, "myColor");
		mcontrol = glGetUniformLocation(program, "control");
		glUniform1f(mcontrol, 1.0);
		glUniform4f(myColor, 1.0, 0.0, 0.0, 0.0);
		glutPostRedisplay();
		break;
	case 1:
		
		myColor = glGetUniformLocation(program, "myColor");
		
		mcontrol = glGetUniformLocation(program, "control");
		glUniform1f(mcontrol, 1.0);
		glUniform4f(myColor, 1.0,1.0, 0.0, 1.0);
		glutPostRedisplay();
		break;
	case 2:
		
		myColor = glGetUniformLocation(program, "myColor");
		
		mcontrol = glGetUniformLocation(program, "control");
		glUniform1f(mcontrol, 1.0);
		glUniform4f(myColor,0.0,1.0, 0.0, 1.0);
		glutPostRedisplay();
		break;
	case 3:/*
		color4(0.0, 0.0, 0.0, 1.0),  // black
			color4(1.0, 0.0, 0.0, 1.0),  // red
			color4(1.0, 1.0, 0.0, 1.0),  // yellow
			color4(0.0, 1.0, 0.0, 1.0),  // green
			color4(0.0, 0.0, 1.0, 1.0),  // blue
			color4(1.0, 0.0, 1.0, 1.0),  // magenta
			color4(1.0, 1.0, 1.0, 1.0),  // white
			color4(0.0, 1.0, 1.0, 1.0)   // cyan*/
		
		myColor = glGetUniformLocation(program, "myColor");
		
		mcontrol = glGetUniformLocation(program, "control");
		glUniform1f(mcontrol, 1.0);
		glUniform4f(myColor, 0.0, 0.0, 1.0, 1.0);
		glutPostRedisplay();
		break;
	case 4:
		
		myColor = glGetUniformLocation(program, "myColor");
		
		mcontrol = glGetUniformLocation(program, "control");
		glUniform1f(mcontrol, 1.0);
		glUniform4f(myColor, 1.0, 0.0, 1.0, 1.0);
		glutPostRedisplay();
		break;
	case 5:
		
		myColor = glGetUniformLocation(program, "myColor");
		
		mcontrol = glGetUniformLocation(program, "control");
		glUniform1f(mcontrol, 1.0);
		glUniform4f(myColor, 0.0, 0.0, 0.0, 1.0);
		glutPostRedisplay();
		break;
	}

}

void MenuHandler::setRotHB(int i) {
	rotation_hearthbeat = i;
}

void subMenu_motion(int opt) {
	switch (opt) {
	case 0:
		menu->rotation_hearthbeat = 0;
		break;
	case 1:
		menu->rotation_hearthbeat = 1;
		break;
	}
}

MenuHandler::MenuHandler() {

	rotation_hearthbeat = 0;
	int obj_type = glutCreateMenu(subMenu_obj);
	glutAddMenuEntry("Cube", 0);
	glutAddMenuEntry("Sphere", 1);
	glutAddMenuEntry("Car", 2);
	glutAddMenuEntry("Triangle", 3);
	glutAddMenuEntry("Cone", 4);

	int motion_menu = glutCreateMenu(subMenu_motion);
	glutAddMenuEntry("Rotate", 0);
	glutAddMenuEntry("Hearth Beat", 1);


	int drawing_mode = glutCreateMenu(subMenu_drawing);
	glutAddMenuEntry("wireframe", 0);
	glutAddMenuEntry("solid", 1);

	int color = glutCreateMenu(subMenu_color);
	glutAddMenuEntry("red", 0);
	glutAddMenuEntry("yellow", 1);
	glutAddMenuEntry("green", 2);
	glutAddMenuEntry("blue", 3);
	glutAddMenuEntry("magenta", 4);
	glutAddMenuEntry("black", 5);

	glutCreateMenu(mainMenu);
	glutAddSubMenu("OBJECT TYPE", obj_type);
	glutAddSubMenu("MOTION TYPE", motion_menu);
	glutAddSubMenu("DRAWING MODE", drawing_mode);
	glutAddSubMenu("COLOR", color);

	glutAttachMenu(GLUT_RIGHT_BUTTON);



}

MenuHandler::~MenuHandler() {

}



