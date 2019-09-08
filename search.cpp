#include <iostream>
#include <bits/stdc++.h>


int player_id;
#include "state.cpp"

using namespace std;


int counter = 0;
typedef vector<vector<int>> matrix;

int check = 0;
vector<string> getAllNeighbour(matrix* state){
	vector<string> neighbour;
	int search_value;

	matrix allSoldier = find_soldiers(state);
	vector<matrix> allCannons= find_cannons(allSoldier);
	for(int i=0; i<allSoldier.size(); i++){
		vector<int> moves = soldier_possible_moves(state, allSoldier[i]);
		for(int j=0; j<moves.size()/2; j++){
			string s ="S " + to_string(allSoldier[i][0]) + " " + to_string(allSoldier[i][1]) + " M " + to_string(moves[2*j]) + " " + to_string(moves[2*j+1]);
			neighbour.push_back(s);
		}

	}

	for(int i=0; i<allCannons.size(); i++){
		vector<string> bCannon= bomb_cannon(state, allCannons[i]);

		vector<string> mCannon = move_cannon(state, allCannons[i]);

		neighbour.insert(neighbour.end(), bCannon.begin(), bCannon.end());
		neighbour.insert(neighbour.end(), bCannon.begin(), bCannon.end());
	}

	return neighbour;
}


int evaluation_function(matrix* state){
	return 0;
}

matrix computeState(matrix* state, string neighbour){
	matrix new_state = (*state);
	int x1 = neighbour[2] - '0';
	int y1 = neighbour[4] - '0';
	int x2 = neighbour[8] - '0';
	int y2 = neighbour[10] - '0';
	if((*state)[y2][x2] == 2 || (*state)[y2][x2] ==4) check = 1;
	else check = 0;
	if(neighbour[6] == 'B'){
		new_state[y2][x2] = 0;
	}
	else{
		new_state[y2][x2] = (*state)[y1][x1];
		new_state[y1][x1] = 0;
	}

	return new_state;
}

int minNode(matrix* state, int alpha, int beta);


int maxNode(matrix* state, int alpha, int beta){
	player_id = 0;
	if(check == 1){
		int count0 = 0;
		int count1 = 0;
		for(int i=0 ; i<(*state).size(); i++){
			for(int j=0; j<(*state)[i].size(); j++){
				if((*state)[i][j] == 2) count1++;
				if((*state)[i][j] == 4) count0++;
			}
		}
		if(count0 <= 2 || count1 <= 2) { cout<<"terminate\n"<<std::flush; return (count0-count1);}
	}
	vector<string> neighbour = getAllNeighbour(state);
	int v = INT_MIN;
	for(int i=0; i<neighbour.size(); i++){
		matrix tempState = computeState(state, neighbour[i]);
		v = max(v, minNode(&tempState, alpha, beta));
		alpha = max(alpha, v);
		if(alpha > beta){
			return alpha;
		}
	}
	return v;
}

int minNode(matrix* state, int alpha, int beta){
	player_id = 1;

	if(check == 1){
		int count0 = 0;
		int count1 = 0;
		for(int i=0 ; i<(*state).size(); i++){
			for(int j=0; j<(*state)[i].size(); j++){
				if((*state)[i][j] == 2) count1++;
				if((*state)[i][j] == 4) count0++;
			}
		}
		if(count0 <= 2 || count1 <= 2) {cout<<"terminate\n"<<std::flush; return count0-count1;}
	}


	vector<string> neighbour = getAllNeighbour(state);

	int v = INT_MAX;
	for(int i=0; i<neighbour.size(); i++){
		matrix tempState = computeState(state, neighbour[i]);
		v = min(v, maxNode(&tempState, alpha, beta));
		beta = min(beta, v);
		if(alpha > beta){
			return beta;
		}
	}
	return v;
}

int main(){
	matrix state;
	std::vector<int> r1={2,1,2,1,2,1,2,1};
	std::vector<int> r2={0,1,0,1,0,1,0,1};
	std::vector<int> r3={0,1,0,1,0,1,0,1};
	std::vector<int> r4={0,0,0,0,0,0,0,0};
	std::vector<int> r5={0,0,0,0,0,0,0,0};
	std::vector<int> r6={3,0,3,0,3,0,3,0};
	std::vector<int> r7={3,0,3,0,3,0,3,0};
	std::vector<int> r8={3,4,3,4,3,4,3,4};
	state.push_back(r1);state.push_back(r2);state.push_back(r3);state.push_back(r4);
	state.push_back(r5);state.push_back(r6);state.push_back(r7);state.push_back(r8);

	player_id = 0;
	check = 0;
	int v = maxNode(&state, INT_MIN, INT_MAX);
	cout<<v<<endl;
}