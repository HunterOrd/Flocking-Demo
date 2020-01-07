//Hunter Ordeneaux
//C00061957
//Assingment 5

#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>


#include <gmtl\gmtl.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "Texture.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "glfw3.lib")
//Variables for set up. Creates the amount of stacks and the size of the list for the object
unsigned int texwidth[5], texheight[5];
unsigned char* imagedata[5];
bool AmbientToggle = true;
bool DiffuseToggle = true;
bool cameraTwo = false;
bool mouseHeld = false;
int count = 0;
double prevMouseX = 0, prevMouseY = 0;
const int meridians = 100;
const int paralles = 100;
const float radius = 1.0;
const int indexLength = ((2 * (meridians + 1) + 1)) * (paralles - 1);
GLfloat vertices[(meridians+1)*paralles][3];
GLuint index_list[indexLength];
GLfloat colors[(meridians + 1) * paralles][3];
GLfloat Sphere_Normals[(meridians + 1) * paralles][3];
GLfloat Sphere_UVs[(meridians + 1) * paralles][2];
GLuint a_parameter_LightID;
GLuint GlobalU;
GLfloat Ambient;
GLfloat Diffuse;
GLuint texture;
GLuint View;
GLuint Project;
GLuint NormalM;
GLuint SkyOnly;
GLuint ColorOnly;
const float near = 0.1;
const float far = 500.0;
const float FOV = gmtl::Math::tan(gmtl::Math::deg2Rad(45.0) / 2.0);
const float AspectRatio = 16.0 / 9.0;
gmtl::Vec4f a_parameter_Light_componenet;
gmtl::Vec4f Global_U_component;
gmtl::Matrix44f Perspective;
gmtl::Matrix44f World;
gmtl::Matrix44f SkySphere;
gmtl::Quatf Quat_NegYRotate(0, -0.087156, 0, 0.9961957);
gmtl::Quatf Quat_YRotate(0, 0.087156, 0, 0.9961957);
gmtl::Quatf Quat_XRotate(0.087156, 0, 0, 0.9961957);
gmtl::Quatf Quat_NegXRotate(-0.087156, 0, 0, 0.9961957);
gmtl::Matrix44f M_O;
gmtl::Matrix44f ViewMatrix;
gmtl::Matrix44f NormalMatrix;
gmtl::Matrix44f NewView;
float rotDelta = gmtl::Math::PI_OVER_4 /2;
gmtl::Matrix44f z_Rotate;
gmtl::Matrix44f negz_Rotate;
gmtl::Matrix44f x_Rotate;
gmtl::Matrix44f negx_Rotate;
gmtl::Matrix44f y_Rotate;
gmtl::Matrix44f Negy_Rotate;
gmtl::Matrix44f X_Translate;
gmtl::Matrix44f NegX_Translate;
gmtl::Matrix44f NegZ_Translate;
gmtl::Matrix44f Y_Translate;
gmtl::Matrix44f Z_Translate;
gmtl::Matrix44f negZ_Translate;
gmtl::Matrix44f SpherePlacement[3];
GLuint Spheres[3];
int SphereRadius[3];
float TimeStep = .01;
float PlaneRaduis = radius + 0.3;
const int PlaneNumber = 30;

gmtl::Matrix44f RandomMatrix(){
	
	float x = rand() % 360;
	float y = rand() % 360;
	float z = rand() % 360;
	gmtl::EulerAngleXYZf Randram = gmtl::EulerAngleXYZf(x, y, z);
 	return gmtl::make<gmtl::Matrix44f>(Randram);
}

struct Plane {
	GLuint PlaneInfo;
	gmtl::Matrix44f M_Plane;
	gmtl::Matrix44f Loc_R;
	gmtl::Matrix44f M_O;
	gmtl::Vec3f currentVelocity;
	gmtl::Vec3f Acceleration;
	float AngularVelocity;
	float VerticalVelocity;
	float DispersionF;
	float CenteringF;
	float Radius;
};
Plane Planeone;
Plane PlaneStorage[PlaneNumber];

char* filetobuf(char* file)
{
	FILE* fptr;
	long length;
	char* buf;

	fopen_s(&fptr, file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
void normalize(gmtl::Matrix44f mat) {
	//normalizes the matrix by deviding all elements by the det
	double determ = 1 / (mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) - mat[0][1] * (mat[0][1] * mat[2][2] - mat[2][2] * mat[2][1]) + mat[0][2] * (mat[0][1] * mat[1][1] - mat[0][2] * mat[1][2]));
	for (int i = 0; i < 4; i++) {
		for (int n = 0; n < 4; n++) {
			mat[i][n] *= determ;
		}
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS && cameraTwo == true) {
		ViewMatrix = negz_Rotate * negz_Rotate * negz_Rotate * negz_Rotate * ViewMatrix;
		//rotates the veiw around the plane by 90 degress
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		cameraTwo = true;
		gmtl::identity(NewView);
		ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * gmtl::invert(PlaneStorage[0].M_Plane);
		gmtl::invert(PlaneStorage[0].M_Plane);
		//Switches the view to the plane location
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		cameraTwo = false;
		ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * World;
		//Switches the view back to world view
	}
	else if (key == GLFW_KEY_M && action == GLFW_PRESS)
		if (cameraTwo) {
			NewView = Z_Translate * NewView;
			ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * NewView * gmtl::invert(PlaneStorage[0].M_Plane);
			gmtl::invert(PlaneStorage[0].M_Plane);
		}
		else
			ViewMatrix = Z_Translate * ViewMatrix;
	else if (key == GLFW_KEY_N && action == GLFW_PRESS)
		if (cameraTwo) {
			NewView = NegZ_Translate * NewView;
			ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * NewView * gmtl::invert(PlaneStorage[0].M_Plane);
			gmtl::invert(PlaneStorage[0].M_Plane);
		}
		else
			ViewMatrix = NegZ_Translate * ViewMatrix;
	//ViewMatrix the passing of multiple matricies so that the camera lands on the plane and the M's are inverted
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		AmbientToggle = not AmbientToggle;
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		TimeStep += .001;
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		TimeStep -= .001;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		DiffuseToggle = not DiffuseToggle;

	count++;
	if (count == 10) {
		normalize(M_O);
		normalize(Planeone.M_Plane);
		normalize(ViewMatrix);
		count = 0;
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x_pos = 0.0;
	double y_pos = 0.0;
	glfwGetCursorPos(window, &x_pos, &y_pos);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mouseHeld = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mouseHeld = false;
	}
}
void mouse_movement_callback(GLFWwindow* window, double xpos, double ypos) {
	if (mouseHeld) {
		//Rotates the view matrix according the mouse movement when the button is held
		if (xpos - prevMouseX > 0) {
			if (cameraTwo) {
				NewView = NewView * gmtl::make<gmtl::Matrix44f>(Quat_YRotate);
				ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * NewView * gmtl::invert(Planeone.M_Plane);
				gmtl::invert(Planeone.M_Plane);
			}
			else
				ViewMatrix = ViewMatrix * gmtl::make<gmtl::Matrix44f>(Quat_YRotate);
		}
		else if (xpos - prevMouseX < 0) {
			if (cameraTwo){
				NewView = NewView * gmtl::make<gmtl::Matrix44f>(Quat_NegYRotate);
				ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * NewView * gmtl::invert(Planeone.M_Plane);
				gmtl::invert(Planeone.M_Plane);
			}
			else
				ViewMatrix = ViewMatrix * gmtl::make<gmtl::Matrix44f>(Quat_NegYRotate);
		}
		else if (ypos - prevMouseY > 0) {
			if (cameraTwo) {
				NewView = NewView * gmtl::make<gmtl::Matrix44f>(Quat_XRotate);
				ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * NewView * gmtl::invert(Planeone.M_Plane);
				gmtl::invert(Planeone.M_Plane);
			}
			else
				ViewMatrix = ViewMatrix * gmtl::make<gmtl::Matrix44f>(Quat_XRotate);
		}
		else if (ypos - prevMouseY < 0) {
			if (cameraTwo) {
				NewView = NewView * gmtl::make<gmtl::Matrix44f>(Quat_NegXRotate);
				ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * NewView * gmtl::invert(Planeone.M_Plane);
				gmtl::invert(Planeone.M_Plane);
			}
			else
				ViewMatrix = ViewMatrix * gmtl::make<gmtl::Matrix44f>(Quat_NegXRotate);
		}
	}
	prevMouseX = xpos;
	prevMouseY = ypos;
}
GLFWwindow* setupWindow()
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);
	
	window = glfwCreateWindow(1920, 1080, "Saki Sphere", glfwGetPrimaryMonitor(), NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_movement_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	glEnable(GL_DEPTH_TEST);

	return window;
}

GLuint setupShaderProgram(char* Vertex, char* Fragment)
{
	GLuint vertex_shader, fragment_shader, shader_program;
	int IsCompiled_VS, IsCompiled_FS, IsLinked, max_length;
	char* vertex_shader_log;
	char* fragment_shader_log;
	char* shader_program_log;

	char* vertex_source = filetobuf(Vertex);
	char* fragment_source = filetobuf(Fragment);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &IsCompiled_VS);
	if (IsCompiled_VS == GL_FALSE)
	{
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

		vertex_shader_log = (char*)malloc(max_length);

		glGetShaderInfoLog(vertex_shader, max_length, &max_length, vertex_shader_log);
		printf("Error: %s", vertex_shader_log);
		free(vertex_shader_log);
		free(vertex_source);
		return 0;
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &IsCompiled_FS);
	if (IsCompiled_FS == GL_FALSE)
	{
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

		fragment_shader_log = (char*)malloc(max_length);

		glGetShaderInfoLog(fragment_shader, max_length, &max_length, fragment_shader_log);
		printf("Error: %s", fragment_shader_log);
		free(fragment_shader_log);
		free(vertex_source);
		free(fragment_source);
		return 0;
	}

	shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);


	glGetProgramiv(shader_program, GL_LINK_STATUS, (int*)& IsLinked);
	if (IsLinked == GL_FALSE)
	{
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &max_length);

		shader_program_log = (char*)malloc(max_length);


		glGetProgramInfoLog(shader_program, max_length, &max_length, shader_program_log);
		printf("Error: %s", shader_program_log);

		free(shader_program_log);
		free(vertex_source);
		free(fragment_source);
		return 0;
	}
	//	glBindAttribLocation(shader_program, SHADER_POSITION_INDEX, "in_position");
	//	glBindAttribLocation(shader_program, SHADER_COLOR_INDEX, "in_color");

	free(vertex_source);
	free(fragment_source);

	return shader_program;

}

void init(GLuint shader_program, GLuint& pos_loc_out, GLuint& color_loc_out, GLuint& m_loc_out, GLuint& normal_loc_out, GLuint& UV_out)
{
	pos_loc_out = glGetAttribLocation(shader_program, "in_position");
	color_loc_out = glGetAttribLocation(shader_program, "in_color");
	normal_loc_out = glGetAttribLocation(shader_program, "in_normal");
	UV_out = glGetAttribLocation(shader_program, "in_UVs");
	m_loc_out = glGetUniformLocation(shader_program, "M");
}

float SphereInfluence(float distance) {
	if (distance <= 40 && distance > 0)
		return 70/distance;
	else
		return 0;
}
float CenteringInfluence(float distance) {
	if (distance < 90)
		return -distance/10;
	else
		return 0;
}
float DisspersionInfluence(float distance) {
	if (distance <= 45 && distance > 0)
		return 70/distance;
	else
		return 0;
}
float AlignmentInfluence(float distance) {
	if (distance < 50 && distance > 0)
		return 40/distance;
	else
		return 0;
}
gmtl::Quatf ThetaRotate( float theta,gmtl::Vec3f Axis) {
	gmtl::Quatf TheQuat = gmtl::Quatf(0, 0, 0, 1);
	TheQuat[0] = Axis[0] * sin(theta / 2.0);
	TheQuat[1] = Axis[1] * sin(theta / 2.0);
	TheQuat[2] = Axis[2] * sin(theta / 2.0);
	TheQuat[3] = cos(theta / 2.0);
	return TheQuat;
}
gmtl::Vec3f Forces(Plane CurrentPlane) {
	gmtl::Vec3f X_AxisI, X_AxisJay, temp, U_vec, HeightControl, Zero, crossVec, Centering, Dispersion, Allignment, Total_Force1, Total_Force2, Total_Force3, Total_Force4 = gmtl::Vec3f(0, 0, 0);
	gmtl::Quatf Generic;
	gmtl::Matrix33f Maxtemp;
	gmtl::identity(Maxtemp);
	float theta;
	float distanceTotal = 0;
	float distanceLateral = 0;
	float distanceVertical = 0;
	float interPlane = 0;
	int contributingCent = 0;
	int contributingDis = 0;
	int contributingAli = 0;
	X_AxisI = gmtl::Vec3f(CurrentPlane.Loc_R[0][0], CurrentPlane.Loc_R[1][0], CurrentPlane.Loc_R[2][0]);
	//Forces
	for (int c = 0; c < PlaneNumber; c++) {
		X_AxisJay = gmtl::Vec3f(PlaneStorage[c].Loc_R[0][0], PlaneStorage[c].Loc_R[1][0], PlaneStorage[c].Loc_R[2][0]);
		if (X_AxisI != X_AxisJay) {
			U_vec = gmtl::makeCross(X_AxisI, X_AxisJay);
			if (U_vec != Zero)
				gmtl::normalize(U_vec);
			theta = acos(gmtl::dot(X_AxisI, X_AxisJay));
			interPlane = (CurrentPlane.Radius + PlaneStorage[c].Radius) / 2.0;
			distanceLateral = ((theta * 180.0)/3.14)  * (interPlane);
			distanceVertical = CurrentPlane.Radius - PlaneStorage[c].Radius;
			distanceTotal = sqrt((distanceLateral * distanceLateral) + (distanceVertical * distanceVertical));
			crossVec = gmtl::makeCross(X_AxisI, U_vec);
			Centering = (distanceLateral * crossVec) + (distanceVertical * X_AxisI);
			if (Centering != Zero)
				gmtl::normalize(Centering);
			Centering *= CenteringInfluence(distanceTotal);
			Dispersion = (distanceLateral * gmtl::makeCross(X_AxisI, U_vec)) + (distanceVertical * X_AxisI);
			if (Dispersion != Zero)
				gmtl::normalize(Dispersion);
			Dispersion *= DisspersionInfluence(distanceTotal);
			Generic = ThetaRotate(theta, U_vec);
			Maxtemp = gmtl::make<gmtl::Matrix33f>(Generic);
			temp = Maxtemp * PlaneStorage[c].currentVelocity;
			Allignment = temp - CurrentPlane.currentVelocity;
			Allignment *= AlignmentInfluence(distanceTotal);
			if (AlignmentInfluence(distanceTotal) != 0)
				contributingAli++;
			if (DisspersionInfluence(distanceTotal) != 0)
				contributingDis++;
			if (CenteringInfluence(distanceTotal) != 0)
				contributingCent++;			
			if (distanceTotal < 15) {
				Total_Force2 += Dispersion;
				Total_Force1 += Zero;
				Total_Force3 += Zero;
			}
			else if (distanceTotal < 20 && distanceTotal >= 15) {
				Total_Force1 += Zero;
				Total_Force3 += Allignment;
				Total_Force2 += Dispersion;
			}
			else {
				Total_Force1 += Centering;
				Total_Force3 += Allignment;
				Total_Force2 += Dispersion;
			}
			
		}
	}
	for (int c = 0; c < 3; c++) {
		X_AxisJay = gmtl::Vec3f(SpherePlacement[c][0][0], SpherePlacement[c][1][0], SpherePlacement[c][2][0]);
		if (X_AxisI != X_AxisJay) {
			U_vec = gmtl::makeCross(X_AxisI, X_AxisJay);
			if (U_vec != Zero)
				gmtl::normalize(U_vec);
			theta = acos(gmtl::dot(X_AxisI, X_AxisJay));
			interPlane = (CurrentPlane.Radius + SphereRadius[c]) / 2.0;
			distanceLateral = ((theta * 180.0) / 3.14) * (interPlane);
			distanceVertical = CurrentPlane.Radius - SphereRadius[0];
			distanceTotal = sqrt((distanceLateral * distanceLateral) + (distanceVertical * distanceVertical));
			crossVec = gmtl::makeCross(X_AxisI, U_vec);
			Dispersion = (distanceLateral * crossVec) + (distanceVertical * X_AxisI);
			if (Dispersion != Zero)
				gmtl::normalize(Dispersion);
			Dispersion *= SphereInfluence(distanceTotal);
			if (distanceTotal < 25) {
				Total_Force4 += Dispersion;
				Total_Force1 -= Total_Force1;
				Total_Force3 -= Total_Force3;
			}
			else if (distanceTotal < 35 && distanceTotal >= 25) {
				Total_Force4 += Dispersion;
				Total_Force1 -= Total_Force1;
			}
			else {
				Total_Force4 += Dispersion;
			}
					
		}	
	}
	if (contributingDis != 0)
		Total_Force2 /= contributingDis;
	if (contributingCent != 0)
		Total_Force1 /= contributingCent;
	if (contributingAli != 0)
		Total_Force3 /= contributingAli;

	return Total_Force1 + Total_Force2 + Total_Force3 + Total_Force4;

	}

void Flocking(){
	//setup
	gmtl::Vec3f Acceleration, Crossvec, X_Collumn, Z_Collumn, Y_Collumn, temp;
	float rotate = 0.0;
	float Bank = 0.0;
	float pitch = 0.0;
	gmtl::Matrix44f Flock_Z, Bank_Y, Pitch_Z;
	gmtl::identity(Bank_Y);
	gmtl::identity(Pitch_Z);
	gmtl::identity(Flock_Z);
	//StepOne
	for (int z = 0; z < PlaneNumber; z++) {
		Y_Collumn = gmtl::Vec3f(PlaneStorage[z].Loc_R[0][1], PlaneStorage[z].Loc_R[1][1], PlaneStorage[z].Loc_R[2][1]);
		X_Collumn = gmtl::Vec3f(PlaneStorage[z].Loc_R[0][0], PlaneStorage[z].Loc_R[1][0], PlaneStorage[z].Loc_R[2][0]);
		float magnitude = PlaneStorage[z].AngularVelocity * PlaneStorage[z].Radius;
		PlaneStorage[z].currentVelocity = Y_Collumn * magnitude;
		PlaneStorage[z].currentVelocity += X_Collumn * PlaneStorage[z].VerticalVelocity;
	}
	for (int a = 0; a < PlaneNumber; a++) {
		//StepTwo	
		Acceleration = Forces(PlaneStorage[a]);
		if(isnan(Acceleration[0]) != true)
			PlaneStorage[a].Acceleration = Acceleration;
	}
	for (int a = 0; a < PlaneNumber; a++) {
		X_Collumn = gmtl::Vec3f(PlaneStorage[a].Loc_R[0][0], PlaneStorage[a].Loc_R[1][0], PlaneStorage[a].Loc_R[2][0]);
		Z_Collumn = gmtl::Vec3f(PlaneStorage[a].Loc_R[0][2], PlaneStorage[a].Loc_R[1][2], PlaneStorage[a].Loc_R[2][2]);
		//StepThree
		PlaneStorage[a].currentVelocity += PlaneStorage[a].Acceleration * TimeStep;
		temp = gmtl::makeNormal(PlaneStorage[a].currentVelocity);
		Bank = -gmtl::dot(PlaneStorage[a].Acceleration, Z_Collumn);
		pitch = -gmtl::dot(temp, X_Collumn);
		Bank = gmtl::Math::aSin(Bank / 5.0);
		pitch = gmtl::Math::aSin(pitch / 10.0);
		Bank_Y[0][0] = Bank_Y[2][2] = gmtl::Math::cos(Bank);
		Bank_Y[0][2] = gmtl::Math::sin(Bank);
		Bank_Y[2][0] = -gmtl::Math::sin(Bank);
		Pitch_Z[0][0] = Pitch_Z[1][1] = gmtl::Math::cos(pitch);
		Pitch_Z[0][1] = -gmtl::Math::sin(pitch);
		Pitch_Z[1][0] = gmtl::Math::sin(pitch);
		PlaneStorage[a].M_O = Pitch_Z * Bank_Y * M_O;
		//StepFour
		if (X_Collumn != PlaneStorage[a].currentVelocity)
			Crossvec = gmtl::makeCross(X_Collumn, PlaneStorage[a].currentVelocity);
		else
			Crossvec = gmtl::makeCross(X_Collumn, gmtl::Vec3f(PlaneStorage[a].Loc_R[0][1], PlaneStorage[a].Loc_R[1][1], PlaneStorage[a].Loc_R[2][1]));
		if (Crossvec != gmtl::Vec3f(0, 0, 0))
			gmtl::normalize(Crossvec);
		PlaneStorage[a].Loc_R[0][2] = Crossvec[0];
		PlaneStorage[a].Loc_R[1][2] = Crossvec[1];
		PlaneStorage[a].Loc_R[2][2] = Crossvec[2];
		Z_Collumn = gmtl::Vec3f(PlaneStorage[a].Loc_R[0][2], PlaneStorage[a].Loc_R[1][2], PlaneStorage[a].Loc_R[2][2]);
		Crossvec = gmtl::makeCross(Z_Collumn, X_Collumn);
		PlaneStorage[a].Loc_R[0][1] = Crossvec[0];
		PlaneStorage[a].Loc_R[1][1] = Crossvec[1];
		PlaneStorage[a].Loc_R[2][1] = Crossvec[2];
		Y_Collumn = gmtl::Vec3f(PlaneStorage[a].Loc_R[0][1], PlaneStorage[a].Loc_R[1][1], PlaneStorage[a].Loc_R[2][1]);
		X_Collumn = gmtl::Vec3f(PlaneStorage[a].Loc_R[0][0], PlaneStorage[a].Loc_R[1][0], PlaneStorage[a].Loc_R[2][0]);
		//StepFive
		PlaneStorage[a].AngularVelocity = gmtl::dot(Y_Collumn, PlaneStorage[a].currentVelocity) / PlaneStorage[a].Radius;
		PlaneStorage[a].VerticalVelocity = gmtl::dot(PlaneStorage[a].currentVelocity, X_Collumn);
		for (int a = 0; a < PlaneNumber; a++) {
			float SpeedControl = 2;
			float x = PlaneStorage[a].currentVelocity[0];
			float y = PlaneStorage[a].currentVelocity[1];
			float z = PlaneStorage[a].currentVelocity[2];
			float speed = sqrt((x * x) + (y * y) + (z * z));
			if (speed >= 1.4) {
				PlaneStorage[a].AngularVelocity += -SpeedControl * TimeStep;
			}
			else if (speed <= 1.1) {
				PlaneStorage[a].AngularVelocity += SpeedControl * TimeStep;
			}
		}
		for (int a = 0; a < PlaneNumber; a++){
			float HeightControl = 1;
			if (PlaneStorage[a].Radius > 1.8)
				PlaneStorage[a].VerticalVelocity += -HeightControl * TimeStep;
			else if (PlaneStorage[a].Radius <= 1.3)
				PlaneStorage[a].VerticalVelocity += HeightControl * TimeStep;
		}
		rotate = PlaneStorage[a].AngularVelocity * TimeStep;
		Flock_Z[0][0] = Flock_Z[1][1] = gmtl::Math::cos(rotate);
		Flock_Z[0][1] = -gmtl::Math::sin(rotate);
		Flock_Z[1][0] = gmtl::Math::sin(rotate);
		PlaneStorage[a].Radius += PlaneStorage[a].VerticalVelocity * TimeStep;
		X_Translate[0][3] = PlaneStorage[a].Radius;
		PlaneStorage[a].Loc_R = PlaneStorage[a].Loc_R * Flock_Z;
		PlaneStorage[a].M_Plane = PlaneStorage[a].Loc_R * X_Translate * PlaneStorage[a].M_O;
	}
}
void CreateList(float Raidus, int meridians, int paralles)
{
	//Set up nessecerry variables and interpolations
	int count = 0;
	int upperIndex = meridians + 1;
	int lowerIndex = 0;
	double theta =  M_PI/(paralles-1.0);
	float phi = (2.0* M_PI)/ meridians;
	float RedInc = 1.0/paralles;
	float GreenInc = 1.0/meridians;
	float BlueInc = 1.0/ indexLength;
	float Red = 0.0;
	float Blue = 0.0;
	float Green = 0.0;
	float U = 1.0 / paralles;
	float V = 1.0 / (meridians+1.0);
	for (int p = 0; p < paralles; p++)
	{
		//set up y change and y points and increments color as told
		float thetaInc = p*theta;
		float Ylocal = -Raidus * cos(thetaInc);
		Red += RedInc;
		Green = 0.0;
		for (int m = 0; m < (meridians+1); m++)
		{
			//fill in the xyz points and colors for each vertex
			float phiInc = m * phi;
			float Dintermediate = Raidus * sin(thetaInc);
			float Xlocal = Dintermediate * sin(phiInc);
			float Zlocal = Dintermediate * cos(phiInc);
			vertices[count][0] = Xlocal;
			vertices[count][1] = Ylocal;
			vertices[count][2] = Zlocal;
			Sphere_Normals[count][0] = Xlocal;
			Sphere_Normals[count][1] = Ylocal;
			Sphere_Normals[count][2] = Zlocal;
			Sphere_UVs[count][0] = m * U;
			Sphere_UVs[count][1] = p * V;
			colors[count][0] = Red;
			colors[count][1] = Green;
			colors[count][2] = Blue;
			Green += GreenInc;
			Blue += BlueInc;
			count++;
		}
	}
	count = 0;
	for (int i = 1; i < indexLength; i += 2)
	{
		//create the index list
		index_list[i - 1] = upperIndex;
		index_list[i] = lowerIndex;
		upperIndex++;
		lowerIndex++;
		count++;
		if (count == meridians+1)
		{
			//at the end of a strip place a restart index and incrment the count
			count = 0;
			i++;
			index_list[i] = 0xFFFF;
		}
	}


}
GLuint CreatePlane(GLuint position_loc, GLuint color_loc, GLuint normal_loc) {
	float Color1 = rand() % 2;
	if (Color1 == 0)
		Color1 = .2;
	float Color2 = rand() % 2;
	if (Color2 == 0)
		Color2 = .2;
	float Color3 = rand() % 2;
	if (Color2 == 0)
		Color2 = .2;

	const GLfloat PlaneVerts[][3] = {{ 0.025,  0.075, 0.0},{ 0.025, -0.075, 0.0525},{ 0.025, -0.075, 0.0375},{-0.025, -0.075, 0.0},{ 0.025, -0.075, -0.0375},{ 0.025, -0.075, -0.0525}};

	const GLuint PlanePoints[] = {0,1,0,2,0,3,0,4,0,5};

	const GLfloat Planecolors[][3] = {{ Color1, Color2, Color3},{ Color1, Color2, Color3},{ Color1, Color2, Color3},{ Color1, Color2, Color3},{ Color1, Color2, Color3},{ Color1, Color2, Color3}};

	const GLfloat PlaneNormals[][3] = {{ 1.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0 },{ -1.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0 }};


	GLuint VAO_out, VBO[3], EAB;

	//Generates buffers
	glGenVertexArrays(1, &VAO_out);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EAB);

	//binds the buffers 
	glBindVertexArray(VAO_out);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//Puts in vertice data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVerts), PlaneVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_loc);
	
	//Puts in color data to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Planecolors), Planecolors, GL_STATIC_DRAW);
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(color_loc);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneNormals), PlaneNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(normal_loc);
	//Puts in the indicies to buffer 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PlanePoints), PlanePoints, GL_STATIC_DRAW);
	//clears the current bindings for next one
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//returns the VAO
	return VAO_out;
}
GLuint setupDrawnObjects(GLuint position_loc, GLuint color_loc, GLuint normal_loc, GLuint UV_Loc)
{
	
	GLuint VAO_out, VBO[4], EAB; 

	glGenVertexArrays(1, &VAO_out);
	glGenBuffers(4, VBO);
	glGenBuffers(1, &EAB);

	glBindVertexArray(VAO_out);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(position_loc);


	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(color_loc);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sphere_Normals), Sphere_Normals, GL_STATIC_DRAW);
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
	glEnableVertexAttribArray(normal_loc);

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sphere_UVs), Sphere_UVs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(UV_Loc);
	glVertexAttribPointer(UV_Loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_list), index_list, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*
		These calls just remove the names/IDs from use. The buffered data are still associated
		with the vertex array object. Since they are only scoped to this function, however,
		we would normally remove them here or GL will never use them again within the application.
		This probably wouldn't cause errors for the assignment, so we will omit it here.
	*/
	// glDeleteBuffers(2, VBO);
	// glDeleteBuffers(1, &EAB);

	return VAO_out;
}

void display(GLFWwindow* window, GLuint shader_program, GLuint m_location, GLuint VAO, GLuint VAOSky, GLuint Spheres[], gmtl::Matrix44f SpherePlacement[],
	gmtl::Matrix44f SkySphere, gmtl::Matrix44f mat, gmtl::Matrix44f V,gmtl::Matrix44f P, gmtl::Matrix44f N, int indexLength)
{

	/* Make our background black */
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	a_parameter_LightID = glGetUniformLocation(shader_program, "a_parameter");
	GlobalU = glGetUniformLocation(shader_program, "Global_U");
	Ambient = glGetUniformLocation(shader_program, "Ambient_Var");
	Diffuse = glGetUniformLocation(shader_program, "Diffuse_Var");
	View = glGetUniformLocation(shader_program, "viewMatrix");
	Project = glGetUniformLocation(shader_program, "projectionMatrix");
	NormalM = glGetUniformLocation(shader_program, "NormalMatrix");
	SkyOnly = glGetUniformLocation(shader_program, "SkyToggle");
	ColorOnly = glGetUniformLocation(shader_program, "ColorToggle");

	/* Load the shader into the rendering pipeline */
	glUseProgram(shader_program);


	if (AmbientToggle)
		glUniform1f(Ambient, 0.3);
	else
		glUniform1f(Ambient, 0.0);

	if (DiffuseToggle)
		glUniform1f(Diffuse, 1.0);
	else
		glUniform1f(Diffuse, 0.0);

	glUniformMatrix4fv(View, 1, GL_FALSE, V.mData);

	glUniformMatrix4fv(Project, 1, GL_FALSE, P.mData);

	glUniformMatrix4fv(NormalM, 1, GL_FALSE, N.mData);
	
	glUniformMatrix4fv(m_location,1,GL_FALSE, mat.mData);

	a_parameter_Light_componenet.set(1.0f, 1.0f, 1.0f, 0.0f);
	Global_U_component.set(0.0f, 1.0f, 0.0f, 0.0f);
	glUniform4f(a_parameter_LightID, a_parameter_Light_componenet[0], a_parameter_Light_componenet[1], a_parameter_Light_componenet[2], a_parameter_Light_componenet[3]);
	glUniform4f(GlobalU, Global_U_component[0], Global_U_component[1], Global_U_component[2], Global_U_component[3]);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth[0], texheight[0], 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindVertexArray(VAO);

	// Draw the triangle strips using 0xFFFF as a restart
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP,indexLength,GL_UNSIGNED_INT,(void*)0);

	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth[1], texheight[1], 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata[1]);

	glUniform1f(SkyOnly, 1.0);
	glBindVertexArray(VAOSky);
	glUniformMatrix4fv(m_location, 1, GL_FALSE, SkySphere.mData);
	glDrawElements(GL_TRIANGLE_STRIP, indexLength, GL_UNSIGNED_INT, (void*)0);
	glUniform1f(SkyOnly, 0.0);

	glActiveTexture(GL_TEXTURE2);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth[2], texheight[2], 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata[2]);

	glBindVertexArray(Spheres[0]);
	glUniformMatrix4fv(m_location, 1, GL_FALSE, SpherePlacement[0].mData);
	glDrawElements(GL_TRIANGLE_STRIP, indexLength, GL_UNSIGNED_INT, (void*)0);

	glActiveTexture(GL_TEXTURE3);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth[3], texheight[3], 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata[3]);

	glBindVertexArray(Spheres[1]);
	glUniformMatrix4fv(m_location, 1, GL_FALSE, SpherePlacement[1].mData);
	glDrawElements(GL_TRIANGLE_STRIP, indexLength, GL_UNSIGNED_INT, (void*)0);

	glActiveTexture(GL_TEXTURE4);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth[4], texheight[4], 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata[4]);

	glBindVertexArray(Spheres[2]);
	glUniformMatrix4fv(m_location, 1, GL_FALSE, SpherePlacement[2].mData);
	glDrawElements(GL_TRIANGLE_STRIP, indexLength, GL_UNSIGNED_INT, (void*)0);

	//Plane object binding
	glUniform1f(ColorOnly, 1.0);
	for (int n = 0; n < PlaneNumber; n++) {
		glBindVertexArray(PlaneStorage[n].PlaneInfo);
		glUniformMatrix4fv(m_location,1,GL_FALSE,PlaneStorage[n].M_Plane.mData);
		//Draws Planes
		glDrawElements(GL_TRIANGLE_STRIP,10,GL_UNSIGNED_INT,(void*)0);
	}
	glUniform1f(ColorOnly, 0.0);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

int main(int argc, char* argv[])
{
	GLFWwindow* mainwindow = NULL;
	GLuint PlaneProgram = NULL, Skyprogram = NULL, program = NULL, VAO = NULL, VAOSky = NULL;
	GLuint pos_location = NULL, color_location = NULL, m_location = NULL, normal_loc = NULL, a_parameter_LightID = NULL, texture_location = NULL, UV_location = NULL;
	
	//sets the matricies for bases and intermidiates
	srand(time(NULL));
	gmtl::identity(World);
	gmtl::identity(ViewMatrix);
	gmtl::identity(Perspective);
	gmtl::identity(NewView);
	gmtl::identity(NormalMatrix);
	gmtl::identity(SkySphere);
	NormalMatrix.setState(gmtl::Matrix44f::AFFINE);
	NewView.setState(gmtl::Matrix44f::AFFINE);
	M_O.setState(gmtl::Matrix44f::AFFINE);
	SkySphere.setState(gmtl::Matrix44f::AFFINE);
	ViewMatrix.setState(gmtl::Matrix44f::AFFINE);
	Perspective.setState(gmtl::Matrix44f::AFFINE);
	Perspective[0][0] = 1.0 / (FOV * AspectRatio);
	Perspective[1][1] = 1.0 / FOV;
	Perspective[2][2] = -(far + near)/(far - near);
	Perspective[2][3] = -2.0 * far * near / (far - near);
	Perspective[3][2] = -1.0;
	Perspective[3][3] = 0.0;
	
	mainwindow = setupWindow();

	program = setupShaderProgram((char*)"VertexShader.vert",(char*)"CombinedFrag.frag");

	
	init(program, pos_location, color_location, m_location, normal_loc, UV_location);
	
	//create the object according to the paramaters and place it into VAO
	CreateList(radius, meridians, paralles);
	VAO = setupDrawnObjects(pos_location, color_location, normal_loc,UV_location);
	CreateList(7.0, meridians, paralles);
	VAOSky = setupDrawnObjects(pos_location, color_location, normal_loc, UV_location);
	CreateList(.1, meridians, paralles);
	Spheres[0] = setupDrawnObjects(pos_location, color_location, normal_loc, UV_location);
	CreateList(.15, meridians, paralles);
	Spheres[1] = setupDrawnObjects(pos_location, color_location, normal_loc, UV_location);
	CreateList(.2, meridians, paralles);
	Spheres[2] = setupDrawnObjects(pos_location, color_location, normal_loc, UV_location);
	SphereRadius[0] = 1.6;
	SphereRadius[1] = 1.2;
	SphereRadius[2] = 1.4;
	X_Translate[0][3] = .2;
	SpherePlacement[0] = RandomMatrix() * X_Translate * X_Translate * X_Translate * X_Translate * X_Translate * X_Translate * X_Translate * X_Translate;
	SpherePlacement[1] = RandomMatrix() * X_Translate * X_Translate * X_Translate * X_Translate * X_Translate * X_Translate;
	SpherePlacement[2] = RandomMatrix() * X_Translate * X_Translate * X_Translate * X_Translate * X_Translate * X_Translate * X_Translate;

	//create identity matricies for rotation and movements and set them up according to their function
	gmtl::identity(z_Rotate);
	gmtl::identity(negz_Rotate);
	gmtl::identity(x_Rotate);
	gmtl::identity(negx_Rotate);
	gmtl::identity(y_Rotate);
	gmtl::identity(Negy_Rotate);
	gmtl::identity(Z_Translate);
	z_Rotate.setState(gmtl::Matrix44f::ORTHOGONAL);
	negz_Rotate.setState(gmtl::Matrix44f::ORTHOGONAL);
	x_Rotate.setState(gmtl::Matrix44f::ORTHOGONAL);
	negx_Rotate.setState(gmtl::Matrix44f::ORTHOGONAL);
	y_Rotate.setState(gmtl::Matrix44f::ORTHOGONAL);
	Negy_Rotate.setState(gmtl::Matrix44f::ORTHOGONAL);
	NegZ_Translate.setState(gmtl::Matrix44f::TRANS);
	Y_Translate.setState(gmtl::Matrix44f::TRANS);
	Z_Translate.setState(gmtl::Matrix44f::TRANS);
	X_Translate.setState(gmtl::Matrix44f::TRANS);
	NegX_Translate.setState(gmtl::Matrix44f::TRANS);
	z_Rotate[0][0] = z_Rotate[1][1] = gmtl::Math::cos(rotDelta);
	z_Rotate[0][1] = -gmtl::Math::sin(rotDelta);
	z_Rotate[1][0] = gmtl::Math::sin(rotDelta);
	negz_Rotate[0][0] = negz_Rotate[1][1] = gmtl::Math::cos(rotDelta);
	negz_Rotate[0][1] = -gmtl::Math::sin(-rotDelta);
	negz_Rotate[1][0] = gmtl::Math::sin(-rotDelta);
	x_Rotate[1][1] = x_Rotate[2][2] = gmtl::Math::cos(rotDelta);
	x_Rotate[1][2] = -gmtl::Math::sin(rotDelta);
	x_Rotate[2][1] = gmtl::Math::sin(rotDelta);
	negx_Rotate[1][1] = negx_Rotate[2][2] = gmtl::Math::cos(rotDelta);
	negx_Rotate[1][2] = -gmtl::Math::sin(-rotDelta);
	negx_Rotate[2][1] = gmtl::Math::sin(-rotDelta);
	y_Rotate[0][0] = y_Rotate[2][2] = gmtl::Math::cos(rotDelta);
	y_Rotate[0][2] = gmtl::Math::sin(rotDelta);
	y_Rotate[2][0] = -gmtl::Math::sin(rotDelta);
	Negy_Rotate[0][0] = Negy_Rotate[2][2] = gmtl::Math::cos(rotDelta);
	Negy_Rotate[0][2] = gmtl::Math::sin(-rotDelta);
	Negy_Rotate[2][0] = -gmtl::Math::sin(-rotDelta);
	Y_Translate[1][3] = -0.17;
	NegZ_Translate[2][3] = -0.1;
	Z_Translate[2][3] = 0.2; 
	negZ_Translate[2][3] = -0.4;
	NegX_Translate[0][3] = -radius - 0.1;
	ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * ViewMatrix;
	gmtl::invert(ViewMatrix);
	SkySphere[0][2] = ViewMatrix[0][2];
	SkySphere[1][2] = ViewMatrix[1][2];
	SkySphere[2][2] = ViewMatrix[2][2];
	gmtl::invert(ViewMatrix);
	NormalMatrix = ViewMatrix * World;
	NormalMatrix = gmtl::transpose(gmtl::invert(NormalMatrix));

	//create all the objects in the world
	for (int i = 0; i < PlaneNumber; i++) {
		PlaneStorage[i].PlaneInfo = CreatePlane(pos_location, color_location, normal_loc);
		PlaneStorage[i].M_Plane.setState(gmtl::Matrix44f::AFFINE);
		PlaneStorage[i].Loc_R.setState(gmtl::Matrix44f::AFFINE);
		PlaneStorage[i].Loc_R = RandomMatrix();
		PlaneStorage[i].Radius = PlaneRaduis + ((rand() % 3) / 10.0);
		X_Translate[0][3] = PlaneStorage[i].Radius;
		PlaneStorage[i].M_Plane = PlaneStorage[i].Loc_R * X_Translate;
		PlaneStorage[i].M_O = gmtl::identity(World);
		PlaneStorage[i].VerticalVelocity = 0;
	}

	LoadPPM((char*)"Saki.ppm", &texwidth[0], &texheight[0], &imagedata[0], 1);
	LoadPPM((char*)"FishingSky.ppm", &texwidth[1], &texheight[1], &imagedata[1], 1);
	LoadPPM((char*)"Shiboru.ppm", &texwidth[2], &texheight[2], &imagedata[2], 1);
	LoadPPM((char*)"FingerGuns.ppm", &texwidth[3], &texheight[3], &imagedata[3], 1);
	LoadPPM((char*)"TakeIt.ppm", &texwidth[4], &texheight[4], &imagedata[4], 1);

	texture = glGetUniformLocation(program, "Saki_Sphere");
	glUniform1i(texture, 0);

	texture = glGetUniformLocation(program, "Sky_Sphere");
	glUniform1i(texture, 1);

	texture = glGetUniformLocation(program, "Padoru");
	glUniform1i(texture, 2);

	texture = glGetUniformLocation(program, "Thoru");
	glUniform1i(texture, 3);

	texture = glGetUniformLocation(program, "Wagner");
	glUniform1i(texture, 4);

	while (!glfwWindowShouldClose(mainwindow))
	{
		// Call our function that performs opengl operations
		// This is where your changes for matrix and vertex manipulation should be. 
		if (cameraTwo) {
			Planeone.M_Plane = PlaneStorage[0].Loc_R * X_Translate * Planeone.M_O;
			ViewMatrix = negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * negZ_Translate * NewView * gmtl::invert(Planeone.M_Plane);
			gmtl::invert(Planeone.M_Plane);
		}
		Flocking();
		display(mainwindow, program, m_location, VAO, VAOSky, Spheres, SpherePlacement, SkySphere, World, ViewMatrix, Perspective, NormalMatrix, indexLength);
		/* Swap our buffers to make our changes visible */
		glfwSwapBuffers(mainwindow);
		glfwPollEvents();
	}
	glfwDestroyWindow(mainwindow);

	glUseProgram(NULL);

	glDisableVertexAttribArray(pos_location);
	glDisableVertexAttribArray(color_location);


	glDeleteProgram(program);
	glDeleteProgram(Skyprogram);
	glDeleteProgram(PlaneProgram);
	glDeleteVertexArrays(1, &VAO);

	return 0;
}