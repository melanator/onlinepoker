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
		break;
	case stage::River:
		st = stage::Final;
		break;
	case stage::Final:
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

Move Player::Trade(const int bank_size, const int to_bet) {
	std::string decision;
	int bet_this_round = to_bet - bet_this_hand;

	if (!in_play) {
		return { action::NoAction, 0 };
	}

	if (bet_this_round == 0 && action != action::NoAction) {
		// Player bet and actioned already
		std::cout << name << "already moved" << std::endl;
		return { action::NoAction, 0 };
	}
	else {
		std::cout << "Bank: " << bank_size << ". Bet: " << to_bet << ". To bet: " << bet_this_round << ". Money: " << money << " "
			<< name << ", your cards: " << ShowCards() << " Choose action : ";
		std::cin >> decision;
		Move move = ReadAction(decision);

		switch (action) {
		case action::Fold:
			in_play = false;
			std::cout << name << " folds!" << std::endl;
			return move;
		case action::Call:
			move.bet = bet_this_round;
			std::cout << name << " calls " << move.bet << "!" << std::endl;
			Pay(move.bet);
			return move;
		case action::Raise:
			move.bet -= bet_this_hand;
			Pay(move.bet);
			std::cout << name << "  raises to " << move.bet << "!" << std::endl;
			return move;
		default:
			return { action::NoAction, 0 };
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
	bet_this_hand += amount;
}

bool Player::FinishedRound(const int high_bet) {
	return !(high_bet - bet_this_hand) && (action != action::NoAction);
}

void Player::Reset() {
	action = action::NoAction;
	bet_this_hand = 0;
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

	deck = Deck{};			// New instance of deck with shuffled card
	players.shift();		// Assign positions 
	ActivatePlayers();		// Activate players
	stage = stage::Preflop;	// Refresh stage
	bank = 0;				// Null the bank
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
		if (stage == stage::Final)
			break;
		stage++;
	    DealOnTable();
		ShowTable(3);
		break;
	case stage::Flop:
		NewRound();
		Trade();
		if (stage == stage::Final)
			break;
		stage++;
		DealOnTable();
		ShowTable(4);
		break;
	case stage::Turn:
		NewRound();
		Trade();
		if (stage == stage::Final)
			break;
		stage++;
		DealOnTable();
		ShowTable(5);
		break;
	case stage::River:
		NewRound();
		Trade();
		if (stage == stage::Final)
			break;
		stage++;
		winner = FindWinner();
		break;
	case stage::Final:
		break;
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
	while (current_player < players.size || !IsEndOfRound(bet)) {
		if (players_ingame == 1){
			// The only player left -> End hand
			stage = stage::Final;
			SetWinner(players[current_player].val);
			return;
		}

		Move player_move = players[current_player].val->Trade(bank, bet);
		player_bet = player_move.bet;
		bank += player_bet;

		if (player_move.act == action::Fold)
			players_ingame--;

		if (players[current_player].val->GetBetThisHand() > bet)
			bet = players[current_player].val->GetBetThisHand();

		current_player++;
	}
}

Player* PlayHand::FindWinner() {
	return players[0].val;
}

void PlayHand::FinishHand() {
	std::cout << winner->name << " won the round" << std::endl;
	winner->AddMoney(bank);
	std::cout << "Bank: " << bank << " -> " << winner->name << "'s money: " << winner->GetMoney() << std::endl;
}

bool PlayHand::IsEndOfRound(const int high_bet) {
	if (players_ingame == 0)
		return true;

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

void PlayHand::SetWinner(Player* player) {
	winner = player;
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
	while (true) {
		playhand.NewHand();
		while (playhand.GetStage() != stage::Final)
			playhand.Round();
	playhand.FinishHand();
	}

	
	std::cout << player1->name << " " << player1->ShowCards() << std::endl
		<< player2->name << " " << player2->ShowCards() << std::endl
		<< player3->name << " " << player3->ShowCards() << std::endl
		<< player4->name << " " << player4->ShowCards() << std::endl;
	
}

