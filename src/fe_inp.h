// fe_inp.h
//
// Header file for anything that is input related. For example,
// reading keyboard, mouse, or gamepad input.

#include <stdint.h>

#include "fe_app.h"

#ifndef FE_INP_H
#define FE_INP_H

A_InputActionID A_CreateInputAction(A_Context* ctx);
bool A_AssociateInputToAction(A_Context* ctx, A_InputActionID actionID, A_Input input);
bool A_IsActionPressed(A_Context* ctx, A_InputActionID actionID);

#endif // FE_INP_H
