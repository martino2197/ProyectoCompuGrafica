#include <iostream>
#include <cmath>
// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
/////////////////
#include "stb_image.h"
////////////////

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

void animacion();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

///////////////////////
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
bool active;


glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

//posicion de mi spotlight
glm::vec3 spotLightPositions[] = {
	glm::vec3(0.0f,  0.0f,  2.0f),
	glm::vec3(0.0f,  0.0f,  -1.0f)
};

glm::vec3 LightP1;
glm::vec3 LightP2;
glm::vec3 LightP3;
glm::vec3 LightP4;


//Animación de lamparas
float movKitX = 0.0;
float movKitZ = 0.0;
float rotKit = 0.0;

float rotKita = 45.0f;

bool circuito = false;

bool circuito2 = false;

bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;
///////////////////////

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "P8-Pt2", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	/////////////////////////
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup and compile our shaders
	////////////////////////
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	//Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");

	////////////////////////
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	///////////////////////////
	// Load models

	Model ourModel((char *)"Models/heyArnold/LibrerosEscritorio.obj");
	Model ourModel2((char *)"Models/heyArnold/libreroMoradoFinal.obj");
	
	Model ourModel3((char *)"Models/heyArnold/baul.obj");
	Model ourModel4((char *)"Models/heyArnold/ParedTecho.obj");
	Model ourModel5((char *)"Models/heyArnold/Lamparas.obj");
	Model ourModel6((char *)"Models/heyArnold/suelo.obj");
	Model ourModel7((char *)"Models/heyArnold/papelTapiz.obj");
	Model ourModel8((char *)"Models/heyArnold/puertaFinal.obj");
	Model ourModel9((char *)"Models/heyArnold/pcM5.obj");

	Model ourModel10((char *)"Models/heyArnold/sueloCama.obj");
	
	Model ourModel11((char *)"Models/heyArnold/camaBase.obj");
	Model ourModel12((char *)"Models/heyArnold/colchon.obj");
	Model ourModel13((char *)"Models/heyArnold/sabanaVerde.obj");
	Model ourModel14((char *)"Models/heyArnold/almohada.obj");
	Model ourModel15((char *)"Models/heyArnold/sofa.obj");




	//Model ourModel15((char *)"Models/heyArnold/camaBase.obj");
	/*Model ourModel8((char *)"Models/heyArnold/sueloCama.obj");
	Model ourModel9((char *)"Models/heyArnold/colchon.obj");
	Model ourModel10((char *)"Models/heyArnold/sabanaVerde.obj");
	Model ourModel11((char *)"Models/heyArnold/almohada.obj");*/
	/*Model ourModel6((char *)"Models/heyArnold/sueloCama.obj");

	Model ourModel8((char *)"Models/heyArnold/colchon.obj");
	Model ourModel9((char *)"Models/heyArnold/sabana.obj");
	Model ourModel10((char *)"Models/heyArnold/sabanaVerde.obj");
	Model ourModel11((char *)"Models/heyArnold/almohada.obj");
	Model ourModel12((char *)"Models/heyArnold/libreroMorado.obj");
	Model ourModel13((char *)"Models/heyArnold/cajonesRojos.obj");
	Model ourModel14((char *)"Models/heyArnold/perillaRoja.obj");
	Model ourModel15((char *)"Models/heyArnold/tvCuerpo.obj");
	Model ourModel16((char *)"Models/heyArnold/botonesTv.obj");
	Model ourModel17((char *)"Models/heyArnold/pantallaTv.obj");
	Model ourModel18((char *)"Models/heyArnold/perillaDorada.obj");
	Model ourModel19((char *)"Models/heyArnold/bocinaLibrero.obj");
	Model ourModel20((char *)"Models/heyArnold/radiador.obj");
	Model ourModel21((char *)"Models/heyArnold/ventana1.obj");
	*/
	// Draw in wireframe
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	//////////////////////////
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Load textures
	GLuint texture1, texture2, texture3, texture4, texture5, texture6, texture7;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);
	glGenTextures(1, &texture3);
	glGenTextures(1, &texture4);
	glGenTextures(1, &texture5);
	glGenTextures(1, &texture6);
	glGenTextures(1, &texture7);

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	image = stbi_load("images/arnoldEd.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	// Specular map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/ladrillos2.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	//Hueso
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/HuesoRGBA.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	//ventana
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/ventana.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	//ventana 2
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/ventana2.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	//ventanaRedonda
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/ventanaRedonda.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	//////puerta
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/puerta.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
	/////////////////////////



	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//////////////////////// Iluminacion
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//// Get the uniform locations
		//GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		//glm::mat4 modela(1);


		glBindVertexArray(VAO);
		///////////////////////

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glm::mat4 modela = glm::mat4(1.0f);
		
		modela = glm::translate(modela, glm::vec3(-11.8f, -6.25f, -11.0f)); // Translate it down a bit so it's at the center of the scene
		modela = glm::scale(modela, glm::vec3(0.5f, 45.0f, 75.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modela));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glm::mat4 modelb = glm::mat4(1.0f);
		modelb = glm::translate(modelb, glm::vec3(13.f, -6.25f, 26.25f)); // Translate it down a bit so it's at the center of the scene
		modelb = glm::scale(modelb, glm::vec3(50.0f, 45.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelb));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glm::mat4 modelc = glm::mat4(1.0f);
		modelc = glm::translate(modelc, glm::vec3(13.0f, -6.25f, -48.25f)); // Translate it down a bit so it's at the center of the scene
		modelc = glm::scale(modelc, glm::vec3(50.0f, 45.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelc));
		glDrawArrays(GL_TRIANGLES, 0, 36);





		glm::mat4 modeld = glm::mat4(1.0f);
		modeld = glm::translate(modeld, glm::vec3(37.75f, -6.25f, -39.5f)); // Translate it down a bit so it's at the center of the scene
		modeld = glm::scale(modeld, glm::vec3(0.5f, 45.0f, 18.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeld));
		glDrawArrays(GL_TRIANGLES, 0, 36);





		glm::mat4 modele = glm::mat4(1.0f);
		modele = glm::translate(modele, glm::vec3(52.5f, -6.25f, -30.5f)); // Translate it down a bit so it's at the center of the scene
		modele = glm::scale(modele, glm::vec3(30.0f, 45.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modele));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelf = glm::mat4(1.0f);
		modelf = glm::translate(modelf, glm::vec3(52.5f, -6.25f, 26.25f)); // Translate it down a bit so it's at the center of the scene
		modelf = glm::scale(modelf, glm::vec3(30.0f, 45.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelf));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glm::mat4 modelg = glm::mat4(1.0f);

		modelg = glm::translate(modelg, glm::vec3(67.25f, -6.25f, -2.0f)); // Translate it down a bit so it's at the center of the scene
		modelg = glm::scale(modelg, glm::vec3(0.5f, 45.0f, 57.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelg));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		///////////fachada
		glm::mat4 modelfa = glm::mat4(1.0f);
		modelfa = glm::translate(modelfa, glm::vec3(62.5f, -6.25f, 30.55f)); // Translate it down a bit so it's at the center of the scene
		modelfa = glm::rotate(modelfa, glm::radians(325.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelfa = glm::scale(modelfa, glm::vec3(1.0f, 45.0f, 10.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelfa));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glm::mat4 modelfb = glm::mat4(1.0f);
		modelfb = glm::translate(modelfb, glm::vec3(52.5f, -6.25f, 35.0f)); // Translate it down a bit so it's at the center of the scene
		modelfb = glm::scale(modelfb, glm::vec3(14.0f, 45.0f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelfb));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		glm::mat4 modelfc = glm::mat4(1.0f);
		modelfc = glm::translate(modelfc, glm::vec3(41.625f, -6.25f, 30.55f)); // Translate it down a bit so it's at the center of the scene
		modelfc = glm::rotate(modelfc, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelfc = glm::scale(modelfc, glm::vec3(1.0f, 45.0f, 10.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelfc));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture7);

		glm::mat4 modelfbp = glm::mat4(1.0f);
		modelfbp = glm::translate(modelfbp, glm::vec3(52.5f, -16.25f, 35.75f)); // Translate it down a bit so it's at the center of the scene
		modelfbp = glm::scale(modelfbp, glm::vec3(14.0f, 30.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelfbp));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);

		glm::mat4 modelfaventana = glm::mat4(1.0f);
		modelfaventana = glm::translate(modelfaventana, glm::vec3(62.5f, 6.5f, 30.75f)); // Translate it down a bit so it's at the center of the scene
		modelfaventana = glm::rotate(modelfaventana, glm::radians(55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelfaventana = glm::scale(modelfaventana, glm::vec3(7.0f, 12.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelfaventana));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		glm::mat4 modelfbpventana = glm::mat4(1.0f);
		modelfbpventana = glm::translate(modelfbpventana, glm::vec3(52.5f, 6.5f, 35.75f)); // Translate it down a bit so it's at the center of the scene
		modelfbpventana = glm::scale(modelfbpventana, glm::vec3(7.0f, 12.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelfbpventana));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glm::mat4 modelfcventana = glm::mat4(1.0f);
		modelfcventana = glm::translate(modelfcventana, glm::vec3(41.625f, 6.5f, 30.75f)); // Translate it down a bit so it's at the center of the scene
		modelfcventana = glm::rotate(modelfcventana, glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelfcventana = glm::scale(modelfcventana, glm::vec3(7.0f, 12.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelfcventana));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);

		glm::mat4 modelah = glm::mat4(1.0f);
		modelah = glm::translate(modelah, glm::vec3(-11.8f, -6.25f, -11.0f)); // Translate it down a bit so it's at the center of the scene
		modelah = glm::scale(modelah, glm::vec3(1.5f, 3.0f, 75.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelah));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelah2 = glm::mat4(1.0f);
		modelah2 = glm::translate(modelah2, glm::vec3(-13.0f, 17.0f, -11.0f)); // Translate it down a bit so it's at the center of the scene
		modelah2 = glm::scale(modelah2, glm::vec3(3.5f, 1.0f, 75.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelah2));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glm::mat4 modelbh = glm::mat4(1.0f);
		modelbh = glm::translate(modelbh, glm::vec3(13.f, -6.25f, 26.25f)); // Translate it down a bit so it's at the center of the scene
		modelbh = glm::scale(modelbh, glm::vec3(50.0f, 3.0f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelbh));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelbh2 = glm::mat4(1.0f);
		modelbh2 = glm::translate(modelbh2, glm::vec3(13.f, 17.0f, 27.45f)); // Translate it down a bit so it's at the center of the scene
		modelbh2 = glm::scale(modelbh2, glm::vec3(50.0f, 1.0f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelbh2));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glm::mat4 modelch = glm::mat4(1.0f);
		modelch = glm::translate(modelch, glm::vec3(13.0f, -6.25f, -48.25f)); // Translate it down a bit so it's at the center of the scene
		modelch = glm::scale(modelch, glm::vec3(50.0f, 3.0f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelch));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelch2 = glm::mat4(1.0f);
		modelch2 = glm::translate(modelch2, glm::vec3(13.0f, 17.0f, -49.45f)); // Translate it down a bit so it's at the center of the scene
		modelch2 = glm::scale(modelch2, glm::vec3(50.0f, 1.0f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelch2));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glm::mat4 modeldh = glm::mat4(1.0f);
		modeldh = glm::translate(modeldh, glm::vec3(37.75f, -6.25f, -39.5f)); // Translate it down a bit so it's at the center of the scene
		modeldh = glm::scale(modeldh, glm::vec3(1.5f, 3.0f, 18.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeldh));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modeldh2 = glm::mat4(1.0f);
		modeldh2 = glm::translate(modeldh2, glm::vec3(38.95f, 17.0f, -39.5f)); // Translate it down a bit so it's at the center of the scene
		modeldh2 = glm::scale(modeldh2, glm::vec3(3.5f, 1.0f, 18.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeldh2));
		glDrawArrays(GL_TRIANGLES, 0, 36);





		glm::mat4 modeleh = glm::mat4(1.0f);
		modeleh = glm::translate(modeleh, glm::vec3(52.5f, -6.25f, -30.5f)); // Translate it down a bit so it's at the center of the scene
		modeleh = glm::scale(modeleh, glm::vec3(30.0f, 3.0f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeleh));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modeleh2 = glm::mat4(1.0f);
		modeleh2 = glm::translate(modeleh2, glm::vec3(52.5f, 17.0f, -31.7f)); // Translate it down a bit so it's at the center of the scene
		modeleh2 = glm::scale(modeleh2, glm::vec3(30.0f, 1.0f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeleh2));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelfh = glm::mat4(1.0f);
		modelfh = glm::translate(modelfh, glm::vec3(52.5f, -6.25f, 26.25f)); // Translate it down a bit so it's at the center of the scene
		modelfh = glm::scale(modelfh, glm::vec3(30.0f, 3.0f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelfh));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelfh2 = glm::mat4(1.0f);
		modelfh2 = glm::translate(modelfh2, glm::vec3(52.5f, 17.0f, 27.45f)); // Translate it down a bit so it's at the center of the scene
		modelfh2 = glm::scale(modelfh2, glm::vec3(30.0f, 1.0f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelfh2));
		glDrawArrays(GL_TRIANGLES, 0, 36);





		glm::mat4 modelgh = glm::mat4(1.0f);
		modelgh = glm::translate(modelgh, glm::vec3(67.25f, -6.25f, -2.0f)); // Translate it down a bit so it's at the center of the scene
		modelgh = glm::scale(modelgh, glm::vec3(1.5f, 3.0f, 57.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelgh));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelgh2 = glm::mat4(1.0f);
		modelgh2 = glm::translate(modelgh2, glm::vec3(68.45f, 17.0f, -2.0f)); // Translate it down a bit so it's at the center of the scene
		modelgh2 = glm::scale(modelgh2, glm::vec3(3.5f, 1.0f, 57.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelgh2));
		glDrawArrays(GL_TRIANGLES, 0, 36);






		//ventana

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);

		glm::mat4 modelav = glm::mat4(1.0f);
		modelav = glm::translate(modelav, glm::vec3(-2.0f, -14.5f, 26.25f)); // Translate it down a bit so it's at the center of the scene
		modelav = glm::scale(modelav, glm::vec3(7.0f, 12.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelav));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelav2 = glm::mat4(1.0f);
		modelav2 = glm::translate(modelav2, glm::vec3(8.5f, -14.5f, 26.25f)); // Translate it down a bit so it's at the center of the scene
		modelav2 = glm::scale(modelav2, glm::vec3(7.0f, 12.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelav2));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelav3 = glm::mat4(1.0f);
		modelav3 = glm::translate(modelav3, glm::vec3(19.0f, -14.5f, 26.25f)); // Translate it down a bit so it's at the center of the scene
		modelav3 = glm::scale(modelav3, glm::vec3(7.0f, 12.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelav3));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//ventana2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);

		glm::mat4 modelavdos = glm::mat4(1.0f);
		modelavdos = glm::translate(modelavdos, glm::vec3(8.5f, 6.5f, 26.15f)); // Translate it down a bit so it's at the center of the scene
		modelavdos = glm::scale(modelavdos, glm::vec3(7.0f, 12.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelavdos));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelavdos2 = glm::mat4(1.0f);
		modelavdos2 = glm::translate(modelavdos2, glm::vec3(19.0f, 6.5f, 26.15f)); // Translate it down a bit so it's at the center of the scene
		modelavdos2 = glm::scale(modelavdos2, glm::vec3(7.0f, 12.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelavdos2));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//ventanaRedondad
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);
		glm::mat4 modelavRedonda = glm::mat4(1.0f);
		modelavRedonda = glm::translate(modelavRedonda, glm::vec3(29.5f, -14.5f, 26.15f)); // Translate it down a bit so it's at the center of the scene
		modelavRedonda = glm::scale(modelavRedonda, glm::vec3(6.5f, 6.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelavRedonda));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
*/







		////////////////Modelos
		view = camera.GetViewMatrix();


		// Draw the loaded model
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel.Draw(lightingShader);

		glm::mat4 model2(1);
		model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel2.Draw(lightingShader);

		glm::mat4 model3(1);
		model3 = glm::translate(model3, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model3 = glm::scale(model3, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel3.Draw(lightingShader);

		glm::mat4 model4(1);
		model4 = glm::translate(model4, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model4 = glm::scale(model4, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel4.Draw(lightingShader);

		///////lamaparas
		glm::mat4 model5(1);
		model5 = glm::translate(model5, glm::vec3(0.0f, -1.75f, 0.0f));; // Translate it down a bit so it's at the center of the scene
		model5 = glm::scale(model5, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down

		//model5 = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel5.Draw(lightingShader);

		glm::mat4 model6(1);
		model6 = glm::translate(model6, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model6 = glm::scale(model6, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel6.Draw(lightingShader);

		//glm::mat4 model6(1);
		//model6 = glm::translate(model6, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model6 = glm::scale(model6, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//ourModel6.Draw(shader);

		glm::mat4 model7(1);
		model7 = glm::translate(model7, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model7 = glm::scale(model7, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel7.Draw(lightingShader);


		glm::mat4 model8(1);
		model8 = glm::translate(model8, glm::vec3(0.0f, -1.75f, 0.0f));// Translate it down a bit so it's at the center of the scene
		model8 = glm::scale(model8, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		model8 = glm::rotate(model8, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel8.Draw(lightingShader);

		glm::mat4 model9(1);
		model9 = glm::translate(model9, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model9 = glm::scale(model9, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel9.Draw(lightingShader);


		glm::mat4 model10(1);
		model10 = glm::translate(model10, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model10 = glm::scale(model10, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel10.Draw(lightingShader);

		glm::mat4 model11(1);
		model11 = glm::translate(model11, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model11 = glm::scale(model11, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel11.Draw(lightingShader);

		glBindVertexArray(0);
		glm::mat4 model12(1);
		model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel12.Draw(lightingShader);



		//////////////////////animacion 
		glm::mat4 model13(1);
		model13 = glm::translate(model13, PosIni + glm::vec3(movKitX, -1.75f, movKitZ)); // Translate it down a bit so it's at the center of the scene
		model13 = glm::scale(model13, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		model13 = glm::rotate(model13, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model13));

		ourModel13.Draw(lightingShader);

		
		
		glm::mat4 model14(1);
		model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel14.Draw(lightingShader);

		glm::mat4 model15(1);
		model15 = glm::translate(model15, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model15 = glm::scale(model15, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		ourModel15.Draw(lightingShader);





















		//glm::mat4 model16(1);
		//model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//ourModel16.Draw(shader);

		//glm::mat4 model17(1);
		//model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//ourModel17.Draw(shader);

		//glm::mat4 model18(1);
		//model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//ourModel18.Draw(shader);

		//glm::mat4 model19(1);
		//model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//ourModel19.Draw(shader);

		//glm::mat4 model20(1);
		//model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//ourModel20.Draw(shader);

		//glm::mat4 model21(1);
		//model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//ourModel21.Draw(shader);


	

		/*modela = glm::mat4(1.0f);
		modela = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modela));
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

		glBindVertexArray(0);

		////////////////////////
		

		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);




		

		//glm::mat4 modelb= glm::mat4(1.0f);
		//modelb = glm::translate(modelb, glm::vec3(11.5f, 7.5f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//modelb = glm::scale(modelb, glm::vec3(1.0f, 20.0f, 50.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelb));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		
		////////////////////////

		






		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (keys[GLFW_KEY_I])
	{
		pointLightPositions[3].x += 0.1f;
		pointLightPositions[3].y += 0.1f;
		pointLightPositions[3].z += 0.1f;
		circuito = true;
		circuito2 = true;
	}

	if (keys[GLFW_KEY_O])
	{
		circuito = false;
		circuito2 = false;
	}
	if (keys[GLFW_KEY_K])
	{
		pointLightPositions[3].x -= 0.1f;
		pointLightPositions[3].y -= 0.1f;
		pointLightPositions[3].z -= 0.1f;
	}
}



//////////////////////////////////Animaciones
void animacion()
{

	//Movimiento del coche
	if (circuito)
	{
		if (recorrido1)
		{
			rotKit = 0;
			movKitZ += 0.1f;
			if (movKitZ > 6)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			rotKit = 0;
			movKitX += 0.1f;
			if (movKitX < 6)
			{
				recorrido2 = false;
				recorrido3 = true;

			}
		}

		if (recorrido3)
		{
			rotKit = 0;
			movKitZ += 0.1f;
			if (movKitZ > 6)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}

		if (recorrido4)
		{
			rotKit = 0;
			movKitX -= 0.1f;
			if (movKitX < 0)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5)
		{
			rotKit = 0;
			movKitZ -= 0.1f;
			if (movKitZ < 0)
			{
				recorrido5 = false;
				recorrido1 = true;
			}
		}


	}
}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f),// Rojo
			LightP2 = glm::vec3(0.0f, 1.0f, 0.0f),// Verde
			LightP3 = glm::vec3(0.0f, 0.0f, 1.0f),// Azul
			LightP4 = glm::vec3(0.88f, 1.0f, 0.0f);// verde limon
		else
			LightP1 = LightP2 = LightP3 = LightP4 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}