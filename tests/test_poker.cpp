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
    EXPECT_EQ(cards1_2[2].value, value::Ace);
}


class EvaluateTestPlayers : public testing::Test {
protected:
    void SetUp(){  
        cards = DealtCards(5);
        cards.Deal({suit::Diamonds, value::Two});
        cards.Deal({suit::Clubs, value::Four});
        cards.Deal({suit::Diamonds, value::King});
        cards.Deal({suit::Diamonds, value::Seven});
        cards.Deal({suit::Spades, value::Jack});
    }

    DealtCards cards;
    Player player1;
    Player player2;
};


TEST_F(EvaluateTestPlayers, TestFlush){
    player1.Deal({suit::Diamonds, value::Ace});
    player1.Deal({suit::Diamonds, value::Queen});
    player2.Deal({suit::Clubs, value::Ace});
    player2.Deal({suit::Clubs, value::Queen});

    EXPECT_EQ(Evaluate(cards).combo_rank, rank::HighCard); // 3014 Flush - 3000, Ace - 14;
    EXPECT_EQ(Evaluate(cards).combo_val, value::King);      // Ace high card
    EXPECT_EQ(Evaluate(cards+player1.hand).combo_rank, rank::Flush);    // Flush with Ace
    EXPECT_EQ(Evaluate(cards+player1.hand).combo_val, value::Ace);      // Ace high card
    EXPECT_EQ(Evaluate(cards+player2.hand).combo_rank, rank::HighCard);      // Ace high card
    EXPECT_EQ(Evaluate(cards+player2.hand).combo_val, value::Ace);      // Ace high card

}

TEST(EvaluateTest, TestPair){
    {
        DealtCards cards(7);
        cards.Deal({suit::Clubs, value::Ace});
        cards.Deal({suit::Spades, value::Three});
        cards.Deal({suit::Hearts, value::Seven});
        cards.Deal({suit::Diamonds, value::Nine});
        cards.Deal({suit::Spades, value::Queen});
        cards.Deal({suit::Diamonds, value::Ace});
        cards.Deal({suit::Clubs, value::Two});
        // A A Q (14 * 10 + 12) = 152
        EXPECT_EQ(Evaluate(cards).combo_rank, rank::Pair);
        EXPECT_EQ(Evaluate(cards).combo_val, value::Ace);
        EXPECT_EQ(Evaluate(cards).kicker[0], value::Queen);

    }
    {
        DealtCards cards(7);
        cards.Deal({suit::Clubs, value::Ace});
        cards.Deal({suit::Spades, value::Three});
        cards.Deal({suit::Hearts, value::Seven});
        cards.Deal({suit::Diamonds, value::Nine});
        cards.Deal({suit::Spades, value::Queen});
        cards.Deal({suit::Diamonds, value::Queen});
        cards.Deal({suit::Clubs, value::Two});
        EXPECT_EQ(Evaluate(cards).combo_rank, rank::Pair);
        EXPECT_EQ(Evaluate(cards).combo_val, value::Queen);
        EXPECT_EQ(Evaluate(cards).kicker[0], value::Ace);
    }
    {
        DealtCards cards(7);
        cards.Deal({suit::Clubs, value::Ace});
        cards.Deal({suit::Spades, value::Three});
        cards.Deal({suit::Hearts, value::Three});
        cards.Deal({suit::Diamonds, value::Nine});
        cards.Deal({suit::Spades, value::Queen});
        cards.Deal({suit::Diamonds, value::Ace});
        cards.Deal({suit::Clubs, value::Two});
        // Best hand A A 3 3 Q
        EXPECT_EQ(Evaluate(cards).combo_rank, rank::TwoPairs);
        EXPECT_EQ(Evaluate(cards).combo_val, value::Ace);
        EXPECT_EQ(Evaluate(cards).kicker[0], value::Three);
    }
}

TEST(EvaluateTest, TestThrees){ 
    {
        DealtCards cards(7);
        cards.Deal({suit::Clubs, value::Ace});
        cards.Deal({suit::Spades, value::Ace});
        cards.Deal({suit::Hearts, value::Seven});
        cards.Deal({suit::Diamonds, value::Nine});
        cards.Deal({suit::Spades, value::Queen});
        cards.Deal({suit::Diamonds, value::Ace});
        cards.Deal({suit::Clubs, value::Two});
        // A A A Q 9 (14 * 1000 + 12) = 152
        EXPECT_EQ(Evaluate(cards).combo_rank, rank::ThreeOfAKind);
        EXPECT_EQ(Evaluate(cards).combo_val, value::Ace);
        EXPECT_EQ(Evaluate(cards).kicker[0], value::Queen);
    }
	{
		DealtCards cards(7);
		cards.Deal({suit::Clubs, value::Ace});
		cards.Deal({suit::Spades, value::Ace});
		cards.Deal({suit::Hearts, value::Seven});
		cards.Deal({suit::Diamonds, value::Seven});
		cards.Deal({suit::Spades, value::Queen});
		cards.Deal({suit::Diamonds, value::Ace});
		cards.Deal({suit::Clubs, value::Two});
		// A A A Q 9 (14 * 1000 + 12) = 152
		EXPECT_EQ(Evaluate(cards).combo_rank, rank::FullHouse);
		EXPECT_EQ(Evaluate(cards).combo_val, value::Ace);
		EXPECT_EQ(Evaluate(cards).kicker[0], value::Seven);
	}
}

TEST(EvaluateTest, TestFour){ 
   {
        DealtCards cards(7);
        cards.Deal({suit::Clubs, value::Ace});
        cards.Deal({suit::Spades, value::Ace});
        cards.Deal({suit::Hearts, value::Ace});
        cards.Deal({suit::Diamonds, value::Nine});
        cards.Deal({suit::Spades, value::Queen});
        cards.Deal({suit::Diamonds, value::Ace});
        cards.Deal({suit::Clubs, value::Two});
        // A A A Q 9 (14 * 1000 + 12) = 152
        EXPECT_EQ(Evaluate(cards).combo_rank, rank::FourOfAKind);
        EXPECT_EQ(Evaluate(cards).combo_val, value::Ace);
        EXPECT_EQ(Evaluate(cards).kicker[0], value::Queen);
    }
}

TEST(EvaluateTest, TestStraight){ 
   {
        DealtCards cards(7);
        cards.Deal({suit::Clubs, value::Four});
        cards.Deal({suit::Spades, value::Five});
        cards.Deal({suit::Hearts, value::Ace});
        cards.Deal({suit::Diamonds, value::Seven});
        cards.Deal({suit::Spades, value::Queen});
        cards.Deal({suit::Diamonds, value::Eight});
        cards.Deal({suit::Clubs, value::Six});
        // straight from Four to Seven
        EXPECT_EQ(Evaluate(cards).combo_rank, rank::Straight);
        EXPECT_EQ(Evaluate(cards).combo_val, value::Eight);
    }
    {
        DealtCards cards(7);
        cards.Deal({suit::Clubs, value::Four});
        cards.Deal({suit::Spades, value::King});
        cards.Deal({suit::Hearts, value::Ace});
        cards.Deal({suit::Diamonds, value::Seven});
        cards.Deal({suit::Spades, value::Queen});
        cards.Deal({suit::Diamonds, value::Three});
        cards.Deal({suit::Clubs, value::Two});
        // straight from Ace to Four
        EXPECT_EQ(Evaluate(cards).combo_rank, rank::Straight);
        EXPECT_EQ(Evaluate(cards).combo_val, value::Four);
	}
	{
		DealtCards cards(7);
		cards.Deal({suit::Clubs, value::Jack});
		cards.Deal({suit::Spades, value::King});
		cards.Deal({suit::Hearts, value::Ace});
		cards.Deal({suit::Diamonds, value::Seven});
		cards.Deal({suit::Spades, value::Queen});
		cards.Deal({suit::Diamonds, value::Ten});
		cards.Deal({suit::Clubs, value::Two});
		// from 10 to Ace
		EXPECT_EQ(Evaluate(cards).combo_rank, rank::Straight);
		EXPECT_EQ(Evaluate(cards).combo_val, value::Ace);
	}
}

TEST(EvaluateTest, TestStraightFlush) {
    {
        DealtCards cards(7);
        cards.Deal({ suit::Clubs, value::Four });
        cards.Deal({ suit::Clubs, value::Five });
        cards.Deal({ suit::Hearts, value::Ace });
        cards.Deal({ suit::Clubs, value::Seven });
        cards.Deal({ suit::Spades, value::Queen });
        cards.Deal({ suit::Clubs, value::Eight });
        cards.Deal({ suit::Clubs, value::Six });
        // straight from Four to Seven
        EXPECT_EQ(Evaluate(cards).combo_rank, rank::StraightFlush);
        EXPECT_EQ(Evaluate(cards).combo_val, value::Eight);
    }
    {
        DealtCards cards(7);
        cards.Deal({ suit::Clubs, value::Four });
        cards.Deal({ suit::Clubs, value::Five });
        cards.Deal({ suit::Clubs, value::Ace });
        cards.Deal({ suit::Clubs, value::Seven });
        cards.Deal({ suit::Spades, value::Queen });
        cards.Deal({ suit::Clubs, value::Eight });
        cards.Deal({ suit::Clubs, value::Six });
        // straight from Four to Seven
        EXPECT_EQ(Evaluate(cards).combo_rank, rank::StraightFlush);
        EXPECT_EQ(Evaluate(cards).combo_val, value::Eight);
    }
}
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

TEST(CombinationTest, TestLess) {
    {
        Combination l, r;
        l.combo_rank = rank::Pair;
        r.combo_rank = rank::FourOfAKind;
        EXPECT_TRUE(l < r);
    }
}

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

TEST(CombinationTest, TestLess) {
    {
        Combination l, r;
        l.combo_rank = rank::Pair;
        r.combo_rank = rank::FourOfAKind;
        EXPECT_TRUE(l < r);
    }
}
