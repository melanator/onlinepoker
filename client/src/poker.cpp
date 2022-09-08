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


int main() {
	Deck deck;
	const Card& next = deck.Deal();
}

