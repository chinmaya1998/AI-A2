#include <bits/stdc++.h>
#include <chrono>
using namespace std;

//bool player_id;

// check my player do not go over townhall while moving cannon
// cannon moves for cannon length > 3
// cannon cannot shot at my player or townhall
// cannon cannot move with capturing other player or townhall
// cannon cannot bomb if direction is blocked.

typedef vector<vector<int>> matrix;

typedef uint64_t Bitboard; 

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
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

class board{
	Bitboard black_soldiers;
	Bitboard white_soldiers;
	Bitboard black_townhall;
	Bitboard white_townhall;
	Bitboard white;
	Bitboard black;
	Bitboard all;
public:
	board(){
		white_soldiers = 0b0000000000000000000000000000000000000000101010101010101010101010;
		white_townhall = 0b0000000000000000000000000000000000000000000000000000000001010101;
		black_soldiers = 0b0101010101010101010101010000000000000000000000000000000000000000;
		black_townhall = 0b1010101000000000000000000000000000000000000000000000000000000000;
		white = white_soldiers | white_townhall;
		black = black_soldiers | black_townhall;
		all   = white | black;
	}
	board(Bitboard black_sol,Bitboard white_sol,Bitboard black_town,Bitboard white_town){
		black_soldiers = black_sol;
		white_soldiers = white_sol;
		black_townhall = black_town;
		white_townhall = white_town;	
		white = white_soldiers | white_townhall;
		black = black_soldiers | black_townhall;
		all   = white | black;	
	}
	Bitboard get_white_soldiers(){return white_soldiers;}
	Bitboard get_black_soldiers(){return black_soldiers;}
	Bitboard get_white_townhall(){return white_townhall;}
	Bitboard get_black_townhall(){return black_townhall;}
	Bitboard get_blacks(){return black;}
	Bitboard get_whites(){return white;}
	Bitboard get_all(){return all;}
	void set_white_soldiers(Bitboard white_sol){white_soldiers = white_sol;
												white = white_soldiers | white_townhall;
												all   = white | black;}
	void set_black_soldiers(Bitboard black_sol){black_soldiers = black_sol;
												black = black_soldiers | black_townhall;
												all   = white | black;}
	void set_white_townhall(Bitboard white_town){white_townhall = white_town;
												 white = white_soldiers | white_townhall;
												 all   = white | black;}
	void set_black_townhall(Bitboard black_town){black_townhall = black_town;
												 black = black_soldiers | black_townhall;
												 all   = white | black;}
	void set_white_soldiers_townhall(Bitboard white_sol, Bitboard white_town){
												 white_soldiers = white_sol;
												 white_townhall = white_town;
												 white = white_soldiers | white_townhall;
												 all   = white | black;}
	void set_black_soldiers_townhall(Bitboard black_sol, Bitboard black_town){
												 black_soldiers = black_sol;
												 black_townhall = black_town;
												 black = black_soldiers | black_townhall;
												 all   = white | black;}

};

uint64_t cap_sd[64];

uint64_t black_fr[64];
uint64_t black_rtr[64];
uint64_t black_adj[64];

uint64_t white_fr[64];
uint64_t white_rtr[64];
uint64_t white_adj[64];
uint64_t adjacent[64];
vector<Bitboard> data;

const int index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

int bitScanForward(uint64_t bb) {
   const uint64_t debruijn64 = 0x03f79d71b4cb0a89;
   assert (bb != 0);
   return index64[((bb & -bb) * debruijn64) >> 58];
}

bool valid_pos(vector<int> *pos, int n = 8, int m = 8){
	if((*pos)[0] >=0 and (*pos)[1] >=0 and (*pos)[0] <=(n-1) and (*pos)[1] <=(m-1)){
		return true;
	}
	return false;
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

void fill_database(){

	Bitboard a;
	for (int i = 0; i < 64; ++i){
		a = pow(2,i);
		data.push_back(a);
	}

	matrix m;
	uint64_t t;
	std::vector<int> pos;
	for (int i = 0; i < 8; ++i){
		for (int j = 0; j < 8; ++j){
			pos = {i,j};

			t =0;
			for(int k =-3 ; k<3; k++){
				for(int l = -3; l<3; l++){
					vector<int> posn = {i+k, j+l};
					if(valid_pos(&posn)){
						t = t + pow(2, posn[0] + 8*posn[1]);
					}
				}
			}
			adjacent[j*8+i] = t; // change for townhalls

			m = capture_side(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			cap_sd[j*8+i] = t;


			m = forward_steps(pos,0);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			black_fr[j*8+i] = t;

			m = retreat_steps(pos,0);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			black_rtr[j*8+i] = t;
			
			m = forward_steps(pos,1);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			white_fr[j*8+i] = t;

			m = retreat_steps(pos,1);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			white_rtr[j*8+i] = t;

			black_adj[j*8+i] = black_fr[j*8+i] | cap_sd[j*8+i];
			white_adj[j*8+i] = white_fr[j*8+i] | cap_sd[j*8+i];
		}
	}
}






void print_intvector(std::vector<int> v){
	for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i){
		std::cerr << *i << ' ' ;
	}
	std::cerr << std::endl;
}
void print_matrix(matrix mat){
	for (matrix::iterator i = mat.begin(); i != mat.end(); ++i){
		print_intvector(*i);
	}
	std::cerr << "---------------" << endl;
}
void print_boardrow(std::vector<int> v){
	std::cerr << '|';
	for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i){
		if(*i == 0){
			std::cerr <<  " |" ;
		}
		else{
			std::cerr << *i << '|' ;
		}
		
	}
	std::cerr << std::endl;
}
bool isKthBitSet(uint64_t n, int k) { 
    if ((n >> k) & 1){
        return true; 
    }
    else{
        return false; 
    }
} 
void print_board(board bd){

	vector<int> v;
	v.resize(8,0);
	matrix mat;
	mat.resize(8,v);
	int k;
	int i;
	int j;
	for (i = 0; i < 8; ++i){
		for (j = 0; j < 8; ++j){
			k = i*8 + j;
			// cout << k << ' ' << i << ' ' << j << endl; 
			if 	   (isKthBitSet(bd.get_black_townhall(),k)){mat[i][j] = 4;}
			else if(isKthBitSet(bd.get_black_soldiers(),k)){mat[i][j] = 3;}
			else if(isKthBitSet(bd.get_white_townhall(),k)){mat[i][j] = 2;}
			else if(isKthBitSet(bd.get_white_soldiers(),k)){mat[i][j] = 1;}
			else   									  	   {mat[i][j] = 0;} 
		}
	}
	for (matrix::iterator i = mat.begin(); i != mat.end(); ++i){
		print_boardrow(*i);
	}
	std::cerr << "-----------------" << endl;
}


// board change_state(string move, board bd){

// 	int k1  = 63 - 8*(move[4] - '0') - (move[2] - '0');
// 	int k2 = 63 - 8*(move[10]- '0') - (move[8]- '0');

	
// 	Bitboard mod2 = data[k2]; 


// 	if(move[6] == 'B'){
// 		return board(bd.get_black_soldiers()&(~mod2), bd.get_white_soldiers()&(~mod2), bd.get_black_townhall()&(~mod2), bd.get_white_townhall()&(~mod2));
// 	}
// 	else{
// 		Bitboard mod1 = data[k1]; 
// 		if(player_id == 1){
// 			return board(bd.get_black_soldiers()&(~mod2), bd.get_white_soldiers()&(~mod1)|mod2, bd.get_black_townhall()&(~mod2), bd.get_white_townhall()) ;		
// 		}
// 		else{
// 			return board(bd.get_black_soldiers()&(~mod1)|mod2, bd.get_white_soldiers()&(~mod2), bd.get_black_townhall(), bd.get_white_townhall()&(~mod2));			
// 		}
// 	}
// }
//|

void print_cannon(vector<uint16_t> cn){
	vector<int> v;
	v.resize(8,0);
	matrix mat;
	mat.resize(8,v);
	int k;
	int i;
	int j;
	int pos;
	int x1,y1;
	int x2,y2;
	for(int l=0; l<cn.size(); l++){
		pos = cn[l] >> 8;
		x1 = pos %8;
		y1 = pos /8;
		pos = cn[l] & 0b0000000011111111;
		x2 = pos %8;
		y2 = pos /8;
		mat[y1][x1] = 1;
		mat[y2][x2] = 1;
	}
	for (matrix::iterator i = mat.begin(); i != mat.end(); ++i){
		print_boardrow(*i);
	}
	cout << "-----------------" << endl;
}


vector<board> cannon_moves(board bd, uint16_t cannon, bool p_id){

	vector<board> res;
	vector<vector<int>> ret;
	vector<int> temp;
	uint64_t pos_occupied  = bd.get_all();
	uint64_t myteam;
	if(p_id) myteam = bd.get_whites();
	else myteam = bd.get_blacks();
	uint16_t bitpos = cannon >> 8;	
	vector<int> posi = {bitpos %8, bitpos/8};
	bitpos = cannon & 0b0000000011111111;
	vector<int> posf = {bitpos %8, bitpos/8};
	vector<int> pos;
	vector<int> old_pos;
	std::vector<int> dir = {sgn(posi[0] - posf[0]), sgn(posi[1] - posf[1])};

	pos = posi + dir;
	if(valid_pos(&pos)){
		if(!(pos_occupied & data[pos[1]*8 + pos[0]])){
			old_pos = posi - dir - dir;
			temp = {old_pos[0],old_pos[1],pos[0], pos[1]};//"S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " M " + to_string(pos[0]) + " " + to_string(pos[1]);
			ret.push_back(temp);
			pos = pos + dir;
			if(valid_pos(&pos) && !isKthBitSet( myteam , pos[1]*8 + pos[0])){
				temp = {-1,-1, pos[0], pos[1]};//"S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " B " + to_string(pos[0]) + " " + to_string(pos[1]);
				ret.push_back(temp);
			}
			pos = pos + dir;
			if(valid_pos(&pos) && !isKthBitSet( myteam , pos[1]*8 + pos[0])){
				temp = {-1,-1, pos[0], pos[1]};//"S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " B " + to_string(pos[0]) + " " + to_string(pos[1]);
				ret.push_back(temp);
			}
		}
	}
	dir[0] = -dir[0];
	dir[1] = -dir[1];
	pos = posf + dir;
	if(valid_pos(&pos)){
		if(!(pos_occupied & data[pos[1]*8 + pos[0]])){
			old_pos = posf - dir - dir;
			temp = {old_pos[0],old_pos[1],pos[0], pos[1]};//"S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " M " + to_string(pos[0]) + " " + to_string(pos[1]);
			ret.push_back(temp);
			pos = pos + dir;
			if(valid_pos(&pos) && !isKthBitSet( myteam , pos[1]*8 + pos[0])){
				temp = {-1,-1, pos[0], pos[1]};//"S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " B " + to_string(pos[0]) + " " + to_string(pos[1]);
				ret.push_back(temp);
			}
			pos = pos + dir;
			if(valid_pos(&pos) && !isKthBitSet( myteam , pos[1]*8 + pos[0])){
				temp = {-1,-1, pos[0], pos[1]};//"S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " B " + to_string(pos[0]) + " " + to_string(pos[1]);
				ret.push_back(temp);
			}
		}
	}

	uint64_t tt;
	board new_state = bd;
	uint64_t oppteam_sol;
	uint64_t oppteam_town;
	if(p_id) {
		for(int i=0; i<ret.size(); i++){
			myteam = bd.get_white_soldiers();
			oppteam_town = bd.get_black_townhall();
			oppteam_sol = bd.get_black_soldiers();
			new_state = bd;
			if(ret[i][0] == -1){
				oppteam_town = oppteam_town & (~data[ret[i][2] + ret[i][3]*8]);
				oppteam_sol = oppteam_sol & (~data[ret[i][2] + ret[i][3]*8]);
				new_state.set_black_soldiers_townhall(oppteam_sol, oppteam_town);
				res.push_back(new_state);
			}
			else{
					myteam = myteam & (~data[ret[i][0] + ret[i][1]*8]);
					myteam = myteam | (data[ret[i][2] + ret[i][3]*8]);
					new_state.set_white_soldiers(myteam);
					res.push_back(new_state);
				} 
		}
	}
	else {
		for(int i=0; i<ret.size(); i++){
			myteam = bd.get_black_soldiers();
			oppteam_sol = bd.get_white_soldiers();
			oppteam_town = bd.get_white_townhall();
			new_state = bd;
			if(ret[i][0] == -1){
				oppteam_town = oppteam_town & (~data[ret[i][2] + ret[i][3]*8]);
				oppteam_sol = oppteam_sol & (~data[ret[i][2] + ret[i][3]*8]);
				new_state.set_white_soldiers_townhall(oppteam_sol, oppteam_town);
				res.push_back(new_state);
			}
			else{
					myteam = myteam & (~data[ret[i][0] + ret[i][1]*8]);
					myteam = myteam | (data[ret[i][2] + ret[i][3]*8]);
					new_state.set_black_soldiers(myteam);
					res.push_back(new_state);
				} 
		}
	}

	return res;

}

vector<string> cannon_moves_string(board bd, uint16_t cannon, bool p_id){

	vector<string> ret;
	string temp;
	uint64_t pos_occupied  = bd.get_all();
	uint64_t myteam;
	if(p_id) myteam = bd.get_whites();
	else myteam = bd.get_blacks();
	uint16_t bitpos = cannon >> 8;	
	vector<int> posi = {bitpos %8, bitpos/8};
	bitpos = cannon & 0b0000000011111111;
	vector<int> posf = {bitpos %8, bitpos/8};
	vector<int> pos;
	vector<int> old_pos;
	std::vector<int> dir = {sgn(posi[0] - posf[0]), sgn(posi[1] - posf[1])};

	pos = posi + dir;
	if(valid_pos(&pos)){
		if(!(pos_occupied & data[pos[1]*8 + pos[0]])){
			old_pos = posi - dir - dir;
			temp = "S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " M " + to_string(pos[0]) + " " + to_string(pos[1]);
			ret.push_back(temp);
			pos = pos + dir;
			if(valid_pos(&pos) && !isKthBitSet( myteam , pos[1]*8 + pos[0])){
				temp = "S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " B " + to_string(pos[0]) + " " + to_string(pos[1]);
				ret.push_back(temp);
			}
			pos = pos + dir;
			if(valid_pos(&pos) && !isKthBitSet( myteam , pos[1]*8 + pos[0])){
				temp = "S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " B " + to_string(pos[0]) + " " + to_string(pos[1]);
				ret.push_back(temp);
			}
		}
	}
	dir[0] = -dir[0];
	dir[1] = -dir[1];
	pos = posf + dir;
	if(valid_pos(&pos)){
		if(!(pos_occupied & data[pos[1]*8 + pos[0]])){
			old_pos = posf - dir - dir;
			temp = "S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " M " + to_string(pos[0]) + " " + to_string(pos[1]);
			ret.push_back(temp);
			pos = pos + dir;
			if(valid_pos(&pos) && !isKthBitSet( myteam , pos[1]*8 + pos[0])){
				temp = "S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " B " + to_string(pos[0]) + " " + to_string(pos[1]);
				ret.push_back(temp);
			}
			pos = pos + dir;
			if(valid_pos(&pos) && !isKthBitSet( myteam , pos[1]*8 + pos[0])){
				temp = "S " + to_string(old_pos[0]) + " " + to_string(old_pos[1]) + " B " + to_string(pos[0]) + " " + to_string(pos[1]);
				ret.push_back(temp);
			}
		}
	}
	return ret;
}

vector<uint16_t> cannons(board bd, bool id){
	vector<uint16_t> ret;
	uint16_t temp;

	uint64_t sd;
	uint64_t sdpe;
	if( id == false ){
		sd = bd.get_black_soldiers();
		sdpe = sd;
	}
	else{
		sd = bd.get_white_soldiers();
		sdpe = sd;
	}

	uint16_t x;
	uint16_t y;
	uint64_t direction[4] = {0};
	int count;
	int i;
	int checkPos[] = {1, 8, 9, 7};
	while(sd!=0){
		x = bitScanForward(sd);
		for(int k = 0; k < 4; k++){
			uint64_t shift = data[x]; 
			if(!(direction[k] & shift)){
				if( k== 0) count = 8 - x%8;
				if( k== 1) count = 8 - x/8;
				if( k== 2) count = 8 - max(x/8, x%8);
				if( k== 3) count = min(7-x/8,x%8) + 1;
				i = 0;
				temp = 0b0000000000000000 | (x << 8);
				while(count > i ){
					direction[k] = direction[k] | shift;
					if(sdpe & shift){
						y = bitScanForward(shift);
						temp = (temp & 0b1111111100000000) | y;
					}
					else{
						break;
					}
					i++;
					shift = (shift << checkPos[k]);
				}
				if(i > 2) { 
					ret.push_back(temp);
				}
			}
			
		}
		sd = sd & (~data[x]);
	}
	return ret;
}
uint64_t bomb_pos (board bd, bool p_id ){
	vector<uint16_t> cannon = cannons(bd, p_id);
	uint64_t pos_occupied  = bd.get_all();
	uint64_t myteam;
	if(p_id) myteam = bd.get_whites();
	else myteam = bd.get_blacks();
	uint64_t res;
	for(int i = 0; i< cannon.size(); i++){
		uint64_t ret;
		uint16_t bitpos = cannon[i] >> 8;
		vector<int> posi = {bitpos %8, bitpos/8};
		bitpos = cannon[i] & 0b0000000011111111;
		vector<int> posf = {bitpos %8, bitpos/8};
		vector<int> pos;
		vector<int> old_pos;
		std::vector<int> dir = {sgn(posi[0] - posf[0]), sgn(posi[1] - posf[1])};
	
		pos = posi + dir;
		if(valid_pos(&pos)){
			if(!(pos_occupied & data[pos[1]*8 + pos[0]])){
				old_pos = posi - dir - dir;
				pos = pos + dir;
				if(valid_pos(&pos)){
					ret = ret | data[pos[0] + 8*pos[1]];
				}
				pos = pos + dir;
				if(valid_pos(&pos)){
					ret = ret | data[pos[0] + 8*pos[1]];
				}
			}
		}
		dir[0] = -dir[0];
		dir[1] = -dir[1];
		pos = posf + dir;
		if(valid_pos(&pos)){
			if(!(pos_occupied & data[pos[1]*8 + pos[0]])){
				old_pos = posf - dir - dir;
				pos = pos + dir;
				if(valid_pos(&pos)){
					ret = ret | data[pos[0] + 8*pos[1]];
				}
				pos = pos + dir;
				if(valid_pos(&pos)){
					ret = ret | data[pos[0] + 8*pos[1]];
				}
			}
		}
		ret = ret ^ (myteam & ret);
		res = res | ret;
	}
	return res;
}

uint64_t soldier_pos(board &bd, bool id){
	uint64_t ret;

	uint64_t ws  = bd.get_white_soldiers();
	uint64_t bs  = bd.get_black_soldiers();
	uint64_t ball= bd.get_blacks();
	uint64_t wall= bd.get_whites();
	uint16_t x;
	uint64_t r;

	if(id == false){
		while(bs!=0){
			x = bitScanForward(bs);
			bs = bs & (~data[x]);  					
			r = (black_fr[x]) & (~ball);			
			r = r | (cap_sd[x] & (wall));			
			if((black_adj[x] & ws) != 0)		
				r = r | (black_rtr[x] & (~ball));
			ret = ret | r;
		}
	}
	else{
		while(ws!=0){
			x = bitScanForward(ws);
			ws = ws & (~data[x]);
			r = white_fr[x] & (~wall);
			r = r | (cap_sd[x] & (ball));
			if((white_adj[x] & bs) != 0)
				r = r | (white_rtr[x] & (~wall));
			ret = ret | r;
		}
	}

	return ret;
}

vector<board> soldier_moves(board bd, bool id){

	vector<board> ret;
	board temp;

	uint64_t ws  = bd.get_white_soldiers();
	uint64_t bs  = bd.get_black_soldiers();
	uint64_t wspe= ws;
	uint64_t bspe= bs;
	uint64_t ball= bd.get_blacks();
	uint64_t wall= bd.get_whites();
	uint64_t bt  = bd.get_black_townhall();
	uint64_t wt  = bd.get_white_townhall();
	uint16_t x;
	uint16_t y;
	uint64_t r;

	if(id == false){
		while(bs!=0){
			x = bitScanForward(bs);
			bs = bs & (~data[x]);  					
			r = black_fr[x] & (~ball);				
			r = r | (cap_sd[x] & (wall));			
			if((black_adj[x] & ws) != 0)		
				r = r | (black_rtr[x] & (~ball));	
			while(r!=0){
				y = bitScanForward(r);
				r = r & (~data[y]);
				temp = bd;
				temp.set_black_soldiers( (bspe&(~data[x])) | data[y] );
				temp.set_white_soldiers_townhall( wspe&(~data[y]) , wt&(~data[y]) );
				// temp = "S " + to_string(x%8) + " " + to_string(x/8) + " M " + to_string(y%8) + " " + to_string(y/8);
				ret.push_back(temp);
			}
		}
	}
	else{

		while(ws!=0){
			x = bitScanForward(ws);
			ws = ws & (~data[x]);
			r = white_fr[x] & (~wall);
			r = r | (cap_sd[x] & (ball));
			if((white_adj[x] & bs) != 0)
				r = r | (white_rtr[x] & (~wall));

			while(r!=0){
				y = bitScanForward(r);
				r = r & (~data[y]);
				temp = bd;
				temp.set_white_soldiers( (wspe&(~data[x])) | data[y] );
				temp.set_black_soldiers_townhall( bspe&(~data[y]) , bt&(~data[y]) );
				// temp = "S " + to_string(x%8) + " " + to_string(x/8) + " M " + to_string(y%8) + " " + to_string(y/8);

				ret.push_back(temp);
			}
		}
	}

	return ret;
}

vector<string> soldier_moves_string(board bd, bool id){

	vector<string> ret;
	string temp;

	uint64_t ws  = bd.get_white_soldiers();
	uint64_t bs  = bd.get_black_soldiers();
	uint64_t wspe= ws;
	uint64_t bspe= bs;
	uint64_t ball= bd.get_blacks();
	uint64_t wall= bd.get_whites();
	uint64_t bt  = bd.get_black_townhall();
	uint64_t wt  = bd.get_white_townhall();
	uint16_t x;
	uint16_t y;
	uint64_t r;

	if(id == false){
		while(bs!=0){
			x = bitScanForward(bs);
			bs = bs & (~data[x]);  					
			r = black_fr[x] & (~ball);				
			r = r | (cap_sd[x] & (wall));			
			if((black_adj[x] & ws) != 0)		
				r = r | (black_rtr[x] & (~ball));	
			while(r!=0){
				y = bitScanForward(r);
				r = r & (~data[y]);
				// temp = bd;
				// temp.set_black_soldiers( (bspe&(~data[x])) | data[y] );
				// temp.set_white_soldiers_townhall( wspe&(~data[y]) , wt&(~data[y]) );
				temp = "S " + to_string(x%8) + " " + to_string(x/8) + " M " + to_string(y%8) + " " + to_string(y/8);
				ret.push_back(temp);
			}
		}
	}
	else{

		while(ws!=0){
			x = bitScanForward(ws);
			ws = ws & (~data[x]);
			r = white_fr[x] & (~wall);
			r = r | (cap_sd[x] & (ball));
			if((white_adj[x] & bs) != 0)
				r = r | (white_rtr[x] & (~wall));

			while(r!=0){
				y = bitScanForward(r);
				r = r & (~data[y]);
				// temp = bd;
				// temp.set_white_soldiers( (wspe&(~data[x])) | data[y] );
				// temp.set_black_soldiers_townhall( bspe&(~data[y]) , bt&(~data[y]) );
				temp = "S " + to_string(x%8) + " " + to_string(x/8) + " M " + to_string(y%8) + " " + to_string(y/8);

				ret.push_back(temp);
			}
		}
	}

	return ret;
}



board fixed_board(){
	Bitboard black_soldiers;
	Bitboard white_soldiers;
	Bitboard black_townhall;
	Bitboard white_townhall;

					 //8888888877777777666666665555555544444444333333332222222211111111
	white_soldiers = 0b0000000000000000000000000000010001000000100010001010101010101010;
	white_townhall = 0b0000000000000000000000000000000000000000000000000000000001010101;
	black_soldiers = 0b0100010101110101010000010010100000000000000000000000000000000000;
	black_townhall = 0b1010101000000000000000000000000000000000000000000000000000000000;
	return board(black_soldiers,white_soldiers,black_townhall,white_townhall);
}


/*
int main(int argc, char const *argv[]){
	bool x= 0;
	cout<< x;
	player_id = stoi(argv[1]);
	fill_database();
	// cout << (bitset<64>)black_fr[43] << endl;
	// cout << (bitset<64>)black_rtr[46] << endl;
	


	board bd = fixed_board();
	print_board(bd);
	// cout << "1111111122222222333333334444444455555555666666667777777788888888" << endl;
	vector<board> neighbors;
	for (int i = 0; i < 6500000; ++i){
		neighbors = soldier_moves(bd,player_id);
	// 		// cout << neighbors.size() << endl;

	}
	// for (int i = 0; i < neighbors.size(); ++i){
	// 	print_board(neighbors[i]);
	// }

	// board bd = board();
	// print_board(bd);

	// fill_database();
	
	// int r;
	// uint64_t b = 0b1000000000000000000000000000000000000000000000000000000000000000;
	// uint64_t c = 0b0000000000100000000000000000000000000000000000000000000000000000;
	// // chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	// r = bitScanForward(b);
	// cout << r << endl;
	// cout << (bitset<64>)r << endl;

	// chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	// cout << "Time difference = " << chrono::duration_cast<chrono::microseconds> (end - begin).count() << "[us]" << endl;
	// cout << r << endl;
	// cout << bitScanForward(b) << endl;

	// bd = change_state("S 2 5 M 3 4", bd);
	// print_board(bd);


	return 0;
}*/