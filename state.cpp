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
void print_board(matrix mat){
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

void change_state(string move, matrix* state){
	int x1 = move[2] - '0';
	int y1 = move[4] - '0';

	if((*state)[y1][x1] != 1 && (*state)[y1][x1] != 3 ){
		cout << "Soldier Not here!" << endl;
		return;
	}
	char t = move[6];
	int x2 = move[8]- '0';
	int y2 = move[10]- '0';
	if(t == 'B'){
		(*state)[y2][x2] = 0;
	}
	else if(t == 'M'){
		(*state)[y2][x2] = (*state)[y1][x1];
		(*state)[y1][x1] = 0;
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

matrix find_soldiers(matrix* state){
	matrix ret;
	ret.reserve(12);
	if (player_id == 0){
		for (int i = 0; i < (*state).size(); ++i){
			for (int j = 0; j < (*state)[0].size(); ++j){
				if((*state)[i][j] == 3){
					ret.push_back({j,i});	
				}	
			}
		}
	}
	else if (player_id == 1){
		for (int i = 0; i < (*state).size(); ++i){
			for (int j = 0; j < (*state)[0].size(); ++j){
				if((*state)[i][j] == 1){
					ret.push_back({j,i});	
				}
			}
		}
	}
	return ret;
}

matrix adj_blocks1 = {{0,-1},{-1,-1},{1,-1},{1,0},{-1,0}};
matrix adj_blocks2 = {{0,1},{-1,1},{1,1},{1,0},{-1,0}};
bool is_enemy_adjacent(matrix* state, vector<int> pos){
	std::vector<int> v;
	std::vector<int> t;
	if(player_id == 0){
		for (int i = 0; i < adj_blocks1.size(); ++i){
			v = pos + adj_blocks1[i];
			if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 1)){
				return true;
			}
		}
	}
	else if (player_id == 1){
		for (int i = 0; i < adj_blocks2.size(); ++i){
			v = pos + adj_blocks2[i];
			if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 1)){
				return true;
			}
		}
	}
	return false;
}

vector<int> soldier_possible_moves(matrix* state, vector<int> pos){

	vector<int> moves;
	moves.reserve(12);
	if(player_id == 1){
		matrix forw_st = database[3][pos[0]*8 + pos[1]];
		for (int i = 0; i < forw_st.size(); ++i){
			if((*state)[forw_st[i][1]][forw_st[i][0]] == 0 || (*state)[forw_st[i][1]][forw_st[i][0]] == 3 || (*state)[forw_st[i][1]][forw_st[i][0]] == 4){
				moves.push_back(forw_st[i][0]);
				moves.push_back(forw_st[i][1]);
			}
		}
		if(is_enemy_adjacent(state, pos)){
			matrix retr_st = database[4][pos[0]*8 + pos[1]];
			for (int i = 0; i < retr_st.size(); ++i){
				if((*state)[retr_st[i][1]][retr_st[i][0]] == 0 || (*state)[retr_st[i][1]][retr_st[i][0]] == 3 || (*state)[retr_st[i][1]][retr_st[i][0]] == 4){
					moves.push_back(retr_st[i][0]);
					moves.push_back(retr_st[i][1]);
				}
			}
		}
		

		matrix capt_sd = database[5][pos[0]*8 + pos[1]];
		for (int i = 0; i < capt_sd.size(); ++i){
			if((*state)[capt_sd[i][1]][capt_sd[i][0]] == 3 || (*state)[capt_sd[i][1]][capt_sd[i][0]] == 4){
				moves.push_back(capt_sd[i][0]);
				moves.push_back(capt_sd[i][1]);
			}
		}
	}

	else{
		matrix forw_st = database[0][pos[0]*8 + pos[1]];
		for (int i = 0; i < forw_st.size(); ++i){
			if((*state)[forw_st[i][1]][forw_st[i][0]] == 0 || (*state)[forw_st[i][1]][forw_st[i][0]] == 1 || (*state)[forw_st[i][1]][forw_st[i][0]] == 2){
				moves.push_back(forw_st[i][0]);
				moves.push_back(forw_st[i][1]);
			}
		}
		if(is_enemy_adjacent(state, pos)){
			matrix retr_st = database[1][pos[0]*8 + pos[1]];
			for (int i = 0; i < retr_st.size(); ++i){
				if((*state)[retr_st[i][1]][retr_st[i][0]] == 0 || (*state)[retr_st[i][1]][retr_st[i][0]] == 1 || (*state)[retr_st[i][1]][retr_st[i][0]] == 2){
					moves.push_back(retr_st[i][0]);
					moves.push_back(retr_st[i][1]);
				}
			}
		}

		matrix capt_sd = database[2][pos[0]*8 + pos[1]];
		for (int i = 0; i < capt_sd.size(); ++i){
			if((*state)[capt_sd[i][1]][capt_sd[i][0]] == 1 || (*state)[capt_sd[i][1]][capt_sd[i][0]] == 2){
				moves.push_back(capt_sd[i][0]);
				moves.push_back(capt_sd[i][1]);
			}
		}
	}

	return moves;
}

matrix find_cont_soldiers(vector<int> pos, string dir, matrix* state){

	vector<int> v = pos;
	matrix ret;
	ret.reserve(4);
	
	if (player_id == 0){
	if(dir == "vertical"){
		while(true){
			v = v + ver_up;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 3)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + ver_down;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 3)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "horizontal"){
		while(true){
			v = v + hor_rigt;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 3)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + hor_left;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 3)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}

	else if(dir == "diagonal_origin"){
		while(true){
			v = v + top_left;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 3)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_rigt;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 3)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "diagonal_non_origin"){
		while(true){
			v = v + top_rigt;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 3)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_left;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 3)){
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
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 1)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + ver_down;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 1)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "horizontal"){
		while(true){
			v = v + hor_rigt;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 1)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + hor_left;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 1)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}

	else if(dir == "diagonal_origin"){
		while(true){
			v = v + top_left;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 1)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_rigt;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 1)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "diagonal_non_origin"){
		while(true){
			v = v + top_rigt;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 1)){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_left;
			if((valid_pos(&v)) && ((*state)[v[1]][v[0]] == 1)){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	}
	return ret;
}

vector<string> dirs = {"vertical", "horizontal", "diagonal_origin", "diagonal_non_origin"};
vector<matrix> find_cannons(matrix sold_pos, matrix* state){
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

vector<string> bomb_cannon(matrix* state, matrix cannon, bool* flag){

	vector<string> ret;
	int k = cannon.size();
	vector<int> dif1 = cannon[0]   - cannon[1];
	vector<int> dif2 = cannon[k-1] - cannon[k-2];

	if(player_id == 1){
		vector<int> t1 = cannon[0] + dif1;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			t1 = t1 + dif1;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 1) && ((*state)[t1[1]][t1[0]] !=  2)){
				if((*flag) || !((*state)[t1[1]][t1[0]] == 0)){
					ret.push_back("S " + to_string(cannon[0][0]) + " " + to_string(cannon[0][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
					*flag = false;
				}
			}
			t1 = t1 + dif1;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 1) && ((*state)[t1[1]][t1[0]] !=  2)){
				if((*flag) || !((*state)[t1[1]][t1[0]] == 0)){
					ret.push_back("S " + to_string(cannon[0][0]) + " " + to_string(cannon[0][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
					*flag = false;
				}
			}
		}

		t1 = cannon[k-1] + dif2;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			t1 = t1 + dif2;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 1) && ((*state)[t1[1]][t1[0]] !=  2)){
				if((*flag) || !((*state)[t1[1]][t1[0]] == 0)){
					ret.push_back("S " + to_string(cannon[k-1][0]) + " " + to_string(cannon[k-1][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
					*flag = false;
				}
			}
			t1 = t1 + dif2;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 1) && ((*state)[t1[1]][t1[0]] !=  2)){
				if((*flag) || !((*state)[t1[1]][t1[0]] == 0)){
					ret.push_back("S " + to_string(cannon[k-1][0]) + " " + to_string(cannon[k-1][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
					*flag = false;
				}
			}
		}
	}

	else if(player_id == 0){
		vector<int> t1   = cannon[0] + dif1;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			t1 = t1 + dif1;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 3) && ((*state)[t1[1]][t1[0]] !=  4)){
				if((*flag) || !((*state)[t1[1]][t1[0]] == 0)){
					ret.push_back("S " + to_string(cannon[0][0]) + " " + to_string(cannon[0][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
					*flag = false;
				}
			}
			t1 = t1 + dif1;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 3) && ((*state)[t1[1]][t1[0]] !=  4)){
				if((*flag) || !((*state)[t1[1]][t1[0]] == 0)){
					ret.push_back("S " + to_string(cannon[0][0]) + " " + to_string(cannon[0][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
					*flag = false;
				}
			}
		}

		t1 = cannon[k-1] + dif2;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			t1 = t1 + dif2;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 3) && ((*state)[t1[1]][t1[0]] !=  4)){
				if((*flag) || !((*state)[t1[1]][t1[0]] == 0)){
					ret.push_back("S " + to_string(cannon[k-1][0]) + " " + to_string(cannon[k-1][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
					*flag = false;
				}
			}
			t1 = t1 + dif2;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 3) && ((*state)[t1[1]][t1[0]] !=  4)){
				if((*flag) || !((*state)[t1[1]][t1[0]] == 0)){
					ret.push_back("S " + to_string(cannon[k-1][0]) + " " + to_string(cannon[k-1][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
					*flag = false;
				}
			}
		}
	}
	return ret;
} 

vector<string> move_cannon(matrix* state, matrix cannon){

	vector<string> ret;
	int k = cannon.size();
	vector<int> dif1 = cannon[0]   - cannon[1];
	vector<int> dif2 = cannon[k-1] - cannon[k-2];
	vector<int> t1 = cannon[0] + dif1;

	if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
		ret.push_back("S " + to_string(cannon[2][0]) + " " + to_string(cannon[2][1]) + " M " + to_string(t1[0]) + " " + to_string(t1[1]));
	}
	t1 = cannon[k-1] + dif2;
	if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
		ret.push_back("S " + to_string(cannon[k-3][0]) + " " + to_string(cannon[k-3][1]) + " M " + to_string(t1[0]) + " " + to_string(t1[1]));
	}

	return ret;
} 

vector<string> next_moves(matrix* state){

	// chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	vector<string> ret;
	matrix t1 = find_soldiers(state);

	vector<int> t2;
	for (int i = 0; i < t1.size(); ++i){
		t2 = soldier_possible_moves(state, t1[i]);
		for (int j = 0; j < t2.size(); j = j + 2){
			ret.push_back("S " + to_string(t1[i][0]) + " " + to_string(t1[i][1]) + " M " + to_string(t2[j]) + " " + to_string(t2[j+1]));	
		}
	}
	// chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	// cout << "Time difference = " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << "[ns]" << endl;

	// begin = std::chrono::steady_clock::now();
	bool flag = true;
	vector<matrix> v = find_cannons(t1, state);
	vector<string> x,y;
	for (int i = 0; i < v.size() ; ++i){
		x = bomb_cannon(state, v[i], &flag);
		ret.insert(ret.end(), x.begin(), x.end());
		y = move_cannon(state, v[i]);
		ret.insert(ret.end(), y.begin(), y.end());	
	}
	// end = std::chrono::steady_clock::now();
	// cout << "Time difference = " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << "[ns]" << endl;

	return ret;
}

vector<int> features(matrix* state){
	std::vector<int> ret;
	ret.resize(10);
	for (int i = 0; i < 8; ++i){
		for (int j = 0; j < 8; ++j){
			if((*state)[i][j] == 1){ // white soldier count
				ret[0] += 1;
			}
			else if((*state)[i][j] == 3){ // black soldier count
				ret[1] += 1;
			}
		}
	}
	for (int i = 0; i < 8; ++i){
		if((*state)[0][i] == 2){ // white townhall count
			ret[2] += 1;
		}
	}
	for (int i = 0; i < 8; ++i){
		if((*state)[7][i] == 4){ // black townhall count
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

matrix get_initial_board(){
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
	// for (int i = 0; i < 8; ++i){
	// 	for (int j = 0; j < 8; ++j){
	// 		database2.insert({i,j});
	// 	}
	// }



	// player_id = stoi(argv[1]);
	
	
	// matrix state;
	// std::vector<int> r1={2,0,2,0,2,0,2,0};
	// std::vector<int> r2={0,0,1,0,0,0,0,0};
	// std::vector<int> r3={0,0,0,1,0,0,0,0};
	// std::vector<int> r4={1,0,3,1,0,1,0,0};
	// std::vector<int> r5={0,0,0,0,0,3,0,0};
	// std::vector<int> r6={0,0,0,0,3,0,3,0};
	// std::vector<int> r7={0,0,0,0,0,0,0,0};
	// std::vector<int> r8={0,4,3,4,0,4,0,4};
	// state.push_back(r1);state.push_back(r2);state.push_back(r3);state.push_back(r4);
	// state.push_back(r5);state.push_back(r6);state.push_back(r7);state.push_back(r8);
	// print_board(state);

	std::vector<string> v;
	// for (int q = 0; q < 100000; ++q){
	// 	v = next_moves(&state);
	// 	// for (int e = 0; e < v.size(); ++e){
	// 	// 	cout << v[e] << endl;
	// 	// }
	// 	// cout << v.size() << endl;
	// }
matrix state;
int qw = 0;
int tot = 0;
srand(time(0));
while(true){
	std::vector<int> feat;
	state = get_initial_board();
	for (int i = 0; i < 500; ++i){
		if(i%2 == 0){player_id = 1;}
		else{player_id = 0;}
		feat = features(&state);
		if(feat[2] == 2){
			// cout << "Player 1 wins!" << endl;
			break;
		}
		if(feat[3] == 2){
			// cout << "Player 0 wins!" << endl;
			break;
		}
		// print_board(state);
		v = next_moves(&state);
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
cout << tot << endl;



	return 0;
}