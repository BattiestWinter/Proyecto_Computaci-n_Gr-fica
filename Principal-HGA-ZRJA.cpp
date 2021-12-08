/*
Semestre 2022-1
Práctica : Iluminación
Cambios en el shader, en lugar de enviar la textura en el shader de fragmentos, enviaremos el finalcolor
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"


//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

const float PI = 3.14159265f;
const float toRadians = 3.14159265f / 180.0f;

//Variables adicionales
float rotPieDer;
float rotPieIzq;
float rotPieDerOt;
float rotPieIzqOt;
int muevex_int;
int muevez_int;
int rotAvatar_int;
float movPieDer;
float movPieIzq;
float movPieDerY;
float movPieIzqY;
float countSB;
float movOffset;

float countGF;
float vGift01;
float vGift02;
float vGift03;
float vGift04;
float marcaR;
float GiftY;
float rGift01Z = 0.0;
float rGift02Z = 0.0;
float rGift03Z = 0.0;
float rGift01Y = 0.0;
float rGift02Y = 0.0;
float rGift03Y = 0.0;
float countNep = -1200.0;
float rotBrazoNep = 0.0;
float rotBrazoNep2 = 0.0;
float movBrazoNep = 0.0;
float movBrazoNep2 = 0.0;
float marcaGary;
float countGary;
float rotOjoDer;
float rotOjoIzq;
float movGary;
float rotGary;
float rGary;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

float reproduciranimacion2, habilitaranimacion2, guardoFrame2, reinicioFrame2, ciclo3, ciclo4, contador2 = 0;

//Regalos Navidad
Texture Regalo01;
Texture Regalo02;
Texture Regalo03;
Texture Regalo04;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;


Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;

Model De_Lorean;

Model helicoptero;
Model helice;

//Characters
Model mKrabs;
Model SpongeBob;
Model SquidWard;
Model PatrickStar;
Model Gary;
Model GaryOjoDer;
Model GaryOjoIzq;
Model Plankton;
Model Karen;
Model Sandy;
Model ArenitaCola;
Model ArenitaPieDer;
Model ArenitaPieIzq;
Model Santa;
Model Neptuno;
Model NeptunoBrazoDer;
Model ManRayTorso;
Model ManRayBrazo;
Model ManRayPiernas;
Model ManRayPies;
Model Geek;

//Buildings
Model KrustyKrab;
Model ChristmasTree;
Model SpongeBobHouse;
Model SquidwardHouse;
Model PatrickHouse;
Model Road;

//Objects
Model Bombilla;
Model Snowmans;
Model Snowman01;
Model Snowman02;
Model Lampara;
Model Sock;
Model Sock2;
Model Gift01;
Model Gift02;
Model Gift03;
Model SnowBall;

Skybox skybox;

//Skybox Bikini bottom
Skybox skyboxFBD;
Skybox skyboxFBN;

//materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};


	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.25f,  0.33f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.5f,	0.33f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.5f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.25f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.5f,  0.33f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.75f,	0.33f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.75f,	0.66f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.5f,	0.66f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.99f,	0.33f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.75f,	0.33f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.75f,	0.66f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.99f,	0.66f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.01f,	0.33f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.25f,  0.33f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.25f,	0.66f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.01f,	0.66f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.5f,	0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.75f,  0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.75f,	0.33f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.5f,	0.33f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.5f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.5f,   0.66f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.75f,	0.66f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.75f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


//APARTADO KEYFRAMES
bool animacion = false;

float rotPies = 0;
float rotPiernas = 0;
float movPiernas = 0;
float rotTorso = 0;
float movTorso = 0;
float rotBrazo1 = 0;
float rotBrazo2 = 0;
float rotBrazo3 = 0;
float movBrazo1Y = 0;
float movBrazo1Z = 0;
float movBrazo2X = 0;
float movBrazo2Y = 0;
float movBrazo3X = 0;
float movBrazo3Y = 0;
float scBall = 0;
float movBallY = 0;
float movBallZ = 0;
float rotGeek = 0;

bool animacion2 = false;

float rotColaVertical = 0;
float rotColaHorizontal = 0;
float movColaX = 0;
float movColaY = 0;
float movColaZ = 0;

#define MAX_FRAMES 40
int i_max_steps = 90;
int i_curr_steps = 39;

#define MAX_FRAMES2 40
int i_max_steps2 = 90;
int i_curr_steps2 = 40;

typedef struct _frame {
	float rotPies;
	float rotPiernas;
	float movPiernas;
	float rotTorso;
	float movTorso;
	float rotBrazo1;
	float rotBrazo2;
	float rotBrazo3;
	float movBrazo1Y;
	float movBrazo1Z;
	float movBrazo2X;
	float movBrazo2Y;
	float movBrazo3X;
	float movBrazo3Y;
	float scBall;
	float movBallY;
	float movBallZ;
	float rotGeek;
	float rotPies_INC;
	float rotPiernas_INC;
	float movPiernas_INC;
	float rotTorso_INC;
	float movTorso_INC;
	float rotBrazo1_INC;
	float rotBrazo2_INC;
	float rotBrazo3_INC;
	float movBrazo1Y_INC;
	float movBrazo1Z_INC;
	float movBrazo2X_INC;
	float movBrazo2Y_INC;
	float movBrazo3X_INC;
	float movBrazo3Y_INC;
	float scBall_INC;
	float movBallY_INC;
	float movBallZ_INC;
	float rotGeek_INC;
}FRAME;

typedef struct _frame2 {
	float rotColaVertical;
	float rotColaHorizontal;
	float movColaX;
	float movColaY;
	float movColaZ;
	float rotColaVertical_INC;
	float rotColaHorizontal_INC;
	float movColaX_INC;
	float movColaY_INC;
	float movColaZ_INC;
}FRAME2;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 39;
bool play = false;
int playIndex = 0;

FRAME2 KeyFrame2[MAX_FRAMES2];
int FrameIndex2 = 40;
bool play2 = false;
int playIndex2 = 0;

void saveFrame(void) {
	FrameIndex++;
}

void saveFrame2(void) {
	FrameIndex2++;
}

void resetElements(void) {
	rotPies = KeyFrame[0].rotPies;
	rotPiernas = KeyFrame[0].rotPiernas;
	movPiernas = KeyFrame[0].movPiernas;
	rotTorso = KeyFrame[0].rotTorso;
	movTorso = KeyFrame[0].movTorso;
	rotBrazo1 = KeyFrame[0].rotBrazo1;
	rotBrazo2 = KeyFrame[0].rotBrazo2;
	rotBrazo3 = KeyFrame[0].rotBrazo3;
	movBrazo1Y = KeyFrame[0].movBrazo1Y;
	movBrazo1Z = KeyFrame[0].movBrazo1Z;
	movBrazo2X = KeyFrame[0].movBrazo2X;
	movBrazo2Y = KeyFrame[0].movBrazo2Y;
	movBrazo3X = KeyFrame[0].movBrazo3X;
	movBrazo3Y = KeyFrame[0].movBrazo3Y;
	scBall = KeyFrame[0].scBall;
	movBallY = KeyFrame[0].movBallY;
	movBallZ = KeyFrame[0].movBallZ;
	rotGeek = KeyFrame[0].rotGeek;
}

void resetElements2(void) {
	rotColaVertical = KeyFrame2[0].rotColaVertical;
	rotColaHorizontal = KeyFrame2[0].rotColaHorizontal;
	movColaX = KeyFrame2[0].movColaX;
	movColaY = KeyFrame2[0].movColaY;
	movColaZ = KeyFrame2[0].movColaZ;
}

void interpolation(void) {
	KeyFrame[playIndex].rotPies_INC = (KeyFrame[playIndex + 1].rotPies - KeyFrame[playIndex].rotPies) / i_max_steps;
	KeyFrame[playIndex].rotPiernas_INC = (KeyFrame[playIndex + 1].rotPiernas - KeyFrame[playIndex].rotPiernas) / i_max_steps;
	KeyFrame[playIndex].movPiernas_INC = (KeyFrame[playIndex + 1].movPiernas - KeyFrame[playIndex].movPiernas) / i_max_steps;
	KeyFrame[playIndex].rotTorso_INC = (KeyFrame[playIndex + 1].rotTorso - KeyFrame[playIndex].rotTorso) / i_max_steps;
	KeyFrame[playIndex].movTorso_INC = (KeyFrame[playIndex + 1].movTorso - KeyFrame[playIndex].movTorso) / i_max_steps;
	KeyFrame[playIndex].rotBrazo1_INC = (KeyFrame[playIndex + 1].rotBrazo1 - KeyFrame[playIndex].rotBrazo1) / i_max_steps;
	KeyFrame[playIndex].rotBrazo2_INC = (KeyFrame[playIndex + 1].rotBrazo2 - KeyFrame[playIndex].rotBrazo2) / i_max_steps;
	KeyFrame[playIndex].rotBrazo3_INC = (KeyFrame[playIndex + 1].rotBrazo3 - KeyFrame[playIndex].rotBrazo3) / i_max_steps;
	KeyFrame[playIndex].movBrazo1Y_INC = (KeyFrame[playIndex + 1].movBrazo1Y - KeyFrame[playIndex].movBrazo1Y) / i_max_steps;
	KeyFrame[playIndex].movBrazo1Z_INC = (KeyFrame[playIndex + 1].movBrazo1Z - KeyFrame[playIndex].movBrazo1Z) / i_max_steps;
	KeyFrame[playIndex].movBrazo2X_INC = (KeyFrame[playIndex + 1].movBrazo2X - KeyFrame[playIndex].movBrazo2X) / i_max_steps;
	KeyFrame[playIndex].movBrazo2Y_INC = (KeyFrame[playIndex + 1].movBrazo2Y - KeyFrame[playIndex].movBrazo2Y) / i_max_steps;
	KeyFrame[playIndex].movBrazo3X_INC = (KeyFrame[playIndex + 1].movBrazo3X - KeyFrame[playIndex].movBrazo3X) / i_max_steps;
	KeyFrame[playIndex].movBrazo3Y_INC = (KeyFrame[playIndex + 1].movBrazo3Y - KeyFrame[playIndex].movBrazo3Y) / i_max_steps;
	KeyFrame[playIndex].scBall_INC = (KeyFrame[playIndex + 1].scBall - KeyFrame[playIndex].scBall) / i_max_steps;
	KeyFrame[playIndex].movBallY_INC = (KeyFrame[playIndex + 1].movBallY - KeyFrame[playIndex].movBallY) / i_max_steps;
	KeyFrame[playIndex].movBallZ_INC = (KeyFrame[playIndex + 1].movBallZ - KeyFrame[playIndex].movBallZ) / i_max_steps;
	KeyFrame[playIndex].rotGeek_INC = (KeyFrame[playIndex + 1].rotGeek - KeyFrame[playIndex].rotGeek) / i_max_steps;
}

void interpolation2(void) {
	KeyFrame2[playIndex2].rotColaVertical_INC = (KeyFrame2[playIndex2 + 1].rotColaVertical - KeyFrame2[playIndex2].rotColaVertical) / i_max_steps2;
	KeyFrame2[playIndex2].rotColaHorizontal_INC = (KeyFrame2[playIndex2 + 1].rotColaHorizontal - KeyFrame2[playIndex2].rotColaHorizontal) / i_max_steps2;
	KeyFrame2[playIndex2].movColaX_INC = (KeyFrame2[playIndex2 + 1].movColaX - KeyFrame2[playIndex2].movColaX) / i_max_steps2;
	KeyFrame2[playIndex2].movColaY_INC = (KeyFrame2[playIndex2 + 1].movColaY - KeyFrame2[playIndex2].movColaY) / i_max_steps2;
	KeyFrame2[playIndex2].movColaZ_INC = (KeyFrame2[playIndex2 + 1].movColaZ - KeyFrame2[playIndex2].movColaZ) / i_max_steps2;
}

void animate(void) {
	if (play) {
		if (i_curr_steps >= i_max_steps) {
			playIndex++;
			if (playIndex > FrameIndex - 2) {
				playIndex = 0;
				play = false;
			}
			else {
				i_curr_steps = 0;
				interpolation();
			}
		}
		else {
			rotPies += KeyFrame[playIndex].rotPies_INC;
			rotPiernas += KeyFrame[playIndex].rotPiernas_INC;
			movPiernas += KeyFrame[playIndex].movPiernas_INC;
			rotTorso += KeyFrame[playIndex].rotTorso_INC;
			movTorso += KeyFrame[playIndex].movTorso_INC;
			rotBrazo1 += KeyFrame[playIndex].rotBrazo1_INC;
			rotBrazo2 += KeyFrame[playIndex].rotBrazo2_INC;
			rotBrazo3 += KeyFrame[playIndex].rotBrazo3_INC;
			movBrazo1Y += KeyFrame[playIndex].movBrazo1Y_INC;
			movBrazo1Z += KeyFrame[playIndex].movBrazo1Z_INC;
			movBrazo2X += KeyFrame[playIndex].movBrazo2X_INC;
			movBrazo2Y += KeyFrame[playIndex].movBrazo2Y_INC;
			movBrazo3X += KeyFrame[playIndex].movBrazo3X_INC;
			movBrazo3Y += KeyFrame[playIndex].movBrazo3Y_INC;
			scBall += KeyFrame[playIndex].scBall_INC;
			movBallY += KeyFrame[playIndex].movBallY_INC;
			movBallZ += KeyFrame[playIndex].movBallZ_INC;
			rotGeek += KeyFrame[playIndex].rotGeek_INC;
			i_curr_steps++;
		}
	}
}

void animate2(void) {
	if (play2) {
		if (i_curr_steps2 >= i_max_steps2) {
			playIndex2++;
			if (playIndex2 > FrameIndex2 - 2) {
				playIndex2 = 0;
				play2 = false;
			}
			else {
				i_curr_steps2 = 0;
				interpolation2();
			}
		}
		else {
			rotColaVertical += KeyFrame2[playIndex2].rotColaVertical_INC;
			rotColaHorizontal += KeyFrame2[playIndex2].rotColaHorizontal_INC;
			movColaX += KeyFrame2[playIndex2].movColaX_INC;
			movColaY += KeyFrame2[playIndex2].movColaY_INC;
			movColaZ += KeyFrame2[playIndex2].movColaZ_INC;
			i_curr_steps2++;
		}
	}
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 2.0f, 0.5f); //penultimo: 0.5

	//Texturas Regalos
	Regalo01 = Texture("Textures/Regalo_Navidad_01.tga");
	Regalo01.LoadTextureA();

	Regalo02 = Texture("Textures/Regalo_Navidad_02.tga");
	Regalo02.LoadTextureA();

	Regalo03 = Texture("Textures/Regalo_Navidad_03.tga");
	Regalo03.LoadTextureA();

	Regalo04 = Texture("Textures/Regalo_Navidad_04.tga");
	Regalo04.LoadTextureA();

	/*brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();*/
	//pisoTexture = Texture("Textures/piso.tga");
	pisoTexture = Texture("Textures/SnowFloor.tga");
	pisoTexture.LoadTextureA();
	/*Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");

	De_Lorean = Model();
	De_Lorean.LoadModel("Models/Renault.obj");

	Dado_M = Model();
	Dado_M.LoadModel("Models/dadoanimales.obj");

	helicoptero = Model();
	helicoptero.LoadModel("Models/helicoptero.obj");

	helice = Model();
	helice.LoadModel("Models/helice.obj");*/

	//Characters
	mKrabs = Model();
	mKrabs.LoadModel("Models/mKrabs.obj");

	SpongeBob = Model();
	SpongeBob.LoadModel("Models/SpongeBob.obj");

	SquidWard = Model();
	SquidWard.LoadModel("Models/SquidWard.obj");

	PatrickStar = Model();
	PatrickStar.LoadModel("Models/PatrickStarV2.obj");

	Gary = Model();
	Gary.LoadModel("Models/Gary2.obj");

	GaryOjoDer = Model();
	GaryOjoDer.LoadModel("Models/Gary2OjoDer.obj");

	GaryOjoIzq = Model();
	GaryOjoIzq.LoadModel("Models/Gary2OjoIzq.obj");

	Plankton = Model();
	Plankton.LoadModel("Models/Plankton.obj");

	Karen = Model();
	Karen.LoadModel("Models/Karen.obj");

	Sandy = Model();
	Sandy.LoadModel("Models/Arenita2.obj");

	ArenitaCola = Model();
	ArenitaCola.LoadModel("Models/Arenita2Cola.obj");

	ArenitaPieDer = Model();
	ArenitaPieDer.LoadModel("Models/ArenitaPieDer.obj");

	ArenitaPieIzq = Model();
	ArenitaPieIzq.LoadModel("Models/ArenitaPieIzq.obj");

	Santa = Model();
	Santa.LoadModel("Models/Santa.obj");

	Neptuno = Model();
	Neptuno.LoadModel("Models/Neptuno.obj");

	NeptunoBrazoDer = Model();
	NeptunoBrazoDer.LoadModel("Models/NeptunoBrazoDer.obj");

	ManRayTorso = Model();
	ManRayTorso.LoadModel("Models/ManRayTorso.obj");

	ManRayBrazo = Model();
	ManRayBrazo.LoadModel("Models/ManRayBrazo.obj");

	ManRayPiernas = Model();
	ManRayPiernas.LoadModel("Models/ManRayPiernas.obj");

	ManRayPies = Model();
	ManRayPies.LoadModel("Models/ManRayPies.obj");

	Geek = Model();
	Geek.LoadModel("Models/Geek.obj");

	//Buildings
	KrustyKrab = Model();
	KrustyKrab.LoadModel("Models/KrustyKrab5.obj");

	SpongeBobHouse = Model();
	SpongeBobHouse.LoadModel("Models/SpongeBobHouse.obj");

	SquidwardHouse = Model();
	SquidwardHouse.LoadModel("Models/SquidwardHouse.obj");

	PatrickHouse = Model();
	PatrickHouse.LoadModel("Models/PatrickHouse.obj");

	Road = Model();
	Road.LoadModel("Models/Road.obj");

	//Objects
	ChristmasTree = Model();
	ChristmasTree.LoadModel("Models/ChristmasTree.obj");

	Bombilla = Model();
	Bombilla.LoadModel("Models/Bombilla.obj");

	Snowmans = Model();
	Snowmans.LoadModel("Models/Snowmans.obj");

	Snowman01 = Model();
	Snowman01.LoadModel("Models/Snowman01.obj");

	Snowman02 = Model();
	Snowman02.LoadModel("Models/Snowman02.obj");

	Lampara = Model();
	Lampara.LoadModel("Models/Lampara.obj");

	Sock = Model();
	Sock.LoadModel("Models/Sock.obj");

	Sock2 = Model();
	Sock2.LoadModel("Models/Sock2.obj");

	Gift01 = Model();
	Gift01.LoadModel("Models/Gift01.obj");

	Gift02 = Model();
	Gift02.LoadModel("Models/Gift02.obj");

	Gift03 = Model();
	Gift03.LoadModel("Models/Gift03.obj");

	SnowBall = Model();
	SnowBall.LoadModel("Models/SnowBall.obj");

	//Skyboxs
	std::vector<std::string> skyboxFacesFBD;
	skyboxFacesFBD.push_back("Textures/SkyboxSBD/bikini_bottom_rt.tga");
	skyboxFacesFBD.push_back("Textures/SkyboxSBD/bikini_bottom_lf.tga");
	skyboxFacesFBD.push_back("Textures/SkyboxSBD/bikini_bottom_dn.tga");
	skyboxFacesFBD.push_back("Textures/SkyboxSBD/bikini_bottom_up.tga");
	skyboxFacesFBD.push_back("Textures/SkyboxSBD/bikini_bottom_bk.tga");
	skyboxFacesFBD.push_back("Textures/SkyboxSBD/bikini_bottom_ft.tga");

	skyboxFBD = Skybox(skyboxFacesFBD);

	std::vector<std::string> skyboxFacesFBN;
	skyboxFacesFBN.push_back("Textures/SkyboxSBN/bikini_bottom_rt.tga");
	skyboxFacesFBN.push_back("Textures/SkyboxSBN/bikini_bottom_lf.tga");
	skyboxFacesFBN.push_back("Textures/SkyboxSBN/bikini_bottom_dn.tga");
	skyboxFacesFBN.push_back("Textures/SkyboxSBN/bikini_bottom_up.tga");
	skyboxFacesFBN.push_back("Textures/SkyboxSBN/bikini_bottom_bk.tga");
	skyboxFacesFBN.push_back("Textures/SkyboxSBN/bikini_bottom_ft.tga");

	skyboxFBN = Skybox(skyboxFacesFBN);

	/*std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);*/

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	////posición inicial del helicóptero
	//glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);

	////posición inicial del faro
	//glm::vec3 posfaro = glm::vec3(-7.0f, 6.0f, 2.0f);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		100.0f, 1.0f,
		400.0f, 50.0f, -300.0f,
		0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	//Declaración de segunda luz puntual
	pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
		100.0f, 1.0f,
		400.0f, 50.0f, -400.0f,
		0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	//Declaración de tercera luz puntual
	pointLights[2] = PointLight(1.0f, 0.0f, 0.0f,
		100.0f, 1.0f,
		400.0f, 50.0f, -200.0f,
		0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	unsigned int spotLightCount = 0;
	//Declaración de primera luz direccional
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		400.0f, 0.0f, -300.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	//pointLightCount++;

	
	////linterna
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	5.0f);
	////spotLightCount++;

	//luz fija
	/*spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;*/
	
	////luz de helicóptero
	//spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
	//	1.0f, 2.0f,
	//	-20.0f, 6.0f, -1.0,
	//	0.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	15.0f);
	//spotLightCount++;
		
	////luz de faro1
	//spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
	//	1.0f, 2.0f,
	//	-5.0f, 1.8f, 1.65f,
	//	-1.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	15.0f);
	//spotLightCount++;
	
	//luz de faro2
	/*spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		-5.0f, 1.8f, -2.95f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;*/

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	//Inicialización de variables
	countSB = 0.0;
	movOffset = 0.5f;
	countGF = -2400.0;
	marcaR = 0.0;
	//marcaR = mainWindow.getTeclaC();
	GiftY = 0.0;
	//marcaGary = 1.0;
	countGary = 0.0;
	movGary = 0.0;

	//KEYFRAMES

	//Animación ManRay
	KeyFrame[0].rotPies = 6;
	KeyFrame[0].rotPiernas = -8;
	KeyFrame[0].movPiernas = 1.2;
	KeyFrame[0].rotTorso = 9;
	KeyFrame[0].movTorso = -1.6;
	KeyFrame[0].rotBrazo1 = 0;
	KeyFrame[0].rotBrazo2 = 0;
	KeyFrame[0].rotBrazo3 = 0;
	KeyFrame[0].movBrazo1Y = 0;
	KeyFrame[0].movBrazo1Z = 0;
	KeyFrame[0].movBrazo2X = 0;
	KeyFrame[0].movBrazo2Y = 0;
	KeyFrame[0].movBrazo3X = 0;
	KeyFrame[0].movBrazo3Y = 0;
	KeyFrame[0].scBall = 0;
	KeyFrame[0].movBallY = 0;
	KeyFrame[0].movBallZ = 0;
	KeyFrame[0].rotGeek = 0;

	KeyFrame[1].rotPies = 12;
	KeyFrame[1].rotPiernas = -16;
	KeyFrame[1].movPiernas = 2.4;
	KeyFrame[1].rotTorso = 18;
	KeyFrame[1].movTorso = -3.2;
	KeyFrame[1].rotBrazo1 = 0;
	KeyFrame[1].rotBrazo2 = 0;
	KeyFrame[1].rotBrazo3 = 0;
	KeyFrame[1].movBrazo1Y = 0;
	KeyFrame[1].movBrazo1Z = 0;
	KeyFrame[1].movBrazo2X = 0;
	KeyFrame[1].movBrazo2Y = 0;
	KeyFrame[1].movBrazo3X = 0;
	KeyFrame[1].movBrazo3Y = 0;
	KeyFrame[1].scBall = 0;
	KeyFrame[1].movBallY = 0;
	KeyFrame[1].movBallZ = 0;
	KeyFrame[1].rotGeek = 0;

	KeyFrame[2].rotPies = 18;
	KeyFrame[2].rotPiernas = -24;
	KeyFrame[2].movPiernas = 3.6;
	KeyFrame[2].rotTorso = 27;
	KeyFrame[2].movTorso = -4.8;
	KeyFrame[2].rotBrazo1 = 0;
	KeyFrame[2].rotBrazo2 = 0;
	KeyFrame[2].rotBrazo3 = 0;
	KeyFrame[2].movBrazo1Y = 0;
	KeyFrame[2].movBrazo1Z = 0;
	KeyFrame[2].movBrazo2X = 0;
	KeyFrame[2].movBrazo2Y = 0;
	KeyFrame[2].movBrazo3X = 0;
	KeyFrame[2].movBrazo3Y = 0;
	KeyFrame[2].scBall = 0;
	KeyFrame[2].movBallY = 0;
	KeyFrame[2].movBallZ = 0;
	KeyFrame[2].rotGeek = 0;

	KeyFrame[3].rotPies = 24;
	KeyFrame[3].rotPiernas = -32;
	KeyFrame[3].movPiernas = 4.8;
	KeyFrame[3].rotTorso = 36;
	KeyFrame[3].movTorso = -6.4;
	KeyFrame[3].rotBrazo1 = 0;
	KeyFrame[3].rotBrazo2 = 0;
	KeyFrame[3].rotBrazo3 = 0;
	KeyFrame[3].movBrazo1Y = 0;
	KeyFrame[3].movBrazo1Z = 0;
	KeyFrame[3].movBrazo2X = 0;
	KeyFrame[3].movBrazo2Y = 0;
	KeyFrame[3].movBrazo3X = 0;
	KeyFrame[3].movBrazo3Y = 0;
	KeyFrame[3].scBall = 0;
	KeyFrame[3].movBallY = 0;
	KeyFrame[3].movBallZ = 0;
	KeyFrame[3].rotGeek = 0;

	KeyFrame[4].rotPies = 30;
	KeyFrame[4].rotPiernas = -40;
	KeyFrame[4].movPiernas = 6;
	KeyFrame[4].rotTorso = 45;
	KeyFrame[4].movTorso = -8;
	KeyFrame[4].rotBrazo1 = 0;
	KeyFrame[4].rotBrazo2 = 0;
	KeyFrame[4].rotBrazo3 = 0;
	KeyFrame[4].movBrazo1Y = 0;
	KeyFrame[4].movBrazo1Z = 0;
	KeyFrame[4].movBrazo2X = 0;
	KeyFrame[4].movBrazo2Y = 0;
	KeyFrame[4].movBrazo3X = 0;
	KeyFrame[4].movBrazo3Y = 0;
	KeyFrame[4].scBall = 0;
	KeyFrame[4].movBallY = 0;
	KeyFrame[4].movBallZ = 0;
	KeyFrame[4].rotGeek = 0;

	KeyFrame[5].rotPies = 30;
	KeyFrame[5].rotPiernas = -40;
	KeyFrame[5].movPiernas = 6;
	KeyFrame[5].rotTorso = 45;
	KeyFrame[5].movTorso = -8;
	KeyFrame[5].rotBrazo1 = 0;
	KeyFrame[5].rotBrazo2 = 0;
	KeyFrame[5].rotBrazo3 = 0;
	KeyFrame[5].movBrazo1Y = 0;
	KeyFrame[5].movBrazo1Z = 0;
	KeyFrame[5].movBrazo2X = 0;
	KeyFrame[5].movBrazo2Y = 0;
	KeyFrame[5].movBrazo3X = 0;
	KeyFrame[5].movBrazo3Y = 0;
	KeyFrame[5].scBall = 0.5;
	KeyFrame[5].movBallY = 0;
	KeyFrame[5].movBallZ = 0;
	KeyFrame[5].rotGeek = 0;

	KeyFrame[6].rotPies = 30;
	KeyFrame[6].rotPiernas = -40;
	KeyFrame[6].movPiernas = 6;
	KeyFrame[6].rotTorso = 45;
	KeyFrame[6].movTorso = -8;
	KeyFrame[6].rotBrazo1 = 0;
	KeyFrame[6].rotBrazo2 = 0;
	KeyFrame[6].rotBrazo3 = 0;
	KeyFrame[6].movBrazo1Y = 0;
	KeyFrame[6].movBrazo1Z = 0;
	KeyFrame[6].movBrazo2X = 0;
	KeyFrame[6].movBrazo2Y = 0;
	KeyFrame[6].movBrazo3X = 0;
	KeyFrame[6].movBrazo3Y = 0;
	KeyFrame[6].scBall = 0.5;
	KeyFrame[6].movBallY = 1.54;
	KeyFrame[6].movBallZ = 0;
	KeyFrame[6].rotGeek = 0;

	KeyFrame[7].rotPies = 30;
	KeyFrame[7].rotPiernas = -40;
	KeyFrame[7].movPiernas = 6;
	KeyFrame[7].rotTorso = 45;
	KeyFrame[7].movTorso = -8;
	KeyFrame[7].rotBrazo1 = 0;
	KeyFrame[7].rotBrazo2 = 0;
	KeyFrame[7].rotBrazo3 = 0;
	KeyFrame[7].movBrazo1Y = 0;
	KeyFrame[7].movBrazo1Z = 0;
	KeyFrame[7].movBrazo2X = 0;
	KeyFrame[7].movBrazo2Y = 0;
	KeyFrame[7].movBrazo3X = 0;
	KeyFrame[7].movBrazo3Y = 0;
	KeyFrame[7].scBall = 0.5;
	KeyFrame[7].movBallY = 3.08;
	KeyFrame[7].movBallZ = 0;
	KeyFrame[7].rotGeek = 0;

	KeyFrame[8].rotPies = 30;
	KeyFrame[8].rotPiernas = -40;
	KeyFrame[8].movPiernas = 6;
	KeyFrame[8].rotTorso = 45;
	KeyFrame[8].movTorso = -8;
	KeyFrame[8].rotBrazo1 = 0;
	KeyFrame[8].rotBrazo2 = 0;
	KeyFrame[8].rotBrazo3 = 0;
	KeyFrame[8].movBrazo1Y = 0;
	KeyFrame[8].movBrazo1Z = 0;
	KeyFrame[8].movBrazo2X = 0;
	KeyFrame[8].movBrazo2Y = 0;
	KeyFrame[8].movBrazo3X = 0;
	KeyFrame[8].movBrazo3Y = 0;
	KeyFrame[8].scBall = 0.5;
	KeyFrame[8].movBallY = 4.62;
	KeyFrame[8].movBallZ = 0;
	KeyFrame[8].rotGeek = 0;

	KeyFrame[9].rotPies = 30;
	KeyFrame[9].rotPiernas = -40;
	KeyFrame[9].movPiernas = 6;
	KeyFrame[9].rotTorso = 45;
	KeyFrame[9].movTorso = -8;
	KeyFrame[9].rotBrazo1 = 0;
	KeyFrame[9].rotBrazo2 = 0;
	KeyFrame[9].rotBrazo3 = 0;
	KeyFrame[9].movBrazo1Y = 0;
	KeyFrame[9].movBrazo1Z = 0;
	KeyFrame[9].movBrazo2X = 0;
	KeyFrame[9].movBrazo2Y = 0;
	KeyFrame[9].movBrazo3X = 0;
	KeyFrame[9].movBrazo3Y = 0;
	KeyFrame[9].scBall = 0.5;
	KeyFrame[9].movBallY = 6.16;
	KeyFrame[9].movBallZ = 0;
	KeyFrame[9].rotGeek = 0;

	KeyFrame[10].rotPies = 30;
	KeyFrame[10].rotPiernas = -40;
	KeyFrame[10].movPiernas = 6;
	KeyFrame[10].rotTorso = 45;
	KeyFrame[10].movTorso = -8;
	KeyFrame[10].rotBrazo1 = 0;
	KeyFrame[10].rotBrazo2 = 0;
	KeyFrame[10].rotBrazo3 = 0;
	KeyFrame[10].movBrazo1Y = 0;
	KeyFrame[10].movBrazo1Z = 0;
	KeyFrame[10].movBrazo2X = 0;
	KeyFrame[10].movBrazo2Y = 0;
	KeyFrame[10].movBrazo3X = 0;
	KeyFrame[10].movBrazo3Y = 0;
	KeyFrame[10].scBall = 0.5;
	KeyFrame[10].movBallY = 7.7;
	KeyFrame[10].movBallZ = 0;
	KeyFrame[10].rotGeek = 0;

	KeyFrame[11].rotPies = 24;
	KeyFrame[11].rotPiernas = -32;
	KeyFrame[11].movPiernas = 4.8;
	KeyFrame[11].rotTorso = 36;
	KeyFrame[11].movTorso = -6.4;
	KeyFrame[11].rotBrazo1 = 0;
	KeyFrame[11].rotBrazo2 = 0;
	KeyFrame[11].rotBrazo3 = 0;
	KeyFrame[11].movBrazo1Y = 0;
	KeyFrame[11].movBrazo1Z = 0;
	KeyFrame[11].movBrazo2X = 0;
	KeyFrame[11].movBrazo2Y = 0;
	KeyFrame[11].movBrazo3X = 0;
	KeyFrame[11].movBrazo3Y = 0;
	KeyFrame[11].scBall = 0.5;
	KeyFrame[11].movBallY = 8.34;
	KeyFrame[11].movBallZ = 0;
	KeyFrame[11].rotGeek = 0;

	KeyFrame[12].rotPies = 18;
	KeyFrame[12].rotPiernas = -24;
	KeyFrame[12].movPiernas = 3.6;
	KeyFrame[12].rotTorso = 27;
	KeyFrame[12].movTorso = -4.8;
	KeyFrame[12].rotBrazo1 = 0;
	KeyFrame[12].rotBrazo2 = 0;
	KeyFrame[12].rotBrazo3 = 0;
	KeyFrame[12].movBrazo1Y = 0;
	KeyFrame[12].movBrazo1Z = 0;
	KeyFrame[12].movBrazo2X = 0;
	KeyFrame[12].movBrazo2Y = 0;
	KeyFrame[12].movBrazo3X = 0;
	KeyFrame[12].movBrazo3Y = 0;
	KeyFrame[12].scBall = 0.5;
	KeyFrame[12].movBallY = 8.98;
	KeyFrame[12].movBallZ = 0;
	KeyFrame[12].rotGeek = 0;

	KeyFrame[13].rotPies = 12;
	KeyFrame[13].rotPiernas = -16;
	KeyFrame[13].movPiernas = 2.4;
	KeyFrame[13].rotTorso = 18;
	KeyFrame[13].movTorso = -3.2;
	KeyFrame[13].rotBrazo1 = 0;
	KeyFrame[13].rotBrazo2 = 0;
	KeyFrame[13].rotBrazo3 = 0;
	KeyFrame[13].movBrazo1Y = 0;
	KeyFrame[13].movBrazo1Z = 0;
	KeyFrame[13].movBrazo2X = 0;
	KeyFrame[13].movBrazo2Y = 0;
	KeyFrame[13].movBrazo3X = 0;
	KeyFrame[13].movBrazo3Y = 0;
	KeyFrame[13].scBall = 0.5;
	KeyFrame[13].movBallY = 9.62;
	KeyFrame[13].movBallZ = 0;
	KeyFrame[13].rotGeek = 0;

	KeyFrame[14].rotPies = 6;
	KeyFrame[14].rotPiernas = -8;
	KeyFrame[14].movPiernas = 1.2;
	KeyFrame[14].rotTorso = 9;
	KeyFrame[14].movTorso = -1.6;
	KeyFrame[14].rotBrazo1 = 0;
	KeyFrame[14].rotBrazo2 = 0;
	KeyFrame[14].rotBrazo3 = 0;
	KeyFrame[14].movBrazo1Y = 0;
	KeyFrame[14].movBrazo1Z = 0;
	KeyFrame[14].movBrazo2X = 0;
	KeyFrame[14].movBrazo2Y = 0;
	KeyFrame[14].movBrazo3X = 0;
	KeyFrame[14].movBrazo3Y = 0;
	KeyFrame[14].scBall = 0.5;
	KeyFrame[14].movBallY = 10.26;
	KeyFrame[14].movBallZ = 0;
	KeyFrame[14].rotGeek = 0;

	KeyFrame[15].rotPies = 0;
	KeyFrame[15].rotPiernas = 0;
	KeyFrame[15].movPiernas = 0;
	KeyFrame[15].rotTorso = 0;
	KeyFrame[15].movTorso = 0;
	KeyFrame[15].rotBrazo1 = 0;
	KeyFrame[15].rotBrazo2 = 0;
	KeyFrame[15].rotBrazo3 = 0;
	KeyFrame[15].movBrazo1Y = 0;
	KeyFrame[15].movBrazo1Z = 0;
	KeyFrame[15].movBrazo2X = 0;
	KeyFrame[15].movBrazo2Y = 0;
	KeyFrame[15].movBrazo3X = 0;
	KeyFrame[15].movBrazo3Y = 0;
	KeyFrame[15].scBall = 0.5;
	KeyFrame[15].movBallY = 10.9;
	KeyFrame[15].movBallZ = 0;
	KeyFrame[15].rotGeek = 0;

	KeyFrame[16].rotPies = 0;
	KeyFrame[16].rotPiernas = 0;
	KeyFrame[16].movPiernas = 0;
	KeyFrame[16].rotTorso = 0;
	KeyFrame[16].movTorso = 0;
	KeyFrame[16].rotBrazo1 = -45;
	KeyFrame[16].rotBrazo2 = 0;
	KeyFrame[16].rotBrazo3 = 0;
	KeyFrame[16].movBrazo1Y = 4.5;
	KeyFrame[16].movBrazo1Z = 9.75;
	KeyFrame[16].movBrazo2X = 0;
	KeyFrame[16].movBrazo2Y = 0;
	KeyFrame[16].movBrazo3X = 0;
	KeyFrame[16].movBrazo3Y = 0;
	KeyFrame[16].scBall = 0.5;
	KeyFrame[16].movBallY = 11.9;
	KeyFrame[16].movBallZ = 2;
	KeyFrame[16].rotGeek = 0;

	KeyFrame[17].rotPies = 0;
	KeyFrame[17].rotPiernas = 0;
	KeyFrame[17].movPiernas = 0;
	KeyFrame[17].rotTorso = 0;
	KeyFrame[17].movTorso = 0;
	KeyFrame[17].rotBrazo1 = -90;
	KeyFrame[17].rotBrazo2 = 0;
	KeyFrame[17].rotBrazo3 = 0;
	KeyFrame[17].movBrazo1Y = 14.5;
	KeyFrame[17].movBrazo1Z = 13.5;
	KeyFrame[17].movBrazo2X = 0;
	KeyFrame[17].movBrazo2Y = 0;
	KeyFrame[17].movBrazo3X = 0;
	KeyFrame[17].movBrazo3Y = 0;
	KeyFrame[17].scBall = 0.5;
	KeyFrame[17].movBallY = 14.4;
	KeyFrame[17].movBallZ = 3;
	KeyFrame[17].rotGeek = 0;

	KeyFrame[18].rotPies = 0;
	KeyFrame[18].rotPiernas = 0;
	KeyFrame[18].movPiernas = 0;
	KeyFrame[18].rotTorso = 0;
	KeyFrame[18].movTorso = 0;
	KeyFrame[18].rotBrazo1 = -90;
	KeyFrame[18].rotBrazo2 = 0;
	KeyFrame[18].rotBrazo3 = 0;
	KeyFrame[18].movBrazo1Y = 14.5;
	KeyFrame[18].movBrazo1Z = 13.5;
	KeyFrame[18].movBrazo2X = 0;
	KeyFrame[18].movBrazo2Y = 0;
	KeyFrame[18].movBrazo3X = 0;
	KeyFrame[18].movBrazo3Y = 0;
	KeyFrame[18].scBall = 0.5;
	KeyFrame[18].movBallY = 14.4;
	KeyFrame[18].movBallZ = 3;
	KeyFrame[18].rotGeek = 0;

	KeyFrame[19].rotPies = 0;
	KeyFrame[19].rotPiernas = 0;
	KeyFrame[19].movPiernas = 0;
	KeyFrame[19].rotTorso = 0;
	KeyFrame[19].movTorso = 0;
	KeyFrame[19].rotBrazo1 = -90;
	KeyFrame[19].rotBrazo2 = -11.25;
	KeyFrame[19].rotBrazo3 = 0;
	KeyFrame[19].movBrazo1Y = 14.5;
	KeyFrame[19].movBrazo1Z = 13.5;
	KeyFrame[19].movBrazo2X = -2.5;
	KeyFrame[19].movBrazo2Y = 0.5;
	KeyFrame[19].movBrazo3X = 0;
	KeyFrame[19].movBrazo3Y = 0;
	KeyFrame[19].scBall = 0.5;
	KeyFrame[19].movBallY = 14.4;
	KeyFrame[19].movBallZ = 1;
	KeyFrame[19].rotGeek = 0;

	KeyFrame[20].rotPies = 0;
	KeyFrame[20].rotPiernas = 0;
	KeyFrame[20].movPiernas = 0;
	KeyFrame[20].rotTorso = 0;
	KeyFrame[20].movTorso = 0;
	KeyFrame[20].rotBrazo1 = -90;
	KeyFrame[20].rotBrazo2 = -22.5;
	KeyFrame[20].rotBrazo3 = 0;
	KeyFrame[20].movBrazo1Y = 14.5;
	KeyFrame[20].movBrazo1Z = 13.5;
	KeyFrame[20].movBrazo2X = -5;
	KeyFrame[20].movBrazo2Y = 1;
	KeyFrame[20].movBrazo3X = 0;
	KeyFrame[20].movBrazo3Y = 0;
	KeyFrame[20].scBall = 0.5;
	KeyFrame[20].movBallY = 14.4;
	KeyFrame[20].movBallZ = -0.5;
	KeyFrame[20].rotGeek = 0;

	KeyFrame[21].rotPies = 0;
	KeyFrame[21].rotPiernas = 0;
	KeyFrame[21].movPiernas = 0;
	KeyFrame[21].rotTorso = 0;
	KeyFrame[21].movTorso = 0;
	KeyFrame[21].rotBrazo1 = -90;
	KeyFrame[21].rotBrazo2 = -33.75;
	KeyFrame[21].rotBrazo3 = 0;
	KeyFrame[21].movBrazo1Y = 14.5;
	KeyFrame[21].movBrazo1Z = 13.5;
	KeyFrame[21].movBrazo2X = -7.5;
	KeyFrame[21].movBrazo2Y = 1.5;
	KeyFrame[21].movBrazo3X = 0;
	KeyFrame[21].movBrazo3Y = 0;
	KeyFrame[21].scBall = 0.5;
	KeyFrame[21].movBallY = 14.4;
	KeyFrame[21].movBallZ = -1.2;
	KeyFrame[21].rotGeek = 0;

	KeyFrame[22].rotPies = 0;
	KeyFrame[22].rotPiernas = 0;
	KeyFrame[22].movPiernas = 0;
	KeyFrame[22].rotTorso = 0;
	KeyFrame[22].movTorso = 0;
	KeyFrame[22].rotBrazo1 = -90;
	KeyFrame[22].rotBrazo2 = -45;
	KeyFrame[22].rotBrazo3 = 0;
	KeyFrame[22].movBrazo1Y = 14.5;
	KeyFrame[22].movBrazo1Z = 13.5;
	KeyFrame[22].movBrazo2X = -10;
	KeyFrame[22].movBrazo2Y = 2;
	KeyFrame[22].movBrazo3X = 0;
	KeyFrame[22].movBrazo3Y = 0;
	KeyFrame[22].scBall = 0.5;
	KeyFrame[22].movBallY = 14.4;
	KeyFrame[22].movBallZ = -1.7;
	KeyFrame[22].rotGeek = 0;

	KeyFrame[23].rotPies = 0;
	KeyFrame[23].rotPiernas = 0;
	KeyFrame[23].movPiernas = 0;
	KeyFrame[23].rotTorso = 0;
	KeyFrame[23].movTorso = 0;
	KeyFrame[23].rotBrazo1 = -90;
	KeyFrame[23].rotBrazo2 = -45;
	KeyFrame[23].rotBrazo3 = 0;
	KeyFrame[23].movBrazo1Y = 14.5;
	KeyFrame[23].movBrazo1Z = 13.5;
	KeyFrame[23].movBrazo2X = -10;
	KeyFrame[23].movBrazo2Y = 2;
	KeyFrame[23].movBrazo3X = 0;
	KeyFrame[23].movBrazo3Y = 0;
	KeyFrame[23].scBall = 0.5;
	KeyFrame[23].movBallY = 14.4;
	KeyFrame[23].movBallZ = -1.7;
	KeyFrame[23].rotGeek = 0;

	KeyFrame[24].rotPies = 0;
	KeyFrame[24].rotPiernas = 0;
	KeyFrame[24].movPiernas = 0;
	KeyFrame[24].rotTorso = 0;
	KeyFrame[24].movTorso = 0;
	KeyFrame[24].rotBrazo1 = -90;
	KeyFrame[24].rotBrazo2 = -33.75;
	KeyFrame[24].rotBrazo3 = 0;
	KeyFrame[24].movBrazo1Y = 14.5;
	KeyFrame[24].movBrazo1Z = 13.5;
	KeyFrame[24].movBrazo2X = -7.5;
	KeyFrame[24].movBrazo2Y = 1.5;
	KeyFrame[24].movBrazo3X = 0;
	KeyFrame[24].movBrazo3Y = 0;
	KeyFrame[24].scBall = 0.5;
	KeyFrame[24].movBallY = 14.4;
	KeyFrame[24].movBallZ = -1.2;
	KeyFrame[24].rotGeek = 0;

	KeyFrame[25].rotPies = 0;
	KeyFrame[25].rotPiernas = 0;
	KeyFrame[25].movPiernas = 0;
	KeyFrame[25].rotTorso = 0;
	KeyFrame[25].movTorso = 0;
	KeyFrame[25].rotBrazo1 = -90;
	KeyFrame[25].rotBrazo2 = -22.5;
	KeyFrame[25].rotBrazo3 = 0;
	KeyFrame[25].movBrazo1Y = 14.5;
	KeyFrame[25].movBrazo1Z = 13.5;
	KeyFrame[25].movBrazo2X = -5;
	KeyFrame[25].movBrazo2Y = 1;
	KeyFrame[25].movBrazo3X = 0;
	KeyFrame[25].movBrazo3Y = 0;
	KeyFrame[25].scBall = 0.5;
	KeyFrame[25].movBallY = 14.4;
	KeyFrame[25].movBallZ = -0.5;
	KeyFrame[25].rotGeek = 0;

	KeyFrame[26].rotPies = 0;
	KeyFrame[26].rotPiernas = 0;
	KeyFrame[26].movPiernas = 0;
	KeyFrame[26].rotTorso = 0;
	KeyFrame[26].movTorso = 0;
	KeyFrame[26].rotBrazo1 = -90;
	KeyFrame[26].rotBrazo2 = -11.25;
	KeyFrame[26].rotBrazo3 = 0;
	KeyFrame[26].movBrazo1Y = 14.5;
	KeyFrame[26].movBrazo1Z = 13.5;
	KeyFrame[26].movBrazo2X = -2.5;
	KeyFrame[26].movBrazo2Y = 0.5;
	KeyFrame[26].movBrazo3X = 0;
	KeyFrame[26].movBrazo3Y = 0;
	KeyFrame[26].scBall = 0.5;
	KeyFrame[26].movBallY = 14.4;
	KeyFrame[26].movBallZ = 1;
	KeyFrame[26].rotGeek = 0;

	KeyFrame[27].rotPies = 0;
	KeyFrame[27].rotPiernas = 0;
	KeyFrame[27].movPiernas = 0;
	KeyFrame[27].rotTorso = 0;
	KeyFrame[27].movTorso = 0;
	KeyFrame[27].rotBrazo1 = -90;
	KeyFrame[27].rotBrazo2 = 0;
	KeyFrame[27].rotBrazo3 = 0;
	KeyFrame[27].movBrazo1Y = 14.5;
	KeyFrame[27].movBrazo1Z = 13.5;
	KeyFrame[27].movBrazo2X = 0;
	KeyFrame[27].movBrazo2Y = 0;
	KeyFrame[27].movBrazo3X = 0;
	KeyFrame[27].movBrazo3Y = 0;
	KeyFrame[27].scBall = 0.5;
	KeyFrame[27].movBallY = 14.4;
	KeyFrame[27].movBallZ = 3;
	KeyFrame[27].rotGeek = 0;

	KeyFrame[28].rotPies = 0;
	KeyFrame[28].rotPiernas = 0;
	KeyFrame[28].movPiernas = 0;
	KeyFrame[28].rotTorso = 0;
	KeyFrame[28].movTorso = 0;
	KeyFrame[28].rotBrazo1 = -90;
	KeyFrame[28].rotBrazo2 = 0;
	KeyFrame[28].rotBrazo3 = 11.25;
	KeyFrame[28].movBrazo1Y = 14.5;
	KeyFrame[28].movBrazo1Z = 13.5;
	KeyFrame[28].movBrazo2X = 0;
	KeyFrame[28].movBrazo2Y = 0;
	KeyFrame[28].movBrazo3X = 2.25;
	KeyFrame[28].movBrazo3Y = 1.5;
	KeyFrame[28].scBall = 0.5;
	KeyFrame[28].movBallY = 14.4;
	KeyFrame[28].movBallZ = 3.2;
	KeyFrame[28].rotGeek = 0;

	KeyFrame[29].rotPies = 0;
	KeyFrame[29].rotPiernas = 0;
	KeyFrame[29].movPiernas = 0;
	KeyFrame[29].rotTorso = 0;
	KeyFrame[29].movTorso = 0;
	KeyFrame[29].rotBrazo1 = -90;
	KeyFrame[29].rotBrazo2 = 0;
	KeyFrame[29].rotBrazo3 = 22.5;
	KeyFrame[29].movBrazo1Y = 14.5;
	KeyFrame[29].movBrazo1Z = 13.5;
	KeyFrame[29].movBrazo2X = 0;
	KeyFrame[29].movBrazo2Y = 0;
	KeyFrame[29].movBrazo3X = 4.5;
	KeyFrame[29].movBrazo3Y = 3;
	KeyFrame[29].scBall = 0.5;
	KeyFrame[29].movBallY = 14.4;
	KeyFrame[29].movBallZ = 8;
	KeyFrame[29].rotGeek = 0;

	KeyFrame[30].rotPies = 0;
	KeyFrame[30].rotPiernas = 0;
	KeyFrame[30].movPiernas = 0;
	KeyFrame[30].rotTorso = 0;
	KeyFrame[30].movTorso = 0;
	KeyFrame[30].rotBrazo1 = -90;
	KeyFrame[30].rotBrazo2 = 0;
	KeyFrame[30].rotBrazo3 = 33.75;
	KeyFrame[30].movBrazo1Y = 14.5;
	KeyFrame[30].movBrazo1Z = 13.5;
	KeyFrame[30].movBrazo2X = 0;
	KeyFrame[30].movBrazo2Y = 0;
	KeyFrame[30].movBrazo3X = 6.75;
	KeyFrame[30].movBrazo3Y = 4.5;
	KeyFrame[30].scBall = 0.5;
	KeyFrame[30].movBallY = 14.4;
	KeyFrame[30].movBallZ = 18;
	KeyFrame[30].rotGeek = 0;

	KeyFrame[31].rotPies = 0;
	KeyFrame[31].rotPiernas = 0;
	KeyFrame[31].movPiernas = 0;
	KeyFrame[31].rotTorso = 0;
	KeyFrame[31].movTorso = 0;
	KeyFrame[31].rotBrazo1 = -90;
	KeyFrame[31].rotBrazo2 = 0;
	KeyFrame[31].rotBrazo3 = 45;
	KeyFrame[31].movBrazo1Y = 14.5;
	KeyFrame[31].movBrazo1Z = 13.5;
	KeyFrame[31].movBrazo2X = 0;
	KeyFrame[31].movBrazo2Y = 0;
	KeyFrame[31].movBrazo3X = 9.0;
	KeyFrame[31].movBrazo3Y = 6;
	KeyFrame[31].scBall = 0.5;
	KeyFrame[31].movBallY = 14.4;
	KeyFrame[31].movBallZ = 28;
	KeyFrame[31].rotGeek = 0;

	KeyFrame[32].rotPies = 0;
	KeyFrame[32].rotPiernas = 0;
	KeyFrame[32].movPiernas = 0;
	KeyFrame[32].rotTorso = 0;
	KeyFrame[32].movTorso = 0;
	KeyFrame[32].rotBrazo1 = -90;
	KeyFrame[32].rotBrazo2 = 0;
	KeyFrame[32].rotBrazo3 = 45;
	KeyFrame[32].movBrazo1Y = 14.5;
	KeyFrame[32].movBrazo1Z = 13.5;
	KeyFrame[32].movBrazo2X = 0;
	KeyFrame[32].movBrazo2Y = 0;
	KeyFrame[32].movBrazo3X = 9.0;
	KeyFrame[32].movBrazo3Y = 6;
	KeyFrame[32].scBall = 0.5;
	KeyFrame[32].movBallY = 14.4;
	KeyFrame[32].movBallZ = 38;
	KeyFrame[32].rotGeek = 0;

	KeyFrame[33].rotPies = 0;
	KeyFrame[33].rotPiernas = 0;
	KeyFrame[33].movPiernas = 0;
	KeyFrame[33].rotTorso = 0;
	KeyFrame[33].movTorso = 0;
	KeyFrame[33].rotBrazo1 = -90;
	KeyFrame[33].rotBrazo2 = 0;
	KeyFrame[33].rotBrazo3 = 45;
	KeyFrame[33].movBrazo1Y = 14.5;
	KeyFrame[33].movBrazo1Z = 13.5;
	KeyFrame[33].movBrazo2X = 0;
	KeyFrame[33].movBrazo2Y = 0;
	KeyFrame[33].movBrazo3X = 9.0;
	KeyFrame[33].movBrazo3Y = 6;
	KeyFrame[33].scBall = 0.5;
	KeyFrame[33].movBallY = 14.4;
	KeyFrame[33].movBallZ = 48;
	KeyFrame[33].rotGeek = 0;

	KeyFrame[34].rotPies = 0;
	KeyFrame[34].rotPiernas = 0;
	KeyFrame[34].movPiernas = 0;
	KeyFrame[34].rotTorso = 0;
	KeyFrame[34].movTorso = 0;
	KeyFrame[34].rotBrazo1 = -90;
	KeyFrame[34].rotBrazo2 = 0;
	KeyFrame[34].rotBrazo3 = 45;
	KeyFrame[34].movBrazo1Y = 14.5;
	KeyFrame[34].movBrazo1Z = 13.5;
	KeyFrame[34].movBrazo2X = 0;
	KeyFrame[34].movBrazo2Y = 0;
	KeyFrame[34].movBrazo3X = 9.0;
	KeyFrame[34].movBrazo3Y = 6;
	KeyFrame[34].scBall = 0.5;
	KeyFrame[34].movBallY = 14.4;
	KeyFrame[34].movBallZ = 58;
	KeyFrame[34].rotGeek = 0;

	KeyFrame[35].rotPies = 0;
	KeyFrame[35].rotPiernas = 0;
	KeyFrame[35].movPiernas = 0;
	KeyFrame[35].rotTorso = 0;
	KeyFrame[35].movTorso = 0;
	KeyFrame[35].rotBrazo1 = -90;
	KeyFrame[35].rotBrazo2 = 0;
	KeyFrame[35].rotBrazo3 = 45;
	KeyFrame[35].movBrazo1Y = 14.5;
	KeyFrame[35].movBrazo1Z = 13.5;
	KeyFrame[35].movBrazo2X = 0;
	KeyFrame[35].movBrazo2Y = 0;
	KeyFrame[35].movBrazo3X = 9.0;
	KeyFrame[35].movBrazo3Y = 6;
	KeyFrame[35].scBall = 0.5;
	KeyFrame[35].movBallY = 14.4;
	KeyFrame[35].movBallZ = 68;
	KeyFrame[35].rotGeek = 0;

	KeyFrame[36].rotPies = 0;
	KeyFrame[36].rotPiernas = 0;
	KeyFrame[36].movPiernas = 0;
	KeyFrame[36].rotTorso = 0;
	KeyFrame[36].movTorso = 0;
	KeyFrame[36].rotBrazo1 = -90;
	KeyFrame[36].rotBrazo2 = 0;
	KeyFrame[36].rotBrazo3 = 45;
	KeyFrame[36].movBrazo1Y = 14.5;
	KeyFrame[36].movBrazo1Z = 13.5;
	KeyFrame[36].movBrazo2X = 0;
	KeyFrame[36].movBrazo2Y = 0;
	KeyFrame[36].movBrazo3X = 9.0;
	KeyFrame[36].movBrazo3Y = 6;
	KeyFrame[36].scBall = 0;
	KeyFrame[36].movBallY = 14.4;
	KeyFrame[36].movBallZ = 68;
	KeyFrame[36].rotGeek = 22.5;

	KeyFrame[37].rotPies = 0;
	KeyFrame[37].rotPiernas = 0;
	KeyFrame[37].movPiernas = 0;
	KeyFrame[37].rotTorso = 0;
	KeyFrame[37].movTorso = 0;
	KeyFrame[37].rotBrazo1 = -90;
	KeyFrame[37].rotBrazo2 = 0;
	KeyFrame[37].rotBrazo3 = 45;
	KeyFrame[37].movBrazo1Y = 14.5;
	KeyFrame[37].movBrazo1Z = 13.5;
	KeyFrame[37].movBrazo2X = 0;
	KeyFrame[37].movBrazo2Y = 0;
	KeyFrame[37].movBrazo3X = 9.0;
	KeyFrame[37].movBrazo3Y = 6;
	KeyFrame[37].scBall = 0;
	KeyFrame[37].movBallY = 14.4;
	KeyFrame[37].movBallZ = 68;
	KeyFrame[37].rotGeek = 45;

	KeyFrame[37].rotPies = 0;
	KeyFrame[37].rotPiernas = 0;
	KeyFrame[37].movPiernas = 0;
	KeyFrame[37].rotTorso = 0;
	KeyFrame[37].movTorso = 0;
	KeyFrame[37].rotBrazo1 = -90;
	KeyFrame[37].rotBrazo2 = 0;
	KeyFrame[37].rotBrazo3 = 45;
	KeyFrame[37].movBrazo1Y = 14.5;
	KeyFrame[37].movBrazo1Z = 13.5;
	KeyFrame[37].movBrazo2X = 0;
	KeyFrame[37].movBrazo2Y = 0;
	KeyFrame[37].movBrazo3X = 9.0;
	KeyFrame[37].movBrazo3Y = 6;
	KeyFrame[37].scBall = 0;
	KeyFrame[37].movBallY = 14.4;
	KeyFrame[37].movBallZ = 68;
	KeyFrame[37].rotGeek = 67.5;

	KeyFrame[38].rotPies = 0;
	KeyFrame[38].rotPiernas = 0;
	KeyFrame[38].movPiernas = 0;
	KeyFrame[38].rotTorso = 0;
	KeyFrame[38].movTorso = 0;
	KeyFrame[38].rotBrazo1 = -90;
	KeyFrame[38].rotBrazo2 = 0;
	KeyFrame[38].rotBrazo3 = 45;
	KeyFrame[38].movBrazo1Y = 14.5;
	KeyFrame[38].movBrazo1Z = 13.5;
	KeyFrame[38].movBrazo2X = 0;
	KeyFrame[38].movBrazo2Y = 0;
	KeyFrame[38].movBrazo3X = 9.0;
	KeyFrame[38].movBrazo3Y = 6;
	KeyFrame[38].scBall = 0;
	KeyFrame[38].movBallY = 14.4;
	KeyFrame[38].movBallZ = 68;
	KeyFrame[38].rotGeek = 90;

	//Animación Cola
	KeyFrame2[0].rotColaVertical = 9;
	KeyFrame2[0].rotColaHorizontal = 0;
	KeyFrame2[0].movColaX = 0;
	KeyFrame2[0].movColaY = 0;
	KeyFrame2[0].movColaZ = -0.6;

	KeyFrame2[1].rotColaVertical = 18;
	KeyFrame2[1].rotColaHorizontal = 0;
	KeyFrame2[1].movColaX = 0;
	KeyFrame2[1].movColaY = 0;
	KeyFrame2[1].movColaZ = -1.2;

	KeyFrame2[2].rotColaVertical = 27;
	KeyFrame2[2].rotColaHorizontal = 0;
	KeyFrame2[2].movColaX = 0;
	KeyFrame2[2].movColaY = 0;
	KeyFrame2[2].movColaZ = -1.8;

	KeyFrame2[3].rotColaVertical = 36;
	KeyFrame2[3].rotColaHorizontal = 0;
	KeyFrame2[3].movColaX = 0;
	KeyFrame2[3].movColaY = 0;
	KeyFrame2[3].movColaZ = -2.4;

	KeyFrame2[4].rotColaVertical = 45;
	KeyFrame2[4].rotColaHorizontal = 0;
	KeyFrame2[4].movColaX = 0;
	KeyFrame2[4].movColaY = 0;
	KeyFrame2[4].movColaZ = -3.0;

	KeyFrame2[5].rotColaVertical = 36;
	KeyFrame2[5].rotColaHorizontal = 0;
	KeyFrame2[5].movColaX = 0;
	KeyFrame2[5].movColaY = 0;
	KeyFrame2[5].movColaZ = -2.4;

	KeyFrame2[6].rotColaVertical = 27;
	KeyFrame2[6].rotColaHorizontal = 0;
	KeyFrame2[6].movColaX = 0;
	KeyFrame2[6].movColaY = 0;
	KeyFrame2[6].movColaZ = -1.8;

	KeyFrame2[7].rotColaVertical = 18;
	KeyFrame2[7].rotColaHorizontal = 0;
	KeyFrame2[7].movColaX = 0;
	KeyFrame2[7].movColaY = 0;
	KeyFrame2[7].movColaZ = -1.2;

	KeyFrame2[8].rotColaVertical = 9;
	KeyFrame2[8].rotColaHorizontal = 0;
	KeyFrame2[8].movColaX = 0;
	KeyFrame2[8].movColaY = 0;
	KeyFrame2[8].movColaZ = -0.6;

	KeyFrame2[9].rotColaVertical = 0;
	KeyFrame2[9].rotColaHorizontal = 0;
	KeyFrame2[9].movColaX = 0;
	KeyFrame2[9].movColaY = 0;
	KeyFrame2[9].movColaZ = 0;

	KeyFrame2[10].rotColaVertical = -9;
	KeyFrame2[10].rotColaHorizontal = 0;
	KeyFrame2[10].movColaX = 0;
	KeyFrame2[10].movColaY = 0.44;
	KeyFrame2[10].movColaZ = 0.5;

	KeyFrame2[11].rotColaVertical = -18;
	KeyFrame2[11].rotColaHorizontal = 0;
	KeyFrame2[11].movColaX = 0;
	KeyFrame2[11].movColaY = 0.88;
	KeyFrame2[11].movColaZ = 1.0;

	KeyFrame2[12].rotColaVertical = -27;
	KeyFrame2[12].rotColaHorizontal = 0;
	KeyFrame2[12].movColaX = 0;
	KeyFrame2[12].movColaY = 1.32;
	KeyFrame2[12].movColaZ = 1.5;

	KeyFrame2[13].rotColaVertical = -36;
	KeyFrame2[13].rotColaHorizontal = 0;
	KeyFrame2[13].movColaX = 0;
	KeyFrame2[13].movColaY = 1.76;
	KeyFrame2[13].movColaZ = 2.0;

	KeyFrame2[14].rotColaVertical = -45;
	KeyFrame2[14].rotColaHorizontal = 0;
	KeyFrame2[14].movColaX = 0;
	KeyFrame2[14].movColaY = 2.2;
	KeyFrame2[14].movColaZ = 2.5;

	KeyFrame2[15].rotColaVertical = -36;
	KeyFrame2[15].rotColaHorizontal = 0;
	KeyFrame2[15].movColaX = 0;
	KeyFrame2[15].movColaY = 1.76;
	KeyFrame2[15].movColaZ = 2.0;

	KeyFrame2[16].rotColaVertical = -27;
	KeyFrame2[16].rotColaHorizontal = 0;
	KeyFrame2[16].movColaX = 0;
	KeyFrame2[16].movColaY = 1.32;
	KeyFrame2[16].movColaZ = 1.5;

	KeyFrame2[17].rotColaVertical = -18;
	KeyFrame2[17].rotColaHorizontal = 0;
	KeyFrame2[17].movColaX = 0;
	KeyFrame2[17].movColaY = 0.88;
	KeyFrame2[17].movColaZ = 1.0;

	KeyFrame2[18].rotColaVertical = -9;
	KeyFrame2[18].rotColaHorizontal = 0;
	KeyFrame2[18].movColaX = 0;
	KeyFrame2[18].movColaY = 0.44;
	KeyFrame2[18].movColaZ = 0.5;

	KeyFrame2[19].rotColaVertical = 0;
	KeyFrame2[19].rotColaHorizontal = 0;
	KeyFrame2[19].movColaX = 0;
	KeyFrame2[19].movColaY = 0;
	KeyFrame2[19].movColaZ = 0;

	KeyFrame2[20].rotColaVertical = 0;
	KeyFrame2[20].rotColaHorizontal = 9;
	KeyFrame2[20].movColaX = 0.3;
	KeyFrame2[20].movColaY = 0;
	KeyFrame2[20].movColaZ = 0;

	KeyFrame2[21].rotColaVertical = 0;
	KeyFrame2[21].rotColaHorizontal = 18;
	KeyFrame2[21].movColaX = 0.6;
	KeyFrame2[21].movColaY = 0;
	KeyFrame2[21].movColaZ = 0;

	KeyFrame2[22].rotColaVertical = 0;
	KeyFrame2[22].rotColaHorizontal = 27;
	KeyFrame2[22].movColaX = 0.9;
	KeyFrame2[22].movColaY = 0;
	KeyFrame2[22].movColaZ = 0;

	KeyFrame2[23].rotColaVertical = 0;
	KeyFrame2[23].rotColaHorizontal = 36;
	KeyFrame2[23].movColaX = 1.2;
	KeyFrame2[23].movColaY = 0;
	KeyFrame2[23].movColaZ = 0;

	KeyFrame2[24].rotColaVertical = 0;
	KeyFrame2[24].rotColaHorizontal = 45;
	KeyFrame2[24].movColaX = 1.5;
	KeyFrame2[24].movColaY = 0;
	KeyFrame2[24].movColaZ = 0;

	KeyFrame2[25].rotColaVertical = 0;
	KeyFrame2[25].rotColaHorizontal = 36;
	KeyFrame2[25].movColaX = 1.2;
	KeyFrame2[25].movColaY = 0;
	KeyFrame2[25].movColaZ = 0;

	KeyFrame2[26].rotColaVertical = 0;
	KeyFrame2[26].rotColaHorizontal = 27;
	KeyFrame2[26].movColaX = 0.9;
	KeyFrame2[26].movColaY = 0;
	KeyFrame2[26].movColaZ = 0;

	KeyFrame2[27].rotColaVertical = 0;
	KeyFrame2[27].rotColaHorizontal = 18;
	KeyFrame2[27].movColaX = 0.6;
	KeyFrame2[27].movColaY = 0;
	KeyFrame2[27].movColaZ = 0;

	KeyFrame2[28].rotColaVertical = 0;
	KeyFrame2[28].rotColaHorizontal = 9;
	KeyFrame2[28].movColaX = 0.3;
	KeyFrame2[28].movColaY = 0;
	KeyFrame2[28].movColaZ = 0;

	KeyFrame2[29].rotColaVertical = 0;
	KeyFrame2[29].rotColaHorizontal = 0;
	KeyFrame2[29].movColaX = 0;
	KeyFrame2[29].movColaY = 0;
	KeyFrame2[29].movColaZ = 0;

	KeyFrame2[30].rotColaVertical = 0;
	KeyFrame2[30].rotColaHorizontal = -9;
	KeyFrame2[30].movColaX = -0.3;
	KeyFrame2[30].movColaY = 0;
	KeyFrame2[30].movColaZ = 0;

	KeyFrame2[31].rotColaVertical = 0;
	KeyFrame2[31].rotColaHorizontal = -18;
	KeyFrame2[31].movColaX = -0.6;
	KeyFrame2[31].movColaY = 0;
	KeyFrame2[31].movColaZ = 0;

	KeyFrame2[32].rotColaVertical = 0;
	KeyFrame2[32].rotColaHorizontal = -27;
	KeyFrame2[32].movColaX = -0.9;
	KeyFrame2[32].movColaY = 0;
	KeyFrame2[32].movColaZ = 0;

	KeyFrame2[33].rotColaVertical = 0;
	KeyFrame2[33].rotColaHorizontal = -36;
	KeyFrame2[33].movColaX = -1.2;
	KeyFrame2[33].movColaY = 0;
	KeyFrame2[33].movColaZ = 0;

	KeyFrame2[34].rotColaVertical = 0;
	KeyFrame2[34].rotColaHorizontal = -45;
	KeyFrame2[34].movColaX = -1.5;
	KeyFrame2[34].movColaY = 0;
	KeyFrame2[34].movColaZ = 0;

	KeyFrame2[35].rotColaVertical = 0;
	KeyFrame2[35].rotColaHorizontal = -36;
	KeyFrame2[35].movColaX = -1.2;
	KeyFrame2[35].movColaY = 0;
	KeyFrame2[35].movColaZ = 0;

	KeyFrame2[36].rotColaVertical = 0;
	KeyFrame2[36].rotColaHorizontal = -27;
	KeyFrame2[36].movColaX = -0.9;
	KeyFrame2[36].movColaY = 0;
	KeyFrame2[36].movColaZ = 0;

	KeyFrame2[37].rotColaVertical = 0;
	KeyFrame2[37].rotColaHorizontal = -18;
	KeyFrame2[37].movColaX = -0.6;
	KeyFrame2[37].movColaY = 0;
	KeyFrame2[37].movColaZ = 0;

	KeyFrame2[38].rotColaVertical = 0;
	KeyFrame2[38].rotColaHorizontal = -9;
	KeyFrame2[38].movColaX = -0.3;
	KeyFrame2[38].movColaY = 0;
	KeyFrame2[38].movColaZ = 0;

	KeyFrame2[39].rotColaVertical = 0;
	KeyFrame2[39].rotColaHorizontal = 0;
	KeyFrame2[39].movColaX = 0;
	KeyFrame2[39].movColaY = 0;
	KeyFrame2[39].movColaZ = 0;

	//Vectores para animación
	glm::vec3 sGift01(0.0f, 0.0f, 0.0f);
	glm::vec3 sGift02(0.0f, 0.0f, 0.0f);
	glm::vec3 sGift03(0.0f, 0.0f, 0.0f);

	glm::vec3 NeptunoTranslate(0.0f, 0.0f, 0.0f);
	glm::vec3 NeptunoScale(0.0f, 0.0f, 0.0f);
	glm::vec3 NeptunoBrazoTranslate(0.0f, 0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Para KeyFrames
		inputKeyframes(mainWindow.getsKeys());
		animate();
		animate2();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//skyboxFBD.DrawSkybox(camera.calculateViewMatrix(), projection);

		//Manejo del contador
		if (countSB <= 400.0f) {
			countSB += movOffset * deltaTime;
		}
		else {
			countSB = 0;
		}

		//Ciclo Día-Noche
		if (countSB <= 200) {
			skyboxFBD.DrawSkybox(camera.calculateViewMatrix(), projection);
			pointLightCount = 0;
		}
		else {
			skyboxFBN.DrawSkybox(camera.calculateViewMatrix(), projection);
			pointLightCount = 3;
		}
		


		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		////luz ligada a la cámara de tipo flash 
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//luz ligada al faro del coche
		//glm::vec3 lowerLightC = posfaro;
		////lowerLightC.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLightC, posfaro);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);

		glm::mat4 matrizaux(1.0);

		glm::mat4 matrizaux2(1.0);

		glm::mat4 matrizauxAvatar(1.0);

		glm::mat4 matrizauxKC(1.0);

		glm::mat4 matrizauxRegalo(1.0);

		glm::mat4 matrizauxTree(1.0);

		glm::mat4 matrizauxBrazo(1.0);

		glm::mat4 matrizauxGary(1.0);

		glm::mat4 matrizauxManRay(1.0);

		glm::mat4 matrizauxManRayBrazo(1.0);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(60.0f, 1.0f, 60.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		//agregar material al plano de piso
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//CHARACTERS
		//-----------------------------------------------------------------------------------------
		//Don Cangrejo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(440.0f, 0.0f, -350.0f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mKrabs.RenderModel();

		//Bob Esponja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(380.0f, 0.0f, -230.0f));
		model = glm::scale(model, glm::vec3(4.0f, 5.0f, 4.0f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SpongeBob.RenderModel();

		//Calamardo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(440.0f, 0.0f, -300.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SquidWard.RenderModel();

		//Patricio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(360.0f, 0.0f, -230.0f));
		model = glm::scale(model, glm::vec3(27.0f, 27.0f, 27.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PatrickStar.RenderModel();

		//Animación Gary
		//movGary += 1.0;
		if (countGary <= 5800.0) {
			countGary += 1.0;
			//rGary += 0.05;
			if (countGary <= 700.0) {
				rGary += 0.05;
				rotGary = 45;
				movGary -= 0.05;
				rotOjoDer = 0.0;
				rotOjoIzq = -15.0;
				if (countGary <= 350.0) {
					rotOjoDer = -15.0;
					rotOjoIzq = 0.0;
				}
			}
			else if (countGary <= 1400.0) {
				rGary += 0.05;
				rotGary = 0;
				rotOjoDer = 0.0;
				rotOjoIzq = -15.0;
				if (countGary <= 1050) {
					rotOjoDer = -15.0;
					rotOjoIzq = 0.0;
				}
			}
			else if (countGary <= 3150.0) {
				rotGary = -120;
				movGary += 0.05;
				rotOjoDer = 0.0;
				rotOjoIzq = -15.0;
				rGary -= 0.025;
				if (countGary <= 1750) {
					rotOjoDer = 0.0;
					rotOjoIzq = -15.0;
				}
				else if (countGary <= 2100){
					rotOjoDer = -15.0;
					rotOjoIzq = 0.0;
				}
				else if (countGary <= 2450) {
					rotOjoDer = 0.0;
					rotOjoIzq = -15.0;
				}
				else if (countGary <= 2800) {
					rotOjoDer = -15.0;
					rotOjoIzq = 0.0;
				}
			}
			else if (countGary <= 4200) {
				rotGary = -180;
				rGary -= 0.05;
				rotOjoDer = -15.0;
				rotOjoIzq = 0.0;
				//movGary = 0.0;;
				if (countGary <= 3500) {
					rotOjoDer = -15.0;
					rotOjoIzq = 0.0;
				}
				else if (countGary <= 3850) {
					rotOjoDer = 0.0;
					rotOjoIzq = -15.0;
				}
			}
			else if (countGary <= 5250) {
				rotGary = -270;
				movGary -= 0.05;
				rotOjoDer = 0.0;
				rotOjoIzq = -15.0;
				if (countGary <= 4550) {
					rotOjoDer = 0.0;
					rotOjoIzq = -15.0;
				}
				else if (countGary <= 4900) {
					rotOjoDer = -15.0;
					rotOjoIzq = 0.0;
				}
			}
			else if (countGary <= 5800) {
				rotGary = -360;
				rGary += 0.05;
				rotOjoDer = 0.0;
				rotOjoIzq = -15.0;
				if (countGary <= 5600) {
					rotOjoDer = -15.0;
					rotOjoIzq = 0.0;
				}
			}
		}
		else {
			rGary = 0.0;
			countGary = 0.0;
			rotGary = 0.0;
			movGary = 0.0;
			rotOjoIzq = 0.0;
			rotOjoDer = 0.0;
		}
		

		//Gary
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(330.0f + rGary, 0.0f, -400.0f + movGary));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, rotGary * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizauxGary = model;
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gary.RenderModel();

		//Gary Ojo Derecho
		model = matrizauxGary;
		/*model = glm::translate(model, glm::vec3(400.0f, 0.0f, -300.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));*/
		model = glm::rotate(model, rotOjoDer * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GaryOjoDer.RenderModel();

		//Gary Ojo Izquierdo
		model = matrizauxGary;
		/*model = glm::translate(model, glm::vec3(400.0f, 0.0f, -300.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));*/
		model = glm::rotate(model, rotOjoIzq * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GaryOjoIzq.RenderModel();

		//Plankton
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(320.0f, 0.0f, -340.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Plankton.RenderModel();

		//Karen
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(330.0f, 0.0f, -340.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Karen.RenderModel();

		//Santa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(400.0f, 121.5f, -350.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Santa.RenderModel();

		//ManRayTorso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, -2.0f, -100.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizauxManRay = model;
		/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));*/
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, movTorso));
		model = glm::rotate(model, rotTorso * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		matrizauxManRayBrazo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManRayTorso.RenderModel();

		//ManRayBrazo
		model = matrizauxManRayBrazo;
		//POSICIÓN
		/*model = glm::translate(model, glm::vec3(0.0f, 14.5f, 13.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));*/
		model = glm::translate(model, glm::vec3(0.0f, movBrazo1Y, movBrazo1Z));
		model = glm::rotate(model, rotBrazo1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//CARGAR
		/*model = glm::translate(model, glm::vec3(-2.5f, 0.5f, 0.0f));
		model = glm::rotate(model, -11 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));*/
		model = glm::translate(model, glm::vec3(movBrazo2X, movBrazo2Y, 0.0f));
		model = glm::rotate(model, rotBrazo2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//LANZAR
		/*model = glm::translate(model, glm::vec3(9.0f, 6.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));*/
		model = glm::translate(model, glm::vec3(movBrazo3X, movBrazo3Y, 0.0f));
		model = glm::rotate(model, rotBrazo3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManRayBrazo.RenderModel();

		//ManRayPiernas
		model = matrizauxManRay;
		/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, 6.0f));
		model = glm::rotate(model, -40 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));*/
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, movPiernas));
		model = glm::rotate(model, rotPiernas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManRayPiernas.RenderModel();

		//ManRayPies
		model = matrizauxManRay;
		/*model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));*/
		model = glm::rotate(model, rotPies * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManRayPies.RenderModel();
		
		//Geek
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -2.0f, -100.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizauxManRay = model;
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		/*model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));*/
		model = glm::rotate(model, rotGeek * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Geek.RenderModel();

		//-----------------------------------------------------------------------------------------

		//BUILDINGS
		//-----------------------------------------------------------------------------------------
		//Crustáceo Cascarudo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(400.0f, 0.0f, -300.0f));
		/*model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 60.0f));*/
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		//Jerarquía del Crustáceo Cascarudo
		matrizauxKC = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		KrustyKrab.RenderModel();

		//Casa de Bob Esponja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-500.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.0f, 17.0f, 17.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SpongeBobHouse.RenderModel();

		//Casa de Calamardo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-500.0f, 0.0f, 250.0f));
		model = glm::scale(model, glm::vec3(13.0f, 13.0f, 13.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SquidwardHouse.RenderModel();

		//Casa de Patricio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-500.0f, 0.0f, 500.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PatrickHouse.RenderModel();

		//Camino01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-271.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 3.5f, 21.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Road.RenderModel();

		//Camino02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -300.0f));
		model = glm::scale(model, glm::vec3(8.6f, 3.5f, 7.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Road.RenderModel();
		//------------------------------------------------------------------------------------------

		//OBJECTS 
		//-----------------------------------------------------------------------------------------
		//Árbol de Navidad
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(40.0f, 4.0f, 60.0f));
		//matrizauxTree = model;
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ChristmasTree.RenderModel();

		//Luz para la animación de Regalos
		spotLights[0].SetFlash(glm::vec3(416.0f, 70.0f, -245.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		marcaR = mainWindow.getTeclaC();

		//Animación de Regalos
		if (marcaR > 0.0) {

			marcaR = 1.0;

			////Neptuno
			//model = glm::mat4(1.0);
			//model = glm::translate(model, glm::vec3(300.0f + countNep, 0.0f, -300.0f));
			//model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
			//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::rotate(model, rotBrazoNep * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			//matrizauxBrazo = model;
			//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			//Neptuno.RenderModel();

			////Neptuno Brazo Derecho
			//model = matrizauxBrazo;
			//model = glm::rotate(model, rotBrazoNep2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			////model = glm::translate(model, glm::vec3(0.0, -1.4, 2.5));
			//model = glm::translate(model, glm::vec3(0.0, movBrazoNep, movBrazoNep2));
			//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			//NeptunoBrazoDer.RenderModel();

			NeptunoTranslate = glm::vec3(300.0f + countNep, 0.0f, -300.0f);
			NeptunoScale = glm::vec3(8.0f, 8.0f, 8.0f);

			NeptunoBrazoTranslate = glm::vec3(0.0, movBrazoNep, movBrazoNep2);

			countGF += 1.0;

			if (countGF <= 0.0f) {

				sGift01 = glm::vec3(0.0, 0.0, 0.0);
				sGift02 = glm::vec3(0.0, 0.0, 0.0);
				sGift03 = glm::vec3(0.0, 0.0, 0.0);

				if (countGF <= -1200) {
					countNep += 0.08;
				}
				else if (countGF <= -600) {
					rotBrazoNep -= 0.1;
					//countNep = 0.0;
				}
				else if (countGF <= -300) {
					rotBrazoNep2 -= 0.2;
					movBrazoNep -= 0.0046666;
					movBrazoNep2 += 0.0083333;
					//countNep = 0.0;
				}
				else if (countGF <= -200.0) {
					//countNep = 0.0;
					spotLightCount = 1;
				}
				else if (countGF <= -100.0) {
					//countNep = 0.0;
					spotLightCount = 0;
				}
				else if (countGF <= -50.0) {
					//countNep = 0.0;
					spotLightCount = 1;
				}
				else if (countGF <= 0.0) {
					//countNep = 0.0;
					spotLightCount = 0;
				}
			}

			else if (countGF <= 1200.0f) {
				//countGF += 1.0;
				vGift01 += 0.0016665;
				vGift02 += 0.0025;
				vGift03 += 0.00333325;
				vGift04 += 0.00208325;
				sGift01 = glm::vec3(0.0 + vGift01, 0.0 + vGift01, 0.0 + vGift01);
				sGift02 = glm::vec3(0.0 + vGift01, 0.0 + vGift02, 0.0 + vGift03);
				sGift03 = glm::vec3(0.0 + vGift04, 0.0 + vGift04, 0.0 + vGift04);
				GiftY += 0.00333325;

				rGift01Z = 0.0;
				rGift02Y = 0.0;
				rGift03Z = 0.0;
			}

			else if (countGF <= 2400.0) {

				vGift01 = 0.0;
				vGift02 = 0.0;
				vGift03 = 0.0;
				vGift04 = 0.0;
				if (countGF <= 1350.0) {
					GiftY -= 0.026666;
				}
				else if (countGF <= 1355.0 /*and countGF <= 1050.0*/) {
					rGift01Z += 1.0;
					rGift02Y += 1.0;
					rGift03Z += 1.0;
				}
				else if (countGF <= 1360.0) {
					rGift01Z -= 1.0;
					rGift02Y -= 1.0;
					rGift03Z -= 1.0;
				}
				else if (countGF <= 1365.0) {
					rGift01Z -= 1.0;
					rGift02Y -= 1.0;
					rGift03Z -= 1.0;
				}
				else if (countGF <= 1370.0) {
					rGift01Z += 1.0;
					rGift02Y += 1.0;
					rGift03Z += 1.0;
				}
				else if (countGF <= 2400.0) {
					rGift01Z = 0.0;
					rGift02Y = 0.0;
					rGift03Z = 0.0;
				}
			}
			else {
				countGF = -2400.0;
				countNep = 0.0;
				rotBrazoNep = 0.0;
				rotBrazoNep2 = 0.0;
				movBrazoNep = 0.0;
				movBrazoNep2 = 0.0;
				marcaR = 0.0;
			}
		}

		//Neptuno
		model = glm::mat4(1.0);
		model = glm::translate(model, NeptunoTranslate);
		model = glm::scale(model, NeptunoScale);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotBrazoNep * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizauxBrazo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Neptuno.RenderModel();

		//Neptuno Brazo Derecho
		model = matrizauxBrazo;
		model = glm::rotate(model, rotBrazoNep2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0, -1.4, 2.5));
		model = glm::translate(model, NeptunoBrazoTranslate);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		NeptunoBrazoDer.RenderModel();

		////Manejo del contador de la animación de Regalos
		//if (marcaR > 0.0) {

		//	//Neptuno
		//	model = glm::mat4(1.0);
		//	model = glm::translate(model, glm::vec3(300.0f + countNep, 0.0f, -300.0f));
		//	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		//	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//	model = glm::rotate(model, rotBrazoNep * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//	matrizauxBrazo = model;
		//	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//	Neptuno.RenderModel();

		//	//Neptuno Brazo Derecho
		//	model = matrizauxBrazo;
		//	/*model = glm::mat4(1.0);
		//	model = glm::translate(model, glm::vec3(300.0f + countNep + movBrazoNep, 0.0f, -300.0f + movBrazoNep));
		//	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		//	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//	model = glm::rotate(model, rotBrazoNep * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));*/
		//	model = glm::rotate(model, rotBrazoNep2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//	//model = glm::translate(model, glm::vec3(0.0, -1.4, 2.5));
		//	model = glm::translate(model, glm::vec3(0.0, movBrazoNep, movBrazoNep2));
		//	/*model = glm::translate(model, glm::vec3(0.0, 0.0 + movBrazoNep/6, movBrazoNep/2));*/
		//	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//	NeptunoBrazoDer.RenderModel();

		//	if (countGF <= 0.0f) {

		//		sGift01 = glm::vec3(0.0, 0.0, 0.0);
		//		sGift02 = glm::vec3(0.0, 0.0, 0.0);
		//		sGift03 = glm::vec3(0.0, 0.0, 0.0);

		//		

		//		countGF += 1.0;

		//		if (countGF <= -1200) {
		//			countNep += 0.08;
		//		}

		//		else if (countGF <= -600) {
		//			rotBrazoNep -= 0.1;
		//			//countNep = 0.0;
		//		}
		//		else if (countGF <= -300) {
		//			rotBrazoNep2 -= 0.2;
		//			movBrazoNep -= 0.0046666;
		//			/*movBrazoNep = -1.4;
		//			movBrazoNep2 = 2.5;*/
		//			movBrazoNep2 += 0.0083333;
		//			//countNep = 0.0;
		//		}
		//		else if (countGF <= -200.0) {
		//			//countNep = 0.0;
		//			spotLightCount = 1;
		//		}
		//		else if (countGF <= -100.0) {
		//			//countNep = 0.0;
		//			spotLightCount = 0;
		//		}
		//		else if (countGF <= -50.0) {
		//			//countNep = 0.0;
		//			spotLightCount = 1;
		//		}
		//		else if (countGF <= 0.0) {
		//			//countNep = 0.0;
		//			spotLightCount = 0;
		//		}
		//	}

		//	
		//	else if (countGF <= 1200.0f) {
		//		//countNep = 0.0;
		//		//countGF += movOffset * deltaTime;
		//		countGF += 1.0;
		//		//printf("%d", countGF);
		//		/*vGift01 += 0.006666;
		//		vGift02 += 0.01;
		//		vGift03 += 0.013333;
		//		vGift04 += 0.008333;*/
		//		vGift01 += 0.0016665;
		//		vGift02 += 0.0025;
		//		vGift03 += 0.00333325;
		//		vGift04 += 0.00208325;
		//		sGift01 = glm::vec3(0.0 + vGift01, 0.0 + vGift01, 0.0 + vGift01);
		//		sGift02 = glm::vec3(0.0 + vGift01, 0.0 + vGift02, 0.0 + vGift03);
		//		sGift03 = glm::vec3(0.0 + vGift04, 0.0 + vGift04, 0.0 + vGift04);
		//		GiftY += 0.00333325;

		//		rGift01Z = 0.0;
		//		rGift02Y = 0.0;
		//		rGift03Z = 0.0;
		//	}
		//	else {
		//		marcaR = -1.0;
		//	}
		//}
		//else{
		//	if (countGF >= 0.0) {
		//		//countGF -= movOffset * deltaTime;
		//		countGF -= 1.0;
		//		vGift01 = 0.0;
		//		vGift02 = 0.0;
		//		vGift03 = 0.0;
		//		vGift04 = 0.0;
		//		if (countGF >= 1050.0) {
		//			GiftY -= 0.026666;
		//		}
		//		else if(countGF >= 1045.0 /*and countGF <= 1050.0*/){
		//			rGift01Z += 1.0;
		//			rGift02Y += 1.0;
		//			rGift03Z += 1.0;
		//		}
		//		else if (countGF >= 1039.0) {
		//			rGift01Z -= 1.0;
		//			rGift02Y -= 1.0;
		//			rGift03Z -= 1.0;
		//		}
		//		else if (countGF >= 1033.0) {
		//			rGift01Z -= 1.0;
		//			rGift02Y -= 1.0;
		//			rGift03Z -= 1.0;
		//		}
		//		else if (countGF >= 1027.0) {
		//			rGift01Z += 1.0;
		//			rGift02Y += 1.0;
		//			rGift03Z += 1.0;
		//		}
		//		else if (countGF >= 0.0) {
		//			rGift01Z = 0.0;
		//			rGift02Y = 0.0;
		//			rGift03Z = 0.0;
		//		}
		//		/*sGift01 = glm::vec3(0.0, 0.0, 0.0);
		//		sGift02 = glm::vec3(0.0, 0.0, 0.0);
		//		sGift03 = glm::vec3(0.0, 0.0, 0.0);*/

		//		/*vGift01 = 2.0;
		//		vGift02 = 3.0;
		//		vGift03 = 4.0;
		//		vGift04 = 2.5;*/

		//	}
		//	else {
		//		marcaR = 1.0;
		//		countGF = -2400.0;
		//		countNep = 0.0;
		//		rotBrazoNep = 0.0;
		//		rotBrazoNep2 = 0.0;
		//		movBrazoNep = 0.0;
		//		movBrazoNep2 = 0.0;
		//	}
		//}

		//GIFT01
		//model = matrizauxTree;
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(15.0f, 0.0f + GiftY, 50.0f));
		model = glm::scale(model, sGift01);
		model = glm::rotate(model, rGift01Z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gift01.RenderModel();

		//GIFT02
		//model = matrizauxTree;
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(20.0f, 0.0f + GiftY, 55.0f));
		model = glm::scale(model, sGift02);
		model = glm::rotate(model, rGift02Y * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 3.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gift02.RenderModel();

		//GIFT03
		//model = matrizauxTree;
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(15.0f, 0.0f + GiftY, 60.0f));
		model = glm::scale(model, sGift03);
		model = glm::rotate(model, rGift03Z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gift03.RenderModel();

		//Bola de Nieve
		model = matrizauxManRay;
		//model = glm::translate(model, glm::vec3(-8.0f, 14.5f, -68.7f));    //-8   11   -67
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(-8.0f, 0.1f + movBallY, -67.0f + movBallZ));
		model = glm::scale(model, glm::vec3(scBall, scBall, scBall));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SnowBall.RenderModel();

		//REGALOS
		//-------------------------------------------------------------------------------
		//Regalo01
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(30.0f, 11.0f, 45.0f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		matrizauxRegalo = model;
		model = glm::scale(model, glm::vec3(9.0f, 12.0f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Regalo01.UseTexture();
		meshList[4]->RenderMesh();

		//Regalo02
		model = matrizauxRegalo;
		model = glm::translate(model, glm::vec3(2.0f, -8.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 4.0f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Regalo02.UseTexture();
		meshList[4]->RenderMesh();

		//Regalo03
		model = matrizauxRegalo;
		model = glm::translate(model, glm::vec3(-2.0f, -8.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 4.0f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Regalo03.UseTexture();
		meshList[4]->RenderMesh();

		//Regalo04
		model = matrizauxRegalo;
		model = glm::translate(model, glm::vec3(-2.0f, -10.5f, 0.5f));
		model = glm::scale(model, glm::vec3(1.4f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Regalo04.UseTexture();
		meshList[4]->RenderMesh();

		//Regalo05
		model = matrizauxRegalo;
		model = glm::translate(model, glm::vec3(2.0f, -10.5f, 0.5f));
		model = glm::scale(model, glm::vec3(1.4f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Regalo04.UseTexture();
		meshList[4]->RenderMesh();

		//Regalo06
		model = matrizauxRegalo;
		model = glm::translate(model, glm::vec3(-1.5f, 2.0f, 2.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bombilla.RenderModel();

		//Regalo07
		model = matrizauxRegalo;
		model = glm::translate(model, glm::vec3(1.5f, 2.0f, 2.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bombilla.RenderModel();

		//Muñecos de nieve
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(-150.0f, -3.0f, 100.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowmans.RenderModel();

		//Muñeco de nieve02
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(-150.0f, -3.0f, -100.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowman02.RenderModel();

		//Lampara01
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(0.0f, 58.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Lampara02
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(0.0f, 58.0f, -100.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Lampara03
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(0.0f, 58.0f, 100.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Bota01
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(-94.0f, 78.0f, -100.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sock.RenderModel();

		//Bota02
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(-94.0f, 78.0f, -50.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sock2.RenderModel();

		//Bota03
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(-94.0f, 78.0f, 50.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sock.RenderModel();

		//Bota04
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(-94.0f, 78.0f, 100.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sock2.RenderModel();

		//-------------------------------------------------------------------------------


		//-----------------------------------------------------------------------------------------

		////agregar su coche y ponerle material
		//model = glm::mat4(1.0);
		////model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(),  0.5f, -1.5f));
		////Faro1
		//glm::vec3 lowerLightC1(-7.0f + mainWindow.getmuevex(), 1.8f, 1.65f + mainWindow.getmuevez());
		////Faro2
		////glm::vec3 lowerLightC2(-7.0f + mainWindow.getmuevex(), 1.8f, -2.95f);
		//model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.0f, 0.0f)); //IMP
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f + mainWindow.getmuevez()));
		////model = glm::translate(model, lowerLightC);
		////model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		//spotLights[2].SetFlash(lowerLightC1, glm::vec3(-1.0f, 0.0f, 0.0f));
		////spotLights[3].SetFlash(lowerLightC2, glm::vec3(-1.0f, 0.0f, 0.0f));

		//matrizaux = model;

		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////Kitt_M.RenderModel();
		//De_Lorean.RenderModel();

		////Agregar llantas con jerarquía y rotación propia
		////LLanta1
		//model = matrizaux;
		////model = glm::mat4(1.0);
		////model = glm::translate(model, glm::vec3(5.0f, 0.0f, 4.0f));
		//model = glm::translate(model, glm::vec3(-6.44f, 0.45f, 2.0f));
		//model = glm::scale(model, glm::vec3(0.53f, 0.53f, 0.53f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getLlanta1()), glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getmuevex()*5), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////LLanta2
		//model = matrizaux;
		//model = glm::translate(model, glm::vec3(-6.44f, 0.45f, -3.3f));
		//model = glm::scale(model, glm::vec3(0.53f, 0.53f, 0.53f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getLlanta2()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getmuevex() * 5), glm::vec3(-1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////LLanta3
		//model = matrizaux;
		//model = glm::translate(model, glm::vec3(3.55f, 0.45f, 2.0f));
		//model = glm::scale(model, glm::vec3(0.53f, 0.53f, 0.53f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getLlanta3()), glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getmuevex() * 5), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////LLanta4
		//model = matrizaux;
		//model = glm::translate(model, glm::vec3(3.55f, 0.45f, -3.3f));
		//model = glm::scale(model, glm::vec3(0.53f, 0.53f, 0.53f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getLlanta4()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getmuevex() * 5), glm::vec3(-1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////agregar incremento en X con teclado
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-20.0f + mainWindow.getmuevex2(), 6.0f + mainWindow.getmuevey(), -1.0f));
		//matrizaux2 = model;
		//model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////agregar material al helicóptero
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//helicoptero.RenderModel();
		////Helice
		////model = matrizaux2;
		//model = glm::rotate(model, glm::radians(mainWindow.getmuevehelice()*10), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//helice.RenderModel();
		////Blackhawk_M.RenderModel();
		////¿Cómo ligas la luz al helicóptero?
		//glm::vec3 lowerLightH(-20.0f + mainWindow.getmuevex2(), 5.5f + mainWindow.getmuevey(), -1.0f);
		//spotLights[1].SetFlash(lowerLightH, glm::vec3(0.0f, -1.0f, 0.0f));

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -1.53f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Camino_M.RenderModel();

		//SPECIAL MODELS
		//------------------------------------------------------------------------------------------------------------
		//Arenita
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f + mainWindow.getmuevex(), 0.0f, 0.0f + mainWindow.getmuevez()));
		model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotAvatar()), glm::vec3(0.0f, 1.0f, 0.0f));

		matrizauxAvatar = model;

		//Uso de GL_BLEND por la textura del casco
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Sandy.RenderModel();
		glDisable(GL_BLEND);


		//Variables para realizar la rotación de las piernas
		muevex_int = int(mainWindow.getmuevex());
		muevez_int = int(mainWindow.getmuevez());
		rotAvatar_int = int(mainWindow.getrotAvatar());

		//Estructura para realizar la rotación de las piernas
		//Rotación en movimiento horizontal
		if (rotAvatar_int == 180 || rotAvatar_int == 0) {
			if (muevex_int % 2 == 0) {
				rotPieDer = -45.0;
				rotPieIzq = 0.0;
				rotPieDerOt = 0.0;
				rotPieIzqOt = 0.0;

				/*if (rotAvatar_int == 180) {
					movPieDer = 2.0;
					movPieIzq = 0.0;
				}
				else {
					movPieDer = 2.0;
					movPieIzq = 0.0;
				}*/

				movPieDer = 2.5;
				movPieIzq = 0.0;

				movPieDerY = 1.0;
				movPieIzqY = 0.0;
			}
			else {
				rotPieDer = 0.0;
				rotPieIzq = -45.0;
				rotPieDerOt = 0.0;
				rotPieIzqOt = 0.0;

				/*if (rotAvatar_int == 180) {
					movPieDer = 0.0;
					movPieIzq = 2.0;
				}
				else {
					movPieDer = 0.0;
					movPieIzq = 2.0;
				}*/

				movPieIzq = 2.5;
				movPieDer = 0.0;

				movPieDerY = 0.0;
				movPieIzqY = 1.0;
			}
		}

		//Rotación en movimiento vertical
		if (rotAvatar_int == -90 || rotAvatar_int == 90) {
			if (muevez_int % 2 == 0) {
				rotPieDerOt = -45.0;
				rotPieIzqOt = 0.0;
				rotPieDer = 0.0;
				rotPieIzq = 0.0;

				movPieDer = 2.5;
				movPieIzq = 0.0;

				movPieDerY = 1.0;
				movPieIzqY = 0.0;
			}
			else {
				rotPieDerOt = 0.0;
				rotPieIzqOt = -45.0;
				rotPieDer = 0.0;
				rotPieIzq = 0.0;

				movPieIzq = 2.5;
				movPieDer = 0.0;

				movPieDerY = 0.0;
				movPieIzqY = 1.0;
			}
		}

		//Arenita Piernas

		model = matrizauxAvatar;
		model = glm::translate(model, glm::vec3(+0.0f, +movPieDerY, +movPieDer));
		model = glm::rotate(model, rotPieDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotPieDerOt * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArenitaPieDer.RenderModel();

		model = matrizauxAvatar;
		model = glm::translate(model, glm::vec3(+0.0f, +movPieIzqY, +movPieIzq));
		model = glm::rotate(model, rotPieIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotPieIzqOt * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArenitaPieIzq.RenderModel();

		//Arenita Cola
		model = matrizauxAvatar;
		model = glm::translate(model, glm::vec3(movColaX, movColaY, movColaZ));
		model = glm::rotate(model, rotColaVertical * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotColaHorizontal * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArenitaCola.RenderModel();
		//------------------------------------------------------------------------------------------------------------

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys) {
	
	if (keys[GLFW_KEY_1]) {
		if (reproduciranimacion < 1) {
			if (play == false && (FrameIndex > 1)) {
				resetElements();
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				habilitaranimacion = 0;
			}
			else {
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_2]) {
		if (habilitaranimacion < 1) {
			reproduciranimacion = 0;
		}
	}
	//Manejo por teclas
	if (keys[GLFW_KEY_3]) {
		if (reproduciranimacion2 < 1) {
			if (play2 == false && (FrameIndex2 > 1)) {
				resetElements2();
				interpolation2();
				play2 = true;
				playIndex2 = 0;
				i_curr_steps2 = 0;
				reproduciranimacion2++;
				habilitaranimacion2 = 0;
			}
			else {
				play2 = false;
			}
		}
	}
	if (keys[GLFW_KEY_4]) {
		if (habilitaranimacion2 < 1) {
			reproduciranimacion2 = 0;
		}
	}
}