#include<stdio.h>
#include<math.h>
#include<algorithm>
#include<iostream>
#include<unordered_map>
#include<unordered_set>
#include<math.h>
#include<limits.h>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<deque>

#define F first
#define S second

using namespace std;

map<string, int> mp;
vector<string> to;
int cnt;
vector< vector< pair<int, int> > > adj;
vector<int> Win;
string b;
/*
	Interface:
		+ void displayBOARD(): Xuất ra màn hình ma trận vừa được cập nhật
		+ void playTurn(char c): Nhập vào toạ độ của người chơi, c là kí tự X hoặc O.
		+ void robotTurn(): Nước đi của người máy.
		+ void gameStart(): Bắt đầu trò chơi (Người chơi chọn kí tự X và O và bắt đầu nước đi đầu tiên), xuất ra số điểm của mỗi người khi kết thúc trò chơi.
	Engine:
		+ bool isWin(): Kiểm tra xem đã tồn tại các cột hay hàng hay đường chéo gồm các kí tự giống nhau chưa?.
		+ bool isTie(): Kiểm tra xem nếu thực hiện hết 9 nước đi nhưng không phân thắng thua => hoà, kết thúc trò chơi.
		+ void bruteFore(int turn): Ta sẽ duyệt tất cả những những trạng thái bắt đầu từ trạng thái ban đầu.
		-> Khi đó ta sẽ tạo được đồ thị có hướng, không chu trình.
		+ void Solve(int u): Xử lý mỗi trạng thái u nếu trạng thái u là O thì sẽ chọn được đi mà X có số điểm là bé nhất, ngước lại X sẽ chọn nước đi mà nó sẽ có số điểm tối ưu nhất.
		-> Sau khi đã duyệt được mọi trạng thái và đưa ra được số điểm tối ưu cho từng trạng thái.
		+ string getPlay(): trả về ma trận nước đi tối ưu nhất đã được xử lý cho người thực hiện trạng thái đó.
*/


void init() {
	b = string(9, ' ');
	adj.assign(400000, vector< pair<int, int> > ());
	Win.assign(400000, -1);
	to.resize(400000);
	cnt = 0;
}
void resetBoard()
{
	b = string(9, ' ');
}
void playerTurn(char c)
{
	cout << "Input coordinate: ";
	int x, y;
	cin >> x >> y;
	while (x < 0 || x > 3 || y < 0 || y > 3 || b[(x - 1) * 3 + (y - 1)] != ' ')
	{
		cout << "INVALID, choose again: ";
		cin >> x >> y;

	}
	b[(x - 1) * 3 + (y - 1)] = c;
}
void displayBOARD()
{
	for (int i = 0; i < 14; ++i) cout << "_";
	cout << '\n';
	for (int i = 0; i < 3; ++i){
		cout << "| ";
		for (int j = 0; j < 3; ++j)
		{
			if (b[i * 3 + j] == ' ') cout << "   |";
			else if (b[i * 3 + j] == 'X') cout << " X |";
			else cout << " O |";
		}
		cout << '\n';
		for (int j = 0; j < 14; ++j) cout << "_";
		cout << '\n';
	}
}
void displayBOARD(string b) //debugger
{
	for (int i = 0; i < 14; ++i) cout << "_";
	cout << '\n';
	for (int i = 0; i < 3; ++i){
		cout << "| ";
		for (int j = 0; j < 3; ++j)
		{
			if (b[i * 3 + j] == ' ') cout << "   |";
			else if (b[i * 3 + j] == 'X') cout << " X |";
			else cout << " O |";
		}
		cout << '\n';
		for (int j = 0; j < 14; ++j) cout << "_";
		cout << '\n';
	}
}
bool isTie()
{
	for (int i = 0; i < 9; ++i) if (b[i] == ' ') return false;
	return true;
}
string getPlay()
{
	if (isTie()) return b; 
	int idx = mp[b];
	int turnPlay = adj[idx][0].S;
	int ans, pos;
	if (turnPlay)
	{
		ans = -1;
	} else 
	{
		ans = 3;
	}
	for (int i = 0; i < adj[idx].size(); ++i)
	{
		pair<int, int> e = adj[idx][i];
		if (turnPlay){
			if (ans < Win[e.F])
			{
				ans = Win[e.F];
				pos = e.F;
			}
		}
		else{
			if (ans > Win[e.F])
			{
				ans = Win[e.F];
				pos = e.F;
			}
		}
	}
	return to[pos];
}
void robotTurn()
{
	cout << "Robot turn: \n";
	b = getPlay();
}

bool isWin()
{
	//0 1 2
	//3 4 5
	//6 7 8
	return ((b[0] != ' ' && b[0] == b[1] && b[1] == b[2]) ||
				(b[3] != ' ' && b[3] == b[4] && b[4] == b[5]) ||
				(b[6] != ' ' && b[6] == b[7] && b[7] == b[8]) ||
				(b[0] != ' ' && b[0] == b[3] && b[3] == b[6]) ||
				(b[1] != ' ' && b[1] == b[4] && b[4] == b[7]) ||
				(b[2] != ' ' && b[2] == b[5] && b[5] == b[8]) ||
				(b[0] != ' ' && b[0] == b[4] && b[4] == b[8]) ||
				(b[2] != ' ' && b[2] == b[4] && b[4] == b[6]));
}

int _cnt = 0;

void bruteForce(int turn)
{
	string a = b;
	for (int i = 0; i < 9; ++i) if (b[i] == ' ')
	{
		b[i] = (turn & 1 ? 'O' : 'X');
		if (!mp[b]){
			mp[b] = ++cnt;
			to[cnt] = b;
			adj[mp[a]].push_back(make_pair(mp[b], turn & 1));
			if (isWin())
			{
				Win[mp[b]] = (turn & 1 ? 2 : 0);
			} else if (isTie())
			{
				Win[mp[b]] = 1;
				
			} else
				bruteForce(turn + 1);
		} else
		{
			adj[mp[a]].push_back(make_pair(mp[b], turn & 1));
		}
		b[i] = ' ';
	}
}

bool isVisited[400000];

void Solve(int u)
{
	isVisited[u] = 1;
	for (int idx = 0; idx < adj[u].size(); ++idx)
	{
		pair<int, int> e = adj[u][idx];
		if (isVisited[e.F])
		{
			if (e.S)
				Win[u] = max(Win[u], Win[e.F]);
			else {
				if (Win[u] == -1) Win[u] = 100000000;
				Win[u] = min(Win[u], Win[e.F]); 
			}
			continue;
		}
		Solve(e.F);
		if (e.S)
			Win[u] = max(Win[u], Win[e.F]);
		else {
			if (Win[u] == -1) Win[u] = 100000000;
			Win[u] = min(Win[u], Win[e.F]); 
		}
	}
}

void gameStart()
{
	int Score[2] = {};
	int turn = 0;
	for (int i = 0; i < 9; ++i) b[i] = ' ';
	char c;
	while (1){
		cout << "Choose X or O:";
		cin >> c;
		switch (c){
		case 'X':
			while (1)
			{
				switch (turn)
				{
					case 0:
						playerTurn(c);
						break;
					case 1:
						robotTurn();
						break;
				}
				displayBOARD();
				if (isWin() || isTie()){
					if (isWin()) Score[turn]++;
					cout << "Human: " << Score[0] << '\n' << "Engine: " << Score[1] << '\n';
					resetBoard();
					break;
				}
				turn ^= 1;
				
			}
			break;
		case 'O':
			while (1)
			{
				switch (turn)
				{
					case 0:
						robotTurn();
						break;
					case 1:
						playerTurn(c);
						break;
				}
				displayBOARD();
				if (isWin() || isTie()){
					if (isWin()) Score[turn ^ 1]++;
					cout << "Human: " << Score[0] << '\n' << "Engine: " << Score[1] << '\n';
					resetBoard();
					break;
				}
				turn ^= 1;
				
			}
			break;

		}
	}
}


int main()
{
	init();
	mp[b] = 0;
	bruteForce(0);
	Solve(0);
	gameStart();
	return 0;
}
