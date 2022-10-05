#include "../include/poker.h"

using namespace Poker;

std::ostream& operator<<(std::ostream& os, const value& val){
	switch (val){
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
		os << static_cast<int>(val);
		break;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const suit& s){	
	switch (s){
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

std::ostream& operator<<(std::ostream& os, const Card& card){
	os << card.value << card.suit;
	return os;
}

std::ostream& operator<<(std::ostream& os, const rank& r){
	switch (r){
	case rank::HighCard:
		os << "High card";
		break;
	case rank::Pair:
		os << "Pair";
		break;
	case rank::TwoPairs:
		os << "Two Pairs";
		break;
	case rank::ThreeOfAKind:
		os << "Three of a Kind";
		break;
	case rank::Straight:
		os << "Straight";
		break;
	case rank::Flush:
		os << "Flush";
		break;
	case rank::FullHouse:
		os << "Full House";
		break;
	case rank::FourOfAKind:
		os << "Four of a Kind";
		break;
	case rank::StraightFlush:
		os << "Straight Flush";
		break;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Combination& combination){
	os << combination.combo_rank << " of " << combination.combo_val;
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
	std::default_random_engine rng;
	if (is_seeded)
		rng = std::default_random_engine{1};		// 1 - seed
	else
		rng = std::default_random_engine{rd()};
	std::shuffle(std::begin(deck_), std::end(deck_), rng);
}

void Player::GetCards(const Card& lcard, const Card& rcard) {
	hand.Deal(lcard);
	hand.Deal(rcard);
}

std::string Player::ShowCards() {
	std::stringstream ss;
	ss << hand[0] << " " << hand[1];
	return ss.str();
}

Move Player::Trade(const int bank_size, const int to_bet, std::istream& input) {
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
		input >> decision;
		Move move = ReadAction(decision, input);

		switch (action) {
		case action::Fold:
			in_play = false;
			std::cout << name << " folds!" << std::endl;
			return move;
		case action::Call:
			move.bet = money > bet_this_round ? bet_this_round : money;
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

Move Player::ReadAction(std::string& decision, std::istream& input) {
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
		input >> raise;
		if (raise > money) {
			while (raise > money) {
				std::cout << "You dont have this much money. Please change bet size: ";
				input >> raise;
			}
		}
		return { action, raise };
	}
	else {
		std::cout << "Wrong command. Please repeat";
		input >> decision;
		goto START;
	}
}

void Player::Deal(const Card& card){
	hand.Deal(card);
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
	combo.combo_rank = rank::HighCard;
	combo.combo_val = value::Two;
	combo.kicker[0] = value::Two;
}

PlayHand& PlayHand::AddPlayer(Player* player) {
	players.push_back(player);
	return *this;
}

void PlayHand::DealOnTable() {
	if (stage == stage::Flop) {
		dealt_cards.Deal(deck.Deal());
		dealt_cards.Deal(deck.Deal());
		dealt_cards.Deal(deck.Deal());
	}
	else if (stage == stage::Turn)
		dealt_cards.Deal(deck.Deal());
	else if (stage == stage::River)
		dealt_cards.Deal(deck.Deal());
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

	deck.Shuffle();			// New instance of deck with shuffled card
	players.shift();		// Assign positions 
	ActivatePlayers();		// Activate players
	stage = stage::Preflop;	// Refresh stage
	bank = 0;				// Null the bank
	dealt_cards.Reset();	// Reset dealtcards
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
			// Iterate overplayer to find one that is InGame()
			for (int i = 0; i < players.size; i++) {
				if (players[i].val->IsInPlay())
					SetWinner(players[i].val);
			}
			return;
		}

		Move player_move = players[current_player].val->Trade(bank, bet, input);
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
	Combination winning_combo;
	Player* winner = nullptr;
	for(int i = 0; i < players.size; i++){
		if(players[i].val->IsInPlay()){
			players[i].val->combo = Evaluate(dealt_cards + players[i].val->hand);
			winner = (winning_combo < players[i].val->combo) ? players[i].val : winner;
			winning_combo = (winning_combo < players[i].val->combo) ? players[i].val->combo : winning_combo;
			std::cout << players[i].val->name << " has " << players[i].val->combo << std::endl;
		}
	}

	return winner;
}

Combination Poker::Evaluate(const DealtCards& dealt){
	int used_cards = 0;
	int rank = 0;
	Combination result;
	Combination highest_comb;

	std::vector<Card> all_cards(dealt.cards);
	std::sort(std::begin(all_cards), std::end(all_cards));

	if(all_cards.size() < 5)
		return result;									// Not enough cards

	std::map<value, int> hash_value; 	  				// Map to count values
	std::unordered_map<suit, int> hash_suit;			// Map to count suits
	for(int i = 0; i < all_cards.size(); i++){
		hash_value[all_cards[i].value]++;
		hash_suit[all_cards[i].suit]++;
	}


	for(const auto& it: hash_suit){
		if(it.second >= 5){ 							// Check flush and straightflush
			result.combo_rank = rank::Flush;
			suit flush_suit = it.first;
			value high_card = value::Two;

			std::map<value, int> hash_flush;
			for(const auto& flush_card: all_cards)
				if (flush_card.suit == flush_suit)
					hash_flush[flush_card.value]++;
			
			if(hash_flush.size() >= 5){
				Combination straightflush = StraightCheck(hash_flush);
				if (straightflush.combo_rank == rank::Straight){
					result.combo_rank = rank::StraightFlush;
					result.combo_val = straightflush.combo_val;
					result.kicker[0] = straightflush.kicker[0];
					return result;
				}
			}

			for(Card& card: all_cards){
				if (card.suit == flush_suit && card.value > result.combo_val)
					result.combo_val = card.value;
			}

			return result;
		}
	}

	result = StraightCheck(hash_value);

	for(const auto &it: hash_value){
		if(it.second == 2){
			if (result.combo_rank > rank::TwoPairs)
				continue;

			// Check for pair
			value kicker_val = value::Two;
			
			highest_comb.combo_rank = rank::Pair;
			highest_comb.combo_val = it.first;
			highest_comb.kicker[0] = value::Two;

			for(const auto &pair: hash_value){
				// Check for two pairs
				if(pair.second == 2 && it.first != pair.first){
					highest_comb.combo_rank = rank::TwoPairs;
					value kicker_val = value::Two;
					
					// Search higher kicker
					for(const auto &kicker: hash_value){
						if (kicker.first != pair.first && kicker.first != it.first)
							if (kicker.first > kicker_val)
								kicker_val = kicker.first;
					}	

					highest_comb.combo_val = (it.first > pair.first) ? it.first : pair.first;
					highest_comb.kicker[0] = (it.first > pair.first) ? pair.first : it.first;

					result = (result < highest_comb) ? highest_comb : result;
				}
				else if (it.first != pair.first) {
					if (result.combo_rank > rank::Pair)
						continue;
					
					highest_comb.kicker[0] = pair.first;
					result = highest_comb;
				}
			}
		}
		else if (it.second == 3){
			// Check for fullhouse
			value kicker_val = value::Two;
			value two_pairs_check = value::Two;
			for(auto &fh: hash_value){
				if(fh.second == 2){
					highest_comb.combo_rank = rank::FullHouse;
					highest_comb.combo_val = it.first;
				
					// For 7 cards, might be Trips and two pairs, so check for a higher rank
					if (fh.first > two_pairs_check && fh.second < 3)
						highest_comb.kicker[0] = two_pairs_check = fh.first;

					result = (result < highest_comb) ? highest_comb : result;
				}
				else if(fh.second == 3 && it.first != fh.first){
					result.combo_rank = rank::FullHouse;
					// For 7 cards, we might have 2 trips
					result.combo_val = (it.first > fh.first) ? it.first : fh.first; 	// Setting Fullhouse value
					result.kicker[0] = (it.first > fh.first) ? fh.first : it.first;		// Kicker value vice versa
					return result;	// The only combination, stop iterating
				}
				else {
					// Just three
					highest_comb.combo_rank = rank::ThreeOfAKind;
					highest_comb.combo_val = it.first;
					if (fh.first > kicker_val && fh.first != it.first)
						kicker_val = highest_comb.kicker[0] = fh.first;
					
					result = (result < highest_comb) ? highest_comb : result;
				}
			}
		}
		else if (it.second == 4){
			// Check for FourofaKind

			result.combo_rank = rank::FourOfAKind;
			result.combo_val = it.first;
			for(auto &it: hash_value){
				if(it.first > result.kicker[0] && it.second < 4)
					result.kicker[0] = it.first;
			}
			return result;			// The only combination, stop iterating
		}
		else {
			highest_comb.combo_rank = rank::HighCard;
			highest_comb.combo_val = it.first;
			highest_comb.kicker[0] = it.first;

			result = (result < highest_comb) ? highest_comb : result;
		}
	}
	return result;
}

Combination Poker::StraightCheck(const std::map<value, int>& hash_value){
	Combination result;

	if (hash_value.size() < 5)
		return result;
	
	value last = value::Two;
	int counter = 4;
	for(const auto& it: hash_value){
		if (counter == 0){
			if (hash_value.count((value)((int) it.first - 1))){
				last = it.first;
				continue;
			}
			result.combo_rank = rank::Straight;
			result.combo_val = last;
			result.kicker[0] = last;
			return result;
		}
		if(it.first == value::Two){
			if (hash_value.count(value::Ace))
				counter--;
		}
		else {
			if (hash_value.count((value)((int) it.first - 1))){
				counter--;
				last = it.first;
			}
			else
				counter = 4;
		}
	}

	if (counter == 0){
		// Iteration ends, but condition were met, last card or straight was Ace
		result.combo_rank = rank::Straight;
		result.combo_val = last;
		result.kicker[0] = last;
	}

	return result;
}

void PlayHand::SetBlind(const int blind) {
	blind_size = blind;
}

void PlayHand::FinishHand() {
	std::cout << winner->name << " won the round" << std::endl;
	winner->AddMoney(bank);
	std::cout << "Bank: " << bank << " -> " << winner->name << "'s money: " << winner->GetMoney() << std::endl;
	std::cout << "-------------------------------------" << std::endl << std::endl;
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

DealtCards::DealtCards(Card* cards_, int count): open_cards(count){
	for (int i = 0; i < count; i++)
		cards.push_back(cards_[i]);
}

void DealtCards::Reset(){
	open_cards = 0;
}

void DealtCards::Deal(Card card){
	cards[open_cards++] = card;
}

#if 0
int main() {
	Player* player1 = new Player("Ivan", 1000);
	Player* player2 = new Player("Petr", 1000);
	Player* player3 = new Player("Sergey", 1000);
	Player* player4 = new Player("Dmitriy", 1000);
	

	PlayHand playhand;
	playhand.SetBlind(5);
	playhand.AddPlayer(player1).AddPlayer(player2).AddPlayer(player3).AddPlayer(player4);
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
#endif
