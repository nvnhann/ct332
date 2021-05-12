//Nguyen Van Nhan
//B809272

#include <stdio.h>
#include <stdlib.h>
#include <stack>			

using namespace std;		


#define tankempty 0
#define tankcapacity_X 9	
#define tankcapacity_Y 4	
#define goal 6



const char* action[] = {"First State", 
						"pour Walter Full X",
						"pour Walter Full Y",
						"pour Walter Empty X",
						"pour Walter Empty Y",
						"pour Walter X to Y",
						"pour Walter Y to X"};




typedef struct{
	int x;
	int y;
}State;


int min(int x, int y) {
	return x<y ? x : y;
}

int max(int x, int y) {
	return x>y ? x : y;
}

int pourWalterFullX(State cur_state, State *result) {
	if(cur_state.x < tankcapacity_X) {
		result->x = tankcapacity_X;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}


int pourWalterFullY(State cur_state, State *result) {
	if(cur_state.y < tankcapacity_Y) {
		result->x = cur_state.x;
		result->y = tankcapacity_Y;
		return 1;
	}
	return 0;
}


int pourWalterEmptyX(State cur_state, State *result) {
	if(cur_state.x > 0) {
		result->x = tankempty;
		result->y = cur_state.y;
		return 1;	
	}
	return 0;
}


int pourWalterEmptyY(State cur_state, State *result) {
	if(cur_state.y > 0) {
		result->y = tankempty;
		result->x = cur_state.x;
		return 1;	
	}
	return 0;
}


int pourWalterXY(State cur_state, State *result) {
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y) {
		result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), tankempty);
		result->y = min(cur_state.x + cur_state.y, tankcapacity_Y);
		return 1;	
	}
	return 0;
}


int pourWalterYX(State cur_state, State *result) {
	if(cur_state.y > 0 && cur_state.x < tankcapacity_X) {
		result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), tankempty);
		result->x = min(cur_state.y + cur_state.x, tankcapacity_X);
		return 1;	
	}
	return 0;
}

int callOperator(State cur_state, State *result, int opt) {
	switch(opt) {
	
		case 1: return pourWalterFullX(cur_state, result);
		case 2: return pourWalterFullY(cur_state, result);
		case 3: return pourWalterEmptyX(cur_state, result);
		case 4: return pourWalterEmptyY(cur_state, result);
		case 5: return pourWalterXY(cur_state, result);
		case 6: return pourWalterYX(cur_state, result);
		default: printf("\nError calls operators");
		return 0;
	}
	return 0;
}


void makeNullState(State *state) {
	state->x = state->y = 0;
}



void print_State(State state){
	printf("\n    X:%d --- Y:%d", state.x, state.y);
}

int checkGoal(State state){
	return (state.x == goal || state.y == goal);
}


int compareState(State state1, State state2){
	return (state1.x == state2.x && state1.y==state2.y);
}

typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
} Node;

int find_State(State state, stack<Node*> openStack) {
	while(!openStack.empty()) {
		if(compareState(openStack.top()->state, state))
			return 1;
		openStack.pop();
	}
	return 0;
}

Node* DFS_Algorithm(State state){
	stack<Node*> Open_DFS;
	stack<Node*> Close_DFS;
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	Open_DFS.push(root);
	while(!Open_DFS.empty()){
		Node* node = Open_DFS.top();
		Open_DFS.pop();
		Close_DFS.push(node);
		if(checkGoal(node->state))
			return node;
		for(int opt=1; opt<=6; opt++) {
			State newstate;
			makeNullState(&newstate);
			if(callOperator(node->state, &newstate, opt)) {
				if(find_State(newstate, Open_DFS) || find_State(newstate, Close_DFS))
					continue;
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_function = opt;
				Open_DFS.push(newNode);
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node) {
	stack<Node*> stackPrint;
		while(node->Parent != NULL) {
		stackPrint.push(node);
		node = node->Parent;
	}
	stackPrint.push(node);
	int no_action = 0;
	while(!stackPrint.empty()){
		printf("\nAction %d: %s", no_action, action[stackPrint.top()->no_function]);
		print_State(stackPrint.top()->state);
		stackPrint.pop();
		no_action++;
	}
}



int main() {
	State cur_state = {0, 0};
	
	printf("Trang thai bat dau");
	print_State(cur_state);
	
	Node* p = DFS_Algorithm(cur_state);
	print_WaysToGetGoal(p);
}
