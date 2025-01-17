#include "GameObject.h"

GameObject::GameObject()
{
	initialize();
}

void GameObject::initialize()
{
	//Por enquanto trabalharemos apenas com sprites retangulares
	//com cor e textura -- e por enquanto sem spritesheet
	
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	transform = glm::mat4(1); //matriz identidade
	texID = -1; //ainda n�o temos
	shader = NULL; //ainda n�o temos
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	angle = 0.0f;

	iFrame = 1;
	iAnim = 1;
}

void GameObject::setTexture(int texID)
{
	this->texID = texID;
}

void GameObject::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::rotate(transform, angle, axis);
}

void GameObject::setTranslation(glm::vec3 displacements, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::translate(transform, displacements);
}

void GameObject::setScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::scale(transform, scaleFactors);
	scale = scaleFactors;
}

void GameObject::setVelocidadeMovimento(double velocidade_movimento)
{
	velocidadeMovimento = velocidade_movimento;
}

void GameObject::moveUp() {
	pos.y += velocidadeMovimento;
}

void GameObject::moveDown() {
	pos.y -= velocidadeMovimento;
}

void GameObject::moveLeft() {
	pos.x -= velocidadeMovimento;
}

void GameObject::moveRight() {
	pos.x += velocidadeMovimento;
}

void GameObject::draw()
{
	glActiveTexture(GL_TEXTURE0); // para funcionar com os filtros multitexture
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GameObject::update()
{
	//Por enquanto o update � basicamente pedir para atualizar no shader 
	//a matriz de transforma��o
	shader->Use();

	setTranslation(pos);
	setRotation(angle, glm::vec3(0.0f, 0.0f, 1.0f), false);
	setScale(scale, false);

	GLint transformLoc = glGetUniformLocation(shader->Program, "model");
	// Pass them to the shaders
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	//TO DO: quando tiver anima��o por spritesheet, passar os deslocamentos
	//nas coords de textura

	// GLint modelTex = glGetUniformLocation(shader->Program, "detex");
	// glUniform2f(modelTex, iFrame, iAnim);
}

double GameObject::getPositionX()
{
	return pos.x;
}

double GameObject::getPositionY()
{
	return pos.y;
}

bool GameObject::collidesWith(GameObject* otherObject)
{
	bool collides = false;

	// Calcula o tamanho ocupado pelo objeto e ve se ele toca ou atravessa o que est� sendo comparado
	double borderLeft   = (scale.x / 2) - pos.x;
	double borderTop    = (scale.y / 2) + pos.y;
	double borderRight  = (scale.x / 2) + pos.x;
	double borderBottom = (scale.y / 2) - pos.y;
	
	//  double positionOtherObject = otherObject->getPositionX();

	// if (positionOtherObject >= borderLeft && positionOtherObject <= borderRight)
	// 	collides = true;

	return collides;
}

void GameObject::printPosition(string objeto)
{
	cout << " Objeto " << objeto << ": [ " << pos.x << " | " << pos.y << " | " << pos.z << " ] " << endl;
}