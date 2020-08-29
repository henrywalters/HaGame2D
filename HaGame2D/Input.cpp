#include "stdafx.h"
#include "Input.h"
#include <iostream>

Input::Input()
{
	up = false; down = false; left = false; right = false;
	origin = Vector::Zero();
	SDL_StartTextInput();
}


Input::~Input()
{
}

void Input::pollEvents() {
	SDL_Event e;

	SDL_GetMouseState(&mouseX, &mouseY);

	//printf("Mouse: %f, %f\n", mouseX, mouseY);


	fire2Up = false;
	fire1Up = false;
	fire1Down = false;
	fire2Down = false;

	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			quit = true;
		}
		else {
			quit = false;
		}

		//Special key input
		if (e.type == SDL_KEYDOWN)
		{
		//Handle backspace
			if (e.key.keysym.sym == SDLK_BACKSPACE && textStream.length() > 0)
			{
				//lop off character
				textStream.pop_back();
			}
			//Handle copy
			else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
			{
				SDL_SetClipboardText(textStream.c_str());
			}
			//Handle paste
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			{
				textStream = SDL_GetClipboardText();
			}
		}

		streamingText = false;

		if (e.type == SDL_TEXTINPUT)
		{
			//Not copy or pasting
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
			{
				streamingText = true;
				textStream += e.text.text;
				currentTextStream = e.text.text;
			}
		}

		if (e.type == SDL_MOUSEWHEEL) {
			std::cout << e.wheel.direction << std::endl;
			if (e.wheel.y > 0) {
				mouseScroll = 1;
			}
			else if (e.wheel.y < 0) {
				mouseScroll = -1;
			}
			else {
				mouseScroll = 0;
			}
		}
		else {
			mouseScroll = 0;
		}

		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				if (!fire1) {
					fire1Down = true;
				}

				fire1 = true;
			}

			if (e.button.button == SDL_BUTTON_RIGHT) {
				if (!fire2) {
					fire2Down = true;
				}

				fire2 = true;
			}
		}

		if (e.type == SDL_MOUSEBUTTONUP) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				if (fire1) {
					fire1Up = true;
				}
				fire1 = false;
			}

			if (e.button.button == SDL_BUTTON_RIGHT) {
				if (fire2) {
					fire2Up = true;
				}
				fire2 = false;
			}
		}

		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_w:
				up = true;
				break;
			case SDLK_UP:
				up = true;
				break;
			case SDLK_a:
				left = true;
				break;
			case SDLK_LEFT:
				left = true;
				break;
			case SDLK_s:
				down = true;
				break;
			case SDLK_DOWN:
				down = true;
				break;
			case SDLK_d:
				right = true;
				break;
			case SDLK_RIGHT:
				right = true;
				break;
			case SDLK_SPACE:
				space = true;
				break;
			case SDLK_q:
				q = true;
				break;
			case SDLK_1:
				one = true;
				break;
			case SDLK_2:
				two = true;
				break;
			case SDLK_3:
				three = true;
				break;
			case SDLK_LSHIFT:
				shift = true;
				break;
			}
		}

		if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym) {
			case SDLK_w:
				up = false;
				break;
			case SDLK_UP:
				up = false;
				break;
			case SDLK_a:
				left = false;
				break;
			case SDLK_LEFT:
				left = false;
				break;
			case SDLK_s:
				down = false;
				break;
			case SDLK_DOWN:
				down = false;
				break;
			case SDLK_d:
				right = false;
				break;
			case SDLK_RIGHT:
				right = false;
				break;
			case SDLK_SPACE:
				space = false;
				break;
			case SDLK_q:
				q = false;
				break;
			case SDLK_1:
				one = false;
				break;
			case SDLK_2:
				two = false;
				break;
			case SDLK_3:
				three = false;
				break;
			case SDLK_LSHIFT:
				shift = false;
				break;
			}
		}
	}
}

