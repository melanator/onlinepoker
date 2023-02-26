#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>
#include <tuple>
#include <memory>
#include "table.h"

#define START_BANK 1000

namespace Poker {
	enum class suit {Spades, Clubs, Hearts, Diamonds};
	enum class value {Two=2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack=11, Queen=12, King=13, Ace=14};
	enum class stage {Preflop, Flop, Turn, River, Final};
	enum class action {NoAction, Fold, Call, Raise};
	enum class rank {
        HighCard,
        Pair=10,
        TwoPairs=100,
        ThreeOfAKind=1000,
        Straight=2000,
        Flush=3000,
        FullHouse=10000,
        FourOfAKind=100000,
        StraightFlush=1000000
    };

	struct Card {
		suit suit;
		value value;

		friend bool operator<(const Card& l, const Card& r){
			return (int) l.value < (int) r.value;
		}
	};	

	struct Combination; 
	bool operator<(const Combination& l, const Combination& r);
	bool operator==(const Combination& l, const Combination& r);

	struct Move {
		action act;
		int bet;
	};

	class Deck {
	public:	
		Deck(bool seed = false): is_seeded(seed) { Shuffle(); };
		const Card& Deal() { return deck_[current_card++]; }
		void Burn() { current_card++; }
		void Shuffle();

	private:
		std::vector<Card> deck_  {
			{suit::Spades, value::Two},
			{suit::Spades, value::Three},
			{suit::Spades, value::Four},
			{suit::Spades, value::Five},
			{suit::Spades, value::Six},
			{suit::Spades, value::Seven},
			{suit::Spades, value::Eight},
			{suit::Spades, value::Nine},
			{suit::Spades, value::Ten},
			{suit::Spades, value::Jack},
			{suit::Spades, value::Queen},
			{suit::Spades, value::King},
			{suit::Spades, value::Ace},
			{suit::Clubs, value::Two},
			{suit::Clubs, value::Three},
			{suit::Clubs, value::Four},
			{suit::Clubs, value::Five},
			{suit::Clubs, value::Six},
			{suit::Clubs, value::Seven},
			{suit::Clubs, value::Eight},
			{suit::Clubs, value::Nine},
			{suit::Clubs, value::Ten},
			{suit::Clubs, value::Jack},
			{suit::Clubs, value::Queen},
			{suit::Clubs, value::King},
			{suit::Clubs, value::Ace},
			{suit::Hearts, value::Two},
			{suit::Hearts, value::Three},
			{suit::Hearts, value::Four},
			{suit::Hearts, value::Five},
			{suit::Hearts, value::Six},
			{suit::Hearts, value::Seven},
			{suit::Hearts, value::Eight},
			{suit::Hearts, value::Nine},
			{suit::Hearts, value::Ten},
			{suit::Hearts, value::Jack},
			{suit::Hearts, value::Queen},
			{suit::Hearts, value::King},
			{suit::Hearts, value::Ace},
			{suit::Diamonds, value::Two},
			{suit::Diamonds, value::Three},
			{suit::Diamonds, value::Four},
			{suit::Diamonds, value::Five},
			{suit::Diamonds, value::Six},
			{suit::Diamonds, value::Seven},
			{suit::Diamonds, value::Eight},
			{suit::Diamonds, value::Nine},
			{suit::Diamonds, value::Ten},
			{suit::Diamonds, value::Jack},
			{suit::Diamonds, value::Queen},
			{suit::Diamonds, value::King},
			{suit::Diamonds, value::Ace}
		};
		size_t current_card = 0;
		bool is_seeded;
	};
	
	class DealtCards{
	public:
		friend Combination Evaluate(const DealtCards& dealt);
		DealtCards(int count = 2): cards(count) {}
		DealtCards(Card* cards, int count = 2);
		void Reset();
		void Deal(Card card);
		Card& operator[](int index) { return cards[index]; };
		Card operator[](int index) const { return cards[index]; }
		friend DealtCards operator+(const DealtCards& lhs, const DealtCards& rhs){
			DealtCards result(lhs.open_cards + rhs.open_cards);
			for(int i = 0; i < lhs.open_cards; i++){
				result.Deal(lhs[i]);
			}
			for(int i = 0; i < rhs.open_cards; i++){
				result.Deal(rhs[i]);
			}
			return result;
		}
	private:
		std::vector<Card> cards;
		int open_cards = 0;
	};
	
	struct Combination{
		Combination() : cards(5), combo_rank(rank::HighCard), combo_val(value::Two), kicker({ value::Two }) {};
		DealtCards cards;
		rank combo_rank;
		value combo_val;
		std::vector<value> kicker;

		bool operator<(const Poker::Combination& r) {
			return std::tie(this->combo_rank, this->combo_val, this->kicker) < std::tie(r.combo_rank, r.combo_val, r.kicker);
		}

		bool operator==(const Poker::Combination& r) {
			return (this->combo_rank == r.combo_rank) && (this->combo_val == r.combo_val) && (this->kicker == r.kicker);
		}

	};

	class Player{
	public:
		friend class PlayHand;
		Player(const std::string& name_ = "blank", int money_ = 0) : name(name_), money(money_) {}
		void GetCards(const Card& lcard, const Card& rcard);
		const std::string ShowCards() const;
		Move Trade(const int bank_size, const int bet, std::istream& input = std::cin);
		void Deal(const Card& card);
		void Pay(const int amount);
		const std::string name;
		void SetStatus(bool status) { in_play = status; }
		const int GetMoney() const { return money; }
		void AddMoney(const int amount) { money += amount;  }
		bool FinishedRound(const int high_bet);
		void Reset();
		const bool IsInPlay() const { return in_play; }
		const int GetBetThisHand() const { return bet_this_hand; }
		DealtCards hand;	
		Combination combo = Combination();
		
	private:
		Move ReadAction(std::string& decision, std::istream& input = std::cin);

		int money = START_BANK;
        int hand_power;
		int bet_this_hand = 0;
        action action = action::NoAction;
        bool in_play = false;
		rank rank = rank::HighCard;

	};


	class PlayHand{
	public:
		PlayHand(std::istream& str = std::cin) : dealt_cards(5), input(str) {};
		void SetBlind(const int blind);
		void NewHand(const int new_blind = 0);
		void DealOnTable();
		PlayHand& AddPlayer(Player* player);
		void ActivatePlayers();
		void Round();
		void ShowTable(const int cards);
		void SetWinner(Player* player);
		void FinishHand();
		int GetBank() const { return bank; }
		Player* GetWinner() const { return winner; }
		Player* FindWinner();
		stage GetStage() const { return stage; }
	
	protected:
		Deck deck;
		std::istream& input = std::cin;

	private:
		Table<Player> players;
		Player* winner;
		DealtCards dealt_cards;
		size_t current_card = 0;
		int bank = 0;
		stage stage = stage::Preflop;
		int blind_size;		// small blind
		int players_ingame;
		
		void Trade();
		void TradePreflop();
		void DealToPlayers();
		void BetFromPlayer(Player* player, const int amount);
		bool IsEndOfRound(const int high_bet);
		void NewRound();
	};
	
	class TestPlayHand : public PlayHand {
	/* Class for testing poker with predetermined deck */
	public:
		TestPlayHand(std::istream& str = std::cin) : PlayHand(str) { deck = Deck(true); }
	};


	bool operator<(const Combination& l, const Combination& r);

	Combination Evaluate(const DealtCards& dealt);
	Combination StraightCheck(const std::map<value, int>& hash_value);

    class iHandState {
        virtual void action(const PlayHand& hand);
    };

    class PreFlop : public iHandState{};
    class Flop : public iHandState{};
    class Turn : public iHandState{};
    class River : public iHandState{};
}
