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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

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
Model Plankton;
Model Karen;
Model Sandy;
Model ArenitaPieDer;
Model ArenitaPieIzq;

//Buildings
Model KrustyKrab;
Model ChristmasTree;
Model SpongeBobHouse;
Model SquidwardHouse;
Model PatrickHouse;

//Objects
Model Bombilla;
Model Snowmans;
Model Snowman01;
Model Snowman02;

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

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	//pisoTexture = Texture("Textures/piso.tga");
	pisoTexture = Texture("Textures/SnowFloor.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
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
	helice.LoadModel("Models/helice.obj");

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
	Gary.LoadModel("Models/Gary.obj");

	Plankton = Model();
	Plankton.LoadModel("Models/Plankton.obj");

	Karen = Model();
	Karen.LoadModel("Models/Karen.obj");

	Sandy = Model();
	Sandy.LoadModel("Models/Arenita.obj");

	ArenitaPieDer = Model();
	ArenitaPieDer.LoadModel("Models/ArenitaPieDer.obj");

	ArenitaPieIzq = Model();
	ArenitaPieIzq.LoadModel("Models/ArenitaPieIzq.obj");

	//Buildings
	KrustyKrab = Model();
	KrustyKrab.LoadModel("Models/KrustyKrab2.obj");

	SpongeBobHouse = Model();
	SpongeBobHouse.LoadModel("Models/SpongeBobHouse.obj");

	SquidwardHouse = Model();
	SquidwardHouse.LoadModel("Models/SquidwardHouse.obj");

	PatrickHouse = Model();
	PatrickHouse.LoadModel("Models/PatrickHouse.obj");

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

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//posición inicial del helicóptero
	glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);

	//posición inicial del faro
	glm::vec3 posfaro = glm::vec3(-7.0f, 6.0f, 2.0f);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	/*spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;*/
	
	//luz de helicóptero
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		-20.0f, 6.0f, -1.0,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
		
	//luz de faro1
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		-5.0f, 1.8f, 1.65f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
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
		}
		else {
			skyboxFBN.DrawSkybox(camera.calculateViewMatrix(), projection);
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

		//luz ligada a la cámara de tipo flash 
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

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
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mKrabs.RenderModel();

		//Bob Esponja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 5.0f, 4.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SpongeBob.RenderModel();

		//Calamardo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SquidWard.RenderModel();

		//Patricio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.0f, 27.0f, 27.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PatrickStar.RenderModel();

		//Gary
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gary.RenderModel();

		//Plankton
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Plankton.RenderModel();

		//Karen
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Karen.RenderModel();

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

		//-----------------------------------------------------------------------------------------

		//BUILDINGS
		//-----------------------------------------------------------------------------------------
		//Crustáceo Cascarudo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 60.0f));
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

		//------------------------------------------------------------------------------------------

		//OBJECTS 
		//-----------------------------------------------------------------------------------------

		//Árbol de Navidad
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(40.0f, 4.0f, 60.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ChristmasTree.RenderModel();

		//REGALOS
		//-------------------------------------------------------------------------------
		//Regalo01
		model = matrizauxKC;
		model = glm::translate(model, glm::vec3(-20.0f, 11.0f, -50.0f));
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


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
