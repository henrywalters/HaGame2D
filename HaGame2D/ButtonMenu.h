#pragma once
#include "HaGame2D.h"
#include "ButtonComponent.h"

const RGB FG_COLOR = Color::white();
const RGB BG_COLOR = Color::black();
const RGB BORDER_COLOR = Color::black();

const int BTN_WIDTH = 350;
const int BTN_HEIGHT = 60;
const int MARGIN = 25;

template <class T>
struct RawMenuItem {
	std::string label;
	T value;
	GameObject *transform;
};

template<class T>
struct MenuItem {
	std::string label;
	T value;
};

template<class T>
class ButtonMenu : public Component
{

	std::vector<RawMenuItem<T>> items;

public:

	std::function<void(T)> selectOptionFunc;

	ButtonMenu(std::vector<MenuItem<T>> itemDefs);
	~ButtonMenu();

	void onCreate();
};

template<class T>
ButtonMenu<T>::ButtonMenu(std::vector<MenuItem<T>> itemDefs)
{
	int index = 0;
	for (MenuItem<T> itemDef : itemDefs) {
		RawMenuItem<T> item;
		
		item.label = itemDef.label;
		item.value = itemDef.value;

		items.push_back(item);
	}
}

template<class T>
ButtonMenu<T>::~ButtonMenu()
{
}

template<class T>
void ButtonMenu<T>::onCreate() {
	int index = 0;
	for (RawMenuItem<T> item : items) {

		item.transform = gameScene->instantiate(transform->add());

		auto btnContainer = item.transform->add();

		auto btn = btnContainer->addComponent(new ButtonComponent(BTN_WIDTH, BTN_HEIGHT));

		btnContainer->move(Vector(0, index * (MARGIN + BTN_HEIGHT)));

		btn->color = BG_COLOR;

		btn->onClickFunc = [this, item]() {
			this->selectOptionFunc(item.value);
		};

		auto btnLabel = btnContainer->addComponent(new TextRenderer(BTN_WIDTH, BTN_HEIGHT));
		btnLabel->setFontSize(30);
		btnLabel->setMessage(item.label);
		btnLabel->setAllignment(TextAllignments::Center);
		btnLabel->setFontColor(Color::black());
		btnLabel->setFont("../Assets/Fonts/Casino.ttf");

		gameScene->instantiate(btnContainer);

		index++;
	}
}