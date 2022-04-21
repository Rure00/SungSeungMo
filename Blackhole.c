#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <stdbool.h>

typedef struct card
{
    int number;
    char* Suit;
} Card;
void PrintDeck(Card* deck);
void PrintCard(Card card);
void PrintDummy(int dummy[17][3], Card* deck, Card blackHole);

void MakeDeck(Card* deck);
void DivideDummy(int dummy[17][3]);
void SelectCard(Card* deck, int dummy[17][3], Card* blackHole);
bool CheckRelay(Card card, Card blackHole);
bool IsEnd(Card blackhole, int dummy[17][3], Card* deck);


int main()
{
    Card Deck[52];
    int Dummy[17][3];
    Card BlackHole;

    srand(time(NULL));

    MakeDeck(Deck);
    BlackHole = Deck[0];

    DivideDummy(Dummy);

    while (!IsEnd(BlackHole, Dummy, Deck))
    {
        PrintDummy(Dummy, Deck, BlackHole);

        SelectCard(Deck, Dummy, &BlackHole);
    }
}

void MakeDeck(Card* deck)
{
    for (int i = 0; i < 4; i++)
    {
        char* suit = " ";

        switch (i)
        {
        case 0:
            suit = "��";
            break;
        case 1:
            suit = "��";
            break;
        case 2:
            suit = "��";
            break;
        case 3:
            suit = "��";
            break;
        }

        for (int j = 0; j < 13; j++)
        {
            deck[i * 13 + j].number = j;
            deck[i * 13 + j].Suit = suit;
        }
    }
}

void DivideDummy(int dummy[17][3])
{
    int usedNums[52] = { 0, };
    int index = 1;      //Dummy[0][0] : Spade Ace

    while (index < 52)
    {
        int random = rand() % 51 + 1;
        int isDuplicated = 0;       // 0 : false, 1 : true

        for (int i = 0; i < index; i++)
        {
            if (random == usedNums[i])
            {
                isDuplicated = 1;
                break;
            }
        }

        if (isDuplicated == 0)
        {
            int firstIndex = (index - 1) / 3;
            int secondIndex = (index - 1) % 3;

            dummy[firstIndex][secondIndex] = random;
            usedNums[index] = random;
            index++;
        }
    }
}

void SelectCard(Card* deck, int dummy[17][3], Card* blackHole)
{
    int firstIndex = 0, secondIndex = 0;
    printf("\n���ڸ� ������� �Է��Ͽ� ī�带 �����ϼ���. : ");
    scanf("%d %d", &firstIndex, &secondIndex);

    firstIndex--;       // UI���� ���� ����
    secondIndex--;      // +1�� �Ͽ� ǥ���Ͽ����Ƿ� -1�� ����� �Ѵ�.
    if (firstIndex > 17 || firstIndex < 0 || secondIndex > 3 || secondIndex < 0)
    {
        printf("\n�߸��� ���ڸ� �Է��߽��ϴ�. �ٽ� �Է����ּ���.\n");

        SelectCard(deck, dummy, blackHole);
        return;
    }


    if (dummy[firstIndex][secondIndex] == 0)
    {
        printf("\n�߸��� ���ڸ� �Է��߽��ϴ�. �ٽ� �Է����ּ���.\n");

        SelectCard(deck, dummy, blackHole);
        return;
    }
    for (int i = 0; i < secondIndex; i++)
    {
        if (dummy[firstIndex][i] != 0 && i < secondIndex)
        {
            printf("\n�߸��� ���ڸ� �Է��߽��ϴ�. �ٽ� �Է����ּ���.\n");

            SelectCard(deck, dummy, blackHole);
            return;
        }
    }

    int cardIndex = dummy[firstIndex][secondIndex];
    Card selectedCard = deck[cardIndex];

    if (!CheckRelay(selectedCard, *blackHole))
    {
        printf("\n������ ī��� ��Ȧ�� ���� �� �����ϴ�. �ٽ� �Է����ּ���.\n");

        SelectCard(deck, dummy, blackHole);
        return;
    }

    //Allocate
    (*blackHole) = selectedCard;
    dummy[firstIndex][secondIndex] = 0;
}
bool CheckRelay(Card card, Card blackHole)
{
    bool isStraight = false;

    if (abs(card.number - blackHole.number) == 1)
    {
        isStraight = true;
    }

    if (card.number == 0 && blackHole.number == 12)
    {
        isStraight = true;
    }
    else if (blackHole.number == 0 && card.number == 12)
    {
        isStraight = true;
    }

    return isStraight;
}

bool IsEnd(Card blackhole, int dummy[17][3], Card* deck)
{
    bool isEnd = true;
    bool isWin = true;

    //��� ī�带 ��Ȧ�� ��ġ�� ���
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (dummy[i][j] != 0)
            {
                isEnd = false;
            }
        }
    }
    if (isEnd == true)
    {
        isWin = true;

        printf("\n\n ���ӿ��� �¸��߽��ϴ�!!!!\n\n");

        return isEnd;
    }

    isEnd = true;


    //��Ȧ�� ��ġ�� ī�尡 ���� ���
    for (int i = 0; i < 17; i++)
    {
        int j = 0;
        while (j < 3)
        {
            if (dummy[i][j] != 0)
            {
                int cardIndex = dummy[i][j];
                Card topCard = deck[cardIndex];

                if (CheckRelay(topCard, blackhole) == true)
                {
                    isEnd = false;
                }

                break;
            }

            j++;
        }
    }
    if (isEnd == true)
    {
        isWin = false;

        printf("\n\n��ġ�� �� ���� ī�尡 �����Ƿ� ���ӿ��� �й��߽��ϴ�.......\n");

        return isEnd;
    }



    return isEnd;
}


#pragma region Print_Function

void PrintDeck(Card* deck)
{
    for (int i = 0; i < 52; i++)
    {
        PrintCard(deck[i]);

        if ((i + 1) % 13 == 0)
            printf("\n\n");
    }
}
void PrintCard(Card card)
{
    int number = card.number + 1;
    char num_print[3];

    switch (number)
    {
    case(1):
        strcpy(num_print, "A");
        break;
    case(11):
        strcpy(num_print, "J");
        break;
    case(12):
        strcpy(num_print, "Q");
        break;
    case(13):
        strcpy(num_print, "K");
        break;
    default:
        sprintf(num_print, "%d", number);
        break;
    }

    printf("%s%s\t", card.Suit, num_print);
}
void PrintDummy(int dummy[17][3], Card* deck, Card blackHole)
{
    system("cls");
    printf("-----------------------------------------------------------------\n");
    printf("BlackHole Card : ");
    PrintCard(blackHole);
    printf("\n");

    printf("Dummy)\n");

    for (int i = 0; i < 17; i++)
    {
        printf("%d.\t", i + 1);

        for (int j = 0; j < 3; j++)
        {
            if (dummy[i][j] == 0)
            {
                printf(" -\t");
            }
            else
            {
                //printf("%d\t", dummy[i][j]);
                int deckIndex = dummy[i][j];
                PrintCard(deck[deckIndex]);
            }
        }

        printf("\n");
    }

    printf("-----------------------------------------------------------------\n");



}

#pragma endregion

