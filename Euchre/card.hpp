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
Card() : rank(Ace),suit(Spades) {}
Card(Rank r, Suit s) : rank(r), suit(s) {}

private:
Rank rank;
Suit suit;
};

#endif // CARD_HPP_INCLUDED
