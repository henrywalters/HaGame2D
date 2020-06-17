#include "stdafx.h"
#include "QuadTree.h"


QuadTree::QuadTree(float width, float height)
{
	root = new Quadrant;
	root->init(NULL, Box{ 0, 0, width, height });
}


QuadTree::~QuadTree()
{
}

void QuadTree::clear() {
	std::queue<Quadrant *> quadQueue;
	quadQueue.push(root);

	while (!quadQueue.empty()) {

		Quadrant * currentQuad = quadQueue.front();
		quadQueue.pop();
		if (currentQuad->quad1 != NULL) {
			quadQueue.push(currentQuad->quad1);
			quadQueue.push(currentQuad->quad2);
			quadQueue.push(currentQuad->quad3);
			quadQueue.push(currentQuad->quad4);
		}
		currentQuad->gameObjects.clear();
		if ( currentQuad != root) {
			free(currentQuad);
		}
	}

	free(root);
}

void QuadTree::split(Quadrant * quadrant) {
	float subWidth = quadrant->width / 2;
	float subHeight = quadrant->height / 2;
	float midX = quadrant->x + (quadrant->width / 2);
	float midY = quadrant->y + (quadrant->height / 2);

	//std::cout << "Splitting: " + Box{ quadrant->x, quadrant->y, quadrant->width, quadrant->height }.toString() << "\n";

	Quadrant *quad1 = new Quadrant;
	quad1->init(quadrant, Box{ midX, quadrant->y, subWidth, subHeight });
	Quadrant *quad2 = new Quadrant;
	quad2->init(quadrant, Box{ quadrant->x, quadrant->y, subWidth, subHeight });
	Quadrant *quad3 = new Quadrant;
	quad3->init(quadrant, Box{ quadrant->x, midY, subWidth, subHeight });
	Quadrant *quad4 = new Quadrant;
	quad4->init(quadrant, Box{ midX, midY, subWidth, subHeight });
	
	quadrant->quad1 = quad1;
	quadrant->quad2 = quad2;
	quadrant->quad3 = quad3;
	quadrant->quad4 = quad4;
	
	display->drawLine(midX, quadrant->y, midX, quadrant->y + quadrant->height, Color::green(), 18);
	display->drawLine(quadrant->x, midY, quadrant->x + quadrant->width, midY, Color::green(),18);
	
}

void drawQuadObject(Display * display, Quadrant * quad, GameObject * gameObject) {

}

QuadSet QuadTree::find(GameObject * gameObject) {
	BoxCollider * boxColl = gameObject->getComponent<BoxCollider>();
	Box box = boxColl->getBox();
	Quadrant * current = root;

	QuadSet results;

	bool found = false;

	int depth = 1;

	while (!found) {

		Quadrant * boundingBox = current->getBoundingQuad(gameObject);

		if (boundingBox != NULL) {
			current = boundingBox;
			depth += 1;
		}
		else {
			// In this case, it was found on an edge, so lets try and find the quads from the 4 pts. 
			
			//Check the quadrants that contain each point in the game object
			Quadrant * p1Quad = current->getBoundingQuad(Vector(gameObject->position.x + boxColl->width, gameObject->position.y));
			Quadrant * p2Quad = current->getBoundingQuad(Vector(gameObject->position.x, gameObject->position.y));
			Quadrant * p3Quad = current->getBoundingQuad(Vector(gameObject->position.x, gameObject->position.y + boxColl->height));
			Quadrant * p4Quad = current->getBoundingQuad(Vector(gameObject->position.x + boxColl->width, gameObject->position.y + boxColl->height));

			//Ugly lil bit of hard coded conditionals. Could be cleaned up, but for now it'll be chill.
			
			results.add(p1Quad);

			if (p2Quad != p1Quad) {
				results.add(p2Quad);
			}
			if (p3Quad != p1Quad && p3Quad != p2Quad) {
				results.add(p3Quad);
			}
			if (p4Quad != p1Quad && p4Quad != p2Quad && p4Quad != p3Quad) {
				results.add(p4Quad);
			}

			found = true;
		}

		if (!current->isSplit()) {
			//std::cout << "Found at depth: " << depth << " with " << current->gameObjects.size() << " inhabitants \n";
			results.add(current);
			found = true;
		}
	}

	return results;
}

std::vector<GameObject *> QuadTree::getNeighbors(GameObject * gameObject) {
	QuadSet quads = find(gameObject);
	std::vector<GameObject *> objects;

	for (int i = 0; i < quads.quadCount; i++) {
		objects.insert(objects.end(), quads.quads[i]->gameObjects.begin(), quads.quads[i]->gameObjects.end());
	}

	return objects;
}

void QuadTree::insert(GameObject * gameObject) {
	bool inserted = false;
	Quadrant * current = root;
	int depth = 1;
	BoxCollider * coll = gameObject->getComponent<BoxCollider>();

	if (!coll->getBox().boundedBy(root->getBox())) {

		//If the object isn't in the quad space, ignore it entirely.

		return;
	}

	if (coll != NULL) {
		Box currentBox = coll->getBox();

		while (!inserted) {
			if (current->isSplit()) {
				Quadrant * boundingBox = current->getBoundingQuad(gameObject);

				current->gameObjects.push_back(gameObject);

				if (boundingBox != NULL) {
					current = boundingBox;
					depth += 1;
				}
				else {
					inserted = true;
					Quadrant * p1Quad = current->getBoundingQuad(Vector(gameObject->position.x + currentBox.width, gameObject->position.y));
					Quadrant * p2Quad = current->getBoundingQuad(Vector(gameObject->position.x, gameObject->position.y));
					Quadrant * p3Quad = current->getBoundingQuad(Vector(gameObject->position.x, gameObject->position.y + currentBox.height));
					Quadrant * p4Quad = current->getBoundingQuad(Vector(gameObject->position.x + currentBox.width, gameObject->position.y + currentBox.height));

					if (p1Quad != boundingBox) {
						p1Quad->gameObjects.push_back(gameObject);
					}

					if (p2Quad != boundingBox) {
						p2Quad->gameObjects.push_back(gameObject);
					}

					if (p3Quad != boundingBox) {
						p3Quad->gameObjects.push_back(gameObject);
					}

					if (p4Quad != boundingBox) {
						p4Quad->gameObjects.push_back(gameObject);
					}
				}
			}
			else {

				current->gameObjects.push_back(gameObject);
				inserted = true;

				if (current->gameObjects.size() >= MAX_CHILDREN_IN_QUADRANT && depth < MAX_DEPTH) {
					split(current);

					/*
						Drag all the current game objects that are bounded down to the next level.
						If no box is bounding don't do anything because it's already there. We 
						Might be able to remove this step entirely.. it feels wasteful but I'm 
						not quite sure how yet.
					*/
					for (GameObject * currentGameObject : current->gameObjects) {
						
						//Check if Bounded or partially bounded
						std::vector<Quadrant *> quads = current->getPartiallyBoundingQuads(currentGameObject);

						for (Quadrant * quad : quads) {
							quad->gameObjects.push_back(currentGameObject);
						}
					}
					

					Quadrant * boundingBox = current->getBoundingQuad(gameObject);

					if (boundingBox != NULL) {
						boundingBox->gameObjects.push_back(gameObject);
					}
					else {
						inserted = true;
					}
				}
				else {
					inserted = true;
				}
			}
		}
	}
}

/*void QuadTree::insert(GameObject * gameObject) {
	bool inserted = false;
	Quadrant * current = root;
	int depth = 1;
	BoxCollider *coll = gameObject->getComponent<BoxCollider>();
	if (coll != NULL) {
		Box box = coll->getBox();
		while (!inserted) {
			if (current->quad1 == NULL) { // Current quadrant hasn't been split up;
				current->gameObjects.push_back(gameObject);
				inserted = true;
				if (gameObject->tag == "wall") {
					std::cout << "Depth of " << gameObject->tag << ": " << depth << "\n";
				}
				if (current->gameObjects.size() >= MAX_CHILDREN_IN_QUADRANT && depth < MAX_DEPTH) {
					split(current);
				}
			}
			else {
				//check each quadrant if it binds the gameObject, move to that node else add it to the current node
				Box b1 = current->quad1->getBox();
				Box b2 = current->quad2->getBox();
				Box b3 = current->quad3->getBox();
				Box b4 = current->quad4->getBox();

				/*if (gameObject->tag == "player") {
					std::cout << "Box 1: " << b1.toString() << std::endl;
					std::cout << "Box 2: " << b2.toString() << std::endl;
					std::cout << "Box 3: " << b3.toString() << std::endl;
					std::cout << "Box 4: " << b4.toString() << std::endl;

					std::cout << "Player: " << box.toString() << std::endl;
				}
				
				if (b1 == b2 && b2 == b3 && b3 == b4) {
					std::cout << "ERROR Split quad all children same\n";
				}
				
				if (box.boundedBy(current->quad1->getBox())) {
					current->gameObjects.push_back(gameObject);
					current = current->quad1;
					depth += 1;
				}
				else if (box.boundedBy(current->quad2->getBox())) {
					current->gameObjects.push_back(gameObject);
					current = current->quad2;
					depth += 1;
				}
				else if (box.boundedBy(current->quad3->getBox())) {
					current->gameObjects.push_back(gameObject);
					current = current->quad3;
					depth += 1;
				}
				else if (box.boundedBy(current->quad4->getBox())) {
					current->gameObjects.push_back(gameObject);
					current = current->quad4;
					depth += 1;
				}
				else {
					current->gameObjects.push_back(gameObject);
					inserted = true;
					if (gameObject->tag == "wall") {
						std::cout << "Depth of " << gameObject->tag << ": " << depth << "\n";
					}
					
					//Don't split here because it's already been split up.
				}
			}
		}
	}
}

*/

/*
Quadrant * QuadTree::find(GameObject * gameObject) {
	std::queue<Quadrant *> quadQueue;
	BoxCollider * boxColl = gameObject->getComponent<BoxCollider>();
	Box box = boxColl->getBox();
	quadQueue.push(root);
	int index = 0;
	while (!quadQueue.empty()) {

		Quadrant * current = quadQueue.front();

		//Should find visual index not id ?

		//if object is not bound by the current object or current Object is NULL, return the parent.

		if (box.boundedBy(current->quad1->getBox())) {
			current = current->quad1;
		}
		else if (box.boundedBy(current->quad2->getBox())) {
			current = current->quad2;
		}
		else if (box.boundedBy(current->quad3->getBox())) {
			current = current->quad3;
		}
		else if (box.boundedBy(current->quad4->getBox())) {
			current = current->quad4;
		}
		else {

		/*for (int i = 0; i < currentQuad->gameObjects.size(); i++) {
			if (currentQuad->gameObjects[i]->id == gameObject->id) {
				return currentQuad;
			}
		}
		*/

/*	
		quadQueue.pop();

		if (current->quad1 != NULL) {
			quadQueue.push(current->quad1);
			quadQueue.push(current->quad2);
			quadQueue.push(current->quad3);
			quadQueue.push(current->quad4);
		}
		index++;
	}
}

*/

