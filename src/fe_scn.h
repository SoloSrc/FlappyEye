// fe_scn.h
//
// Header file for anything that is Scene related. For example,
// creating a Node, setting it as a Scene root, etc.

#ifndef FE_SCN_H
#define FE_SCN_H

typedef enum D_ComponentType {
	S_COMPONENT_TYPE_UNKNOWN = 0,
	// Add more component types as needed
} D_ComponentType;

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

#endif // FE_SCN_H
