#ifndef GO_MAN_H
#define GO_MAN_H

#include <vector>
#include "GameObject.h"

extern std::vector<GameObject*> game_objects;

class GOManager {
public:
	GOManager();

	virtual ~GOManager();

protected:
private:
	GOManager(const GOManager& other) {}
	GOManager& operator=(const GOManager& other) {}

};

#endif