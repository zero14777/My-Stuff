#ifndef GAME_OBJ_H
#define GAME_OBJ_H

class GameObject {
public:
	GameObject(float x, float y);
	virtual ~GameObject();

	float x;
	float y;
protected:
private:
	GameObject(const GameObject& other) {}
	GameObject& operator=(const GameObject& other) {}

};

#endif