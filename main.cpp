//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "Angel.h"
#include "OffModel.h"
#include "MenuHandler.h";
#include "mat.h"


//Variables --------------------------------------------------------------------------------------------------------------------------
using namespace std;
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
using namespace std;
point4 points[NumVertices];
color4 colors[NumVertices];
MenuHandler *menuHandler;
float scale_r = 1.0;
float scale_rh = 0.001;
float rot_speed = 0.04;
int control = 0;
int rothb = 0;
int speed = 1;
OffModel* off_object;
GLuint mainprogram;
bool minit();
// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
void reshape(int x, int y);
bool init();
GLfloat xRotated, yRotated, zRotated;
GLdouble radius = 1;
mat4  transform = 1;
int Index = 0;
// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
	point4(-0.5, -0.5,  0.5, 1.0),
	point4(-0.5,  0.5,  0.5, 1.0),
	point4(0.5,  0.5,  0.5, 1.0),
	point4(0.5, -0.5,  0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5,  0.5, -0.5, 1.0),
	point4(0.5,  0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};
// RGBA
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 1.0, 1.0),  // white
	color4(0.0, 1.0, 1.0, 1.0)   // cyan
};
//-------------------------------------------------------------------------------------------------------------------------------------------



void
quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}


void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'i':
		off_object->control = 0;
		off_object->reinit = 1;
		Theta[Axis] = 0;
		rot_speed = 0.04;
		GLuint mcontrol;
		mcontrol = glGetUniformLocation(mainprogram, "control");
		glUniform1f(mcontrol, 1.0);
		break;
	case 'h':
		std::cout << "Welcome" << endl;
		std::cout << "------HELP------" << endl;
		std::cout << "i : You can restart by typing i" << endl;
		std::cout << "h : You can type h to access help" << endl;
		std::cout << "q : You can exit by typing q" << endl;
		std::cout << "You can increase the speed by using arrow keys" << endl;
		std::cout << "You can use menu by clicking right button of the mouse" << endl;
		std::cout << "---------------------" << endl;
		break;
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;

	}
}


void keyboardplusplus(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		//speed++;
		rot_speed += 0.1;
		scale_rh += 0.001;
		break;
	case GLUT_KEY_DOWN:
		if (speed != 0) {
			rot_speed -= 0.1;
			//speed--;
		}
		break;
	}
}


void
display(void)
{
	if (off_object->control == 0) {
		if (off_object->reinit == 1) {
			glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
				NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);


			// Load shaders and use the resulting shader program
			GLuint program = InitShader("vshader32.glsl", "fshader32.glsl");
			mainprogram = program;
			menuHandler->setProgram(mainprogram);
			glUseProgram(mainprogram);

			// set up vertex arrays
			GLuint vPosition = glGetAttribLocation(program, "vPosition");
			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(0));

			GLuint vColor = glGetAttribLocation(program, "vColor");
			glEnableVertexAttribArray(vColor);
			glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(sizeof(points)));

			glEnable(GL_DEPTH_TEST);
			glClearColor(1.0, 1.0, 1.0, 1.0);


			off_object->reinit = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (menuHandler->rotation_hearthbeat == 0) {
			transform = (RotateX(Theta[Xaxis]) *
				RotateY(Theta[Yaxis]) *
				RotateZ(Theta[Zaxis]));
		}
		else {
			transform = Scale(scale_r, scale_r, scale_r);
		}
		point4  transformed_points[NumVertices];


		if (menuHandler->rotation_hearthbeat == 0) {
			for (int i = 0; i < NumVertices; ++i) {
				int m = 0;
				point4 temp_p = points[i];
				for (m = 0; m < speed; m++) {
					temp_p = transform * temp_p;
				}
				transformed_points[i] = temp_p;
			}
		}
		else {

			for (int i = 0; i < NumVertices; ++i) {
				int m = 0;
				point4 temp_p = points[i];
				for (m = 0; m < speed; m++) {
					temp_p = transform * temp_p;
				}
				transformed_points[i] = temp_p;
			}

		}

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(transformed_points),
			transformed_points);

		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
		glutSwapBuffers();
		//cout << "control 0" << endl;
	}
	else {
		//cout << "control 1" << endl;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		point4 temp_point1, temp_point2, temp_point3;

		int num_points = off_object->num_points;
		int num_triangles = off_object->num_triangles;
		int num_vertices = num_triangles * 3;
		point4  *transformed_points;
		transformed_points = new point4[num_triangles * 3];

		vec3* pts = off_object->points;
		point4 *pointss;
		pointss = new point4[num_triangles * 3];
		//cout << num_triangles << " NUM TRIANGLES NUM VERTICES " << num_vertices * 16 << endl;
		//getchar();
		if (off_object->reinit == 1) {

			glBufferData(GL_ARRAY_BUFFER, num_vertices * 16 + sizeof(colors),
				NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * 16, pointss);
			glBufferSubData(GL_ARRAY_BUFFER, num_vertices * 16, sizeof(colors), colors);
			GLuint program = InitShader("vshader32.glsl", "fshader32.glsl");
			mainprogram = program;
			menuHandler->setProgram(mainprogram);
			glUseProgram(mainprogram);

			// set up vertex arrays
			GLuint vPosition = glGetAttribLocation(program, "vPosition");
			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(0));

			GLuint vColor = glGetAttribLocation(program, "vColor");
			glEnableVertexAttribArray(vColor);
			glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(num_triangles));
			off_object->reinit = 0;





		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mat4  transform = 1;
		if (menuHandler->rotation_hearthbeat == 0) {
			transform = (RotateX(Theta[Xaxis]) *
				RotateY(Theta[Yaxis]) *
				RotateZ(Theta[Zaxis]));
		}
		else {
			transform = Scale(scale_r, scale_r, scale_r);
		}


		int i;
		for (size_t k = 0; k < off_object->num_triangles; k++)
		{

			temp_point1.x = off_object->points[(unsigned int)off_object->triangles[k][v1]].x;
			temp_point1.y = off_object->points[(unsigned int)off_object->triangles[k][v1]].y;
			temp_point1.z = off_object->points[(unsigned int)off_object->triangles[k][v1]].z;
			temp_point1.w = 1;
			temp_point2.x = off_object->points[(unsigned int)off_object->triangles[k][v2]].x;
			temp_point2.y = off_object->points[(unsigned int)off_object->triangles[k][v2]].y;
			temp_point2.z = off_object->points[(unsigned int)off_object->triangles[k][v2]].z;
			temp_point2.w = 1;
			temp_point3.x = off_object->points[(unsigned int)off_object->triangles[k][v3]].x;
			temp_point3.y = off_object->points[(unsigned int)off_object->triangles[k][v3]].y;
			temp_point3.z = off_object->points[(unsigned int)off_object->triangles[k][v3]].z;
			temp_point3.w = 1;


			pointss[3 * k] = normalize(temp_point1);
			pointss[3 * k].w = 1;
			pointss[(3 * k) + 1] = normalize(temp_point2);
			pointss[(3 * k) + 1].w = 1;
			pointss[(3 * k) + 2] = normalize(temp_point3);
			pointss[(3 * k) + 2].w = 1;

			/*
						pointss[3 * k] =temp_point1;
						pointss[3 * k].w = 1;
						pointss[(3 * k) + 1] = temp_point2;
						pointss[(3 * k) + 1].w = 1;
						pointss[(3 * k) + 2] = temp_point3;
						pointss[(3 * k) + 2].w = 1;
			*/


		}



		for (int i = 0; i < num_vertices; ++i) {
			int m = 0;
			point4 temp_p = pointss[i];
			for (m = 0; m < speed; m++) {
				temp_p = transform * temp_p;
			}
			transformed_points[i] = temp_p;
		}
		//std::cout << transformed_points[13].x << " TP 10 " << transformed_points[13].y << "  TP 10   " << transformed_points[13].z << endl;
		glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * 16,
			transformed_points);


		glDrawArrays(GL_TRIANGLES, 0, num_vertices);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glutSwapBuffers();
	}
}

void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
			//case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
		case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
		case GLUT_LEFT_BUTTON:   Axis = Zaxis;  break;
		}
	}
}

//----------------------------------------------------------------------------


void
idle(void)
{

	if (menuHandler->rotation_hearthbeat == 1) {
		if (scale_r > 1.6) {
			scale_rh *= -1;
		}
		if (scale_r < 1) {
			scale_rh *= -1;
		}
		scale_r += scale_rh;

	}
	Theta[Axis] += rot_speed;

	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------



void reshape(int x, int y)
{

	if (y == 0 || x == 0) return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.f, (GLdouble)x / (GLdouble)y, 0.05f, 50.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, x, y);

}


// OpenGL initialization
bool
init()
{
	colorcube();
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);


	//cout << "    ssss " << sizeof(points) << "SIZEOFPOINTS" << endl;

	minit();

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader32.glsl", "fshader32.glsl");
	mainprogram = program;
	glUseProgram(mainprogram);
	menuHandler->setProgram(mainprogram);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	return true;
}
bool minit() {

	off_object = new OffModel();
	if (!off_object->load("sphere.off"))
	{
		std::cout << "error" << std::endl;
		return false;
	}
	return true;

}

int main(int argc, char** argv)
{
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitContextVersion(3, 1);
	int id = glutCreateWindow("etuna - A1");

	
	glewExperimental = GL_TRUE;
	glewInit();


	if (!init())
	{
		//getchar();
		glutDestroyWindow(id);
		return EXIT_FAILURE;
	}

	//off_object->control = 1;
	

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboardplusplus);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	//Default wireframes
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	menuHandler = new MenuHandler();
	menuHandler->setOffObj(off_object);
	menuHandler->setGlobalMenuHandler(menuHandler);

	glutMainLoop();
	return EXIT_SUCCESS;
}








