#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include<map>
#include<random>
#include<list>
#include<algorithm>

//宣言
double PathL;										//二分木の高さの数
double PathZ;										//バケツ内のブロック数
double PathN;										//パス内のブロック数
double Label;										//ラベルの数
std::string trace;									//トレース1行分
std::map<std::string, int >mp;								//文字列→整数の連想配列
struct Block										//データブロック宣言
	{
		std::string addr;							//アドレス
		int label;								//リーフラベル
		Block *blockAddr;							//ブロックの自アドレス		
	};
struct SNode										//ノードの宣言
	{	
		std::string hand;							//left or right
		SNode *pChild[2];							//子バケツのアドレス
		SNode *pRoot;								//親バケツのアドレス
		SNode *myAddr;								//バケツの自アドレス（child_listに入れる前のアドレス）
		Block *block;								//バケツ内のブロックのアドレス
	};
std::list<Block> stash_list;
std::list<SNode> child_list;
SNode root;

class Perse{										//Perceクラス
public:
//変数	
	std::ifstream ifs;
	std::string a[3];
	std::string str;
	char del;
	std::vector<std::string> subStr;
//関数
	void fileopen(std::string filename);						//入力トレースを読み込む関数

	void cut(std::string);								//入力からアドレス、命令タイプ、時刻を切り出す関数
};

class PosMap{										//PosMapクラス
public:
//変数
	int OldLabel,Label,NewLabel;
	std::map<std::string, int >mp;     						//文字列→整数の連想配列
	
//関数
	int checkleaf(std::string address);						//メモリアドレスのリーフラベルを取得する関数
	void updatelabel(std::string address);						//アドレスのラベルを更新する関数
};

class PathTree{										//PathTreeクラス
public:
//変数

	int depth;									//Pathmake 
	int numberblock;								//Pathmake InitNode AllocateChild TraceToRoot
	SNode updateNode;								//InitNode 
	SNode tmpNode;									//InitNode Allocatechild
	std::string address;								//PathWrite TraceToRoot
	int leaflabel;									//PathWrite TraceToRoot
	int GB;										//PathUpdate
	SNode checkNode;								//Leftchceker Rightchecker

//関数
	void Pathmake(int depth,std::list<SNode> &child_list,int numberblock);		//ツリーを作成する関数
	void InitNodeL(SNode& updateNode,SNode& tmpNode,int numberblock);		//ノードの初期化関数
	void InitNodeR(SNode& updateNode,SNode& tmpNode,int numberblock);		//ノードの初期化関数
	void AllocateChild(SNode* tmpNode,std::list<SNode> &child_list,int numberblock);//２つの子を生成する関数
	void PathWrite(std::string address,int leaflabel,std::list<SNode> &child_list,int numberblock);							//初パス書き込み関数
	void TraceToRoot(int leaflabel,std::list<Block> &stash_list,std::list<SNode> &child_list,int numberblock,std::string address,int NewLabel);	//パス読み込み関数
	void PathUpdate(int leaflabel,int numberblock,int GB,std::list<SNode> &child_list,std::list<Block> &stash_list);				//パス更新する関数
	int Leftchecker(SNode* checkNode);						//パス書き込みで各ノードに格納可能な左側リーフラベルを調べる関数
	int Rightchecker(SNode* checkNode);						//パス書き込みで各ノードに格納可能な右側リーフラベルを調べる関数
};

