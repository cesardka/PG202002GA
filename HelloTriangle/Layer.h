#pragma once

#include "GameObject.h" 

//Esta classe vai ir sofrendo alterações durante o curso
//Aqui ela está preparada apenas para sprites texturizados

#include "Shader.h"
#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Layer
{
public:
	Layer();
	~Layer() {}

	void initialize();
	void addItem(GameObject item);
	void addItems(vector <GameObject> newItems);
	void setVelocidadeMovimento(double velocidadeMov);

	void moveItemsUp();
	void moveItemsDown();
	void moveItemsLeft();
	void moveItemsRight();

private:
	vector <GameObject> items;
	double velocidadeMovimento;
};

