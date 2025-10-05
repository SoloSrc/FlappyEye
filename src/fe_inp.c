#include "stb_ds.h"

#include "fe_inp.h"

static bool a_checkInput(A_Input input)
{
	const bool* state = SDL_GetKeyboardState(NULL);
	switch (input) {
	case A_INPUT_UP_PRESSED:
		return state[SDL_SCANCODE_UP];
	case A_INPUT_DOWN_PRESSED:
		return state[SDL_SCANCODE_DOWN];
	case A_INPUT_LEFT_PRESSED:
		return state[SDL_SCANCODE_LEFT];
	case A_INPUT_RIGHT_PRESSED:
		return state[SDL_SCANCODE_RIGHT];
	case A_INPUT_SPACE_PRESSED:
		return state[SDL_SCANCODE_SPACE];
	case A_INPUT_LEFT_MOUSE_PRESSED: {
		uint32_t mouseState = SDL_GetMouseState(NULL, NULL);
		return (mouseState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;
	}
	default:
		return false;
	}
}

A_InputActionID A_CreateInputAction(A_Context* ctx)
{
	A_InputActionID newID = stbds_arrlen(ctx->inputMap);
	A_InputAction newAction = { .actionID = newID, .inputs = NULL };
	stbds_arrpush(ctx->inputMap, newAction);
	return newID;
}

bool A_AssociateInputToAction(A_Context* ctx, A_InputActionID actionID, A_Input input)
{
	if (actionID < 0 || actionID > ((uint64_t)stbds_arrlen(ctx->inputMap))) {
		return false;
	}
	A_InputAction* action = &(ctx->inputMap[actionID]);
	for (int i = 0; i < stbds_arrlen(action->inputs); i++) {
		if (action->inputs[i] == input) {
			// Input already associated with this action
			return true;
		}
	}
	stbds_arrpush(action->inputs, input);
	return true;
}

bool A_IsActionPressed(A_Context* ctx, A_InputActionID actionID)
{
	A_InputAction* action = &(ctx->inputMap[actionID]);
	for (int i = 0; i < stbds_arrlen(action->inputs); i++) {
		if (a_checkInput(action->inputs[i])) {
			return true;
		}
	}
	return false;
}
