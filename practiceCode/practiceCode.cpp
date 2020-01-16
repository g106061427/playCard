#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#define CARDNUM 52
using namespace std;

enum SortType{
	SORT_NONE,
	SORT_BY_ORDER_NUM,
	SORT_BY_ORDER_VALUE,
	SORT_BY_SUIT
};

class Dealer{
private:
	int playerNum;
	
	vector<int> cardNum;
public:
	Dealer() {};
	static vector<string> setNumNsuit(){
		vector<string> temp;
		vector<string> cardSuit{ "C", "D", "H", "S" };
		vector<string> cardNum{ "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
		for (int i = 0; i < cardSuit.size(); i++) {
			for (int j = 2; j < 13; j++) {
				string k = cardNum[j] + " " + cardSuit[i];
				temp.push_back(k);
			}
		}
		for (int i = 1; i < 5; i++) {//set Ace and 2 become the second and first largest card in a same suit deck
			for (int j = 0; j < 2; j++) {
				string k = cardNum[j] + " " + cardSuit[4 - i];
				temp.insert(temp.end() + 13 * (1 - i), k);
			}
		}
		return temp;
	};
	void setPlayerNumber(int playerNum) {
		if (playerNum <= 4 && playerNum >= 2) {
			this->playerNum = playerNum;
		}
		else
		{
			if (playerNum > 4) {
				cout << "Too much players." << endl;
			}
			if (playerNum < 2) {
				cout << "Player needs more than one." << endl;
			}
			exit(0);
		}
	};
	void preparingCards(){//set card number and suits
		for (int i = 0; i < CARDNUM; i++) {
			cardNum.push_back(i);
		}
		srand((unsigned)time(NULL));
		for (int i = 0;i <= 1000;i++) {
			int m = rand() % CARDNUM;
			int n = rand() % CARDNUM;
			swap(cardNum[m], cardNum[n]);
		}
	};
	vector<int>* getCards(int player) {
		vector<int>* temp = new vector<int>;
		for (int i = 0; i < CARDNUM/ playerNum; i++) {
			temp->push_back(cardNum[CARDNUM / playerNum * player + i]);
		}
		vector<int>::iterator it = find(temp->begin(), temp->end(), 0);
		if (it != temp->end() && playerNum == 3) {
			temp->push_back(cardNum[51]);
		}
		return temp;
	};
};

class CardSorter {
public:
	static vector<int>* sortCard(SortType type, vector<int>* cards) {
		switch (type) {
		case SORT_NONE:
			return cards;
			break;
		case SORT_BY_ORDER_NUM:
			for (int i = cards->size() - 1; i > 0; i--){
				for (int j = 0; j <= i - 1; j++){
					int Q1 = (*cards)[j] / 13;
					int R1 = (*cards)[j] % 13;
					int Q2 = (*cards)[j + 1] / 13;
					int R2 = (*cards)[j + 1] % 13;

					if (R1 > R2){
						swap((*cards)[j], (*cards)[j + 1]);
					}
					if (R1 == R2 && Q1 > Q2){
						swap((*cards)[j], (*cards)[j + 1]);
					}
				}
			}
			return cards;
			break;
		case SORT_BY_ORDER_VALUE:
			sort((*cards).begin(), (*cards).end());
			return cards;
			break;

		case SORT_BY_SUIT:
			for (int i = cards->size(); i > 0; i--) {
				for (int j = 0; j <= i - 1; j++) {
					int Q1 = (*cards)[j] / 13;
					int R1 = (*cards)[j] % 13;
					int Q2 = (*cards)[j + 1] / 13;
					int R2 = (*cards)[j + 1] % 13;

					if (R1 > R2)
						swap(cards[j], cards[j + 1]);
				}
			}
			return cards;
			break;
		default:
			return cards;
			break;
		}
	}
};

class Player
{
private:
	string name;
	SortType preferredSortType;
	vector<int>* cards;

public:
	Player(SortType type, string name) {
		preferredSortType = type;
		this->name = name;
	};
	void printCards() {
		cout << name << ": ";
		for (int i = 0; i < cards->size();i++) {
			cout << Dealer::setNumNsuit()[(*cards)[i]] <<"\t";
		}
		cout << endl;
	};
	void setCards(vector<int>* cards) {
		this->cards = cards;
		CardSorter::sortCard(preferredSortType, cards);
	}
};

int main() {
	int playerNum = 4;
	cout << "Please enter player number: " << endl;
	cin >> playerNum;
	Dealer dealer;
	dealer.setPlayerNumber(playerNum);
	dealer.preparingCards();
	vector<Player*> playerList;
	for (int i = 0; i < playerNum; i++)
	{
		string temp = "Player " + to_string(i + 1);
		Player* player = new Player(SORT_BY_ORDER_VALUE, temp);
		playerList.push_back(player);
	}
	for (int i = 0; i < playerNum;i++)
	{
		playerList[i]->setCards(dealer.getCards(i));
		playerList[i]->printCards();
	}
	return 0;
}

