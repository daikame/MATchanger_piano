#include"main.h"
#define Fname "/home/dai/git/MATchanger_piano/MATchanger/ver1/output_trace"


void Perse::fileopen(std::string filename){					//ファイルを読み込む関数	
	std::ifstream ifs(filename);
	if(ifs.fail()){
		std::cout<<"ファイルオープンに失敗しました"<<std::endl;
	}
	else{
		std::cout<<"ここ入ってる"<<std::endl;
	}
	//	return = file;
}


std::vector<std::string>split(std::string str, char del){			//行トレースを分割する関数
	int first =0;	
	int last = str.find_first_of(del);
	std::vector<std::string> result;
	while(first < str.size())
	{
		std::string subStr(str,first,last-first);
		result.push_back(subStr);
		first = last + 1;
		last = str.find_first_of(del,first);
		if(last == std::string::npos)
		{
			last = str.size();
		}
	}
	return result;
}	

void Perse::cut(std::string access){						//入力トレースからアドレス、命令タイプ、時刻を切り出す関数
	
	//分割するために1行トレースを調整//
	if(access[15]!=' '){
		access.erase(access.begin() +17);				//行トレースの17番目の文字を削除
	}
	else{
		access.erase(access.begin() +15);				//行トレースの15番目を削除
	}
	del=' ';
	subStr = split(access,del);
	for(auto itr = subStr.begin(); itr != subStr.end(); ++itr){
		a[itr - subStr.begin()] = *itr;
		std::string address = a[0];     				//実際のアドレス
		std::string type = a[1];       					//実際の命令タイプ
		std::string cycle = a[2];       				//実際のサイクル時間
	}
}

//ノードの初期化関数
void PathTree::InitNodeL(SNode& updateNode,SNode& tmpNode,int numberblock){
   updateNode.pRoot=tmpNode.myAddr;                                             //子ノードの親のアドレス
   updateNode.myAddr=&updateNode;                                               //自アドレスにそのノードの実アドレスを格納
   for(int t=0;t<numberblock;t++){                                              //ノード内のブロック数分繰り返し
        updateNode.block[t].addr="0";                                           //子ノードのアドレスの初期化
        updateNode.block[t].label=0;                                            //子ノードのラベルの初期化
        updateNode.block[t].blockAddr=&updateNode.block[t];                     //自アドレスにそのデータブロックの実アドレスを格納
        updateNode.hand = "left";                                               //左の手から生まれた子
   }
}
void PathTree::InitNodeR(SNode& updateNode,SNode& tmpNode,int numberblock){
   updateNode.pRoot=tmpNode.myAddr;                                             //子ノードの親のアドレス
   updateNode.myAddr=&updateNode;                                               //自アドレスにそのノードの実アドレスを格納
   for(int t=0;t<numberblock;t++){                                              //ノード内のブロック数分繰り返し
        updateNode.block[t].addr="0";                                           //子ノードのアドレスの初期化
        updateNode.block[t].label=0;                                            //子ノードのラベルの初期化
        updateNode.block[t].blockAddr=&updateNode.block[t];                     //自アドレスにそのデータブロックの実アドレスを格納
        updateNode.hand = "right";                                              //右の手から生まれた子
   }
}

// ２つの子を生成する関数
void PathTree::AllocateChild(SNode* tmpNode,std::list<SNode> &child_list,int numberblock)
{
   //子バケツのメモリ領域の確保
   tmpNode->pChild[0] = new SNode;
   tmpNode->pChild[1] = new SNode;

   //子バケツ内のブロックのメモリ領域をnumberblock分、動的に確保
   tmpNode->pChild[0]->block = new Block[numberblock];
   tmpNode->pChild[1]->block = new Block[numberblock];

   std::cout<<"list挿入前"<<tmpNode->pChild[0]<<std::endl;

   SNode node1,node2,node3;

   InitNodeL(*tmpNode->pChild[0],*tmpNode,numberblock);                         //初期化関数　L
   InitNodeR(*tmpNode->pChild[1],*tmpNode,numberblock);                         //初期化関数　R
   //std::cout<<"init前"<<tmpNode->pChild[0]<<std::endl;
   //std::cout<<"init後"<<tmpNode->pChild[0]<<std::endl;

   child_list.push_back(*tmpNode->pChild[0]);                                   //子供リストの後ろに子供追加
   child_list.push_back(*tmpNode->pChild[1]);                                   //子供リストの後ろに子供追加
   child_list.pop_front();                                                      //先頭の親を削除
   std::cout<<"list挿入後"<<&child_list.front()<<std::endl;
   std::cout<<"in Allocate "<<child_list.front().pRoot<<std::endl;
}


//メモリアドレスのリーフラベルを取得する関数
/**
int checkleaf(std::string address){
	int OldLabel = mp[address];
}
**/

void PathTree::Pathmake(int depth,std::list<SNode> &child_list,int numberblock){
   auto itr =child_list.begin();
   for(int level=0;level<depth;level++){                                        //２分木の深さ
        int numparent = child_list.size();                                      //世代の親の数
        std::cout<<"世代"<<numparent<<std::endl;
        for(int o=0;o<numparent;o++){                                            //現レベルごとの子供を産む
                AllocateChild(child_list.front().myAddr,child_list,numberblock);
                std::cout<<child_list.size()<<std::endl;
        }
   }

   //葉ノードにそれぞれラベルを書き込んでいる
   int i =1;
   for(auto itr = child_list.begin(); itr != child_list.end(); ++itr) {
        for(int s=0;s<numberblock;s++)
        {
                itr->myAddr->block[s].label=i;
        }
        i++;
   }
}

//初パス書き込み
void PathTree::PathWrite(std::string address,int leaflabel,std::list<SNode> &child_list,int numberblock){

   auto itr = child_list.begin();                                               //leaflabelのノードにアクセスするためにイテレータの移動
   for(int s=1;s<leaflabel;s++){
        ++itr;
   }
   //リーフノードから各パスの空いている部分にデータを格納
   SNode* tmpnode = itr->myAddr;                                                //*tmpnodeに現在のリーフノードのアドレスを渡している

   int p=0;                                                                     //ブロック番号
   int s=0;                                                                     //
   while(tmpnode != NULL){
        for(p = numberblock-1;0<=p;p--){
                //std::cout<<"as"<<std::endl;
                if(tmpnode->block[p].addr=="0"){
                        tmpnode->block[p].addr = address;
                        tmpnode->block[p].label = leaflabel;
                        s=p;
                        std::cout<<s<<std::endl;
                        std::cout<<"[]"<<tmpnode->block[s].addr<<std::endl;
                        break;                                                  //ちっちゃいfor文抜ける
                }
        }
        if(tmpnode->block[s].addr==address){
                break;
        }
        std::cout<<"ひとつ上のノードで書き込めるか探します"<<std::endl;
        tmpnode = tmpnode->pRoot;                                               //ひとつ上のノードに移動
   }
}

//パス読み込み関数 
void TraceToRoot(int leaflabel,std::list<Block> &stash_list,std::list<SNode> &child_list,int numberblock,std::string address,int NewLabel){     //本物のリクエストのアドレスはスタッシュに読み込む際、ラベルを新しいものに変える

   std::ofstream ofs(Fname, std::ios::app);                                     //ファイルオープン 
   if(!ofs)
   {
        std::cout << "ファイルが開けませんでした。" << std::endl;
        //std::cin.get();
        return ;
   }
   //読み込みたいパスのリーフラベルのノードにアクセスするためにイテレータを移動
   auto itr = child_list.begin();
   for(int s=1;s<leaflabel;s++){
        ++itr;
   }
   SNode* tmpnode = itr->myAddr;                                                //*tmpnodeに読み込みたいリーフノードのアドレスを渡している
   //スタッシュに繰り返しデータを入れるwhile文
   Block  tmpblock;                                                             //ブロックの宣言
   int cleancounter=0;
   int stashin=0;                                                               //スタッシュに格納した回数
   while(tmpnode != NULL){                                                      //スタッシュに繰り返し
        for(int p = numberblock-1;0<=p;p--){
                tmpblock.addr = tmpnode->block[p].addr;                         //tmpblockに今操作しているブロックのアドレス情報を渡す
                tmpblock.label = tmpnode->block[p].label;                       //tmpblockに今操作しているブロックのラベル情報を渡す

                if(tmpblock.addr==address){                                     //要求されたリクエストのアドレスをスタッシュに読み込む前に、ラベルを更新する
                        tmpblock.label=NewLabel;
                }

                //アクセス中のパスを空っぽにするための操作
                tmpnode->block[p].addr="0";                                     //パス内のブロックのアドレスとラベルを０にしてブロックを空にする
                if(cleancounter!=0){                                                    //葉ノード以外のノードのブロックのラベルは0にする
                        tmpnode->block[p].label=0;
                }

                if(tmpblock.addr != "0"){                                       //ダミー以外のデータのパス読み込み（アドレスが0のものはダミーであることを明記する）     
                ofs <<tmpblock.addr<<" READ"<<std::endl;                        //トレースファイルの出力
                stash_list.push_back(tmpblock);                                 //スタッシュにデータブロックを格納していく
                stashin++;
                }
        }
        tmpnode = tmpnode->pRoot;                                               //親ノードに移動
        cleancounter++;
   }

  auto itr1 = stash_list.begin();                                               //itr1をスタッシュの先頭に設置(確認用?)
  for(int v=0;v<stashin-1;v++){                                                 //スタッシュに格納した分イテレータを移動している
        itr1++;
  }
  ofs.close();                                                                  //ファイルクローズ
}



int main(){

	
	//入力パラメータの宣言//
	std::cout << "二分木の高さを入力してください"<< std::endl; 
	std::cin >> PathL;
	std::cout << "バケツ内のブロック数を入力してください"<< std::endl; 
	std::cin >> PathZ;
	Label =pow(2,PathL);	

	//クラスのインスタンス化//
	Perse perse;
	PosMap positionmap;	
 	PathTree pathtree;

	//2分木を作成する関数//
	int num_blocks = PathZ;
	root.block = new Block[num_blocks];
	root.block[1].addr="10";						//根ノード関係
 	root.block[1].label = 5;						//↓
   	root.block[0].addr="12";						//↓
   	root.block[0].label = 3;						//↓
   	root.block[0].blockAddr=&root.block[0];					//↓
   	root.block[1].blockAddr=&root.block[1];					//↓
   	root.pRoot = nullptr;							//↓
   	root.myAddr = &root;							//↓
   	child_list.push_back(root);
	pathtree.Pathmake(PathL,child_list,PathZ);

	//入力トレースを読み込む関数//
	std::ifstream ifs("mase.txt");						// ファイルの読み込み
	if (ifs.fail())
	{
		std::cerr << "失敗" << std::endl;
		return -1;
	}
/////////////////////////////////ループ開始部//////////////////////////////////////////////////////
	while(std::getline(ifs,trace))						//1行ずつ読み込み
	{
		std::cout<<"----[命令]----"<<std::endl;
		std::cout << "[" << trace << "]" << std::endl;			//1行ずつ標準出力
		perse.cut(trace);						//入力からアドレス、命令タイプ、時刻を切り出す関数

	//*perse.a[0] アドレス ,perse.a[1] 命令タイプ ,perse.a[2] サイクル数
	//std::map<std::string, int >mp;//文字列→整数の連想配列
		std::cout<<"["<<mp[perse.a[0]]<<"]"<<std::endl;
		positionmap.OldLabel= mp[perse.a[0]];				//メモリアドレスのリーフラベルを取得する関数
		std::cout<<"OldLabelは"<<positionmap.OldLabel<<std::endl;

		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> randLabel(1,Label);
		mp[perse.a[0]]=randLabel(mt); 					//アドレスのラベルを更新する関数
		std::cout <<mp[perse.a[0]]<<"\n";
		positionmap.NewLabel = mp[perse.a[0]];
	//ポジションマップにアドレスが登録されているか確認
	if(positionmap.OldLabel==0){						//ポジションマップにリーフラベルが登録されていない場合
		std::cout<<"登録されていない"<<std::endl;
		//(アクセスされたアドレスのみを書き込む関数)
		pathtree.PathWrite(perse.a[0],positionmap.NewLabel,child_list,PathZ);						//パス内の1ノードのみを更新する関数
	}
	else{									//ポジションマップにリーフラベルが登録されている場合
		std::cout<<"登録済み"<<std::endl;
	}


	}
	return 0;

}

