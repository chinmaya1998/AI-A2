#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

// player_id = 0 is me.
bool player_id;
#include "bitboard.cpp"

using namespace std;

extern std::ostream cerr;

int counter = 0;
typedef vector<vector<int>> matrix;


string mv = "";
int check = 0;
int my_id = 0;

vector<string> getAllNeighbour(board &state, bool player_id){
	vector<string> neighbour = soldier_moves(state, player_id);
	vector<uint16_t> cns = cannons(state, player_id);	
	vector<string> temp;	
	for(int i=0; i<cns.size(); i++){
		temp = cannon_moves(state, cns[i], player_id);
		neighbour.insert(neighbour.end(), temp.begin(), temp.end());
	}

	return neighbour;
}

int countSetBits(uint64_t n) 
{ 
	int count = 0; 
	while (n) 
	{ 
	    count += n & 1; 
	    n >>= 1; 
	} 
	return count; 
} 

int evaluation_function(board &state, bool myturn){
	int value = 0;
	int damage = 0;

	int wsafety = 1;
	int wcannon = 1;
	int wtombsafety = 1;
	int wOpponentPlayers = 1;

	// int townblack;
	// int townwhite;

	int safe[64];
	int attacking_factor[64];
	//number of soldiers and towns
	int countws = countSetBits(state.get_white_soldiers());
	int countbs = countSetBits(state.get_black_soldiers());
	int countwt = countSetBits(state.get_white_townhall());
	int countbt = countSetBits(state.get_black_townhall());
	value += countbs + 30*countbt - countws - 30*countwt;

	// primary concern: opponent's damage or my safety
	int x;
	uint64_t bs = state.get_black_soldiers();
	uint64_t ws = state.get_white_soldiers();
	uint64_t allSoldier = state.get_all();
	uint64_t bspe = bs;
	uint64_t wspe = ws;
	uint64_t bt = state.get_black_townhall();
	uint64_t wt = state.get_white_townhall();
	uint64_t wall = state.get_whites();
	uint64_t ball = state.get_blacks();

	vector<uint64_t> ba;
	vector<uint64_t> wa;
	while(bs){
		x = bitScanForward(bs);
		int cnt = countSetBits(bspe & (white_fr[x] | cap_sd[x]));
		safe[x] = cnt;
		cnt = countSetBits(wspe & (black_fr[x] | cap_sd[x]));
		attacking_factor[x] = cnt;
		bs = bs & (~data[x]);
		if(adjacent[x] & wt) ba.push_back(data[x]);
	}
	while(ws){
		x = bitScanForward(ws);
		int cnt = countSetBits(wspe & (black_fr[x] | cap_sd[x]));
		safe[x] = cnt;
		cnt = countSetBits(bspe & (white_fr[x] | cap_sd[x]));
		attacking_factor[x] = cnt;
		ws = ws & (~data[x]);
		if(adjacent[x] & bt) wa.push_back(data[x]);
	}
	uint64_t bfree = soldier_pos(state, 0);
	uint64_t wfree = soldier_pos(state, 1);
	uint64_t bbomb = bomb_pos(state, 0);
	uint64_t wbomb = bomb_pos(state, 1);
	
	//free moves and attacking positions
	int countbsas = countSetBits((bfree) & wspe);  // beneficial when my turn and count is odd
	int countbcas = countSetBits((bbomb) & wspe);  // more beneficial when my turn else less beneficial
	int countwsas = countSetBits((wfree) & bspe);
	int countwcas = countSetBits((wbomb) & bspe);
	int countbta = countSetBits((bfree | bbomb) & wt);
	int countwta = countSetBits((wfree | wbomb) & bt);

	//free moves
	int freedom;
	if(myturn){
		if(my_id == false){
			freedom = countSetBits(bfree & ((~wfree) | (~wbomb)));
		}
		else{
			freedom = countSetBits(wfree & ((~bfree) | (~bbomb)));
		}
	}
	if(!myturn){
		if(my_id == false){
			freedom = countSetBits(bfree & ((~wfree) | (~wall) | (~wbomb)));
		}
		else{
			freedom = countSetBits(wfree & ((~bfree) | (~bbomb) | (~ball)));
		}
	}

	// freedom of attacking soldier and safety of attacking soldier and attack on soldier safety of defending soldier
	int battackAdv= 0;
	int bsolsafety= 0;
	int wattackAdv= 0;
	int wsolsafety= 0;
	for(int i=0; i<ba.size(); i++){
		int attackPos = bitScanForward(ba[i]);
		if(myturn ^ my_id){
			battackAdv += countSetBits( black_fr[attackPos] & (~wfree));
		}
		else{
			battackAdv += countSetBits( black_fr[attackPos] & (~wfree) & (~wall));
		}
		bsolsafety += safe[attackPos] - attacking_factor[attackPos];
	}

	for(int i=0; i<ba.size(); i++){
		int attackPos = bitScanForward(wa[i]);
		if( (~myturn) ^ my_id){
			wattackAdv += countSetBits( white_fr[attackPos] & (~bfree));
		}
		else{
			wattackAdv += countSetBits( white_fr[attackPos] & (~bfree) & (~ball));			
		}
		wsolsafety += safe[attackPos] - attacking_factor[attackPos];
	}

	if(my_id) value = -1*value;

	return value;
}


uint64_t remove_st(uint64_t val, int pos){
	return (val & (~data[pos]));
}

void computeState(board &state, string neighbour){
	int x1 = neighbour[2] - '0';
	int y1 = neighbour[4] - '0';
	int x2 = neighbour[8] - '0';
	int y2 = neighbour[10] - '0';

	bool black = 0;

	int spos = 8*y1 + x1;
	int npos = 8*y2 + x2;

	if(isKthBitSet(state.get_black_soldiers(), spos)) black = 1;

	uint64_t ns;
	uint64_t nt;
	if(black){
		ns = remove_st(state.get_white_soldiers(), npos);
		nt = remove_st(state.get_white_townhall(), npos);
		state.set_white_soldiers_townhall(ns, nt);
		if(neighbour[6] != 'B'){
				ns = remove_st(state.get_black_soldiers(), spos);
				ns = ns | (data[npos]);
				state.set_black_soldiers( ns );
			}
	}

	if(!black){

		ns = remove_st(state.get_black_soldiers(), npos);
		nt = remove_st(state.get_black_townhall(), npos);
		state.set_black_soldiers_townhall(ns, nt);
		if( neighbour[6] != 'B'){
			ns = remove_st(state.get_white_soldiers(), spos);
			ns = ns | data[npos];
			state.set_white_soldiers(ns);
		}
	}

}

vector<int> setRank(board &state, vector<string> &neighbour, char order = 'd', bool myturn = true){
	int N = neighbour.size();
	vector<int> V(N);
	board tempState;
	for(int i=0; i<N; i++){
		tempState = state;
		computeState(tempState, neighbour[i]);
		V[i] = evaluation_function(tempState, myturn);
	}
	vector<int> ranking(N);
	int x=0;
	std::iota(ranking.begin(),ranking.end(),x++); //Initializing
	if(order == 'd' )	sort( ranking.begin(),ranking.end(), [&](int i,int j){return V[i]>V[j];} );
	else	sort( ranking.begin(),ranking.end(), [&](int i,int j){return V[i]<V[j];} );

	return ranking;
}


int minNode(board &state, int alpha, int beta, int depth);


int maxNode(board &state, int alpha, int beta, int depth){
	player_id = my_id;
	if(check == 1){
		int bt = countSetBits(state.get_black_townhall());
		int wt = countSetBits(state.get_white_townhall());
		if(wt <= 2 || bt <= 2) {
			return evaluation_function(state, true);
		}
	}
	if(depth == 0){
		return evaluation_function(state, true);
	}
	vector<string> neighbour = getAllNeighbour(state, player_id);
	vector<int> ranking = setRank(state, neighbour, 'd', true);

	int v = INT_MIN;
	int pos;
	board tempState;
	for(int i=0; i<neighbour.size(); i++){
		tempState = state;
		computeState(tempState, neighbour[ranking[i]]);
		int t = minNode(tempState, alpha, beta, depth-1);
		if(t> v){
			pos = ranking[i];
		}
		v = max(v, t);
		alpha = max(alpha, v);
		if(alpha > beta){
			return alpha;
		}
	}
	mv = neighbour[pos];
	return v;
}

int minNode(board &state, int alpha, int beta, int depth){
	player_id = (my_id + 1)%2;
	if(check == 1){
		int bt = countSetBits(state.get_black_townhall());
		int wt = countSetBits(state.get_white_townhall());
		if(wt <= 2 || bt <= 2) {
			return evaluation_function(state, false);
		}
	}
	if(depth == 0){
		return evaluation_function(state, false);
	}

	vector<string> neighbour = getAllNeighbour(state, player_id);
	vector<int> ranking = setRank(state, neighbour, 'i', false);
	board tempState;
	int v = INT_MAX;
	for(int i=0; i<neighbour.size(); i++){
		tempState = state;
		computeState(tempState, neighbour[ranking[i]]);
		v = min(v, maxNode(tempState, alpha, beta, depth-1));
		beta = min(beta, v);
		if(alpha > beta){
			return beta;
		}
	}
	return v;
}

int main(int argc, char const *argv[]){
	fill_database();
	board state;
	
	// print_board(state);
	// vector<string> neighbour = getAllNeighbour(state, 1);

	// for(int i=0 ;i<neighbour.size(); i++){
	// 	cout<<neighbour[i]<<"\n";
	// }
	// cout<<"\n";
	// getline(cin, mv);
	// computeState(state, mv);
	// print_board (state);
	// neighbour = getAllNeighbour(state, 1);
	// for(int i=0 ;i<neighbour.size(); i++){
	// 	cout<<neighbour[i]<<"\n";
	// }
	
	string client;
	getline(cin, client);
	int opponentTurn = 0;
	my_id = client[0] - '1';
	if(my_id == 1){
		opponentTurn = 1;
	}
	
	player_id = 0;
	check = 0;

	std::cerr << client;
	// my_id = 0;
	// player_id = 0;
	// check = 0;

	while(true){
		if(opponentTurn == 1){
			mv = "";
			while( mv == "" ){
				getline(cin,mv);
			}
			computeState(state, mv);
		}

		int v = maxNode(state, INT_MIN, INT_MAX, argv[1][0] - '0');
		computeState(state, mv);
		cout<<mv<<endl;
		opponentTurn = 1;
		
	 }
}