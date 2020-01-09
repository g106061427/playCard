#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#define CARDSNUM 52

#if 0
using namespace std;
class dealer {//set dealer
public:
	int playerNumber;//set number of players
	void plaNum(int k) {
		playerNumber = k;
	}
	vector<string> numNsuit1() {//set card number and suits
		vector<string> Temp;
		vector<string> cardSuit1{ "C", "D", "H", "S" };
		for (int i = 0; i < cardSuit1.size(); i++) {
			for (int j = 2; j < 13; j++) {
				string temp = cardSuit1[i] + " " + to_string(j + 1);
				Temp.push_back(temp);
			}
		}
		for (int i = 1; i < 5; i++) {//set Ace and 2 become the second and first largest card in a same suit deck
			for (int j = 1; j < 3; j++) {
				string temp = cardSuit1[4 - i] + " " + to_string(j);
				Temp.insert(Temp.end() + 13 * (1 - i), temp);
			}
		}
		return Temp;
	}
	vector<vector<int>> distribute1() {//Distribute the cards
		vector<int> cardNum;
		vector<vector<int>> Temp;
		for (int i = 0; i < cards; i++) {
			cardNum.push_back(i);
		}
		srand((unsigned)time(NULL));
		for (int i = 0;i <= 1000;i++) {
			int m = rand() % cards;
			int n = rand() % cards;
			swap(cardNum[m], cardNum[n]);
		}
		int playerCard = cards / playerNumber;//Deal the cards
		for (int i = 0; i < playerNumber; i++) {
			vector<int> temp;
			for (int j = 0; j < playerCard; j++) {
				temp.push_back(cardNum[i* playerCard+j]);
			}
				Temp.push_back(temp);
		}
		return Temp;
	}
};
class player {//set player
public:
	vector<int> getCard(vector<int> Cards, int method) {//set the sort method
		if (method == 0) {//by value and suit
			sort(Cards.begin(), Cards.end());
			return Cards;
		}
		else if (method == 1)//by number
		{
			for (int i = Cards.size() - 1; i > 0; i--)
			{
				for (int j = 0; j <= i - 1; j++)
				{
					int Q1 = Cards[j] / 13;
					int R1 = Cards[j] % 13;
					int Q2 = Cards[j+1] / 13;
					int R2 = Cards[j+1] % 13;
					if (R1 > R2)
					{
						swap(Cards[j], Cards[j+1]);
					}
					if (R1 == R2 && Q1 > Q2) {
						swap(Cards[j], Cards[j + 1]);
					}
				}
			}
			return Cards;
		}
		else
		{
			return Cards;
		}
	}
};

int main() {
	int playercount = 4;//set player number
	dealer Dealer1;//set dealer object
	Dealer1.plaNum(playercount);

	vector < vector<int> > getCards;//get cards
	for (int i = 0;i < Dealer1.distribute1().size();i++) {
		getCards.push_back(Dealer1.distribute1()[i]);
	}
	player Player;//set player object
	int p = cards / playercount;
	for (int j = 0; j < playercount; j++) {//print the result
		string name = "Player " + to_string(j+1) + "：";
		cout << name;
		for (int k = 0; k < p; k++) {
				cout << Dealer1.numNsuit1()[Player.getCard(getCards[j], 0)[k]] << "\t";	
		}
		cout << endl;
	}
	return 0;
}
#endif
enum class sortMethods
{
	SORT_BY_NUMBER,
	SORT_BY_SUIT,
	SORT_BY_VALUE,
	NONE,
};