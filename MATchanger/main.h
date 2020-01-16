#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include<map>
#include<random>


//宣言
double PathL;//二分木の高さの数
double PathZ;//バケツ内のブロック数
double PathN;//パス内のブロック数
double Label;//ラベルの数
std::string trace;//トレース1行分
std::map<std::string, int >mp;//文字列→整数の連想配列

class Perse{

public:
//変数	
	std::ifstream ifs;
	std::string a[3];
	std::string str;
	char del;
	std::vector<std::string> subStr;
//関数
	void fileopen(std::string filename);//入力トレースを読み込む関数

	void cut(std::string);//入力からアドレス、命令タイプ、時刻を切り出す関数
};

class PosMap{

public:
//変数
	int OldLabel,Label,NewLabel;
	std::map<std::string, int >mp;     //文字列→整数の連想配列
	
//関数
	int checkleaf(std::string address);//メモリアドレスのリーフラベルを取得する関数
	void updatelabel(std::string address);//アドレスのラベルを更新する関数
};



