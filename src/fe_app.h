// fe_app.h
//
// Header file for anything that is application related. For example,
// initializing SDL, setting up the Window. Running the main loop, etc.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef FE_APP_H
#define FE_APP_H

typedef struct A_Context A_Context;

bool A_Init(A_Context**);
void A_Quit(A_Context*);
void A_Run(A_Context*);

#endif // FE_APP_H
