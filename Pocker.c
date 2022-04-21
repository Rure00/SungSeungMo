#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>


struct Card
{
    char* name;

    char* Symbol; // ♡, ◇, ♠, ♣
    int num; // 1 ~ 13
};

float startCash = 0;
float goal = 0;
float betMoney;
float reward;

struct Card Deck[52];
int DeckNum = 0;

struct Card playerCards[5];
struct Card computerCards[5];

int playerScore[2]; // [0] : Score by Combi
int computerScore[2]; // [1] : Highest Number

//함수 선언
void setStart();
void ResetHand();
void ResetHand();
void startRound();
void SetDeck();
void DistributeCard(struct Card Obj[], int num);
int checkCardNum(struct Card Obj[]);
void SortingDeck(int num);
void ShowCard(struct Card Obj[]);
void Bet(float ratio);
int* WhatCombi(struct Card Obj[]);
bool isStraight(struct Card Obj[], int *hnum);
bool isPair(struct Card Obj[], int num, int pairNum, int *hnum);
bool isFlush(struct Card Obj[], int *hnum);
void Descending(struct Card Obj[]);
void getResult();
void PrintingResult();
void EndGame(bool isWin);


void main()
{
    srand((int)time(NULL));
    for (int i = 0; i < 5; i++)
    {
        playerCards[i].name = "playerCards";
        computerCards[i].name = "computerCards";
    }
    /*
    playerCards[0].num = 6;
    playerCards[1].num = 3;
    playerCards[2].num = 2;
    playerCards[3].num = 3;
    playerCards[4].num = 2;

    playerCards[0].Symbol = "h";
    playerCards[1].Symbol = "d";
    playerCards[2].Symbol = "s";
    playerCards[3].Symbol = "h";
    playerCards[4].Symbol = "h";

    Descending(playerCards);

    printf("%d ", WhatCombi(playerCards)[0]);

    for (int i = 0; i < 5; i++)
    {
        playerCards[i].num;
    }
    */
    
    //Set initial money and goal.
    setStart();
    
    
    return;
}

void setStart()
{
    printf("시작금액을 입력해주세요. : ");
    scanf("%f", &startCash);

    printf("\n목표 금액을 입력해주세요. : ");
    scanf("%f", &goal);

    //if startCash > goal : Call SetValue() again.
    if (startCash >= goal)
    {
        printf("\n시작 금액이 목표금액보다 크거나 같습니다. 다시 입력해주세요.\n\n");
        setStart();
        return;
    }
    else if (startCash <= 0)
    {
        printf("시작금액이 잘못됐습니다.");
        setStart();
        return;
    }
    else
    {
        //게임 시작
        startRound();
        return;
    }
}

void ResetHand()
{
    for (int i = 0; i < 5; i++)
    {
        playerCards[i].num = -1;
        computerCards[i].num = -1;
    }
}

void startRound()
{
    system("cls");

    SetDeck();
    ResetHand();

    //Distribute 3 cards.
    DistributeCard(playerCards, 3);
    DistributeCard(computerCards, 3);

    //Bet. Bet ratio == 2.5
    ShowCard(playerCards);
    Bet(2.5);

    //Distribute cards  2 : 2
    DistributeCard(playerCards, 2);
    DistributeCard(computerCards, 2);

    //Bet. Bet ratio == 2
    
    ShowCard(playerCards);
    Bet(2);

    //result
    getResult();
    
}

void SetDeck()
{
    printf("SetDeck's DeckNumb : %d\n", DeckNum);

    for (int i = 0; i < 4; i++)
    {
        char * Sym = "";

        if (i == 0)
        {
            Sym = "♡";
        }
        else if (i == 1)
        {
            Sym = "◇";
        }
        else if (i == 2)
        {
            Sym = "♠";
        }
        else if (i == 3)
        {
            Sym = "♣";
        }

        for (int index = 0; index < 13; index++)
        {
            Deck[DeckNum].Symbol = Sym;
            Deck[DeckNum].num = index;

            DeckNum++;
        }
    }
}

void DistributeCard(struct Card Obj[], int num)
{
    for (int i = 0; i < num; i++)
    {
        int cardNum = checkCardNum(Obj);
        int randomindex = (rand() % DeckNum);

        char** pSymbol = &Obj[cardNum].Symbol;
        int* pNumb = &Obj[cardNum].num;

        *pNumb = Deck[randomindex].num;
        *pSymbol = Deck[randomindex].Symbol;

        SortingDeck(randomindex);
        DeckNum--;
    }
}

int checkCardNum(struct Card Obj[])
{
    int Num = 0;

    for (int i = 0; i < 5; i++)
    {
        if (Obj[i].num != -1)
        {
            Num++;
        }
    }

    return Num;
}

void SortingDeck(int num)
{
    struct Card* p1 = &Deck[num];
    Deck[num] = Deck[DeckNum - 1];

    Deck[DeckNum - 1].num = -1;
    Deck[DeckNum - 1].Symbol = "";
}

void ShowCard(struct Card Obj[])
{
    system("cls");

    printf("----------------------------------------------------------------------------------------------------------------------\n");
    printf("목표 금액 : %f     현재 보유 금액 : %f        배팅금액 : %f       얻을금액 : %f \n", goal, startCash, betMoney, reward);
    printf("----------------------------------------------------------------------------------------------------------------------\n");

    printf("My Cards : \n");

    for (int i = 0; i < checkCardNum(Obj); i++)
    {
        printf("%s ", playerCards[i].Symbol);

        if (playerCards[i].num == 10)
        {
            printf("J\n");
        }
        else if (playerCards[i].num == 11)
        {
            printf("Q\n");
        }
        else if (playerCards[i].num == 12)
        {
            printf("K\n");
        }
        else
        {
            printf("%d\n", (playerCards[i].num + 1));;
        }
    }
}

void Bet(float ratio)
{
    if (startCash <= 0)
    {
        return;
    }

    int bet;
    printf("배팅하세요. : ");
    scanf("%d", &bet);

    if (bet > startCash)
    {
        printf("가지고 있는 돈보다 많이 배팅할 수 없습니다. 다시 배팅해주세요.\n");
        Bet(ratio);
        return;
    }
    else
    {
        startCash -= bet;
        betMoney += bet;
        reward += ratio * bet;
    }
}

int* WhatCombi(struct Card Obj[])
{
    int score = 0;
    int* headNum = (int*)malloc(sizeof(int));
    int* result = (int*)malloc(sizeof(int) * 2);
    result[0] = 0;
    result[1] = -1;
    *headNum = -1;
        

    if (isFlush(Obj, headNum) && isStraight(Obj, headNum) && *headNum == 0)
    {
        //Royal Straight Flush
        score = 9;
    }
    else if (isFlush(Obj, headNum) && isStraight(Obj, headNum))
    {
        //Straight Flush
        score = 8;
    }
    else if (isPair(Obj, 4, 1, headNum))
    {
        // Four Card
        score = 7;
    }
    else if (isPair(Obj, 3, 1, headNum) && isPair(Obj, 2, 1, headNum))
    {
        // Full House
        score = 6;
    }
    else if (isFlush(Obj, headNum))
    {
        //Flush
        score = 5;
    }
    else if (isStraight(Obj, headNum))
    {
        //Straight
        score = 4;
    }
    else if (isPair(Obj, 3, 1, headNum))
    {
        //Triple
        score = 3;
    }
    else if (isPair(Obj, 2, 2, headNum))
    {
        //Two Pair
        score = 2;
    }
    else if (isPair(Obj, 2, 1, headNum))
    {
        //One Pair
        score = 1;
    }
    else
    {
        score = 0;
    }

    result[0] = score;
    result[1] = *headNum;

    return result;
}

bool isStraight(struct Card Obj[], int* hnum)    //5개의 카드의 숫자가 이어지는가
{
    bool isTrue = true;

    for (int i = 0; i < 4; i++)
    {
        if (Obj[i].num != Obj[i + 1].num + 1)
        {
            isTrue = false;
            break;
        }
    }

    if(isTrue)
        *hnum = Obj[0].num;

    if (Obj[4].num == 0 && Obj[0].num == 12 && Obj[1].num == 11 && Obj[2].num == 10 && Obj[3].num == 9)
    {
        isTrue = true;
        *hnum = 0;
    }

    return isTrue;
}

bool isPair(struct Card Obj[], int num, int pairNum, int *hnum)
{
    //Obj : playerCard or computerCard
    //num : 몇 장의 카드가 한 쌍을 이루는가 ex) Triple일 경우 num = 3, One(Two)pair일 경우 num = 2.
    //pairNum : 페어인 카드가 몇 쌍인가 ex) Onepair일 경우 pairNum = 1, Twopair일 경우 pairNum = 2.
    bool isTrue = true;

    if (pairNum == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = i + 1; j < i + num; j++)
            {
                if (Obj[i].num != Obj[j].num)
                {
                    isTrue = false;
                }
                else
                {
                    isTrue = true;

                    if (j == (i + num - 1))
                    {
                        break;
                    }   
                }
            }

            if ((i + num) < 5)
            {
                if (Obj[i].num == Obj[i + num].num)
                {
                    isTrue = false;
                    break;
                }
            }

            if (isTrue)
            {
                *hnum = Obj[i].num;
                break;
            }
        }
    }
    else //pairNum == 2
    {
        int firstNumber = -1;

        for (int i = 0; i < 4; i++)
        {
            for (int j = i + 1; j < i + num; j++)
            {
                if (Obj[i].num != Obj[j].num)
                {
                    isTrue = false;
                }
                else
                {
                    isTrue = true;

                    if (j == (i + num - 1))
                    {
                        break;
                    }

                }
            }

            if ((i + num) < 5)
            {
                if (Obj[i].num == Obj[i + num].num)
                {
                    isTrue = false;
                }
            }

            if (isTrue)
            {
                *hnum = Obj[i].num;
                firstNumber = Obj[i].num;
                break;
            }
        }

        if (firstNumber == -1)
        {
            isTrue = false;
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = i + 1; j < i + num; j++)
                {
                    if (Obj[i].num != Obj[j].num)
                    {
                        isTrue = false;
                    }
                    else if (Obj[i].num == Obj[j].num && Obj[i].num != firstNumber)
                    {
                        isTrue = true;

                        if (j == (i + num - 1))
                        {
                            break;
                        }
                    }
                    else
                        isTrue = false;
                }

                if ((i + num) < 5)
                {
                    if (Obj[i].num == Obj[i + num].num)
                    {
                        isTrue = false;
                    }
                }

                if (isTrue)
                {
                    break;
                }
            }
        }
    }

    return isTrue;
}

bool isFlush(struct Card Obj[], int *hnum)   //같은 모양 5개
{
    bool isTrue = true;

    for (int i = 0; i < 4; i++)
    {
        if (Obj[i].Symbol != Obj[i + 1].Symbol)
        {
            isTrue = false;
            break;
        }
    }

    if(isTrue)  
        *hnum = Obj[0].num;

    return isTrue;
}

void Descending(struct Card Obj[])
{
    for (int i = 0; i < 5; i++)
    {
        struct Card* piCard = &Obj[i];

        for (int index = i; index < 5; index++)
        {
            struct Card* pindexCard = &Obj[index];

            if ((*piCard).num < (*pindexCard).num)
            {
                struct Card alter = *piCard;
                *piCard = Obj[index];
                *pindexCard = alter;
            }
        }
    }
    /*
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", Obj[i].num);
    }

    printf("\n");
    */
}

void getResult()
{
    int isWin = 0; // (-1 : 짐), (0 : 무승부), (1 : 이김)

    Descending(playerCards);
    Descending(computerCards);

    playerScore[0] = WhatCombi(playerCards)[0];
    playerScore[1] = WhatCombi(playerCards)[1];

    computerScore[0] = WhatCombi(computerCards)[0];
    computerScore[1] = WhatCombi(computerCards)[1];
    
    if (playerScore[0] > computerScore[0])
        isWin = 1;
    else if (playerScore[0] == computerScore[0])
    {
        if (playerScore[1] > computerScore[1] && computerScore[1] != 0)
            isWin = 1;
        else if (playerScore[1] < computerScore[1] && playerScore[1] != 0)
            isWin = -1;
        else if (playerScore[1] == computerScore[1])
            isWin = 0;
        else if (playerScore[1] == 0)
            isWin = 1;
        else if (computerScore[1] == 0)
            isWin = -1;
    }
    else
        isWin = -1;

    PrintingResult();

    if (isWin == 1)
    {
        startCash += reward;
        printf("\n\n이겼습니다!\n\n");
        
    }
    else if(isWin == -1)
    {
        printf("\n\n졌습니다...\n\n");
    }
    else
    {
        startCash += betMoney;
        printf("\n\n무승부입니다.\n\n");
    } 

    if (startCash <= 0)
    {
        EndGame(false);
        return;
    }

    reward = 0;
    betMoney = 0;


    if (startCash > goal)
    {
        EndGame(true);
    }
    else
    {
        system("pause");
        startRound();
    }
}

void PrintingResult()
{
    printf("\n");
    printf("Computer's Card : \n");
    for (int i = 0; i < 5; i++)
    {
        printf("%s ", computerCards[i].Symbol);

        if (computerCards[i].num == 10)
        {
            printf("J\n");
        }
        else if (computerCards[i].num == 11)
        {
            printf("Q\n");
        }
        else if (computerCards[i].num == 12)
        {
            printf("K\n");
        }
        else
        {
            printf("%d\n", (computerCards[i].num + 1));;
        }
    }

    printf("\n");

    switch (playerScore[0])
    {
    case 9:
        printf("Player : Royal Straight Flush");
        break;
    case 8:
        printf("Player : Straight Flush");
        break;
    case 7:
        printf("Player : Four Card");
        break;
    case 6:
        printf("Player : Full House");
        break;
    case 5:
        printf("Player : Flush");
        break;
    case 4:
        printf("Player : Straight");
        break;
    case 3:
        printf("Player : Triple");
        break;
    case 2:
        printf("Player : Two Pair");
        break;
    case 1:
        printf("Player : One Pair");
        break;
    case 0:
        printf("Player : No Combi..");
        break;
    }
    switch (computerScore[0])
    {
    case 9:
        printf("        Computer : Royal Straight Flush");
        break;
    case 8:
        printf("        Computer : Straight Flush");
        break;
    case 7:
        printf("        Computer : Four Card");
        break;
    case 6:
        printf("        Computer : Full House");
        break;
    case 5:
        printf("        Computer : Flush");
        break;
    case 4:
        printf("        Computer : Straight");
        break;
    case 3:
        printf("        Computer : Triple");
        break;
    case 2:
        printf("        Computer : Two Pair");
        break;
    case 1:
        printf("        Computer : One Pair");
        break;
    case 0:
        printf("        Computer : No Combi..");
        break;
    }

    

    if (playerScore[0] == computerScore[0] && playerScore[1] != -1)
    {
        printf("\n\nHighset Number \n");

        char alter = ' ';
        if (playerScore[1] == 12)
            alter = 'K';
        else if (playerScore[1] == 11)
            alter = 'Q';
        else if (playerScore[1] == 10)
            alter = 'J';
        else
            alter = playerScore[1] + '0';
        printf("Player : %c     ", alter);

        if (computerScore[1] == 12)
            alter = 'K';
        else if (computerScore[1] == 11)
            alter = 'Q';
        else if (computerScore[1] == 10)
            alter = 'J';
        else
            alter = computerScore[1] + '0';
        printf("Computer : %c     \n", alter);
    
    }
}

void EndGame(bool isWin)
{
    printf("\n\n");
    if (isWin)
    {
        printf("--------------!!게임 승리!!--------------");
    }
    else
    {
        printf("--------------;;게임 패배;;--------------");
    }
    
}
