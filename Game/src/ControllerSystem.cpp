#include <Systems/ControllerSystem.h>
#include <SDL.h>

void game::ControllerSystem::OnUpdate(utils::SparseSet<Controller>& controllers)
{
	Controller playerController;
	playerController.type = ControllerType::player;

	const Uint8* state = SDL_GetKeyboardState(nullptr);

	playerController.xDir = state[SDL_SCANCODE_D] - state[SDL_SCANCODE_A];
	playerController.yDir = state[SDL_SCANCODE_S] - state[SDL_SCANCODE_W];

	SDL_GetMouseState(&playerController.xMouse, &playerController.yMouse);
	playerController.lMouse = state[SDL_BUTTON_LEFT];
	playerController.rMouse = state[SDL_BUTTON_RIGHT];

	playerController.space = state[SDL_SCANCODE_SPACE];

	for (auto& controller : controllers)
		if (controller.type == ControllerType::player)
			controller = playerController;
}
