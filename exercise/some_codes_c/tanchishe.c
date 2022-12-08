#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>
#include<math.h>


// the scale of the game map is N*N
#define N 20

// the map of the game
int map[N][N];
// the direction of the snake 
int command;
// the length of snake
int len = 3;

/*initial the map with the position of food, the length of snake, 
note that the default length of snake is 3 */ 
void initial_foodsnake()
{
    // initial the position of food
    map[5][5] = -1;
    // initial the position of the snake
    map[1][1] = 3;
    map[1][2] = 2;
    map[1][3] = 1;
    // initial the direction of the snake
    command = 3;
}

// get the command from the keyboard
int kbcommand()
{
    int temp = -1;
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w': temp = 1; break;
        case 'a': temp = 0; break;
        case 'd': temp = 2; break;
        case 's': temp = 3; break;
        }
        // snake cannot change to reverse direction
        if (temp != -1 && abs(command - temp) == 2)
            command = temp;
    }
    return command;
}

// return 0 = alive, 1 = dead
int move()
{
    int i,j,i_head,j_head;
    for (i=0;i<N;i++)
    {
        for (j=0;j<N;j++)
        {
            // end of the snake 
            if (map[i][j] == len)
                map[i][j] = 0;
            // interbody of the snake
            else if(map[i][j] > 0)
                map[i][j]++;
            // head of the snake
            else if(map[i][j] == 1)
            {
                map[i][j]++;
                // review the head of snake
                switch (command)
                {
                    // 0 = a, left direction
                    case 0: i_head = i - 1,j_head = j; break;
                    case 1: i_head = i,j_head = j - 1; break;
                    case 2: i_head = i + 1,j_head = j; break;
                    case 3: i_head = i,j_head = j + 1; break;
                }
            }
        }
    }
    // if the head of snake encounter food
    if (map[i_head][j_head] == -1)
    {
        len++;
        // generate new food 
        while(1)
        {
            i = rand() % N;
            j = rand() % N;
            if (map[i][j] == 0)
            {
                map[i][j] = -1;
                break;
            }
        }
    }
    // dead condition
    else if (map[i_head][j_head] > 0 || i_head < 0 || j_head < 0||i_head == N||j_head == N)
        return 1;
    // alive 
    else 
        map[i_head][j_head] = 1;
    return 0;
}

void draw()
{
    int i,j;
    system("cls");
    // the first row of screen
    for (i=0;i<N;i++)
    {
        switch (i)
        {
        case 0: printf("┏");
        case N: printf("┓");        
        default: printf(" ━");
        }
    }
    printf("\n");
    // 
    for (i=0;i<N;i++)
    {
        switch (i)
        {
        case 0: printf("┗");
        case N: printf("┛");        
        default: printf(" ━");
        }
    }
    // 
    for (i=0;i<N;i++)
    {
        for (j=0;j<N;j++)
        {
            if (j == 0)
                printf("┃ ");
            if (j == N-1)
                printf("┃ ");
            if (map[i][j] == 1)
                printf("○");
            else if(map[i][j] == -1)
                printf("★");
            else if(map[i][j] > 1)
                printf("●");
        }
        printf("\n");
    }
}

int main()
{
    int res = 0;
    initial_foodsnake();
    while (1)
    {
        command = kbcommand();
        res = move();
        if (res){
            printf("dead");
            break;
        }
        else
        {
            draw();
        }
        Sleep(1000);
    }
}