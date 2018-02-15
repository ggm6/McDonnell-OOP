#ifndef CARD_HPP_INCLUDED
#define CARD_HPP_INCLUDED

enum Rank {
Nine,
Ten,
Ace,
King,
Queen,
Jack
};

enum Suit {
Spades,
Clubs,
Diamonds,
Hearts
};

class Card
{
public:
Card() = default;
Card(Rank r, Suit s) : bits ((unsigned)s << 3 | unsigned(r)) {}
bool operator == (Card c) const {return bits==c.bits;}
bool operator != (Card c) const {return bits!=c.bits;}
void operator = (Card c) {bits=c.bits;}
bool operator < (Card c) const {return bits<c.bits;}
bool operator > (Card c) const {return bits >c.bits;}
~Card() {bits=0;}
friend bool operator == (Card a, Card b);

private:
Rank rank:3;
Suit suit:3;
unsigned char bits:3;
};


bool operator == (Card a, Card b)
{
	return a.bits==b.bits;
}
#endif // CARD_HPP_INCLUDED
