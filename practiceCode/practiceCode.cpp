#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <sstream>
#include<numeric>
#define CARDNUM 52
using namespace std;

enum SortType {
	SORT_NONE,
	SORT_BY_ORDER_NUM,
	SORT_BY_ORDER_VALUE,
	SORT_BY_SUIT
};

class Dealer {
private:
	int playerNum;
	vector<int> cardNum;
public:
	int gameStart;
	vector<string> CardsType{ "One_Pair","Two_Pairs","Three_Of_A_Kind","Flush,Full_House","Four_Of_A_Kind","Straight_Flush" };
	Dealer() {};
	static vector<string> setNumNsuit() {//set cards number and suits
		vector<string> temp;
		vector<string> cardSuit{ "C", "D", "H", "S" };
		vector<string> cardNum{ "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
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
	void setPlayerNumber(int playerNum) {//Check players number
		if (playerNum <= 4 && playerNum >= 2) {
			this->playerNum = playerNum;
		}
		else {
			if (playerNum > 4) {
				cout << "Too much players." << endl;
			}
			if (playerNum < 2) {
				cout << "Player needs more than one." << endl;
			}
			exit(0);
		}
	};
	void preparingCards() {//Shuffle cards
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
	vector<int>* getCards(int player) {//Licensing cards
		vector<int>* temp = new vector<int>;
		for (int i = 0; i < CARDNUM / playerNum; i++) {
			temp->push_back(cardNum[CARDNUM / playerNum * player + i]);
		}
		vector<int>::iterator it = find(temp->begin(), temp->end(), 0);
		if (it != temp->end()) {
			gameStart = player;//Find who has Club 3.
			if (playerNum == 3) {//When have 3 player licensing last cards to who hass Club 3.
				temp->push_back(cardNum[51]);
			}
		}
		return temp;
	};
	vector<int> pokerHands(vector<int> play) {//Find poker hands
		if (play.size() == 0) {//pass
			return { 0,1 };
		}
		else {
			vector<int> suit;
			vector<int> number;
			for (int i = 0;i < play.size();i++) {
				suit.push_back(play[i] / 13);
				number.push_back(play[i] % 13);
			}
			sort(suit.begin(), suit.end());
			sort(number.begin(), number.end());
			vector<int> tempSuit = suit;
			vector<int> tempNumber = number;
			rotate(suit.begin(), suit.begin() + 1, suit.end());
			rotate(number.begin(), number.begin() + 1, number.end());
			vector<int> checkSuit;
			vector<int> checkNumber;
			for (int i = 0;i < play.size();i++) {
				checkSuit.push_back(suit[i] - tempSuit[i]);
				checkNumber.push_back(number[i] - tempNumber[i]);
			}
			if (play.size() < 4 && play.size() > 0) {
				if (count(checkNumber.begin(), checkNumber.end(), 0) == play.size()) {//One Pair, Two Pairs, Three of a kind
					return { int(play.size()),int(play.back()) };
				}
				else {
					return { 0,0 };
				}
			}
			if (play.size() == 5) {
				if (count(checkSuit.begin(), checkSuit.end(), 0) == 5) {//Straight
					if (count(checkNumber.begin(), checkNumber.end(), 1) == 4) {//Straight Flush
						return { 7,int(play.back()) };
					}
					if (count(checkNumber.begin(), checkNumber.end(), 1) == 3) {
						if (play[3] % 13 == 3) {//Straight Flush from 2
							return { 7,int(play.back()) };
						}
						if (play[3] % 13 == 11) {//Straight Flush from Ace
							return { 7,int(play[2]) };
						}
						else {
							return { 0,0 };
						}
					}
					else {
						return { 0,0 };
					}
				}
				else {
					if (count(checkNumber.begin(), checkNumber.end(), 1) == 4) {//Straight
						return { 4,play.back() };
					}
					if (count(checkNumber.begin(), checkNumber.end(), 1) == 3) {
						if (play[3] % 13 == 3) {//Straight start from 2
							return { 4,int(play.back()) };
						}
						if (play[3] % 13 == 11) {//Straight start from Ace
							return { 4,int(play[2]) };
						}
						else {
							return { 0,0 };
						}
					}
					if (count(checkNumber.begin(), checkNumber.end(), 0) == 3) {
						if (count(tempNumber.begin(), tempNumber.end(), tempNumber[0]) == 4) {//Four of a Kind
							return { 6,tempNumber.front() };
						}
						if (count(tempNumber.begin(), tempNumber.end(), tempNumber[4]) == 4) {//Four of a Kind
							return { 6,tempNumber.back() };
						}
						if (count(tempNumber.begin(), tempNumber.end(), tempNumber[0]) == 3 && count(tempNumber.begin(), tempNumber.end(), tempNumber[4]) == 2) {//Full house
							return { 5,tempNumber.front() };
						}
						if (count(tempNumber.begin(), tempNumber.end(), tempNumber[4]) == 3 && count(tempNumber.begin(), tempNumber.end(), tempNumber[0]) == 2) {//Full house
							return { 5,tempNumber.back() };
						}
						else {
							return { 0,0 };
						}
					}
					else {
						return { 0,0 };
					}
				}
			}
			else {
				return { 0,0 };
			}
		}
	};
	int discriminate(int round, int player, vector<int> playerCards, int onPlayer, vector<int> onCards) {
		if (playerCards[0] == 0) {
				if (playerCards[1] == 1) {//pass
					return 0;
				}
				if (playerCards[1] == 0) {//Error
					return -1;
				}
				else {
					return -1;
				}
			}
		else {
			if (player == onPlayer) {
					return 1;
				}
			if (player != onPlayer) {
					if (playerCards[0] <= 5) {
						if (playerCards[0] == onCards[0]) {
							if (playerCards[1] % 13 > onCards[1] % 13) {
								return 1;
							}
							if (playerCards[1] % 13 == onCards[1] % 13 && playerCards[1] / 13 > onCards[1] / 13) {
								return 1;
							}
							else {
								return -1;
							}
						}
						else {
							return -1;
						}
					}
					if (playerCards[0] > 5) {
						if (playerCards[0] == onCards[0]) {
							if (playerCards[1] % 13 > onCards[1] % 13) {
								return 1;
							}
							if (playerCards[1] % 13 == onCards[1] % 13 && playerCards[1] / 13 > onCards[1] / 13) {
								return 1;
							}
							else {
								return -1;
							}

						}
						if (playerCards[0] > onCards[0]) {
							return 1;
						}
						else {
							return -1;
						}
					}
				}
		}
		
	}
};

class CardSorter {
public:
	static vector<int>* sortCard(SortType type, vector<int>* cards) {
		switch (type) {
		case SORT_NONE:
			return cards;
			break;
		case SORT_BY_ORDER_NUM:
			for (int i = cards->size() - 1; i > 0; i--) {
				for (int j = 0; j <= i - 1; j++) {
					int Q1 = (*cards)[j] / 13;
					int R1 = (*cards)[j] % 13;
					int Q2 = (*cards)[j + 1] / 13;
					int R2 = (*cards)[j + 1] % 13;

					if (R1 > R2) {
						swap((*cards)[j], (*cards)[j + 1]);
					}
					if (R1 == R2 && Q1 > Q2) {
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
	vector<int> vectValue;
	vector<int> vectPosition;

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
	vector<int> play(string in) {//Convert input to vector
		vectValue.clear();
		vectPosition.clear();
		stringstream ss(in);
		if (in == "pass") {
			return { };
		}
		for (int i; ss >> i;) {
			vectValue.push_back((*cards)[i - 1]);
			vectPosition.push_back(i - 1);
			if (ss.peek() == ',') {
				ss.ignore();
			}
		}
		sort(vectValue.begin(), vectValue.end());
		return vectValue;
	}
	int cardsOut(vector<int> out) {//Erase Cards
		for (int i = 0;i < out.size();i++) {
			(*cards).erase((*cards).begin() + vectPosition[out.size() - i - 1]);
		}
		return cards->size();
	}
};

int main() {
	int playerNum = 4;
	cout << "Please enter player number: " << endl;//Set player number
	cin >> playerNum;
	Dealer dealer;
	dealer.setPlayerNumber(playerNum);
	dealer.preparingCards();
	vector<Player*> playerList;
	for (int i = 0; i < playerNum; i++) {//Set player's name and sort type
		string temp = "Player " + to_string(i + 1);
		Player* player = new Player(SORT_BY_ORDER_NUM, temp);
		playerList.push_back(player);
	}
	for (int i = 0; i < playerNum;i++) {
		playerList[i]->setCards(dealer.getCards(i));
	}
	rotate(playerList.begin(), playerList.begin() + dealer.gameStart, playerList.end());
	int j = 0;
	int onTablePlayer = 0;
	int checkCardsNumber = CARDNUM / playerNum;
	vector<int> onTableCards = { 0,0 };
	for (j = 0;j < 52;j++) {
		for (int i = 0; i < playerNum;) {
			if (i != onTablePlayer) {
				cout << dealer.CardsType[onTableCards[0]-1] << "," << dealer.setNumNsuit()[onTableCards[1]] << endl;
			}
			playerList[i]->printCards();
			string in;
			cin >> in;
			vector<int> input = playerList[i]->play(in);
			if (j == 0 && i == 0 && input[0] != 0) {
			cout << "Play Club 3 first!!!" << endl;
		}
			else {
				vector<int> type = dealer.pokerHands(input);
				int k = dealer.discriminate(j, i, type, onTablePlayer, onTableCards);
				if (k == 1) {
					onTablePlayer = i;
					onTableCards = type;
					checkCardsNumber = playerList[i]->cardsOut(input);
					system("CLS");
					i++;
				}
				if (k == 0) {
					system("CLS");
					i++;
				}
				if (k == -1) {
					system("CLS");
					cout << "FOUL!!!Try again" << endl;
				}
			}
			if (checkCardsNumber == 0) {
				break;
			}
		}
		if (checkCardsNumber == 0) {
			break;
		}
	}
	return 0;
}