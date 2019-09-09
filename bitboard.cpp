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

uint64_t cap_sd[64];

uint64_t black_fr[64];
uint64_t black_rtr[64];
uint64_t black_adj[64];

uint64_t white_fr[64];
uint64_t white_rtr[64];
uint64_t white_adj[64];
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



int main(int argc, char const *argv[]){

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
}