// fe_scn.h
//
// Header file for anything that is Scene related. For example,
// creating a Node, setting it as a Scene root, etc.

#ifndef FE_SCN_H
#define FE_SCN_H

typedef enum D_ComponentType {
	S_COMPONENT_TYPE_UNKNOWN = 0,
	// Add more component types as needed
	S_COMPONENT_TYPE_SPRITE,
} D_ComponentType;

typedef struct D_SpriteComponent {
	D_ComponentType type; // Type of the component, a field shared in all components
	char* path; // Path to the texture file for the sprite, used as key for Sprite-lookup
} D_SpriteComponent;

typedef union D_Component {
	D_ComponentType type; // Type of the component, a field shared in all components
} D_Component;

typedef struct D_Node {
	struct D_Node* parent; // Pointer to the parent node
	struct D_Node** children; // Array of child nodes, managed by stb_ds
	D_Component* components; // Array of components, managed by stb_ds
} D_Node;

typedef struct D_Scene {
	D_Node* root; // The root node of the scene
} D_Scene;

void D_InitScene(D_Scene* scene);
void D_FreeScene(D_Scene** scene);
void D_InitNode(D_Node* node);
void D_FreeNode(D_Node** node);

#endif // FE_SCN_H
