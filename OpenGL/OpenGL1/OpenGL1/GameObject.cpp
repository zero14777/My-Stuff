#include "GameObject.h"
#include "GOManager.h"

#include <iostream>

GameObject::GameObject(float x, float y) {
	game_objects.push_back(this);
	this->x = x;
	this->y = y;
}

GameObject::~GameObject() {

}