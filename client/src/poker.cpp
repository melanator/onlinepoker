#include "../include/poker.h"

using namespace Poker;

Deck::Deck()
{
	Shuffle();
}

const Card& Deck::Deal()
{	
	return deck_[current_card++];
}

void Deck::Shuffle()
{
	std::random_device rd;
	auto rng = std::default_random_engine{rd()};
	std::shuffle(std::begin(deck_), std::end(deck_), rng);
}

void Deck::Burn(){
	current_card++;
}

std::ostream& operator<<(std::ostream& os, const Card& card){
	switch (card.value){
	case value::Jack:
		os << "J";
		break;
	case value::Queen:
		os << "Q";
		break;
	case value::King:
		os << "K";
		break;
	case value::Ace:
		os << "A";
		break;
	default:
		os << static_cast<int>(card.value);
		break;
	}
	switch (card.suit){
	case suit::Spades:
		os << "s";
		break;
	case suit::Clubs:
		os << "c";
		break;
	case suit::Diamonds:
		os << "d";
		break;
	case suit::Hearts:
		os << "h";
		break;
	}
	return os;
}

int main() {
	Deck deck;
	for (int i = 0; i < 5; i++){
		std::cout << deck.Deal() << " ";
	}
	return 0;
}

