#include "SceneManager.h"
#include "Layer.h"

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;
static double xMousePos, yMousePos;
static double espacoShaders;
static GameObject* personagem;
static GameObject* fundoEspacial;
static vector <Layer> camadas;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Hello Filters", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Adicionando os shaders
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");
	shaders[0]->setInt("code", 2);

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

}

void SceneManager::addShader(string vFilename, string fFilename)
{
	Shader *shader = new Shader (vFilename.c_str(), fFilename.c_str());
	shaders.push_back(shader);
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xMousePos, &yMousePos);
		cout << "Mouse clicado na posição X [" << xMousePos << "] : Y [" << yMousePos << "]" << endl;
	}
}

void SceneManager::updatePictureShader(int posObject, int posShader)
{
	objects[posObject]->setShader(shaders[posShader]);
}

void SceneManager::placeSticker(string textureName)
{

	int textureSticker = loadTexture("../textures/" + textureName + ".png");
	GameObject* obj = new GameObject;
	obj->setPosition(glm::vec3(xMousePos, height - yMousePos, 1.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setTexture(textureSticker);
	obj->setShader(shaders[0]);
	objects.push_back(obj);
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (personagem->getPositionX() + 1 > width)
		personagem->moveLeft();

	if (personagem->getPositionX() - 1 < 0)
		personagem->moveRight();

	if (personagem->getPositionY() + 1 > height)
		personagem->moveDown();

	if (personagem->getPositionY() - 1 < 0)
		personagem->moveUp();

	if (keys[GLFW_KEY_UP]) {
		personagem->moveUp();
		for (int i = 0; i < objects.size(); i++)
			objects[i]->moveUp();
		personagem->printPosition("Terra");
	}
	if (keys[GLFW_KEY_DOWN]) {
		personagem->moveDown();
		for (int i = 0; i < objects.size(); i++)
			objects[i]->moveDown();
		personagem->printPosition("Terra");
	}
	if (keys[GLFW_KEY_LEFT]) {
		personagem->moveLeft();
		for (int i = 0; i < objects.size(); i++)
			objects[i]->moveLeft();
		personagem->printPosition("Terra");
	}
	if (keys[GLFW_KEY_RIGHT]) {
		personagem->moveRight();
		for (int i = 0; i < objects.size(); i++)
			objects[i]->moveRight();
		personagem->printPosition("Terra");
	}
	
	// Testa se houve colisão
	for (int i = 0; i < objects.size(); i++)
		if (personagem->collidesWith(objects[i])) {
			delete objects[i];
			delete personagem;
			break;
		}
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	//atualiza e desenha os gameObjects
	fundoEspacial->update();
	fundoEspacial->draw();
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->draw();
	}
	personagem->update();
	personagem->draw();
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		update();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	//Carregamento das texturas (pode ser feito intercalado na criação)
	//Futuramente, utilizar classe de materiais e armazenar dimensoes, etc
	unsigned int texTerra      = loadTexture("../textures/globe-sprite-min-450px.png");
	unsigned int textMarte     = loadTexture("../textures/marte.png");
	unsigned int texAsteroide  = loadTexture("../textures/asteroid.png");
	unsigned int texAsteroide1 = loadTexture("../textures/asteroid1.png");
	unsigned int texAsteroide2 = loadTexture("../textures/asteroid2.png");
	unsigned int texEstrelas   = loadTexture("../textures/stars.png");
	unsigned int texEspaco     = loadTexture("../textures/space.jpg");

	// Carrega objeto Terra
	GameObject* terra = new GameObject;
	terra->setPosition(glm::vec3(width / 2, height / 2, -5.0f));
	terra->setDimension(glm::vec3(100.0f, 100.0f, 5.0f));
	terra->setTexture(texTerra);
	terra->setShader(shaders[0]);
	terra->setVelocidadeMovimento(1.5f);
	// objects.push_back(terra);
	personagem = terra;

	// Carrega objeto Marte
	GameObject* marte = new GameObject;
	marte->setPosition(glm::vec3(50.0f, 450.0f, 0.0f));
	marte->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	marte->setTexture(textMarte);
	marte->setShader(shaders[0]);
	marte->setVelocidadeMovimento(0.5f);
	objects.push_back(marte);

	// Carrega objeto asteroide
	GameObject* asteroide = new GameObject;
	asteroide->setPosition(glm::vec3(200.0f, 450.0f, 0.0f));
	asteroide->setDimension(glm::vec3(100.0f, 100.0f, 1.0f));
	asteroide->setTexture(texAsteroide);
	asteroide->setShader(shaders[0]);
	asteroide->setVelocidadeMovimento(0.8f);
	objects.push_back(asteroide);

	// asteroide 2
	asteroide = new GameObject;
	asteroide->setPosition(glm::vec3(520.0f, 150.0f, 0.0f));
	asteroide->setDimension(glm::vec3(150.0f, 150.0f, 1.0f));
	asteroide->setTexture(texAsteroide2);
	asteroide->setShader(shaders[0]);
	asteroide->setVelocidadeMovimento(0.4f);
	objects.push_back(asteroide);

	// asteroide 3
	asteroide = new GameObject;
	asteroide->setPosition(glm::vec3(240.0f, 270.0f, 0.0f));
	asteroide->setDimension(glm::vec3(200.0f, 200.0f, 1.0f));
	asteroide->setTexture(texAsteroide1);
	asteroide->setShader(shaders[0]);
	asteroide->setVelocidadeMovimento(0.4f);
	objects.push_back(asteroide);

	// asteroide 4
	asteroide = new GameObject;
	asteroide->setPosition(glm::vec3(640.0f, 620.0f, 0.0f));
	asteroide->setDimension(glm::vec3(300.0f, 300.0f, 1.0f));
	asteroide->setTexture(texAsteroide);
	asteroide->setShader(shaders[0]);
	asteroide->setVelocidadeMovimento(0.2f);
	objects.push_back(asteroide);

	// Carrega fundo de estrelas
	GameObject* estrelas = new GameObject;
	estrelas->setPosition(glm::vec3(width / 2, height / 2, 0.0f));
	estrelas->setDimension(glm::vec3(width, height, 2.0f));
	estrelas->setTexture(texEstrelas);
	estrelas->setShader(shaders[0]);
	estrelas->setVelocidadeMovimento(0.05f);
	objects.push_back(estrelas);

	// Carrega fundo de espaço
	GameObject* espaco = new GameObject;
	espaco->setPosition(glm::vec3(width / 2, height / 2, 5.0f));
	espaco->setDimension(glm::vec3(width, height, 2.0f));
	espaco->setTexture(texEspaco);
	espaco->setShader(shaders[0]);
	espaco->setVelocidadeMovimento(0.0f);
	// objects.push_back(espaco);

	fundoEspacial = espaco;

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void SceneManager::setupCamera2D() //TO DO: parametrizar aqui
{
	//corrigindo o aspecto
	float ratio = 1.0;
	float xMin = 0.0, xMax = 800.0, yMin = 0.0, yMax = 600.0, zNear = -5.0, zFar = 5.0;

	projection = glm::ortho(xMin * ratio, xMax * ratio, yMin, yMax, zNear, zFar);

	// Get their uniform location
	for (int i = 0; i < shaders.size(); i++)
	{
		shaders[i]->Use();
		GLint projLoc = glGetUniformLocation(shaders[i]->Program, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
}

unsigned int SceneManager::loadTexture(string filename)
{
	unsigned int texture;

	// load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	return texture;
}
