#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <bits/stdc++.h>

// player_id = 0 is me.
extern std::ostream cerr;

bool player_id;
#include "bitboard1.cpp"

using namespace std;
using namespace std::chrono;

int counter = 0;
typedef vector<vector<int>> matrix;


string mv = "";
int check = 0;
int my_id = 0;

//uncomment this if you use bitboard.cpp
/*vector<board> getAllNeighbour(board &state, bool player_id){
	vector<board> neighbour = soldier_moves(state, player_id);
	vector<uint16_t> cns = cannons(state, player_id);
	vector<board> temp;	
	for(int i=0; i<cns.size(); i++){
		temp = cannon_moves(state, cns[i], player_id);
		neighbour.insert(neighbour.end(), temp.begin(), temp.end());
	}

	return neighbour;
}

vector<string> getAllNeighbour_string(board &state, bool player_id){
	vector<string> neighbour = soldier_moves_string(state, player_id);
	vector<uint16_t> cns = cannons(state, player_id);	
	vector<string> temp;	
	for(int i=0; i<cns.size(); i++){
		temp = cannon_moves_string(state, cns[i], player_id);
		neighbour.insert(neighbour.end(), temp.begin(), temp.end());
	}

	return neighbour;
}*/

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

	int wsafety = 40;
	int wattack = 40;
	int wblock = 10;
	int wfreedom =20;
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
		value += wsafety*cnt;
		cnt = countSetBits(wspe & (black_fr[x] | cap_sd[x]));
		attacking_factor[x] = cnt;
		value -= wattack*cnt;
		bs = bs & (~data[x]);
		if(adjacent[x] & wt) ba.push_back(data[x]);
	}
	while(ws){
		x = bitScanForward(ws);
		int cnt = countSetBits(wspe & (black_fr[x] | cap_sd[x]));
		safe[x] = cnt;
		value -= wsafety*cnt;
		cnt = countSetBits(bspe & (white_fr[x] | cap_sd[x]));
		attacking_factor[x] = cnt;
		value += wattack*cnt;
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
	int freedomb;
	int freedomw;
	if(myturn){
		if(my_id == false){
			freedomb = countSetBits(bfree & ((~wfree) | (~wbomb)));
			freedomw = countSetBits(wfree & ((~bfree) | (~bbomb) | (~ball)));
		}
		else{
			freedomw = countSetBits(wfree & ((~bfree) | (~bbomb)));
			freedomb = countSetBits(bfree & ((~wfree) | (~wall) | (~wbomb)));
		}
	}
	if(!myturn){
		if(my_id == false){
			freedomb = countSetBits(bfree & ((~wfree) | (~wall) | (~wbomb)));
			freedomw = countSetBits(wfree & ((~bfree) | (~bbomb)));
		}
		else{
			freedomw = countSetBits(wfree & ((~bfree) | (~bbomb) | (~ball)));
			freedomb = countSetBits(bfree & ((~wfree) | (~wbomb)));
		}
	}
	value += wblock*countbsas - wblock*countwsas;

	value += wfreedom*freedomb - wfreedom*freedomw;
	// freedom of attacking soldier and safety of attacking soldier and attack on soldier safety of defending soldier
	int battackAdv= 0;
	int bsolsafety= 0;
	int wattackAdv= 0;
	int wsolsafety= 0;
	// std::cerr<<"checking safety"<<std::flush;
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
	// std::cerr<<" : INTERIM SAFEMODE"<<std::flush;
	for(int i=0; i<wa.size(); i++){
		int attackPos = bitScanForward(wa[i]);
		if( (~myturn) ^ my_id){
			wattackAdv += countSetBits( white_fr[attackPos] & (~bfree));
		}
		else{
			wattackAdv += countSetBits( white_fr[attackPos] & (~bfree) & (~ball));			
		}
		wsolsafety += safe[attackPos] - attacking_factor[attackPos];
	}
	int w1 = 120;
	int w2 = 3000;
	int w3 = 120;
	int w4 = 800;
	int w5 = 200;
	int w6 = 200;
	// std::cerr<<" : SAFEMODE \n"<<std::flush;
	value += w3*countbcas - w3*countwcas + w4*countbta - w4*countwta;
	value += w1*countbs + w2*countbt - w1*countws - w2*countwt;
	value += (w5*battackAdv + w6*bsolsafety - w5*wattackAdv - w6*wsolsafety);

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

vector<int> setRank(board &state, vector<board> &neighbour, char order = 'd', bool myturn = true){
	int N = neighbour.size();
	vector<int> V(N);
	for(int i=0; i<N; i++){
		V[i] = evaluation_function(neighbour[i], myturn);
	}
	vector<int> ranking(N);
	int x=0;
	std::iota(ranking.begin(),ranking.end(),x++); //Initializing
	if(order == 'd' )	sort( ranking.begin(),ranking.end(), [&](int i,int j){return V[i]>V[j];} );
	else	sort( ranking.begin(),ranking.end(), [&](int i,int j){return V[i]<V[j];} );

	return ranking;
}


int minNode(board &state, int alpha, int beta, int depth, bool isFirst);


int maxNode(board &state, int alpha, int beta, int depth, bool isFirst){
	player_id = my_id;
	// std::cerr<< depth<<"\n"<<std::flush;
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
	vector<board> neighbour = getAllNeighbour(state, player_id);
	vector<string> neighbour_string;
	if (isFirst){
		neighbour_string = getAllNeighbour_string(state, player_id);
	}
	if (neighbour.size() == 0) return INT_MIN;
	vector<int> ranking = setRank(state, neighbour, 'd', true);

	int v = INT_MIN;
	int pos;
	board tempState;
	for(int i=0; i<neighbour.size(); i++){
		int t = minNode(neighbour[ranking[i]], alpha, beta, depth-1, false);
		if(t> v){
			pos = ranking[i];
		}
		v = max(v, t);
		alpha = max(alpha, v);
		if(alpha > beta){
			return alpha;
		}
	}
	if (isFirst) mv = neighbour_string[pos];
	return v;
}

int minNode(board &state, int alpha, int beta, int depth, bool isFirst){
	player_id = (my_id + 1)%2;
	// std::cerr<<depth<<"\n"<<std::flush;
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

	// std::cerr<<"getting all neighbour"<<std::flush;
	vector<board> neighbour = getAllNeighbour(state, player_id);
	// std::cerr<<"setting rank\n"<<std::flush;
	if (neighbour.size() == 0) return INT_MAX;
	vector<int> ranking = setRank(state, neighbour, 'i', false);
	// std::cerr<<"RANK SET \n"<<std::flush;

	board tempState;
	int v = INT_MAX;
	for(int i=0; i<neighbour.size(); i++){
		// std::cerr << "checking neighbour" << i <<"\n"<<std::flush;
		v = min(v, maxNode(neighbour[ranking[i]], alpha, beta, depth-1, false));
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
	// uint64_t white_soldiers = 0b0100000000000000000000000000000000000000000000000000000000000000;
	// uint64_t white_townhall = 0b0000000000000000000000000000000000000000000000000000000001010101;
	// uint64_t black_soldiers = 0b0000000000000000000000000000000000000000000000000000000000000000;
	// uint64_t black_townhall = 0b0000101000000000000000000000000000000000000000000000000000000000;
	// state.set_white_soldiers(white_soldiers);
	// state.set_white_townhall(white_townhall);
	// state.set_black_soldiers(black_soldiers);
	// state.set_black_townhall(black_townhall);
	// print_board(state);
	// vector<string> neighbour = getAllNeighbour(state, 0);
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
	
	string metaData;
	getline(cin, metaData);
	
	stringstream client(metaData);

	int opponentTurn = 0;
	client >> my_id;
	my_id = my_id - 1;

	int n,m, timeLimit;
	client >> n;
	client >> m;
	client >> timeLimit;


	int allotedTime = timeLimit / 20;
	int start = 0;
	int depth;
	if(my_id == 1){
		opponentTurn = 1;
	}
	
	player_id = 0;
	check = 0;

	std::cerr << timeLimit;
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
		// std::cerr << argv[1][0] - '0';
		auto start = high_resolution_clock::now();
		
		depth = 1;
		while(true){
			int v = maxNode(state, INT_MIN, INT_MAX, depth++, true);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<seconds>(stop - start);
			// std::cerr << duration.count()<<endl;
			if(duration.count()*4 > allotedTime) break;
		}
		std::cerr << depth-1 << endl;
			
		computeState(state, mv);
		cout<<mv<<endl;
		opponentTurn = 1;
		
	 }
}
