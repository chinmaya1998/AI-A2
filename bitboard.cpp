#include <bits/stdc++.h>
#include <chrono>
using namespace std;

bool player_id;

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

bool operator==(board& bd1, board& bd2){

    if(bd1.get_black_townhall() == bd2.get_black_townhall() && bd1.get_white_townhall() == bd2.get_white_townhall() && bd1.get_black_soldiers() == bd2.get_black_soldiers() && bd1.get_white_soldiers() == bd2.get_white_soldiers()){
		return true;
	}
	return false;
}

void print_intvector(std::vector<int> v){
	for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i){
		std::cout << *i << ' ' ;
	}
	std::cout << std::endl;
}
void print_matrix(matrix mat){
	for (matrix::iterator i = mat.begin(); i != mat.end(); ++i){
		print_intvector(*i);
	}
	cout << "---------------" << endl;
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
bool isKthBitSet(uint64_t n, uint64_t k) { 
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
	uint64_t k;
	uint64_t i;
	uint64_t j;
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
	cout << "-----------------" << endl;
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

matrix right_side(vector<int> pos){
	matrix ret;
	ret.reserve(1);
	std::vector<int> v;
	std::vector<int> t;
	t = {1,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix left_side(vector<int> pos){
	matrix ret;
	ret.reserve(1);
	std::vector<int> v;
	std::vector<int> t;
	t = {-1,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix up_side(vector<int> pos){
	matrix ret;
	ret.reserve(1);
	std::vector<int> v;
	std::vector<int> t;
	t = {0,-1};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix down_side(vector<int> pos){
	matrix ret;
	ret.reserve(1);
	std::vector<int> v;
	std::vector<int> t;
	t = {0,1};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}

matrix right_side23(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {2,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {3,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix left_side23(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {-2,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {-3,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix up_side23(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {0,-2};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {0,-3};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix down_side23(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {0,2};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {0,3};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}

matrix diagonal_up(vector<int> pos){
	matrix ret;
	ret.reserve(1);
	std::vector<int> v;
	std::vector<int> t;
	t = {1,-1};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix diagonal_down(vector<int> pos){
	matrix ret;
	ret.reserve(1);
	std::vector<int> v;
	std::vector<int> t;
	t = {-1,1};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix ndiagonal_up(vector<int> pos){
	matrix ret;
	ret.reserve(1);
	std::vector<int> v;
	std::vector<int> t;
	t = {-1,-1};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix ndiagonal_down(vector<int> pos){
	matrix ret;
	ret.reserve(1);
	std::vector<int> v;
	std::vector<int> t;
	t = {1,1};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}

matrix diagonal_up23(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {2,-2};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {3,-3};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix diagonal_down23(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {-2,2};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {-3,3};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix ndiagonal_up23(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {-2,-2};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {-3,-3};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}
matrix ndiagonal_down23(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {2,2};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {3,3};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}

uint64_t cap_sd[64];

uint64_t black_fr[64];
uint64_t black_rtr[64];
uint64_t black_adj[64];

uint64_t white_fr[64];
uint64_t white_rtr[64];
uint64_t white_adj[64];
vector<Bitboard> data;

uint64_t right_sd[64];
uint64_t left_sd[64];
uint64_t up_sd[64];
uint64_t down_sd[64];

uint64_t right_sd23[64];
uint64_t left_sd23[64];
uint64_t up_sd23[64];
uint64_t down_sd23[64];

uint64_t diag_top[64];
uint64_t diag_down[64];
uint64_t ndiag_top[64];
uint64_t ndiag_down[64];

uint64_t diag_top23[64];
uint64_t diag_down23[64];
uint64_t ndiag_top23[64];
uint64_t ndiag_down23[64];

uint64_t cols[8];
uint64_t rows[8];
uint64_t diag[9];
uint64_t ndiag[9];

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

void fill_database2(){

	matrix m;
	uint64_t t;
	std::vector<int> pos;
	for (int i = 0; i < 8; ++i){
		for (int j = 0; j < 8; ++j){
			pos = {i,j};


			m = up_side(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			up_sd[j*8+i] = t;

			m = up_side23(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			up_sd23[j*8+i] = t;

			m = down_side(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			down_sd[j*8+i] = t;

			m = down_side23(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			down_sd23[j*8+i] = t;

			m = right_side(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			right_sd[j*8+i] = t;

			m = right_side23(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			right_sd23[j*8+i] = t;

			m = left_side(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			left_sd[j*8+i] = t;

			m = left_side23(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			left_sd23[j*8+i] = t;





			m = diagonal_up(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			diag_top[j*8+i] = t;

			m = diagonal_up23(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			diag_top23[j*8+i] = t;

			m = diagonal_down(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			diag_down[j*8+i] = t;

			m = diagonal_down23(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			diag_down23[j*8+i] = t;

			m = ndiagonal_up(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			ndiag_top[j*8+i] = t;

			m = ndiagonal_up23(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			ndiag_top23[j*8+i] = t;

			m = ndiagonal_down(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			ndiag_down[j*8+i] = t;

			m = ndiagonal_down23(pos);
			t = 0;
			for (int k = 0; k < m.size(); ++k){
				t = t + pow(2,8*m[k][1] + m[k][0]);
			}
			ndiag_down23[j*8+i] = t;

		}
	}
}

void fill_database3(){

	matrix m;
	uint64_t t;
	std::vector<int> pos;

	for (int i = 0; i < 8; ++i){
		t = 0;
		for (int j = 0; j < 8; ++j){
			// pos = {i,i+8*j};
			t = t + (uint64_t)pow(2, j*8+i);

		}
		cols[i] = t;
	}

	for (int j = 0; j < 8; ++j){
		t = 0;
		for (int i = 0; i < 8; ++i){
			// pos = {i,i+8*j};
			t = t + (uint64_t)pow(2, j*8+i);
		}
		rows[j] = t;
	}
			  //8888888877777777666666665555555544444444333333332222222211111111
	diag[0] = 0b0000000000000000000000000000000000000001000000100000010000001000;
	diag[1] = 0b0000000000000000000000000000000100000010000001000000100000010000;
	diag[2] = 0b0000000000000000000000010000001000000100000010000001000000100000;
	diag[3] = 0b0000000000000001000000100000010000001000000100000010000001000000;
	diag[4] = 0b0000000100000010000001000000100000010000001000000100000010000000;
	diag[5] = 0b0000001000000100000010000001000000100000010000001000000000000000;
	diag[6] = 0b0000010000001000000100000010000001000000100000000000000000000000;
	diag[7] = 0b0000100000010000001000000100000010000000000000000000000000000000;
	diag[8] = 0b0001000000100000010000001000000000000000000000000000000000000000;
			   //8888888877777777666666665555555544444444333333332222222211111111
	ndiag[0] = 0b0000000000000000000000000000000010000000010000000010000000010000;
	ndiag[1] = 0b0000000000000000000000001000000001000000001000000001000000001000;
	ndiag[2] = 0b0000000000000000100000000100000000100000000100000000100000000100;
	ndiag[3] = 0b0000000010000000010000000010000000010000000010000000010000000010;
	ndiag[4] = 0b1000000001000000001000000001000000001000000001000000001000000001;
	ndiag[5] = 0b0100000000100000000100000000100000000100000000100000000100000000;
	ndiag[6] = 0b0010000000010000000010000000010000000010000000010000000000000000;
	ndiag[7] = 0b0001000000001000000001000000001000000001000000000000000000000000;
	ndiag[8] = 0b0000100000000100000000100000000100000000000000000000000000000000;
}

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
				temp = "S " + to_string(x%8) + " " + to_string(x/8) + " M " + to_string(y%8) + " " + to_string(y/8);
				ret.push_back(temp);
			}
		}
	}

	return ret;
}


vector<int> find_cannon(uint64_t file, int type){
	// 0 -> column
	// 1 -> row
	// 2 -> diagonal
	// 3 -> non-diagonal

	vector<int> ret = {64, 64};
	uint16_t x;
	uint16_t y;
	uint16_t i = 1;
	if(file == 0){return ret;}
	x = bitScanForward(file);
	file = file & (~data[x]);
	ret[0] = x;
	// print_intvector(ret);
	if (type == 0){
		while(file!=0){
			y = bitScanForward(file);
			file = file & (~data[y]);	 
			if(y - x == 8 ){
				i++;
				if(i >= 3){ret[1] = y;}
			} 
			else{
				if(i >= 3){return ret;}
				i = 1;
				ret[0] = y;
			}
			x = y;					
		}
	}

	else if (type == 1){
		while(file!=0){
			y = bitScanForward(file);
			file = file & (~data[y]);	 
			if(y - x == 1 ){
				i++;
				if(i >= 3){ret[1] = y;}
			} 
			else{
				if(i >= 3){return ret;}
				i = 1;
				ret[0] = y;
			}
			x = y;					
		}
	}

	else if (type == 2){
		while(file!=0){
			y = bitScanForward(file);
			file = file & (~data[y]);
			if(y - x == 7 ){
				i++;
				if(i >= 3){ret[1] = y;}
			} 
			else{
				if(i >= 3){return ret;}
				i = 1;
				ret[0] = y;
			}
			x = y;					
		}
	}

	else if (type == 3){
		while(file!=0){
			y = bitScanForward(file);
			file = file & (~data[y]);	 
			if(y - x == 9){
				i++;
				if(i >= 3){ret[1] = y;}
			} 
			else{
				if(i >= 3){return ret;}
				i = 1;
				ret[0] = y;
			}
			x = y;					
		}
	}

	return ret;
}
vector<board> cannon_moves(board bd, bool id){

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
	uint64_t all = bd.get_all();
	uint64_t r;
	uint64_t supp;

	uint16_t x;
	uint16_t y;
	uint16_t z;
	std::vector<int> v;
	bool flag = true;

	if(id == false){  // my soldiers are black
		for (int i = 0; i < 8; ++i){ // checking columns
			r = cols[i] & bspe;
			v = find_cannon(r , 0);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = up_sd23[x] & ~ball;
			while(((up_sd[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_white_soldiers_townhall( wspe&(~data[z]) , wt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}				
			}
			r = ~ball & down_sd23[y];
			while(((down_sd[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_white_soldiers_townhall( wspe&(~data[z]) , wt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}

			if((up_sd[x] & (~all)) != 0){
				temp = bd;
				temp.set_black_soldiers( (bspe&(~data[x + 16])) | up_sd[x] );
				ret.push_back(temp);
			}

			if((down_sd[y] & (~all)) != 0){
				temp = bd;
				temp.set_black_soldiers( (bspe&(~data[y - 16])) | down_sd[y] );
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 8; ++i){ // checking rows
			r = rows[i] & bspe;
			v = find_cannon(r , 1);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = left_sd23[x] & ~ball;
			while(((left_sd[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_white_soldiers_townhall( wspe&(~data[z]) , wt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~ball & right_sd23[y];
			while(((right_sd[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_white_soldiers_townhall( wspe&(~data[z]) , wt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			if((left_sd[x] & (~all)) != 0){
				temp = bd;
				temp.set_black_soldiers( (bspe&(~data[x + 2])) | left_sd[x] );
				ret.push_back(temp);
			}

			if((right_sd[y] & (~all)) != 0){
				temp = bd;
				temp.set_black_soldiers( (bspe&(~data[y - 2])) | right_sd[y] );
				ret.push_back(temp);
			}	
		}

		for (int i = 0; i < 9; ++i){ // checking diagonals
			r = diag[i] & bspe;
			v = find_cannon(r , 2);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = diag_top23[x] & ~ball;
			while(((diag_top[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_white_soldiers_townhall( wspe&(~data[z]) , wt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~ball & diag_down23[y];
			while(((diag_down[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_white_soldiers_townhall( wspe&(~data[z]) , wt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			if((diag_top[x] & (~all)) != 0){
				temp = bd;
				temp.set_black_soldiers( (bspe&(~data[x + 14])) | diag_top[x] );
				ret.push_back(temp);
			}

			if((diag_down[y] & (~all)) != 0){
				temp = bd;
				temp.set_black_soldiers( (bspe&(~data[y - 14])) | diag_down[y] );
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 9; ++i){ // checking non-diagonals
			r = ndiag[i] & bspe;
			v = find_cannon(r , 3);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = ndiag_top23[x] & ~ball;
			while(((ndiag_top[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_white_soldiers_townhall( wspe&(~data[z]) , wt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~ball & ndiag_down23[y];
			while(((ndiag_down[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_white_soldiers_townhall( wspe&(~data[z]) , wt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			if((ndiag_top[x] & (~all)) != 0){
				temp = bd;
				temp.set_black_soldiers( (bspe&(~data[x + 18])) | ndiag_top[x] );
				ret.push_back(temp);
			}

			if((ndiag_down[y] & (~all)) != 0){
				temp = bd;
				temp.set_black_soldiers( (bspe&(~data[y - 18])) | ndiag_down[y] );
				ret.push_back(temp);
			}
		}
	}

	else{			   // my soldiers are white
		for (int i = 0; i < 8; ++i){ // checking columns
			
 			r = cols[i] & wspe;
			v = find_cannon(r , 0);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			cout << "cols" << endl;
			r = up_sd23[x] & ~wall;
			while(((up_sd[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_black_soldiers_townhall( bspe&(~data[z]) , bt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~wall & down_sd23[y];
			while(((down_sd[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_black_soldiers_townhall( bspe&(~data[z]) , bt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			if((up_sd[x] & (~all)) != 0){
				temp = bd;
				temp.set_white_soldiers( (wspe&(~data[x + 16])) | up_sd[x] );
				ret.push_back(temp);
			}

			if((down_sd[y] & (~all)) != 0){
				temp = bd;
				temp.set_white_soldiers( (wspe&(~data[y - 16])) | down_sd[y] );
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 8; ++i){ // checking rows
			r = rows[i] & wspe;
			v = find_cannon(r , 1);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = left_sd23[x] & ~wall;
			while(((left_sd[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_black_soldiers_townhall( bspe&(~data[z]) , bt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~wall & right_sd23[y];
			while(((right_sd[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_black_soldiers_townhall( bspe&(~data[z]) , bt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			if((left_sd[x] & (~all)) != 0){
				temp = bd;
				temp.set_white_soldiers( (wspe&(~data[x + 2])) | left_sd[x] );
				ret.push_back(temp);
			}

			if((right_sd[y] & (~all)) != 0){
				temp = bd;
				temp.set_white_soldiers( (wspe&(~data[y - 2])) | right_sd[y] );
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 9; ++i){ // checking diagonals
			r = diag[i] & wspe;
			v = find_cannon(r , 2);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = diag_top23[x] & ~wall;
			while(((diag_top[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_black_soldiers_townhall( bspe&(~data[z]) , bt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~wall & diag_down23[y];
			while(((diag_down[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_black_soldiers_townhall( bspe&(~data[z]) , bt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			if((diag_top[x] & (~all)) != 0){
				temp = bd;
				temp.set_white_soldiers( (wspe&(~data[x + 14])) | diag_top[x] );
				ret.push_back(temp);
			}

			if((diag_down[y] & (~all)) != 0){
				temp = bd;
				temp.set_white_soldiers( (wspe&(~data[y - 14])) | diag_down[y] );
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 9; ++i){ // checking non-diagonals
			r = ndiag[i] & wspe;
			v = find_cannon(r , 3);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = ndiag_top23[x] & ~wall;
			while(((ndiag_top[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_black_soldiers_townhall( bspe&(~data[z]) , bt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~wall & ndiag_down23[y];
			while(((ndiag_down[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = bd;
					temp.set_black_soldiers_townhall( bspe&(~data[z]) , bt&(~data[z]) );
					ret.push_back(temp);
					flag = false;
				}
			}
			if((ndiag_top[x] & (~all)) != 0){
				temp = bd;
				temp.set_white_soldiers( (wspe&(~data[x + 18])) | ndiag_top[x] );
				ret.push_back(temp);
			}

			if((ndiag_down[y] & (~all)) != 0){
				temp = bd;
				temp.set_white_soldiers( (wspe&(~data[y - 18])) | ndiag_down[y] );
				ret.push_back(temp);
			}
		}
	}

	return ret;
}
vector<string> cannon_moves_string(board bd, bool id){

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
	uint64_t all = bd.get_all();
	uint64_t r;

	uint16_t x;
	uint16_t y;
	uint16_t z;
	std::vector<int> v;
	bool flag = true;

	if(id == false){  // my soldiers are black
		for (int i = 0; i < 8; ++i){ // checking columns
			r = cols[i] & bspe;
			v = find_cannon(r , 0);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = up_sd23[x] & ~ball;
			while(((up_sd[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~ball & down_sd23[y];
			while(((down_sd[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}

			if((up_sd[x] & (~all)) != 0){
				temp = "S " + to_string((x+16)%8) + " " + to_string((x+16)/8) + " M " + to_string((x-8)%8) + " " + to_string((x-8)/8);
				ret.push_back(temp);
			}

			if((down_sd[y] & (~all)) != 0){
				temp = "S " + to_string((y-16)%8) + " " + to_string((y-16)/8) + " M " + to_string((y+8)%8) + " " + to_string((y+8)/8);
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 8; ++i){ // checking rows
			r = rows[i] & bspe;
			v = find_cannon(r , 1);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = left_sd23[x] & ~ball;
			while(((left_sd[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~ball & right_sd23[y];
			while(((right_sd[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			if((left_sd[x] & (~all)) != 0){
				temp = "S " + to_string((x+2)%8) + " " + to_string((x+2)/8) + " M " + to_string((x-1)%8) + " " + to_string((x-1)/8);
				ret.push_back(temp);
			}

			if((right_sd[y] & (~all)) != 0){
				temp = "S " + to_string((y-2)%8) + " " + to_string((y-2)/8) + " M " + to_string((y+1)%8) + " " + to_string((y+1)/8);
				ret.push_back(temp);
			}	
		}

		for (int i = 0; i < 9; ++i){ // checking diagonals
			r = diag[i] & bspe;
			v = find_cannon(r , 2);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = diag_top23[x] & ~ball;
			while(((diag_top[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~ball & diag_down23[y];
			while(((diag_down[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			if((diag_top[x] & (~all)) != 0){
				temp = "S " + to_string((x+14)%8) + " " + to_string((x+14)/8) + " M " + to_string((x-7)%8) + " " + to_string((x-7)/8);
				ret.push_back(temp);
			}

			if((diag_down[y] & (~all)) != 0){
				temp = "S " + to_string((y-14)%8) + " " + to_string((y-14)/8) + " M " + to_string((y+7)%8) + " " + to_string((y+7)/8);
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 9; ++i){ // checking non-diagonals
			r = ndiag[i] & bspe;
			v = find_cannon(r , 3);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = ndiag_top23[x] & ~ball;
			while(((ndiag_top[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~ball & ndiag_down23[y];
			while(((ndiag_down[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			if((ndiag_top[x] & (~all)) != 0){
				temp = "S " + to_string((x+18)%8) + " " + to_string((x+18)/8) + " M " + to_string((x-9)%8) + " " + to_string((x-9)/8);
				ret.push_back(temp);
			}

			if((ndiag_down[y] & (~all)) != 0){
				temp = "S " + to_string((y-18)%8) + " " + to_string((y-18)/8) + " M " + to_string((y+9)%8) + " " + to_string((y+9)/8);
				ret.push_back(temp);
			}
		}
	}

	else{			   // my soldiers are white
		for (int i = 0; i < 8; ++i){ // checking columns
			
 			r = cols[i] & wspe;
			v = find_cannon(r , 0);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = up_sd23[x] & ~wall;
			while(((up_sd[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~wall & down_sd23[y];
			while(((down_sd[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			if((up_sd[x] & (~all)) != 0){
				temp = "S " + to_string((x+16)%8) + " " + to_string((x+16)/8) + " M " + to_string((x-8)%8) + " " + to_string((x-8)/8);
				ret.push_back(temp);
			}

			if((down_sd[y] & (~all)) != 0){
				temp = "S " + to_string((y-16)%8) + " " + to_string((y-16)/8) + " M " + to_string((y+8)%8) + " " + to_string((y+8)/8);
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 8; ++i){ // checking rows
			r = rows[i] & wspe;
			v = find_cannon(r , 1);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = left_sd23[x] & ~wall;
			while(((left_sd[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~wall & right_sd23[y];
			while(((right_sd[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			if((left_sd[x] & (~all)) != 0){
				temp = "S " + to_string((x+2)%8) + " " + to_string((x+2)/8) + " M " + to_string((x-1)%8) + " " + to_string((x-1)/8);
				ret.push_back(temp);
			}

			if((right_sd[y] & (~all)) != 0){
				temp = "S " + to_string((y-2)%8) + " " + to_string((y-2)/8) + " M " + to_string((y+1)%8) + " " + to_string((y+1)/8);
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 9; ++i){ // checking diagonals
			r = diag[i] & wspe;
			v = find_cannon(r , 2);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = diag_top23[x] & ~wall;
			while(((diag_top[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~wall & diag_down23[y];
			while(((diag_down[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			if((diag_top[x] & (~all)) != 0){
				temp = "S " + to_string((x+14)%8) + " " + to_string((x+14)/8) + " M " + to_string((x-7)%8) + " " + to_string((x-7)/8);
				ret.push_back(temp);
			}

			if((diag_down[y] & (~all)) != 0){
				temp = "S " + to_string((y-14)%8) + " " + to_string((y-14)/8) + " M " + to_string((y+7)%8) + " " + to_string((y+7)/8);
				ret.push_back(temp);
			}
		}

		for (int i = 0; i < 9; ++i){ // checking non-diagonals
			r = ndiag[i] & wspe;
			v = find_cannon(r , 3);
			x = v[0]; y = v[1];
			if(y==64){continue;}
			r = ndiag_top23[x] & ~wall;
			while(((ndiag_top[x] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			r = ~wall & ndiag_down23[y];
			while(((ndiag_down[y] & (~all)) != 0) && r!=0){
				z = bitScanForward(r);
				r = r & (~data[z]);
				if(flag || ((data[z] & ~all) == 0)){
					temp = "S " + to_string(x%8) + " " + to_string(x/8) + " B " + to_string(z%8) + " " + to_string(z/8);
					ret.push_back(temp);
					flag = false;
				}
			}
			if((ndiag_top[x] & (~all)) != 0){
				temp = "S " + to_string((x+18)%8) + " " + to_string((x+18)/8) + " M " + to_string((x-9)%8) + " " + to_string((x-9)/8);
				ret.push_back(temp);
			}

			if((ndiag_down[y] & (~all)) != 0){
				temp = "S " + to_string((y-18)%8) + " " + to_string((y-18)/8) + " M " + to_string((y+9)%8) + " " + to_string((y+9)/8);
				ret.push_back(temp);
			}
		}
	}

	return ret;
}

vector<board> getAllNeighbour(board bd, bool id){
	vector<board> ret;
	ret = soldier_moves(bd, id);
	vector<board> y = cannon_moves(bd, id);
	ret.insert(ret.end(), y.begin(), y.end());
	return ret;
}
vector<string> getAllNeighbour_string(board bd, bool id){
	vector<string> ret;
	ret = soldier_moves_string(bd, id);
	vector<string> y = cannon_moves_string(bd, id);
	ret.insert(ret.end(), y.begin(), y.end());
	return ret;
}



board fixed_board1(){
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

board fixed_board2(){
	Bitboard black_soldiers;
	Bitboard white_soldiers;
	Bitboard black_townhall;
	Bitboard white_townhall;

					 //8888888877777777666666665555555544444444333333332222222211111111
	white_soldiers = 0b0000000000000000000000000000000000100100110001101011100100100000;
	white_townhall = 0b0000000000000000000000000000000000000000000000000000000001010101;
	black_soldiers = 0b0000010101100101011000011001000000010000000000000000000000000000;
	black_townhall = 0b1010101000000000000000000000000000000000000000000000000000000000;
	return board(black_soldiers,white_soldiers,black_townhall,white_townhall);
}



int main(int argc, char const *argv[]){

	player_id = stoi(argv[1]);
	fill_database();
	fill_database2();
	fill_database3();


	board bd = fixed_board2();
	print_board(bd);

	// vector<board> v1 = cannon_moves(bd,player_id);
	// for (int i = 0; i < v1.size(); ++i){
	// 	print_board(v1[i]);
	// }

	// vector<string> v = cannon_moves_string(bd,player_id);
	// for (int i = 0; i < v.size(); ++i){
	// 	cout << v[i] << endl;
	// }

	std::vector<board> v = getAllNeighbour(bd,player_id);
	for (int i = 0; i < 6500000; ++i){
		getAllNeighbour(bd, player_id);
	}

	
	// print_board(bd);
	// // cout << "1111111122222222333333334444444455555555666666667777777788888888" << endl;
	// vector<board> neighbors;
	// for (int i = 0; i < 6500000; ++i){
	// 	neighbors = soldier_moves(bd,player_id);
	// // 		// cout << neighbors.size() << endl;

	// }
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
}