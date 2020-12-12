#include "Layer.h"

Layer::Layer()
{
	initialize();
}

void Layer::initialize()
{
	//Por enquanto trabalharemos apenas com sprites retangulares
	//com cor e textura -- e por enquanto sem spritesheet

}

void Layer::addItem(GameObject item)
{
	items.push_back(item);
}

void Layer::addItems(vector <GameObject> newItems)
{
	items = newItems;
}

void Layer::setVelocidadeMovimento(double velocidadeMov)
{
	for(int i = 0; i < items.size(); i++)
		items[i].setVelocidadeMovimento(velocidadeMov);
}

void Layer::moveItemsUp()
{
	for (int i = 0; i < items.size(); i++)
		items[i].moveUp();
}

void Layer::moveItemsDown()
{
	for (int i = 0; i < items.size(); i++)
		items[i].moveDown();
}

void Layer::moveItemsLeft()
{
	for (int i = 0; i < items.size(); i++)
		items[i].moveLeft();
}

void Layer::moveItemsRight()
{
	for (int i = 0; i < items.size(); i++)
		items[i].moveRight();
}