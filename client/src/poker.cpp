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

stage operator++(stage& st, int) {
	switch (st) {
	case stage::Preflop:
		st = stage::Flop;
		break;
	case stage::Flop:
		st = stage::Turn;
		break;
	case stage::Turn:
		st = stage::River;
	case stage::River:
		break;
	}
	return st;
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

int Player::Trade(const int bank_size, const int bet) {
	std::string decision;
	int bet_this_round = bet - bet_round;

	if (!in_play) {
		return 0;
	}

	if (bet_this_round == 0 && action != action::NoAction) {
		// Player bet and actioned already
		std::cout << name << "already moved" << std::endl;
		return 0;
	}
	else {
		std::cout << "Bank: " << bank_size << ". Bet: " << bet << ". To bet:" << bet-bet_round << ". "
			<< name << ", your cards: " << ShowCards() << " Choose action : ";
		std::cin >> decision;
		Move move = ReadAction(decision);

		switch (action) {
		case action::Fold:
			in_play = false;
			std::cout << name << " folds!" << std::endl;
			return 0;
		case action::Call:
			move.bet = bet_this_round;
			std::cout << name << " calls " << move.bet << "!" << std::endl;
			Pay(move.bet);
			return move.bet;
		case action::Raise:
			Pay(move.bet);
			std::cout << name << " raises " << move.bet << "!" << std::endl;
			return move.bet;
		}
	}

}

Move Player::ReadAction(std::string& decision) {
START:
	if (decision[0] == 'F') {
		action = action::Fold;
		return { action, 0 };
	}
	else if(decision[0] == 'C') {
		action = action::Call;
		return { action, 0 };
	}
	else if(decision[0] == 'R') {
		action = action::Raise;
		int raise;
		std::cout << "Type amount to bet: ";
		std::cin >> raise;
		return { action, raise };
	}
	else {
		std::cout << "Wrong command. Please repeat";
		std::cin >> decision;
		goto START;
	}
}


void Player::Pay(const int amount) {
	money -= amount;
	bet_round += amount;
}

bool Player::FinishedRound(const int high_bet) {
	return !(high_bet - bet_round) && (action != action::NoAction);
}

void Player::Reset() {
	action = action::NoAction;
	bet_round = 0;
}

PlayHand::PlayHand(){}

void PlayHand::AddPlayer(Player* player) {
	players.push_back(player);
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
	for (int i = 0; i < players.size; i++) {
		players[i].val->GetCards(deck.Deal(), deck.Deal());
	}
}

void PlayHand::NewHand(const int new_blind) {
	// Algorithm for each new hand
	
	if (new_blind) {
		blind_size = new_blind;
	}

	deck = Deck{};		// New instance of deck with shuffled card
	players.shift();	// Assign positions 
	ActivatePlayers();	// Activate players
	// Take blinds
}

void PlayHand::ActivatePlayers() {
	players_ingame = 0;
	for (int i = 0; i < players.size; i++) {
		if (players[i].val->GetMoney() > blind_size) {
			players[i].val->SetStatus(true);
			players_ingame++;
			players[i].val->Reset();
		}
		else {
			std::cout << players[i].val->name << " has no money and left the game" << std::endl;
			players.pop(i);		// Delete player if no money
		}
	}
}

void PlayHand::Round() {
	switch (stage) {
	case stage::Preflop:
		DealToPlayers();
		Trade();
		stage++;
	    DealOnTable();
		ShowTable(3);
		break;
	case stage::Flop:
		NewRound();
		Trade();
		stage++;
		DealOnTable();
		ShowTable(4);
		break;
	case stage::Turn:
		NewRound();
		Trade();
		stage++;
		DealOnTable();
		ShowTable(5);
		break;
	case stage::River:
		NewRound();
		Trade();
		break;
	}
}

void PlayHand::TradePreflop() {
	BetFromPlayer(players[0].val, blind_size);			// Small blind
	BetFromPlayer(players[1].val, blind_size * 2);		// Big blind
	int bet = blind_size * 2;
	int high_bet = bet;
	int current_player = 2;
	int player_bet = 0;
	while (true) {
		player_bet = players[current_player].val->Trade(bank, high_bet);
		bank += player_bet;
		if (player_bet > high_bet)
			high_bet = player_bet;

		if (current_player++ > players.size && IsEndOfRound(high_bet)) {
			break;
		}
	}
}


void PlayHand::Trade() {
	int bet = 0;
	int current_player = 0;
	int player_bet = 0;

	if (stage == stage::Preflop) {
		BetFromPlayer(players[0].val, blind_size);			// Small blind
		BetFromPlayer(players[1].val, blind_size * 2);		// Big blind
		bet = blind_size * 2;
		current_player = 2;
	}
	while (true) {
		player_bet = players[current_player].val->Trade(bank, bet);
		bank += player_bet;
		if (player_bet > bet)
			bet = player_bet;

		if (++current_player > players.size && IsEndOfRound(bet)) {
			break;
		}
	}
}

bool PlayHand::IsEndOfRound(const int high_bet) {
	bool is_end = true;
	for (int i = 0; i < players.size; i++) {
		// (0 to bet and Actioned) OR not in play
		is_end = is_end && (players[i].val->FinishedRound(high_bet) || !players[i].val->IsInPlay());
	}
	return is_end;
}

void PlayHand::ShowTable(const int cards) {
	for (int i = 0; i < cards; i++) {
		std::cout << dealt_cards[i] << " ";
	}
	for (int i = cards; i < 5; i++) {
		std::cout << "XX" << " ";
	}
	std::cout << std::endl;
}

void PlayHand::BetFromPlayer(Player* player, const int amount) {
	player->Pay(amount);
	bank += amount;
	std::cout << player->name << " bet " << amount << "." << std::endl;
}

void PlayHand::NewRound() {
	for (int i = 0; i < players.size; i++) {
		players[i].val->Reset();
	}
}

int main() {
	Player* player1 = new Player("Ivan", 1000);
	Player* player2 = new Player("Petr", 1000);
	Player* player3 = new Player("Sergey", 1000);
	Player* player4 = new Player("Dmitriy", 1000);
	

	PlayHand playhand(5);
	playhand.AddPlayer(player1);
	playhand.AddPlayer(player2);
	playhand.AddPlayer(player3);
	playhand.AddPlayer(player4);
	playhand.NewHand();
	playhand.Round();
	playhand.Round();
	playhand.Round();
	playhand.Round();
	
	
	std::cout << player1->name << " " << player1->ShowCards() << std::endl
		<< player2->name << " " << player2->ShowCards() << std::endl
		<< player3->name << " " << player3->ShowCards() << std::endl
		<< player4->name << " " << player4->ShowCards() << std::endl;
	
}

