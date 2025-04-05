#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int depth(Node* node, int current);

bool equalPaths(Node * root)
{
    // Add your code below
		if (!root) return true;
		return depth(root, 0) !=-1; //-1 will be a "false" meaning that if it doesn't equal -1 then there is 
																//the paths are equal


}

int depth(Node* node, int current){
	if(!node) return current;
	if(!node->left && !node->right){
		return current;
	}
	if(!node ->left) return depth(node->right, current +1); //if there is only a right chid
	if (!node->right) return depth(node->left, current+1); //if there is only a left child

	//two child leafs
	int leftside = depth(node->left, current +1);
	int rightside= depth(node->right, current +1);

	if(leftside != rightside) return -1; //not equal

	return rightside;
}