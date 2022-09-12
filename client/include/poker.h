#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <sstream>
#include <string>
#include "table.h"

#define START_BANK 1000

namespace Poker {
	enum class suit {Spades, Clubs, Hearts, Diamonds};
	enum class value {Two=2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace};
	enum class stage {Preflop, Flop, Turn, River};
	enum class action {Fold, Call, Raise};

	struct Card {
		suit suit;
		value value;
	};

	class Deck {
	public:	
		Deck() { Shuffle(); };
		const Card& Deal() { return deck_[current_card++]; }
		void Burn() { current_card++; }

	private:
		void Shuffle();
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
	};

	class Player{
	public:
		Player(): name("blank"){}
		Player(const std::string& name_, int money_) : name(name_), money(money_) {}
		void GetCards(const Card& lcard, const Card& rcard);
		std::string ShowCards();
		void Trade();
		const std::string name;
		void SetPosition(int pos) { position = pos; }
		void SetStatus(bool status) { in_play = status; }
		const int GetMoney() const { return money; }
		
	private:
		action ReadAction(const std::string& decision);
		action action;
		std::array<Card, 2> hand;
		int money = START_BANK;
		bool in_play = false;
		int position;
	};

	class PlayHand{
	public:
		PlayHand();
		PlayHand(int blind) : blind_size(blind) {}
		void DealOnTable();
		void DealToPlayers();
		void AddPlayer(Player* player);
		void ChangeStage();
		void TradeRound();
		void NewRound(const int new_blind = 0);
		void FinishHand();
		void ActivatePlayers();

	private:
		Table<Player> players;
		std::array<Card, 5> dealt_cards;
		size_t current_card = 0;
		int bank = 0;
		stage stage = stage::Preflop;
		int blind_size;		// small blind
		int players_total;
		int players_ingame;
		Deck deck;
	};

	class TradeStage {
	public:
		TradeStage(PlayHand* play_) { play = play_; }
		virtual void Deal();
		virtual void Trade();
		virtual void Shove();

	private:
		PlayHand* play;
	};

}