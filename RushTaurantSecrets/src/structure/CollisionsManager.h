#pragma once
#include <vector>
using namespace std;
class Scene;
class GameObject;
class CollisionsManager {
private:
	Scene* scene_;
	vector<GameObject*> grp_Player;
	vector<GameObject*> grup_Collisions;

	vector<GameObject*> grp_Customers;
	vector<GameObject*> grp_Interactable;
public:
	CollisionsManager(Scene* scene);
	
	void Collision();
	void Overlap();

	void hasCollision(GameObject* object);

	void hasOverlap(GameObject* object);

};