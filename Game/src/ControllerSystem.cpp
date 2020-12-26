#include <Systems/ControllerSystem.h>
#include <SDL.h>

void game::ControllerSystem::OnUpdate(utils::SparseSet<Controller>& controllers)
{
	_playerController.type = ControllerType::player;

	const Uint8* state = SDL_GetKeyboardState(nullptr);

	_playerController.xDir = state[SDL_SCANCODE_D] - state[SDL_SCANCODE_A];
	_playerController.yDir = state[SDL_SCANCODE_S] - state[SDL_SCANCODE_W];

	SDL_GetMouseState(&_playerController.xMouse, &_playerController.yMouse);
	_playerController.lMouse = state[SDL_BUTTON_LEFT];
	_playerController.rMouse = state[SDL_BUTTON_RIGHT];

	_playerController.space = state[SDL_SCANCODE_SPACE];

	GetJobModule().ToLinearJobs(controllers.GetCount(), [&controllers, this]
		(const int32_t start, const int32_t stop)
		{
			for (int32_t i = start; i < stop; ++i)
			{
				auto& controller = controllers[i];
				if (controller.type == ControllerType::player)
					controller = _playerController;
			}
		});
}
