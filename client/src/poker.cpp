#include "../include/poker.h"

using namespace Poker;

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

void Deck::Shuffle()
{
	std::random_device rd;
	auto rng = std::default_random_engine{rd()};
	std::shuffle(std::begin(deck_), std::end(deck_), rng);
}

void Player::GetCards(const Card& lcard, const Card& rcard) {
	hand[0] = lcard;
	hand[1] = rcard;
}

std::string Player::ShowCards() {
	std::stringstream ss;
	ss << hand[0] << " " << hand[1];
	return ss.str();
}

void Player::Trade() {
	std::string decision;
	std::cout << "Choose action: ";
	std::cin >> decision;
	std::cout << std::endl;
	ReadAction(decision);
}

action Player::ReadAction(const std::string& decision) {
	if (decision[0] == 'F') {
		in_play == false;
		action == action::Fold;
	}
	else if(decision[0] == 'C') {
		action == action::Call;
	}
	else if(decision[0] == 'R') {
		action == action::Raise;
	}
	return action;
}

PlayHand::PlayHand(){}

void PlayHand::AddPlayer(Player& player) {
	players.push_back(&player);
}

void PlayHand::DealOnTable() {
	if (stage == stage::Flop) {
		dealt_cards[0] = deck.Deal();
		dealt_cards[1] = deck.Deal();
		dealt_cards[2] = deck.Deal();
	}
	else if (stage == stage::Turn)
		dealt_cards[3] = deck.Deal();
	else if (stage == stage::River)
		dealt_cards[4] = deck.Deal();
	else
		std::cerr << "Wrong stage!";
}

void PlayHand::DealToPlayers() {
	for (auto player : players) {
		player->GetCards(deck.Deal(), deck.Deal());
	}
}

void PlayHand::TradeRound() {
	for (auto player : players) {
		player->Trade();
	}
}

int main() {
	Player player1("Ivan", 1000);
	Player player2("Petr", 1000);

	PlayHand playhand(5);
	playhand.AddPlayer(player1);
	playhand.AddPlayer(player2);
	playhand.DealToPlayers();
	playhand.DealOnTable();
	
	std::cout << player1.name << " " << player1.ShowCards() << std::endl 
		      << player2.name << " " << player2.ShowCards() << std::endl;
	

}



