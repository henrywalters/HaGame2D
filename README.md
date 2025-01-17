# HaGame2D

## Introduction

HaGame2D is an Engine written in c++ which serves two purposes: Game Engine Dev is incredibily fun, and also, a good application of the data structures and algorithms I am improving on.

## General Engine Structure

HaGame2D utilizes a component based architecture which can be summarized as:

* The Game - contains the scenes and their display ports - can communicate between other scenes using lamda expressions. 

	* The Scene - contains GameObjects in a tree structure, and orchestrates their execution

    	* The GameObject - contains general information including id, tag, position, scale & rotation. Also stores a list of components which may be accessed internally and externally.

        	* The Component - An extendable class which allows onCreate, onDestroy and update functions to be developed which can access the gameObject, the world as well as low-level functionallity.

        		* Low-level functionallity - Wrappers for SDL2 providing display & input functionallity.

HaGame2D implements a Game -> World -> Viewport -> Screen model, illustrated here:

![alt text](/Assets/git/screen_example.png?raw=true "Screen Example")

Specifically, every game object is positioned relative to the world. The viewport then handles what portion of the world should actually be displayed. The viewport is then scaled to fit the screen. This 3 tier system allows flexibillity and simplifies camera implementation. One may also simulate depth and/or zooming with the viewport. All mappings are provided as static members of the WorldMapper class.

## Scrappy Lil Demo

![alt text](/Assets/git/HaGameDemo.gif?raw=true "Demo")

## Basic Usage

Creating a scene is simple and requires little boiler plate

```cpp 
Scene game;
game.title = "New Game";
game.screenHeight = 600;
game.screenWidth = 400;

game.initialize(); //Very important to call initialize as this initializes sub systems.
```

After adding your logic, looping is as simple as

```cpp
game.initializeGameObjects(); //Also very important as this initializes game object sub systems.
game.loop();
```

Before we start the game loop, you will likely want to add some game objects. For example, to add a box which is controlled from a top down perspective and is collidable:

```cpp
const int boxSize = 50;
auto box = game.add();
box.addComponent(new BoxRenderer(boxSize, boxSize));
box.addComponent(new OrthogonalCharacterController());
box.addComponent(new BoxCollider(boxSize, boxSize));
```

All functionallity comes from components. Creating components is very simple. Here's an example of a component that will move according to input.

```cpp
class SimpleComponent : public Component {
	float speed;
	Vector velocity;

	SimpleComponent(float _speed = 3) {
		speed = _speed;
	}

	void update() {
		velocity = Vector::Zero(); //Initialzie to a zero vector on every loop

		if (input->up) {
			velocity.y = -1 * speed;
		}

		if (input->down) {
			velocity.y = speed;
		}

		if (input->left) {
			velocity.x = -1 * speed;
		}

		if (input->right) {
			velocity.x = speed;
		}

		velocity.normalize(); // Normalize velocity so you don't get speed boosts when going diagnols

		transform->move(velocity); // transform refers to the game object.
	}
}
```

Within a component, you have access to the following:

```cpp
GameObject * transform; //The game object that the component is attached to. All modifications should be made to this or component(s) of the transform.
Scene * gameScene; //A pointer to the main scene. This should only be used read-only
Input * input; //Raw input class
Display * display; //Raw display class
```

It is useful to wrap common functionallity in a base class. For example, you may extend the BoxComponent class which has the additional properties:

```cpp
float width; //World Width
float height; //World Height

float relativeWidth; //Screen Width
float relativeHeight; //Screen Height
```