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
    std::stringstream ss;
    Player* player1 = new Player("Ivan", 1000);
	Player* player2 = new Player("Petr", 1000);
	Player* player3 = new Player("Sergey", 1000);
	Player* player4 = new Player("Dmitriy", 1000);
    // Lazy to rename players, make new pointer
    Player* ivan = player1;
    Player* petr = player2;
    Player* sergey = player3;
    Player* dmitriy = player4;
	
};

TEST(TestDealtCards, Test){
    Card card_rand[] = {{suit::Clubs, value::Ace}, {suit::Diamonds, value::Ace}};
    DealtCards cards(card_rand, 2);
    Card card({suit::Clubs, value::Ace});
    EXPECT_EQ(cards[0].suit, card.suit);
}

TEST(TestDealCards, TestSum){
    Card card_rand1[] = {{suit::Clubs, value::King}, {suit::Diamonds, value::King}};
    Card card_rand2[] = {{suit::Clubs, value::Ace}, {suit::Diamonds, value::Ace}};
    DealtCards cards1(card_rand1, 2);
    DealtCards cards2(card_rand2, 2);
    EXPECT_EQ(cards1[0].suit, suit::Clubs);
    EXPECT_EQ(cards2[1].suit, suit::Diamonds);
    DealtCards cards1_2 = cards1 + cards2;
    EXPECT_EQ(cards1[2].value, value::Ace);
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
    player1.Deal({suit::Diamonds, value::Ace});
    player1.Deal({suit::Diamonds, value::Queen});
    player2.Deal({suit::Clubs, value::Ace});
    player2.Deal({suit::Clubs, value::Queen});

    EXPECT_EQ(cards.Evaluate(), 0); // 3014 Flush - 3000, Ace - 14;
}
/*
Need to add selection of stream to automate poker tests */ 

TEST_F(PokerTest, InitialTest) {
    ss = std::stringstream("F F F");

    playhand->NewHand();
	while (playhand->GetStage() != stage::Final)
		playhand->Round();
	playhand->FinishHand();

}

TEST_F(PokerTest, RaiseBet) {
    ss << "F F R 100 C C C R 300 F";
    playhand->NewHand();
	playhand->Round(); // Preflop
    EXPECT_EQ(sergey->GetMoney(), 900);
    EXPECT_EQ(petr->GetMoney(), 900);
    EXPECT_EQ(ivan->GetMoney(), 1000);
    EXPECT_EQ(playhand->GetBank(), 200);
    playhand->Round();
    EXPECT_EQ(playhand->GetBank(), 200);
    playhand->Round();
    EXPECT_EQ(playhand->GetBank(), 500);
    playhand->FinishHand();
    EXPECT_EQ(sergey->GetMoney(), 900);
    EXPECT_EQ(petr->GetMoney(), 1100);
    EXPECT_EQ(playhand->GetWinner()->name, petr->name);
}

TEST_F(PokerTest, RaiseTooMuch) {
    ss << "F R 1500 1000 C F F";
    playhand->NewHand();
	playhand->Round(); // Preflokp
    EXPECT_EQ(ivan->GetMoney(), 0);
    EXPECT_EQ(petr->GetMoney(), 0);
    EXPECT_EQ(playhand->GetBank(), 2010);
	playhand->Round(); // Flop
    playhand->FinishHand();
    EXPECT_EQ(ivan->GetMoney(), 2010);
    EXPECT_EQ(playhand->GetWinner()->name, ivan->name);
}


TEST_F(PokerTest, TestWholeHand) {
    ss << "C C F C C C C C C C C C C C C C C";
    playhand->NewHand();
	playhand->Round();
	playhand->Round();
	playhand->Round();
	playhand->Round();
    playhand->FinishHand();
    EXPECT_EQ(ivan->GetMoney(), 990);
    EXPECT_EQ(sergey->GetMoney(), 990);
    EXPECT_EQ(petr->GetMoney(), 995);
    EXPECT_EQ(dmitriy->GetMoney(), 1025);
    EXPECT_EQ(playhand->GetWinner()->name, dmitriy->name);
}
