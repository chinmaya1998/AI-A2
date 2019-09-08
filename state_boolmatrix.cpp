#include <bits/stdc++.h>
using namespace std;
#include <chrono>

struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

int player_id;
typedef vector<vector<int>> matrix;
vector<vector<matrix>> database;
std::unordered_set<vector<int>, VectorHash> database2;
typedef vector<vector<bool>> bmatrix;
typedef vector<bmatrix> board;

vector<int> ver_down = { 0, 1};
vector<int> ver_up   = { 0,-1};
vector<int> hor_rigt = { 1, 0};
vector<int> hor_left = {-1, 0};
vector<int> top_rigt = { 1,-1};
vector<int> top_left = {-1,-1};
vector<int> bot_rigt = { 1, 1};
vector<int> bot_left = {-1, 1};

template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b){
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(result), std::plus<T>());
    return result;
}
template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b){
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(result), std::minus<T>());
    return result;
}

void print_intvector(std::vector<int> v){
	for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i){
		std::cout << *i << ' ' ;
	}
	std::cout << std::endl;
}
void print_boardrow(std::vector<int> v){
	cout << '|';
	for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i){
		if(*i == 0){
			std::cout <<  " |" ;
		}
		else{
			std::cout << *i << '|' ;
		}
		
	}
	std::cout << std::endl;
}
void print_matrix(matrix mat){
	for (matrix::iterator i = mat.begin(); i != mat.end(); ++i){
		print_intvector(*i);
	}
	cout << "---------------" << endl;
}
void print_board(board state){

	vector<int> v;
	v.resize(8,0);
	matrix mat;
	mat.resize(8,v);

	for (int i = 0; i < 8; ++i){
		for (int j = 0; j < 8; ++j){
			if(state[0][i][j] == true){
				mat[i][j] = 3;
			}
			else if(state[1][i][j] == true){
				mat[i][j] = 1;
			}
			else if(state[2][i][j] == true){
				mat[i][j] = 4;
			}
			else if(state[3][i][j] == true){
				mat[i][j] = 2;
			}
			else{
				mat[i][j] = 0;
			}
		}
	}
	for (matrix::iterator i = mat.begin(); i != mat.end(); ++i){
		print_boardrow(*i);
	}
	cout << "-----------------" << endl;
}
int random(int m){
    return rand() % (m + 1);
}

bool valid_pos(vector<int> *pos, int n = 8, int m = 8){
	if((*pos)[0] >=0 and (*pos)[1] >=0 and (*pos)[0] <=(n-1) and (*pos)[1] <=(m-1)){
		return true;
	}
	return false;
}

void change_state(string move, board* state){
	int x1 = move[2] - '0';
	int y1 = move[4] - '0';
	char t = move[6]      ;
	int x2 = move[8] - '0';
	int y2 = move[10]- '0';

	if(t == 'B'){
		for (int i = 0; i < (*state).size(); ++i){
			(*state)[i][y2][x2] = false;
		}
	}
	else if(t == 'M'){
		if(player_id == 1){
			(*state)[0][y2][x2] = false;
			(*state)[1][y2][x2] = true;
			(*state)[2][y2][x2] = false;
			(*state)[4][y2][x2] = true;
			(*state)[5][y2][x2] = false;
			(*state)[6][y2][x2] = true;

			(*state)[1][y1][x1] = false;
			(*state)[4][y1][x1] = false;
			(*state)[6][y1][x1] = false;
			

		}
		else{
			(*state)[0][y2][x2] = true;
			(*state)[1][y2][x2] = false;
			(*state)[3][y2][x2] = false;
			(*state)[4][y2][x2] = false;
			(*state)[5][y2][x2] = true;
			(*state)[6][y2][x2] = true;

			(*state)[0][y1][x1] = false;
			(*state)[5][y1][x1] = false;
			(*state)[6][y1][x1] = false;
		}
	}
}


matrix forward_steps(vector<int> pos, int player){
	matrix ret;
	ret.reserve(3);
	std::vector<int> v;
	std::vector<int> t;
	if (player == 1){
		t = {0,1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {-1,1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {1,1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
	}
	else{
		t = {0,-1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {-1,-1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {1,-1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
	}
	return ret;
}

matrix retreat_steps(vector<int> pos, int player){

	matrix ret;
	ret.reserve(3);
	std::vector<int> v;
	std::vector<int> t;
	if (player == 1){
		t = {0,-2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {-2,-2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {2,-2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
	}
	else{
		t = {0,2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {-2,2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {2,2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
	}
	return ret;
}

matrix capture_side(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {1,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {-1,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}

matrix find_soldiers(board* state){
	matrix ret;
	ret.reserve(12);
	if (player_id == 0){
		for (int i = 0; i < (*state)[0].size(); ++i){
			for (int j = 0; j < (*state)[0][0].size(); ++j){
				if((*state)[0][i][j] == true){
					ret.push_back({j,i});	
				}	
			}
		}
	}
	else{
		for (int i = 0; i < (*state)[1].size(); ++i){
			for (int j = 0; j < (*state)[1][0].size(); ++j){
				if((*state)[1][i][j] == true){
					ret.push_back({j,i});	
				}
			}
		}
	}
	return ret;
}

matrix adj_blocks1 = {{0,-1},{-1,-1},{1,-1},{1,0},{-1,0}};
matrix adj_blocks2 = {{0,1},{-1,1},{1,1},{1,0},{-1,0}};
bool is_enemy_adjacent(board* state, vector<int> pos){
	std::vector<int> v;
	if(player_id == 0){
		for (int i = 0; i < 5; ++i){
			v = pos + adj_blocks1[i];
			if(valid_pos(&v) && ((*state)[1][v[1]][v[0]] == true)){
				return true;
			}
		}
	}
	else{
		for (int i = 0; i < 5; ++i){
			v = pos + adj_blocks2[i];
			if(valid_pos(&v) && ((*state)[0][v[1]][v[0]] == true)){
				return true;
			}
		}
	}
	return false;
}

vector<int> soldier_possible_moves(board* state, vector<int> pos){
	vector<int> moves;
	moves.reserve(12);
	if(player_id == 1){
		
		matrix forw_st = database[3][pos[0]*8 + pos[1]];
		for (int i = 0; i < forw_st.size(); ++i){
			// if((*state)[6][forw_st[i][1]][forw_st[i][0]] == false || (*state)[5][forw_st[i][1]][forw_st[i][0]] == true){
			if((*state)[4][forw_st[i][1]][forw_st[i][0]] == false){
				moves.push_back(forw_st[i][0]);
				moves.push_back(forw_st[i][1]);
			}
		}
		if(is_enemy_adjacent(state, pos)){
			matrix retr_st = database[4][pos[0]*8 + pos[1]];
			for (int i = 0; i < retr_st.size(); ++i){
				// if((*state)[6][retr_st[i][1]][retr_st[i][0]] == false || (*state)[5][retr_st[i][1]][retr_st[i][0]] == true){
				if((*state)[4][retr_st[i][1]][retr_st[i][0]] == false){
					moves.push_back(retr_st[i][0]);
					moves.push_back(retr_st[i][1]);
				}
			}
		}
		

		matrix capt_sd = database[5][pos[0]*8 + pos[1]];
		for (int i = 0; i < capt_sd.size(); ++i){
			if((*state)[5][capt_sd[i][1]][capt_sd[i][0]] == true){
				moves.push_back(capt_sd[i][0]);
				moves.push_back(capt_sd[i][1]);
			}
		}
	}

	else{
		matrix forw_st = database[0][pos[0]*8 + pos[1]];
		for (int i = 0; i < forw_st.size(); ++i){
			// if((*state)[6][forw_st[i][1]][forw_st[i][0]] == false  || (*state)[4][forw_st[i][1]][forw_st[i][0]] == true){
			if((*state)[5][forw_st[i][1]][forw_st[i][0]] == false){
				moves.push_back(forw_st[i][0]);
				moves.push_back(forw_st[i][1]);
			}
		}
		if(is_enemy_adjacent(state, pos)){
			matrix retr_st = database[1][pos[0]*8 + pos[1]];
			for (int i = 0; i < retr_st.size(); ++i){
				// if((*state)[6][retr_st[i][1]][retr_st[i][0]] == false  || (*state)[4][retr_st[i][1]][retr_st[i][0]] == true){
				if((*state)[5][retr_st[i][1]][retr_st[i][0]] == false){
					moves.push_back(retr_st[i][0]);
					moves.push_back(retr_st[i][1]);
				}
			}
		}

		matrix capt_sd = database[2][pos[0]*8 + pos[1]];
		for (int i = 0; i < capt_sd.size(); ++i){
			if((*state)[4][capt_sd[i][1]][capt_sd[i][0]] == true){
				moves.push_back(capt_sd[i][0]);
				moves.push_back(capt_sd[i][1]);
			}
		}
	}

	return moves;
}

matrix find_cont_soldiers(vector<int> pos, string dir, board* state){

	vector<int> v = pos;
	matrix ret;
	ret.reserve(4);
	
	if (player_id == 0){
	if(dir == "vertical"){
		while(true){
			v = v + ver_up;
			if((valid_pos(&v)) && ((*state)[0][v[1]][v[0]] == true)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + ver_down;
			if((valid_pos(&v)) && ((*state)[0][v[1]][v[0]] == true)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "horizontal"){
		while(true){
			v = v + hor_rigt;
			if((valid_pos(&v)) && ((*state)[0][v[1]][v[0]] == true)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + hor_left;
			if((valid_pos(&v)) && ((*state)[0][v[1]][v[0]] == true)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}

	else if(dir == "diagonal_origin"){
		while(true){
			v = v + top_left;
			if((valid_pos(&v)) && ((*state)[0][v[1]][v[0]] == true)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_rigt;
			if((valid_pos(&v)) && ((*state)[0][v[1]][v[0]] == true)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "diagonal_non_origin"){
		while(true){
			v = v + top_rigt;
			if((valid_pos(&v)) && ((*state)[0][v[1]][v[0]] == true)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_left;
			if((valid_pos(&v)) && ((*state)[0][v[1]][v[0]] == true)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	}

	else{
	
	if(dir == "vertical"){
		while(true){
			v = v + ver_up;
			if((valid_pos(&v)) && ((*state)[1][v[1]][v[0]] == true)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + ver_down;
			if((valid_pos(&v)) && ((*state)[1][v[1]][v[0]] == true)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "horizontal"){
		while(true){
			v = v + hor_rigt;
			if((valid_pos(&v)) && ((*state)[1][v[1]][v[0]] == true)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + hor_left;
			if((valid_pos(&v)) && ((*state)[1][v[1]][v[0]] == true)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}

	else if(dir == "diagonal_origin"){
		while(true){
			v = v + top_left;
			if((valid_pos(&v)) && ((*state)[1][v[1]][v[0]] == true)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_rigt;
			if((valid_pos(&v)) && ((*state)[1][v[1]][v[0]] == true)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "diagonal_non_origin"){
		while(true){
			v = v + top_rigt;
			if((valid_pos(&v)) && ((*state)[1][v[1]][v[0]] == true)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_left;
			if((valid_pos(&v)) && ((*state)[1][v[1]][v[0]] == true)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	}
	return ret;
}

vector<string> dirs = {"vertical", "horizontal", "diagonal_origin", "diagonal_non_origin"};
vector<matrix> find_cannons(matrix sold_pos, board* state){
	vector<matrix> ret;
	matrix temp;

	for (int i = 0; i < sold_pos.size(); ++i)	{
		for (int j = 0; j < dirs.size(); ++j){
			temp = find_cont_soldiers(sold_pos[i], dirs[j], state);
			if(temp.size() >= 3){
				if(find(ret.begin(), ret.end(), temp) != ret.end()) {
    				continue;
				} 
				else {
    				ret.push_back(temp);
				}
			}
		}
	}
	return ret;
}

vector<string> bomb_cannon(board* state, matrix cannon, bool* flag){

	vector<string> ret;
	int k = cannon.size();
	vector<int> dif1 = cannon[0]   - cannon[1];
	vector<int> dif2 = cannon[k-1] - cannon[k-2];
	vector<int> dif;
	vector<int> t1;
	string s;
	if(player_id == 1){
		for (int i = 0; i < 2; ++i){
			if(i==0){dif=dif1;t1 = cannon[0]   + dif;}
			else	{dif=dif2;t1 = cannon[k-1] + dif;}
			if(valid_pos(&t1) && ((*state)[6][t1[1]][t1[0]] == false)){
				for (int j = 0; j < 2; ++j){
					t1 = t1 + dif;
					if(valid_pos(&t1) && ((*state)[4][t1[1]][t1[0]] == false)){
						if((*flag) || ((*state)[6][t1[1]][t1[0]] == true)){
							s = "S ";
							s += to_string(cannon[0][0]);
							s += " ";
							s += to_string(cannon[0][1]);
							s += " B ";
							s += to_string(t1[0]);
							s += " ";
							s += to_string(t1[1]);
							ret.push_back(s);
							*flag = false;
						}
					}
				}
			}
		}
	}

	else{
		for (int i = 0; i < 2; ++i){
			if(i==0){dif = dif1;t1 = cannon[0]   + dif;}
			else	{dif = dif2;t1 = cannon[k-1] + dif;}
			if(valid_pos(&t1) && ((*state)[6][t1[1]][t1[0]] ==  false)){
				for (int j = 0; j < 2; ++j){
					t1 = t1 + dif;
					if(valid_pos(&t1) && ((*state)[5][t1[1]][t1[0]] == false)){
						if((*flag) || ((*state)[6][t1[1]][t1[0]] == true)){
							s = "S ";
							s += to_string(cannon[0][0]);
							s += " ";
							s += to_string(cannon[0][1]);
							s += " B ";
							s += to_string(t1[0]);
							s += " ";
							s += to_string(t1[1]);
							ret.push_back(s);
							*flag = false;
						}
					}
				}
			}
		}
	}
	return ret;
} 

vector<string> move_cannon(board* state, matrix cannon){

	vector<string> ret;
	int k = cannon.size();
	vector<int> dif1 = cannon[0]   - cannon[1];
	vector<int> dif2 = cannon[k-1] - cannon[k-2];
	vector<int> t1 = cannon[0] + dif1;
	string s;
	if(valid_pos(&t1) && ((*state)[6][t1[1]][t1[0]] ==  false)){
		s = "S ";
		s += to_string(cannon[2][0]);
		s += " ";
		s += to_string(cannon[2][1]);
		s += " M ";
		s += to_string(t1[0]);
		s += " ";
		s += to_string(t1[1]);
		ret.push_back(s);
	}
	t1 = cannon[k-1] + dif2;
	if(valid_pos(&t1) && ((*state)[6][t1[1]][t1[0]] ==  false)){
		s = "S ";
		s += to_string(cannon[k-3][0]);
		s += " ";
		s += to_string(cannon[k-3][1]);
		s += " M ";
		s += to_string(t1[0]);
		s += " ";
		s += to_string(t1[1]);
		ret.push_back(s);
	}

	return ret;
} 

vector<string> next_moves(board* state){

	vector<string> ret;
	ret.reserve(30);
	matrix t1 = find_soldiers(state);
	vector<int> t2;
	string s;
	for (int i = 0; i < t1.size(); ++i){
		t2 = soldier_possible_moves(state, t1[i]);
		for (int j = 0; j < t2.size(); j = j + 2){
			s = "S ";
			s += to_string(t1[i][0]);
			s += " ";
			s += to_string(t1[i][1]);
			s += " M ";
			s += to_string(t2[j]);
			s += " ";
			s += to_string(t2[j+1]);
			ret.push_back(s);	
		}
	}


	// cout << ret.size() <<endl;
	bool flag = true;
	vector<matrix> v = find_cannons(t1,state);
	vector<string> x,y;
	for (int i = 0; i < v.size() ; ++i){
		x = bomb_cannon(state, v[i], &flag);
		ret.insert(ret.end(), x.begin(), x.end());
		y = move_cannon(state, v[i]);
		ret.insert(ret.end(), y.begin(), y.end());	
	}
	// cout << ret.size() <<endl;
	return ret;
}

vector<int> features(board* state){
	std::vector<int> ret;
	ret.resize(10);
	for (int i = 0; i < 8; ++i){
		for (int j = 0; j < 8; ++j){
			if((*state)[1][i][j] == true){ // white soldier count
				ret[0] += 1;
			}
			else if((*state)[0][i][j] == true){ // black soldier count
				ret[1] += 1;
			}
		}
	}
	for (int i = 0; i < 8; ++i){
		if((*state)[3][0][i] == true){ // white townhall count
			ret[2] += 1;
		}
	}
	for (int i = 0; i < 8; ++i){
		if((*state)[2][7][i] == true){ // black townhall count
			ret[3] += 1;
		}
	}
	// player_id = 1;
	// matrix t1 = find_soldiers(state);
	// ret[4] = find_cannons(t1,state).size(); // white cannon count
	// player_id = 0;
	// t1 = find_soldiers(state);
	// ret[5] = find_cannons(t1,state).size(); // black cannon count
	return ret;
}

board get_initial_board(){

	bmatrix blacks;
	vector<bool> r1={0,0,0,0,0,0,0,0};
	vector<bool> r2={0,0,0,0,0,0,0,0};
	vector<bool> r3={0,0,0,0,0,0,0,0};
	vector<bool> r4={0,0,0,0,0,0,0,0};
	vector<bool> r5={0,0,0,0,0,0,0,0};
	vector<bool> r6={1,0,1,0,1,0,1,0};
	vector<bool> r7={1,0,1,0,1,0,1,0};
	vector<bool> r8={1,0,1,0,1,0,1,0};
	blacks.push_back(r1);blacks.push_back(r2);blacks.push_back(r3);blacks.push_back(r4);
	blacks.push_back(r5);blacks.push_back(r6);blacks.push_back(r7);blacks.push_back(r8);
	bmatrix whites;
	r1={0,1,0,1,0,1,0,1};
	r2={0,1,0,1,0,1,0,1};
	r3={0,1,0,1,0,1,0,1};
	r4={0,0,0,0,0,0,0,0};
	r5={0,0,0,0,0,0,0,0};
	r6={0,0,0,0,0,0,0,0};
	r7={0,0,0,0,0,0,0,0};
	r8={0,0,0,0,0,0,0,0};
	whites.push_back(r1);whites.push_back(r2);whites.push_back(r3);whites.push_back(r4);
	whites.push_back(r5);whites.push_back(r6);whites.push_back(r7);whites.push_back(r8);
	bmatrix blackt;
	r1={0,0,0,0,0,0,0,0};
	r2={0,0,0,0,0,0,0,0};
	r3={0,0,0,0,0,0,0,0};
	r4={0,0,0,0,0,0,0,0};
	r5={0,0,0,0,0,0,0,0};
	r6={0,0,0,0,0,0,0,0};
	r7={0,0,0,0,0,0,0,0};
	r8={0,1,0,1,0,1,0,1};
	blackt.push_back(r1);blackt.push_back(r2);blackt.push_back(r3);blackt.push_back(r4);
	blackt.push_back(r5);blackt.push_back(r6);blackt.push_back(r7);blackt.push_back(r8);
	bmatrix whitet;
	r1={1,0,1,0,1,0,1,0};
	r2={0,0,0,0,0,0,0,0};
	r3={0,0,0,0,0,0,0,0};
	r4={0,0,0,0,0,0,0,0};
	r5={0,0,0,0,0,0,0,0};
	r6={0,0,0,0,0,0,0,0};
	r7={0,0,0,0,0,0,0,0};
	r8={0,0,0,0,0,0,0,0};
	whitet.push_back(r1);whitet.push_back(r2);whitet.push_back(r3);whitet.push_back(r4);
	whitet.push_back(r5);whitet.push_back(r6);whitet.push_back(r7);whitet.push_back(r8);
	bmatrix white;
	bmatrix black;
	bmatrix all;
	
	for (int i = 0; i < 8; ++i){
		vector<bool> v1;
		vector<bool> v2;
		vector<bool> v3;
		for (int j = 0; j < 8; ++j){
			if(whitet[i][j] == true || whites[i][j] == true){
				v1.push_back(true);
				v2.push_back(false);
				v3.push_back(true);
			}
			else if(blackt[i][j] == true || blacks[i][j] == true){
				v1.push_back(false);
				v2.push_back(true);
				v3.push_back(true);
			}
			else{
				v1.push_back(false);
				v2.push_back(false);
				v3.push_back(false);
			}
		}
		white.push_back(v1);
		black.push_back(v2);
		all.push_back(v3);
	}
	
	board state;
	state.push_back(blacks);
	state.push_back(whites);
	state.push_back(blackt);
	state.push_back(whitet);
	state.push_back(white);
	state.push_back(black);
	state.push_back(all);

	return state;
}

board get_fixed_board(){

	bmatrix blacks;
	vector<bool> r1={0,0,0,0,0,0,0,0};
	vector<bool> r2={0,0,0,0,0,0,0,0};
	vector<bool> r3={0,0,0,0,0,0,0,0};
	vector<bool> r4={0,0,0,0,0,0,1,0};
	vector<bool> r5={1,0,1,0,1,0,0,0};
	vector<bool> r6={1,0,0,1,0,0,1,0};
	vector<bool> r7={1,0,0,0,1,0,0,0};
	vector<bool> r8={0,0,1,0,0,0,1,0};
	blacks.push_back(r1);blacks.push_back(r2);blacks.push_back(r3);blacks.push_back(r4);
	blacks.push_back(r5);blacks.push_back(r6);blacks.push_back(r7);blacks.push_back(r8);
	bmatrix whites;
	r1={0,0,0,1,0,0,0,1};
	r2={0,0,1,0,0,0,1,1};
	r3={1,0,1,1,1,0,0,1};
	r4={0,0,0,0,1,0,0,0};
	r5={0,0,0,0,0,0,0,0};
	r6={0,0,0,0,0,0,0,0};
	r7={0,0,0,0,0,0,0,0};
	r8={0,0,0,0,0,0,0,0};
	whites.push_back(r1);whites.push_back(r2);whites.push_back(r3);whites.push_back(r4);
	whites.push_back(r5);whites.push_back(r6);whites.push_back(r7);whites.push_back(r8);
	bmatrix blackt;
	r1={0,0,0,0,0,0,0,0};
	r2={0,0,0,0,0,0,0,0};
	r3={0,0,0,0,0,0,0,0};
	r4={0,0,0,0,0,0,0,0};
	r5={0,0,0,0,0,0,0,0};
	r6={0,0,0,0,0,0,0,0};
	r7={0,0,0,0,0,0,0,0};
	r8={0,1,0,1,0,1,0,1};
	blackt.push_back(r1);blackt.push_back(r2);blackt.push_back(r3);blackt.push_back(r4);
	blackt.push_back(r5);blackt.push_back(r6);blackt.push_back(r7);blackt.push_back(r8);
	bmatrix whitet;
	r1={1,0,1,0,1,0,1,0};
	r2={0,0,0,0,0,0,0,0};
	r3={0,0,0,0,0,0,0,0};
	r4={0,0,0,0,0,0,0,0};
	r5={0,0,0,0,0,0,0,0};
	r6={0,0,0,0,0,0,0,0};
	r7={0,0,0,0,0,0,0,0};
	r8={0,0,0,0,0,0,0,0};
	whitet.push_back(r1);whitet.push_back(r2);whitet.push_back(r3);whitet.push_back(r4);
	whitet.push_back(r5);whitet.push_back(r6);whitet.push_back(r7);whitet.push_back(r8);
	bmatrix white;
	bmatrix black;
	bmatrix all;
	
	for (int i = 0; i < 8; ++i){
		vector<bool> v1;
		vector<bool> v2;
		vector<bool> v3;
		for (int j = 0; j < 8; ++j){
			if(whitet[i][j] == true || whites[i][j] == true){
				v1.push_back(true);
				v2.push_back(false);
				v3.push_back(true);
			}
			else if(blackt[i][j] == true || blacks[i][j] == true){
				v1.push_back(false);
				v2.push_back(true);
				v3.push_back(true);
			}
			else{
				v1.push_back(false);
				v2.push_back(false);
				v3.push_back(false);
			}
		}
		white.push_back(v1);
		black.push_back(v2);
		all.push_back(v3);
	}
	
	board state;
	state.push_back(blacks);
	state.push_back(whites);
	state.push_back(blackt);
	state.push_back(whitet);
	state.push_back(white);
	state.push_back(black);
	state.push_back(all);

	return state;
}


int main(int argc, char const *argv[]){

	for (int d = 0; d < 2; ++d){
		vector<matrix> temp2;
		vector<matrix> temp3;
		vector<matrix> temp4;
		for (int e = 0; e < 8; ++e){
			for (int f = 0; f < 8; ++f){
				std::vector<int> pos = {e,f};
				matrix tempa = forward_steps(pos,d);
				matrix tempb = retreat_steps(pos,d);
				matrix tempc = capture_side(pos);
				temp2.push_back(tempa);
				temp3.push_back(tempb);
				temp4.push_back(tempc);
			}
		}
		database.push_back(temp2);
		database.push_back(temp3);
		database.push_back(temp4);
	}

	// player_id = stoi(argv[1]);
	// cout << player_id << endl;


	// board state = get_fixed_board();
	// print_board(state);
	// std::vector<string> v = next_moves(&state);
	// for (int i = 0; i < v.size(); ++i){
	// 	cout << v[i] << endl;
	// }
	// cout << v.size() << endl;

std::vector<string> v;
board state;
int qw = 0;
int tot = 0;
int tot1 = 0;
srand(time(0));
while(true){
	std::vector<int> feat;
	state = get_initial_board();
	for (int i = 0; i < 500; ++i){
		if(i%2 == 0){player_id = 1;}
		else{player_id = 0;}
		feat = features(&state);
		if(feat[2] == 2){
			// print_board(state);
			// cout << "Player 1 wins!" << endl;
			break;
		}
		if(feat[3] == 2){
			// print_board(state);
			// cout << "Player 0 wins!" << endl;
			break;
		}
		// print_board(state);
		v = next_moves(&state);
		tot1 = tot1 + v.size();
		// cout << v.size() <<endl;
		tot = tot + 1;
		if(v.size() == 0){
			// cout << "Stalemate" << endl;
			break;
		}
		int r = random(v.size() - 1 );
		// cout << "Player "<< player_id << " moved: " << v[r] << endl;
		// cout << i << endl;
		change_state(v[r],&state);
	}
	qw++;
	if(qw >10000){break;}
}
cout << (float)tot1/tot <<endl;
cout << tot << endl;



	return 0;
}