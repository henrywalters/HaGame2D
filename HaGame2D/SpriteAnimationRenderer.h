#pragma once
#include "GameObject.h"
#include "Component.h"
#include "BoxComponent.h"
#include "BoxCollider.h"
#include "CyclicList.h"
#include "SpriteSheetLoader.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "SDL.h"

const std::string DEFAULT_STATE = "hide";
const std::string IDLE_STATE = "idle";
const float DEFAULT_ANIMATION_LENGTH = .2;

class SpriteAnimationRenderer :
	public BoxComponent
{

	std::unordered_map<std::string, CyclicList<TextureRect>> animationMap;
	std::unordered_map<std::string, float> animationLength;
	std::vector<std::string> states;
	std::string currentState;
	CyclicNode<TextureRect> *currentAnimationFrame;

	Texture spriteSheet;

	int lastTick;
	int currentTicks = 0;
	float defaultAnimationLength;

	char * spriteSheetPath;
	char * spriteSheetConfigPath;
public:
	SpriteAnimationRenderer(char * _spriteSheetPath, char * _spriteSheetConfigPath, float _animationLengthSeconds = DEFAULT_ANIMATION_LENGTH) {
		spriteSheetPath = _spriteSheetPath;
		spriteSheetConfigPath = _spriteSheetConfigPath;
		defaultAnimationLength = _animationLengthSeconds;
	};

	~SpriteAnimationRenderer();

	void onCreate() {
		spriteSheet = display->loadTexture(spriteSheetPath);

		std::vector<SpriteSheetCell> spriteCells = SpriteSheetLoader::load(spriteSheetConfigPath);
		for (SpriteSheetCell spriteCell : spriteCells) {
			if (animationMap.find(spriteCell.cellType) == animationMap.end()) {
				animationMap[spriteCell.cellType] = CyclicList<TextureRect>();
				states.push_back(spriteCell.cellType);
			}

			animationMap[spriteCell.cellType].insert(TextureRect{ spriteCell.x, spriteCell.y, spriteCell.width, spriteCell.height });
		}

		for (std::string state : states) {
			std::cout << "Def: " << defaultAnimationLength << " / " << animationMap[state].size() << "\n";
			animationLength[state] = defaultAnimationLength / animationMap[state].size();
		}

		states.push_back(DEFAULT_STATE);
		states.push_back(IDLE_STATE);
		currentState = DEFAULT_STATE;

		lastTick = SDL_GetTicks();
	}

	bool stateExists(std::string state) {
		return animationMap.find(state) != animationMap.end() || state == DEFAULT_STATE || state == IDLE_STATE;
	}

	void setState(std::string state) {
		if (stateExists(state)) {
			if (state != currentState) {
				currentState = state;

				if (currentState != DEFAULT_STATE && currentState != IDLE_STATE) {
					currentAnimationFrame = animationMap[currentState].begin();
				}
			}
		}
		else {
			std::cout << "WARNING - State: " << state << " does not exist.\n";
		}
	}

	void setStateLength(std::string state, float length) {
		if (stateExists(state)) {
			animationLength[state] = length / animationMap[state].size();
		}
		else {
			std::cout << "WARNING - State: " << state << " does not exist.\n";
		}
	}

	void update() {
		log(currentState);
		if (currentState != DEFAULT_STATE) {
			if (currentState != IDLE_STATE) {
				currentTicks += SDL_GetTicks() - lastTick;
				if (currentTicks >= animationLength[currentState] * 1000) {
					currentAnimationFrame = currentAnimationFrame->next;
					currentTicks = 0;
				}
				lastTick = SDL_GetTicks();
			}
			else if (currentAnimationFrame == NULL) {
				currentAnimationFrame = animationMap[IDLE_STATE].begin();
			}

			if (this->currentAnimationFrame != NULL) {
				auto boxColl = transform->getComponent<BoxComponent>();
				TextureRect rect = TextureRect{ transform->relativePosition.x, transform->relativePosition.y, boxColl->width, boxColl->height };
				TextureRect clip = TextureRect{ currentAnimationFrame->value.x, currentAnimationFrame->value.y, currentAnimationFrame->value.width, currentAnimationFrame->value.height };

				display->drawTexture(rect, spriteSheet, clip, transform->z_index);
			}
			else {
				std::cout << "ERROR - Current Animation Frame is null, state is: " << currentState << "\n";
			}
			
		}
	}
};

