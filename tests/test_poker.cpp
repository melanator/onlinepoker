#include <gtest/gtest.h>
#include "../client/include/poker.h"
#include <string>


using namespace Poker;

class PokerTest : public testing::Test {
protected:
    void SetUp(){
        playhand.AddPlayer(player1).AddPlayer(player2);
    }

    void TearDown(){
        delete player1;
        delete player2;
    }

    PlayHand playhand;
    Player* player1 = new Player("Ivan", 1000);
    Player* player2 = new Player("Petr", 1000);
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

    cards.Evaluate(player1);
}