#include "stdafx.h"
#include "CasinoDeck.h"


CasinoCard CasinoDeck::defineCard(Suits suitType, Cards cardType)
{
	CasinoCard card;
	card.card = cardType;
	card.suit = suitType;
	card.orientation = Orientation::Facedown;
	card.transform = deckObject->add();
	card.transform->addComponent(new SpriteRenderer(texture, deckDef["Facedown"], CASINO_CARD_WIDTH, CASINO_CARD_HEIGHT));
	card.transform->addComponent(new CasinoCardController(deckDef["Facedown"], deckDef[cardTag(suitType, cardType)], Orientation::Facedown));
	card.transform->move(origin + Vector(size(), size() * -1));
	gameScene->instantiate(card.transform);
	return card;
}

CasinoDeck::CasinoDeck(Scene *scene, Vector position) 
{
	gameScene = scene;
	deckObject = new GameObject();
	origin = position;
	scene->instantiate(deckObject);
	texture = scene->display->loadTexture(DECK_TEXTURE);
	deckDef = SpriteSheetLoader::getSpriteMap(DECK_DEFINITION);
}


CasinoDeck::~CasinoDeck()
{
}
