// Copyright Ben Howenstein 2019. All rights reserved.
//

#include "pch.h"
#include <array>
#include <vector>
#include <iostream>
#include <assert.h>


enum class Suit
{
	Heart,
	Diamond,
	Ace,
	Spade
};

struct Card
{
	short m_number{ 0 };
	Suit m_suit;
};

enum class Result
{
	Win,
	Lose,
	Tie
};

std::ostream& operator<<( std::ostream& os, const Result result )
{
	if( result == Result::Lose )
	{
		return os << "Lose!";
	}
	if( result == Result::Win )
	{
		return os << "Win!";
	}
	return os << "Tie";
}

template <int CardCount_t = 5>
class Hand
{
public:

	using Cards = std::array<Card, CardCount_t>;

	Hand() {}

	void Add( const Card card )
	{
		assert( m_numCards < m_cards.size() );
		m_cards[ m_numCards ] = card;
		++m_numCards;
		sort();
	}

	const Cards& GetCards() const { return m_cards; }
	const size_t NumCards() const { return m_numCards; }

private:

	void sort()
	{
		//	Sort cards by rank
		//
		auto EvalRank = []( const Card& first, const Card& second )
		{
			return first.m_number > second.m_number;
		};
		std::sort( m_cards.begin(), m_cards.end(), EvalRank );
	}

	Cards m_cards{};
	size_t m_numCards{ 0 };
};

template<int NumCards_t>
std::ostream& operator<<( std::ostream& os, const Hand<NumCards_t>& hand )
{
	os << " { ";
	for( const auto& card : hand.GetCards() )
	{
		if( card.m_number > 0 )
		{
			os << card.m_number << " ";
		}
	}
	return os << "} ";
}

template <class Hand_t>
std::vector<Hand_t> GenerateSortedSubhands( const Hand_t& hand )
{
	const auto& cards = hand.GetCards();

	//	Group matching cards
	// 
	std::vector<Hand_t> subhands;
	for( size_t cardIndex = 0; cardIndex < cards.size(); )
	{
		const auto rankValue = cards[ cardIndex ].m_number;

		Hand_t subhand;
		subhand.Add( cards[ cardIndex ] );
		while( ++cardIndex < hand.NumCards() )
		{
			if( cards[ cardIndex ].m_number != rankValue )
			{
				break;
			}
			subhand.Add( cards[ cardIndex ] );
		}
		subhands.emplace_back( subhand );
	}

	//	Sort subhand groups
	//
	auto EvalSubHand = []( const Hand_t& first, const Hand_t& second )
	{
		return first.NumCards() > second.NumCards();
	};
	std::sort( subhands.begin(), subhands.end(), EvalSubHand );

	return subhands;
}

template<class Hand_t>
Result EvalHands( const Hand_t& first, const Hand_t& second )
{
	const auto firstSubhands = GenerateSortedSubhands( first );
	const auto secondSubhands = GenerateSortedSubhands( second );

	for( const auto& subhand : firstSubhands )
	{
		std::cout << subhand << " ";
	}

	std::cout << "\n";
	for( const auto& subhand : secondSubhands )
	{
		std::cout << subhand << " ";
	}

	auto EvalRank = []( const auto& first, const auto& second )
	{
		auto cardIndex = 0;
		while( first[ cardIndex ] == second[ cardIndex ] )
		{
			++cardIndex;
		}
		if( cardIndex >= first.size())
		{
			return Result::Tie;
		}
		return first[ cardIndex ] > second[ cardIndex ] ? Result::Win : Result::Lose;
	};

	const auto minNumSubhands = std::min( firstSubhands.size(), secondSubhands.size() );
	for( size_t subhandIndex = 0; subhandIndex < minNumSubhands; ++subhandIndex )
	{
		const auto& firstSubhand = firstSubhands[ subhandIndex ];
		const auto& secondSubhand = secondSubhands[ subhandIndex ];
		if( firstSubhand.NumCards() > secondSubhand.NumCards() )
		{
			return Result::Win;
		}
		if( firstSubhand.NumCards() < secondSubhand.NumCards() )
		{
			return Result::Lose;
		}
	}

	if( firstSubhands.size() > secondSubhands.size() )
	{
		return Result::Win;
	}
	if( firstSubhands.size() < secondSubhands.size() )
	{
		return Result::Lose;
	}

	for( size_t subhandIndex = 0; subhandIndex < firstSubhands.size(); ++subhandIndex )
	{
		const auto& firstSubhand = firstSubhands[ subhandIndex ];
		const auto& secondSubhand = secondSubhands[ subhandIndex ];
		if( firstSubhand.GetCards()[ 0 ].m_number > secondSubhand.GetCards()[ 0 ].m_number )
		{
			return Result::Win;
		}
		if( firstSubhand.GetCards()[ 0 ].m_number < secondSubhand.GetCards()[ 0 ].m_number )
		{
			return Result::Lose;
		}
	}

	return Result::Tie;
}

int main()
{
	Hand<5> hand1;
	hand1.Add( { 1, Suit::Heart } );
	hand1.Add( { 2, Suit::Heart } );
	hand1.Add( { 3, Suit::Ace } );
	hand1.Add( { 6, Suit::Heart } );
	hand1.Add( { 4, Suit::Spade } );

	Hand<5> hand2;
	hand2.Add( { 13, Suit::Heart } );
	hand2.Add( { 2, Suit::Heart } );
	hand2.Add( { 3, Suit::Ace } );
	hand2.Add( { 2, Suit::Heart } );
	hand2.Add( { 3, Suit::Spade } );

	Hand<5> hand3;
	hand3.Add( { 13, Suit::Heart } );
	hand3.Add( { 2, Suit::Heart } );
	hand3.Add( { 11, Suit::Ace } );
	hand3.Add( { 11, Suit::Heart } );
	hand3.Add( { 1, Suit::Spade } );

	Hand<5> hand4;
	hand4.Add( { 13, Suit::Heart } );
	hand4.Add( { 14, Suit::Heart } );
	hand4.Add( { 14, Suit::Ace } );
	hand4.Add( { 14, Suit::Heart } );
	hand4.Add( { 14, Suit::Spade } );

	Hand<5> hand5;
	hand5.Add( { 1, Suit::Heart } );
	hand5.Add( { 2, Suit::Heart } );
	hand5.Add( { 3, Suit::Ace } );
	hand5.Add( { 4, Suit::Heart } );
	hand5.Add( { 5, Suit::Spade } );

	std::cout << "\n" << EvalHands( hand1, hand2 ) << "\n\n";
	std::cout << "\n" << EvalHands( hand4, hand5 ) << "\n\n";
	std::cout << "\n" << EvalHands( hand4, hand4 ) << "\n\n";
	std::cout << "\n" << EvalHands( hand1, hand5 ) << "\n\n";

	Hand<15> hand6;
	hand6.Add( { 1, Suit::Heart } );
	hand6.Add( { 9, Suit::Heart } );
	hand6.Add( { 3, Suit::Ace } );
	hand6.Add( { 11, Suit::Heart } );
	hand6.Add( { 5, Suit::Spade } );
	hand6.Add( { 1, Suit::Heart } );
	hand6.Add( { 7, Suit::Heart } );
	hand6.Add( { 3, Suit::Ace } );
	hand6.Add( { 6, Suit::Heart } );
	hand6.Add( { 1, Suit::Spade } );
	hand6.Add( { 1, Suit::Heart } );
	hand6.Add( { 3, Suit::Heart } );
	hand6.Add( { 13, Suit::Ace } );
	hand6.Add( { 14, Suit::Heart } );
	hand6.Add( { 5, Suit::Spade } );

	Hand<15> hand7;
	hand7.Add( { 1, Suit::Heart } );
	hand7.Add( { 2, Suit::Heart } );
	hand7.Add( { 3, Suit::Ace } );
	hand7.Add( { 4, Suit::Heart } );
	hand7.Add( { 5, Suit::Spade } );
	hand7.Add( { 1, Suit::Heart } );
	hand7.Add( { 2, Suit::Heart } );
	hand7.Add( { 3, Suit::Ace } );
	hand7.Add( { 4, Suit::Heart } );
	hand7.Add( { 5, Suit::Spade } );
	hand7.Add( { 1, Suit::Heart } );
	hand7.Add( { 2, Suit::Heart } );
	hand7.Add( { 3, Suit::Ace } );
	hand7.Add( { 4, Suit::Heart } );
	hand7.Add( { 5, Suit::Spade } );

	std::cout << "\n" << EvalHands( hand6, hand7 ) << "\n\n";

	std::cout << "\n\n";
}
