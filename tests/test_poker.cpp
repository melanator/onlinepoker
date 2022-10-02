#include <gtest/gtest.h>
#include "../client/include/poker.h"
#include <string>
#include <sstream>

using namespace Poker;

class PokerTest : public testing::Test {
protected:
    void SetUp(){
        playhand->SetBlind(5);
		playhand->AddPlayer(player1).AddPlayer(player2).AddPlayer(player3).AddPlayer(player4);
    }

    void TearDown(){
        delete playhand;
    }

    TestPlayHand* playhand = new TestPlayHand(ss);
    std::istringstream ss;
    Player* player1 = new Player("Ivan", 1000);
	Player* player2 = new Player("Petr", 1000);
	Player* player3 = new Player("Sergey", 1000);
	Player* player4 = new Player("Dmitriy", 1000);
	
};

TEST(TestDealtCards, Test){
    Card card_rand[] = {{suit::Clubs, value::Ace}, {suit::Diamonds, value::Ace}};
    DealtCards cards(card_rand, 2);
    Card card({suit::Clubs, value::Ace});
    EXPECT_EQ(cards[0].suit, card.suit);
}


class EvaluateTest : public testing::Test {
protected:
    void SetUp(){  
        cards = DealtCards(5);
        cards.Deal({suit::Diamonds, value::Two});
        cards.Deal({suit::Clubs, value::Four});
        cards.Deal({suit::Diamonds, value::Seven});
        cards.Deal({suit::Spades, value::Jack});
        cards.Deal({suit::Diamonds, value::King});
    }

    DealtCards cards;
    Player player1;
    Player player2;
};

TEST_F(EvaluateTest, Test){
    player1.hand[0] = {suit::Diamonds, value::Ace};
    player1.hand[1] = {suit::Diamonds, value::Queen};
    player2.hand[0] = {suit::Clubs, value::Ace};
    player2.hand[1] = {suit::Clubs, value::Queen};

    EXPECT_EQ(cards.Evaluate(), 0); // 3014 Flush - 3000, Ace - 14;
}
/*
Need to add selection of stream to automate poker tests */ 

TEST_F(PokerTest, TestSmth) {
    ss = std::istringstream("F F F");

    playhand->NewHand();
	while (playhand->GetStage() != stage::Final)
		playhand->Round();
	playhand->FinishHand();

}
