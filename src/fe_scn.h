// fe_scn.h
//
// Header file for anything that is Scene related. For example,
// creating a Node, setting it as a Scene root, etc.

#include <stdbool.h>

#ifndef FE_SCN_H
#define FE_SCN_H

typedef enum D_ComponentType {
	D_COMPONENT_TYPE_UNKNOWN = 0,
	// Add more component types as needed
	D_COMPONENT_TYPE_POSITION,
	D_COMPONENT_TYPE_SPRITE,
	D_COMPONENT_TYPE_CAMERA,
} D_ComponentType;

typedef struct D_PositionComponent {
	D_ComponentType type; // Type of the component, a field shared in all components
	float x; // X-coordinate of the position
	float y; // Y-coordinate of the position
} D_PositionComponent;

typedef struct D_Sprite {
	void* texture;
	float width;
	float height;
	int frames; // Number of frames in the sprite sheet
	int cols; // Number of columns in the sprite sheet
} D_Sprite;

typedef struct D_SpriteComponent {
	D_ComponentType type; // Type of the component, a field shared in all components
	D_Sprite* sprite; // Pointer to the sprite itself with its data
} D_SpriteComponent;

typedef struct D_CameraComponent {
	D_ComponentType type; // Type of the component, a field shared in all components
} D_CameraComponent;

typedef union D_Component {
	D_ComponentType type; // Type of the component, a field shared in all components
	D_SpriteComponent sprite; // Sprite component
	D_PositionComponent position; // Position component
	D_CameraComponent camera; // Camera component
} D_Component;

typedef struct D_Node {
	char* name;
	struct D_Node* parent; // Pointer to the parent node
	struct D_Node** children; // Array of child nodes, managed by stb_ds
	D_Component* components; // Array of components, managed by stb_ds
} D_Node;

typedef struct D_Scene {
	D_Node* root; // Pointer to the root Node of this scene
	D_Node* camera; // Pointer to a special Node that represents the camera
} D_Scene;

D_Scene* D_InitScene(D_Node *root, D_Node* camear);
bool D_ValidateScene(D_Scene *scene);
void D_FreeScene(D_Scene** scene);

D_Node* D_InitNode(const char* name);
D_Node* D_InitCameraNode(const char* name);
void D_FreeNode(D_Node** node);

void D_AddPositionComponent(D_Node* node, float x, float y);
void D_AddSpriteComponent(D_Node* node, D_Sprite* sprite);

#endif // FE_SCN_H
