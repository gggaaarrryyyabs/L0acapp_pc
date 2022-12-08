#include<stdio.h>
#include<Windows.h>

#define N 20

/* the scale of chessboard is N*N and the value is 2 means no chessman in that position, 0 means black and 1 means white */
int chessboard[N][N];

void paintboard(){
    for (int i=0;i<N;i++)
    {
        for (int j=0;j<N;j++)
        {
            switch (chessboard[i][j])
            {
                case 0:
                    printf("● ");
                    break;
                case 1:
                    printf("o ");
                    break;
                case 2:
                    if (i == 0 && j == 0)
                        printf("╔ ");
                    else if (i == 0 && j != N-1)
                        printf("╤ ");
                    else if (i == 0 && j == N-1)
                        printf("╗");
                    else if (i == N-1 && j == N-1)
                        printf("╝");
                    else if (0 < i && i < N-1 && j == 0)
                        printf("╟ ");
                    else if (j == N-1 && 0 < i && i < N-1)
                        printf("╢");
                    else if (i == N-1 && j == 0)
                        printf("╚ ");
                    else if (i == N-1 && 0 < j && j < N-1)
                        printf("╧ ");
                    else if (0 < i && i < N-1 && 0 < j && j < N-1)
                        printf("┼ ");
                    break;
            }
        }
        printf("\n");
    }
}


int main()
{
    // the cordinates of chessman in chessboard 
    int i,j;
    // color = 0 or 1 is black or white
    int color;
    // the color_flag is the temp judgement 
    int color_flag = 0;

    // the number of the chessman with the same color in one row or one colomum or one diag
    int count_temp; 
    // the start checkpoint of the chessman we judge
    int i_count,j_count;
    // initial the chessboard with all positions no chessman
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            chessboard[i][j] = 2;
    // 

    while(1)
    {
        printf("please input the position of the chessman\n");
        if (color_flag == 0)
            printf("black player\n");
        else
            printf("white player\n");

        scanf("%d,%d",&i,&j);
        // the chessman's position is out of range
        if (i<0||j<0||i>N-1||j>N-1){
            printf("the position you gives is out of range of the chessboard\n");
            continue;
        }
        // the chessman's position has already a chessman
        if (chessboard[i][j] != 2){ 
            printf("the position you gives has been already another chessman\n");
            // clear the cash
            fflush(stdin);
            continue;
        }
        // confirm the color of chessman
        color_flag = (color_flag + 1) % 2;
        // 
        chessboard[i][j] = color_flag;
        
        // paint the chessboard according to values of chessboard points
        paintboard();

        // judge the win or loss 
        for (i=0;i<N;i++)
        {
            for (j=0;j<N;j++)
            {
                // if the position (i,j) has a chessman, judge win or loss with the same color chessman
                if (chessboard[i][j] != 2)
                {
                    color_flag = chessboard[i][j];
                    count_temp = 1;
                    i_count = i;
                    j_count = j;
                    /* without 5 chessman and then test position (i,j) whether has possibility to win when judge from here to below */
                    while(count_temp != 5 && ++j_count < N)
                    {
                        // whether chessman in below position is the same color with chessman in (i,j)
                        if (chessboard[i][j_count] == color_flag)
                        {
                            count_temp++;
                            if (count_temp == 5)
                            {
                                if (color_flag == 1)
                                    printf("black win");
                                else
                                    printf("white win");
                                goto game_end;
                            }
                        }
                        // not same color with (i,j)
                        else
                        {
                            count_temp = 0;
                            break;
                        }
                    }
                    /* without 5 chessman and then test position (i,j) whether has possibility to win when judge from here to right */
                    count_temp = 1;
                    i_count = i;
                    j_count = j;
                    while(count_temp != 5 && ++i_count < N)
                    {
                        // whether chessman in below position is the same color with chessman in (i,j)
                        if (chessboard[i_count][j] == color_flag)
                        {
                            count_temp++;
                            if (count_temp == 5)
                            {
                                if (color_flag == 1)
                                    printf("black win");
                                else
                                    printf("white win");
                                goto game_end;
                            }
                        }
                        // not same color with (i,j)
                        else
                        {
                            count_temp = 0;
                            break;
                        }
                    }
                    /* without 5 chessman and then test position (i,j) whether has possibility to win when judge from here to right below */
                    count_temp = 1;
                    i_count = i;
                    j_count = j;
                    while(count_temp != 5 && ++i_count < N && ++j_count < N)
                    {
                        // whether chessman in below position is the same color with chessman in (i,j)
                        if (chessboard[i_count][j_count] == color_flag)
                        {
                            count_temp++;
                            if (count_temp == 5)
                            {
                                if (color_flag == 1)
                                    printf("black win");
                                else
                                    printf("white win");
                                goto game_end;
                            }
                        }
                        // not same color with (i,j)
                        else
                        {
                            count_temp = 0;
                            break;
                        }
                    }
                    /* without 5 chessman and then test position (i,j) whether has possibility to win when judge from here to left below */
                    count_temp = 1;
                    i_count = i;
                    j_count = j;
                    while(count_temp != 5 && ++i_count < N && --j_count < N)
                    {
                        // whether chessman in below position is the same color with chessman in (i,j)
                        if (chessboard[i_count][j_count] == color_flag)
                        {
                            count_temp++;
                            if (count_temp == 5)
                            {
                                if (color_flag == 1)
                                    printf("black win");
                                else
                                    printf("white win");
                                goto game_end;
                            }
                        }
                        // not same color with (i,j)
                        else
                        {
                            count_temp = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
    game_end:
    printf("\nend");
}

