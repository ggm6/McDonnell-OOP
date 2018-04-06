#include <iostream>
#include "card.hpp"
#include <ctime>
#include <cmath>
#include <cstdlib>

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    srand(time(0));
    Card cards[4];
    for (int i=0; i<4; ++i)
    {
        int r={rand()%6};
        int s={rand()%4};
        Rank ran;
        Suit suit;
        if (r==0)
            ran=Nine;
        else if (r==1)
            ran=Ten;
        else if (r==2)
            ran=Ace;
        else if (r==3)
            ran=King;
        else if (r==4)
            ran=Queen;
        else if (r==5)
            ran=Jack;
        if (s==0)
            suit=Spades;
        if (s==1)
            suit=Clubs;
        if (s==2)
            suit=Diamonds;
        if (s==3)
            suit=Hearts;
        Card c {ran,suit};
        cards[i] = c;
    }

    return 0;
}
