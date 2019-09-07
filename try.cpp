#include <bits/stdc++.h>
#include <chrono>
using namespace std;

int player_id;

typedef vector<vector<int>> matrix;

typedef uint64_t Bitboard; 

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
		white_soldiers = 0b0101010101010101010101010000000000000000000000000000000000000000;
		white_townhall = 0b1010101000000000000000000000000000000000000000000000000000000000;
		black_soldiers = 0b0000000000000000000000000000000000000000101010101010101010101010;
		black_townhall = 0b0000000000000000000000000000000000000000000000000000000001010101;
		white = white_soldiers & white_townhall;
		black = black_soldiers & black_townhall;
		all   = white & black;
	}
	board(Bitboard black_sol,Bitboard white_sol,Bitboard black_town,Bitboard white_town){
		black_soldiers = black_sol;
		white_soldiers = white_sol;
		black_townhall = black_town;
		white_townhall = white_town;	
		white = white_soldiers & white_townhall;
		black = black_soldiers & black_townhall;
		all   = white & black;	
	}
	Bitboard get_white_soldiers(){return white_soldiers;}
	Bitboard get_black_soldiers(){return black_soldiers;}
	Bitboard get_white_townhall(){return white_townhall;}
	Bitboard get_black_townhall(){return black_townhall;}
	Bitboard get_blacks(){return black;}
	Bitboard get_whites(){return white;}
	Bitboard get_all(){return all;}
};


// template <typename T>
// std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b){
//     assert(a.size() == b.size());

//     std::vector<T> result;
//     result.reserve(a.size());

//     std::transform(a.begin(), a.end(), b.begin(), 
//                    std::back_inserter(result), std::plus<T>());
//     return result;
// }
// template <typename T>
// std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b){
//     assert(a.size() == b.size());

//     std::vector<T> result;
//     result.reserve(a.size());

//     std::transform(a.begin(), a.end(), b.begin(), 
//                    std::back_inserter(result), std::minus<T>());
//     return result;
// }
vector<Bitboard> data;

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
			k = 63 - (i*8 + j);
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


board change_state(string move, board bd){

	int k1  = 63 - 8*(move[4] - '0') - (move[2] - '0');
	int k2 = 63 - 8*(move[10]- '0') - (move[8]- '0');

	
	Bitboard mod2 = data[k2]; 


	if(move[6] == 'B'){
		return board(bd.get_black_soldiers()&(~mod2), bd.get_white_soldiers()&(~mod2), bd.get_black_townhall()&(~mod2), bd.get_white_townhall()&(~mod2));
	}
	else{
		Bitboard mod1 = data[k1]; 
		if(player_id == 1){
			return board(bd.get_black_soldiers()&(~mod2), bd.get_white_soldiers()&(~mod1)|mod2, bd.get_black_townhall()&(~mod2), bd.get_white_townhall()) ;		
		}
		else{
			return board(bd.get_black_soldiers()&(~mod1)|mod2, bd.get_white_soldiers()&(~mod2), bd.get_black_townhall(), bd.get_white_townhall()&(~mod2));			
		}
	}
}








int main(int argc, char const *argv[]){

	player_id = stoi(argv[1]);
	Bitboard a;
	for (int i = 0; i < 64; ++i){
		a = pow(2,i);
		data.push_back(a);
	}

	board bd = board();
	print_board(bd);
	chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	bd = change_state("S 2 5 M 3 4", bd);
	chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	cout << "Time difference = " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << "[ns]" << endl;

	print_board(bd);





	return 0;
}