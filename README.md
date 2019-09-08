# AI-A2
Cannon - Game Bot

Either use state.cpp or state_boolmatrix.cpp </br>
**state.cpp** - uses 1 vector<vector<int>> of 32 bit(4 Bytes) ints </br>
**state_boolmatrix.cpp** - uses 7 vector<vector<bool>> of 8 bit(1 Byte) bools 

Maintaining the state - 

1. **Identifying the soldiers (DONE)**
    1. Their moves (free)
    2. Their moves (enemy is adjacent)
    3. Attacks on enemy

2.  **Identifying the cannons (DONE)**
    1. Their moves
    2. Their targets
    
  **Later** - Will try to implement the bitboard representation
