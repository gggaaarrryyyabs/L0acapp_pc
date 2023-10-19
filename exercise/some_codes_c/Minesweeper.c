#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// #define the size of map and the number of mines
#define Map_size 20
#define Mine_number 40

// swao two number by exchanging the pointers of the two
#define SWAP(x,y) {int temp = *x; *x = *y; *y = temp;}

// the map of mines is described as two dimensional array
int Map[Map_size][Map_size];
// -1 means unclicked, 0 means clicked and 1 mean mine_flag
int maskMap[Map_size][Map_size];
// the diffset of two points, at most 8 elements in the set
int diffSet[8][2];

// Fail if flag_game is FAIL
int flag_game;
const FAIL = 1;
const SUCCESS = 0;


// Fisher-Yates Shuffle
void Fisher_Yates_Shuffle(int list[],int lenInput){
    for (int i = lenInput - 1; i > 0; i--){
        srand(time(NULL));
        int randLocation = rand() % (i + 1);
        SWAP(&list[randLocation], &list[i]);
    }
}
/* initial the map of mines using Fisher-Yates shuffle, we don't use random seed to generate the locations of mines since there are possibilities that two different mines share a same location, and the larger numbers of mines, the more possible that some mines share a same location.
    Note that we define 9 the mine cells and 0 the safe cells 
*/
void genRandomMines(){
    int mine[Map_size*Map_size] = {0};
    for (int i = 0;i < Mine_number; i++)
        mine[i] = 9;
    int * p_mine = mine;
    Fisher_Yates_Shuffle(p_mine,Map_size*Map_size);
    // for (int i = 0; i < Map_size*Map_size;i++)
    //     printf("%d ",mine[i]);
    for (int i = 0; i < Map_size;i++)
        for (int j = 0; j < Map_size;j++)
            Map[i][j] = *(p_mine + j + i * Map_size);
}

/* it marked a cell with a number in {0,1,...,8} such that there are those number of mines surround the cell*/
void countMineSurroundNumber(){
    // the corners, the first row and the last row need to consider seperately
    if (Map[0][0] >= 9){
        Map[0][1]++;Map[1][1]++;Map[1][0]++;
    }
    if (Map[0][Map_size - 1] >= 9){
        Map[0][Map_size - 2]++; Map[1][Map_size - 1]++; Map[1][Map_size - 2]++; 
    }
    if (Map[Map_size - 1][0] >= 9){
        Map[Map_size - 2][0]++;Map[Map_size - 1][1]++;Map[Map_size - 2][1]++;
    }
    if (Map[Map_size - 1][Map_size - 1] >= 9){
        Map[Map_size - 2][Map_size - 1]++;Map[Map_size - 1][Map_size - 2]++;Map[Map_size - 2][Map_size - 2]++;
    }
    for (int j = 1;j < Map_size - 1;j++){
        if (Map[0][j] >= 9){
            Map[0][j - 1]++; Map[0][j + 1]++; Map[1][j - 1]++; Map[1][j    ]++; Map[1][j + 1]++; 
        }
    }
    for (int j = 1;j < Map_size - 1;j++){
        if (Map[Map_size - 1][j] >= 9){
            Map[Map_size - 1][j - 1]++; Map[Map_size - 1][j + 1]++; Map[Map_size - 2][j - 1]++; Map[Map_size - 2][j    ]++; Map[Map_size - 2][j + 1]++; 
        }
    }
    // set surround number for common cases
    for (int i = 1;i < Map_size - 1; i++){
        for (int j = 0;j < Map_size; j++){
            if (Map[i][j] >= 9){
                if (j == 0){
                    Map[i - 1][0]++;Map[i - 1][1]++;Map[i    ][1]++;Map[i + 1][1]++;Map[i + 1][0]++;
                }
                else if (j == Map_size - 1){
                    Map[i - 1][Map_size - 1]++;Map[i - 1][Map_size - 2]++;Map[i    ][Map_size - 2]++;Map[i + 1][Map_size - 2]++;Map[i + 1][Map_size - 1]++;
                }
                else {
                    Map[i][j + 1]++;Map[i][j - 1]++;Map[i + 1][j + 1]++;Map[i + 1][j - 1]++;Map[i + 1][j    ]++;Map[i - 1][j + 1]++;Map[i - 1][j - 1]++;Map[i - 1][j    ]++;
                }
            }
        }    
    }
    for (int i = 0; i < Map_size;i++)
        for (int j = 0; j < Map_size;j++)
            if (Map[i][j] >= 9)
                Map[i][j] = 9;
}

void printMap(){
    for (int i = 0; i < Map_size;i++){
        for (int j = 0; j < Map_size;j++){
            if (maskMap[i][j] == 0)
                printf("%2d ",Map[i][j]);
            else if (maskMap[i][j] == 1)
                printf(" # ");
            else{
                printf("%2d ",maskMap[i][j]);
            }
        }
        printf("\n");
    }
}

// count the number of flaged cell surrounded by (position_x,potsition_y)
int countNumberFlag(int Position_x,int Position_y){
    int mineNumberFlag = 0;
    for (int i = -1;i < 2 && Position_x + i > -1 && Position_x + i < Map_size;i++)
        for (int j = -1;j < 2 && Position_y + j > -1 && Position_y + j < Map_size;j++)
            if (maskMap[Position_x + i][Position_y + j] == 1)
                mineNumberFlag++;
    return mineNumberFlag;
}

// count the number of unclicked cell surrounded by (position_x,potsition_y)
int countUnmaskCell(int Position_x,int Position_y){
    int numberUnmaskCell = 0;
    for (int i = -1;i < 2 && Position_x + i > -1 && Position_x + i < Map_size;i++)
        for (int j = -1;j < 2 && Position_y + j > -1 && Position_y + j < Map_size;j++)
            if (maskMap[Position_x + i][Position_y + j] == -1)
                numberUnmaskCell++;
    return numberUnmaskCell;
}

// click the cell at (position[0],position[1]), print fail if there is a mine in the cell
void removeMask(int clickPosition_x,int clickPosition_y){
    // set the cell you clicked unmask 
    maskMap[clickPosition_x][clickPosition_y] = 0;
    // fail if the cell number is 9, i.e. you clicked a mine 
    if (Map[clickPosition_x][clickPosition_y] == 9){
        printf("===================================\n");
        printf("End, you have lost\n");
        printf("===================================\n");
        flag_game = FAIL;
    }
    else{
        // count the mines you labeled in surrounding of the cell clicked
        int mineNumberFlag = countNumberFlag(clickPosition_x,clickPosition_y);
        // special case: 
        // click all surrounded unmasked cell, if cell number equals to minnumberflag, which means cells without flag don't contain mines
        if (Map[clickPosition_x][clickPosition_y] == mineNumberFlag)
            for (int i = -1;i < 2 && clickPosition_x + i > -1 && clickPosition_x + i < Map_size;i++)
                for (int j = -1;j < 2 && clickPosition_y + j > -1 && clickPosition_y + j < Map_size;j++)
                    // recursive click the cell
                    if (maskMap[clickPosition_x + i][clickPosition_y + j] == -1)
                        removeMask(clickPosition_x + i, clickPosition_y + j); 
    }
}


// flag the cell with mines with #, but it only works for the simplest situations, such as the corner  * 1
//                                                                                                     1 1
// and we are sure that * must contain a mine.
void flagSimpleCell(){
    // range over all cells with nonzero numbers
    for (int x = 0; x < Map_size;x++){
        for (int y = 0; y < Map_size;y++){
            if (maskMap[x][y] == 0 && Map[x][y] != 0){
                // count the number of cells with mine flags and the number of masked cells surrounding the cell
                int mineNumberFlag = countNumberFlag(x,y);
                int numberUnmaskCell = countUnmaskCell(x,y);
                // all masked cells are mines if the number of the cell equals to the sum of the number of masked cells and mineflag cells,  
                if (Map[x][y] == mineNumberFlag + numberUnmaskCell)
                    for (int i = -1;i < 2 && x + i > -1 && x + i < Map_size;i++)
                        for (int j = -1;j < 2 && y + j > -1 && y + j < Map_size;j++)
                            if (maskMap[x + i][y + j] == -1){
                                maskMap[x + i][y + j] = 1;
                                removeCellsBesidesFlag(x + i,y + j);
                            }
            }
        }
    }
}

// after flagging the cells, we can usually go on to remove Mask 
void removeCellsBesidesFlag(int Position_x,int Position_y){
    // range over the cells with mine_flag
    for (int i_mine = -1;i_mine < 2 && Position_x + i_mine > -1 && Position_x + i_mine < Map_size;i_mine++){
        for (int j_mine = -1;j_mine < 2 && Position_y + j_mine > -1 && Position_y + j_mine < Map_size;j_mine++){
            if (maskMap[Position_x + i_mine][Position_y + j_mine] == 0 && Map[Position_x + i_mine][Position_y + j_mine] == countNumberFlag(Position_x + i_mine,Position_y + j_mine)){
                for (int i = -1; i < 2 && i + Position_x + i_mine > -1 && i + Position_x + i_mine < Map_size;i++)
                    for (int j = -1; j < 2 && j + Position_y + j_mine > -1 && j + Position_y + j_mine < Map_size;j++)
                        if (maskMap[i + Position_x + i_mine][j + Position_y + j_mine] == -1)
                            removeMask(i + Position_x + i_mine,j + Position_y + j_mine);
            }
        }
    }
}

// it returns 1 if two maskMaps equal and 0 otherwise
int equalMaskmaps(int Map[Map_size][Map_size],int afterMap[Map_size][Map_size]){
    for (int i = 0; i < Map_size;i++)
        for (int j = 0; j < Map_size;j++)
            if (Map[i][j] != afterMap[i][j])
                return 0;
    return 1;
}
// different set of (1x,1y)\(2x,2y)
int diffSetOfTwoPoint(int Position_1x, int Position_1y, int Position_2x, int Position_2y){
    int count = 0;
    for (int i = -1; i < 2 && Position_1x + i > -1 && Position_1x + i < Map_size;i++){
        for (int j = -1; j < 2 && Position_1y + j > -1 && Position_1y + j < Map_size;j++){
            if ( maskMap[Position_1x + i][Position_1y + j] == -1 && (Position_1x + i - Position_2x)*(Position_1x + i - Position_2x) > 1 && (Position_1y + j - Position_2y)*(Position_1y + j - Position_2y) > 1){
                diffSet[count][0] = Position_1x + i;
                diffSet[count][1] = Position_1y + j;
                count++;
            }
        }
    }
    for (int i = count;i < 8;i++){
        diffSet[i][0] = 0;
        diffSet[i][1] = 0;
    }
}

void clickOrFlag(){
    for (int x = 0; x < Map_size;x++){
        for (int y = 0; y < Map_size;y++){
            if (maskMap[x][y] == 0 && countUnmaskCell(x,y) != 0 && maskMap[x][y + 1] == 0 && countUnmaskCell(x,y + 1) != 0){
                int cnt = diffSetOfTwoPoint(x,y,x,y + 1);
                if (Map[x][y] - Map[x][y + 1] >= cnt){
                    for (int i = 0;i < cnt;i++)
                        maskMap[diffSet[i][0]][diffSet[i][1]] = 1;
                    int clickcnt = diffSetOfTwoPoint(x,y + 1,x,y);
                    for (int i = 0;i < clickcnt;i++)
                        removeMask(diffSet[i][0],diffSet[i][1]);
                }
            }
            else if (maskMap[x][y] == 0 && countUnmaskCell(x,y) != 0 && maskMap[x + 1][y] == 0 && countUnmaskCell(x + 1,y) != 0){
                int cnt = diffSetOfTwoPoint(x,y,x + 1,y);
                if (Map[x][y] - Map[x + 1][y] >= cnt){
                    for (int i = 0;i < cnt;i++)
                        maskMap[diffSet[i][0]][diffSet[i][1]] = 1;
                    int clickcnt = diffSetOfTwoPoint(x + 1,y,x,y);
                    for (int i = 0;i < clickcnt;i++)
                        removeMask(diffSet[i][0],diffSet[i][1]);
                }
            }
            
        }
    }
}

int main(){
    // initial masked map by setting all cell number -1, the masked cells
    for (int i = 0; i < Map_size;i++)
        for (int j = 0; j < Map_size;j++)
            maskMap[i][j] = -1;
    // generate mines map
    genRandomMines();
    countMineSurroundNumber(); 
    // click the cell at position (11,2)
    removeMask(11,2);
    // simple flag and click, end if map has not changed
    int tempMap[Map_size][Map_size];
    do
    {
        for (int i = 0; i < Map_size;i++)
            for (int j = 0; j < Map_size;j++)
                    tempMap[i][j] = maskMap[i][j];
        flagSimpleCell();
    } while (equalMaskmaps(maskMap,tempMap) == 0);
    
    // print the map
    printMap();
}
