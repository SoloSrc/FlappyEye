#include <SDL3/SDL.h>

#include "fe_app.h"
#include "fe_scn.h"

#ifndef FE_SYS_H
#define FE_SYS_H

void S_RenderScene(A_Context* ctx, D_Scene* scene, float deltaTime);
void S_ApplyVelocity(A_Context* ctx, D_Scene* scene, float deltaTime);
void S_ApplyFrameUpdates(A_Context* ctx, D_Scene* scene, float deltaTime);

#endif // !FE_SYS_H
