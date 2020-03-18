#include <stdio.h>

#include <string.h>
#include <stdlib.h> //rand();
#include <stdint.h> //uint32_t

#include <unistd.h> //delay


//Definitii
char tabla[]={'-','-','-','-','-','-','-','-','-'};

static uint32_t TimingDelay;

	
void Delay( uint32_t nTime )
{
    usleep( nTime * 1000 );
}

void sendString( char a[] )
{
	int i, n;
	n = strlen(a);
	for (i = 0; i < n; i++)
	{
		putchar( a[i] );
	}
}

void sendChar( char a )
{
	putchar( a );
}

char readChar()
{
	char chr;
	
	chr = getchar();
	return chr;
}

void afisare() 
{
	uint8_t i;
	
    for (i=0; i<9; i++) 
	{
		sendChar( tabla[i] );
		sendString( " " );
       
		if( i==2 || i==5 || i==8 ) 
			sendString( "\n" );
    }
}

_Bool victorie() 
{
	char winner = '-';
	uint8_t i;
  
	if( tabla[0]==tabla[1] && tabla[1]==tabla[2] ) winner = tabla[0]; // linie 1
	if( tabla[3]==tabla[4] && tabla[4]==tabla[5] ) winner = tabla[3]; // linie 2
	if( tabla[6]==tabla[7] && tabla[7]==tabla[8] ) winner = tabla[6]; // linie 3
	if( tabla[0]==tabla[3] && tabla[3]==tabla[6] ) winner = tabla[0]; // coloana 1
	if( tabla[1]==tabla[4] && tabla[4]==tabla[7] ) winner = tabla[1]; // coloana 2
	if( tabla[2]==tabla[5] && tabla[5]==tabla[8] ) winner = tabla[2]; // coloana 3
	if( tabla[0]==tabla[4] && tabla[4]==tabla[8] ) winner = tabla[0]; // diagonala principala
	if( tabla[2]==tabla[4] && tabla[4]==tabla[6] ) winner = tabla[2]; // diagonala secundara

	if( winner == '-' ) 
	{
		winner = '*';
		for(i=0; i<9; i++)
			if( tabla[i] == '-' ) 
				winner = '-';
	}
      
	if( winner == '-' ) 
		return 0;
	else 
	{
		if( winner == 'X' ) 
		{
			afisare(); 
			sendString( "Ati castigat! Felicitari!\n" );
		}
		if( winner == '0' ) 
			sendString( "Nucleo a castigat! Mai incercati!\n" );
		 
		if( winner == '*' ) 
			sendString( "Remiza! Plictisitor!\n" );
		 
		return 1;
   }
}


int* convertToInt(char b[9], int result[9]){
    int i;
    
    for(i=0; i<9; i++){
        if(b[i] == '-'){
            result[i] = 0;
        }
        else if(b[i] == '0'){
            result[i] = 1;
        }
        else if(b[i] == 'X'){
            result[i] = -1;
        }
    }
    
   return result;
}

int win(const int board[9]) {
    //determines if a player has won, returns 0 otherwise.
    unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i = 0; i < 8; ++i) {
        if(board[wins[i][0]] != 0 &&
           board[wins[i][0]] == board[wins[i][1]] &&
           board[wins[i][0]] == board[wins[i][2]])
            return board[wins[i][2]];
    }
    return 0;
}

int minimax(int board[9], int player) {
    //How is the position like for player (their turn) on board?
    int winner = win(board);
    if(winner != 0) return winner*player;

    int move = -1;
    int score = -2;//Losing moves are preferred to no move
    int i;
    for(i = 0; i < 9; ++i) {//For all moves,
        if(board[i] == 0) {//If legal,
            board[i] = player;//Try the move
            int thisScore = -minimax(board, player*-1);
            if(thisScore > score) {
                score = thisScore;
                move = i;
            }//Pick the one that's worst for the opponent
            board[i] = 0;//Reset board after try
        }
    }
    if(move == -1) return 0;
    return score;
}

void mutare0(char* b) 
{    
    int j = -1;
	while( j == -1 ){
    int i;
    int r[9] = {0,0,0,0,0,0,0,0,0};
    int* board = convertToInt(b, r);
    
    int move = -1;
    int score = -2;
    
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = 1;
            int tempScore = -minimax(board, -1);
            board[i] = 0;
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    //returns a score based on minimax tree at a given node.
    board[move] = 1;
    
    		j = move;
		if( tabla[j] == '-' ) 
		{
			tabla[j] = '0';
			sendString( "Nucleo a ales pozitia: \n" );
			sendChar( j+1+'0' );
			sendString( "\n" );
			break;
		}	
		else j=-1;
	}

}

void mutareX() 
{
	_Bool valid = 0;
	uint8_t m0 = 0;
	while (!valid) 
	{
		sendString( "Alege o pozitie de la 1 la 9.\n" );
		while( m0<'1' || m0>'9' ) 
		{
			m0 = readChar();
			Delay(1000);
		}
     
		sendString( "Ati ales pozitia: \n" );
		sendChar( m0 );
		m0 = m0 - 48;
		sendString( "\n" );
		 
		if( tabla[m0-1] != '-' )
			sendString( "Pozitie eronata!\n" );
		else 
		{ 
			tabla[m0-1] = 'X';
			valid = 1;
		}
	}
}

int main()
{		
	uint8_t i;
	
	while(1)
	{
		sendString("Salut!\n");
		sendString("Tu vei muta primul - esti cu X.\n");
		sendString("Tabla de joc are pozitiile numerotate de la 1 la 9.\n");
		sendString("1 2 3\n");
		sendString("4 5 6\n");
		sendString("7 8 9\n");
		
		do
		{
			mutareX();
			if( victorie() ) break;
			mutare0(tabla);
			afisare();
		}while( !victorie() );
			
		Delay(5000);
		
		for( i=0; i<100; i++ ) 
			sendString("\n");
		for( i=0; i<9; i++ ) 
			tabla[i] = '-';
	}
}
