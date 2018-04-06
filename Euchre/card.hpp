#ifndef CARD_HPP_INCLUDED
#define CARD_HPP_INCLUDED

#include <iostream>
#include <vector>

enum Rank {
Nine,
Ten,
Ace,
King,
Queen,
Jack,
J
};

enum Suit {
Spades,
Clubs,
Diamonds,
Hearts,
Joker
};

// Discriminated Union Based Card Class
class Card {
	public:
	enum {Regular,Joker} data_type;
	union
	{
		int regular;
		int joker;
	};
	Card(Rank r, Suit s) : rank(r),suit(s) {};
	
	private:
	Rank rank;
	Suit suit;
};

// Bitfield Based Card Class
class Card2
{
public:
Card2() = default;
Card2(Rank r, Suit s) : bits ((unsigned)s << 3 | unsigned(r)) {}
bool operator == (Card2 c) const {return bits==c.bits;}
bool operator != (Card2 c) const {return bits!=c.bits;}
void operator = (Card2 c) {bits=c.bits;}
bool operator < (Card2 c) const {return bits<c.bits;}
bool operator > (Card2 c) const {return bits >c.bits;}
~Card2() {bits=0;}
friend bool operator == (Card2 a, Card2 b);

private:
Rank rank:3;
Suit suit:3;
unsigned char bits:3;
};

bool operator == (Card2 a, Card2 b)
{
	return a.bits==b.bits;
}

// Inheritance Based Card Class
struct Rank2 {
	std::string rank2;
};

struct Suit2 {
	std::string suit2;
};

class Card3 : public Rank2, Suit2{
	public:
	Card3(Rank2 r, Suit2 s) : rank(r),suit(s) {};
	
	Rank2 rank;
	Suit2 suit;
};

// Deck Data Structure
struct Deck {
	std::vector<Card3> deck;
	friend std::ostream& operator << (std::ostream& obj, std::vector<Card3> deck);
};
	
std::ostream& operator << (std::ostream& obj, std::vector<Card3> deck)
{
	auto it=deck.begin();
	while (it!=deck.end())
	{
		obj << it->rank.rank2 << ' ' << it->suit.suit2 << std::endl;
		++it;
	}
    return obj;
}

#endif // CARD_HPP_INCLUDED
