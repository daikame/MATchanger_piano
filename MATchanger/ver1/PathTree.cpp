#include<iostream>
#include<list>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
//#include "Path.cpp"

#define Fname "/home/dai/git/MATchanger_piano/MATchanger/ver1/output_trace"
//関数プロトタイプ宣言


struct Block  									//データブロック宣言
   {
        std::string addr; 							//アドレス
        int label;        							//リーフラベル
 	Block *blockAddr;       						//ブロックの自アドレス
   };

struct SNode									//ノードの宣言
   {
        std::string hand; 							//left or right
        SNode *pChild[2]; 							//子バケツのアドレス
        SNode *pRoot;     							//親バケツのアドレス
        SNode *myAddr;    							//バケツの自アドレス（child_listに入れる前のアドレス）
        Block *block;     							//バケツ内のブロックのアドレス
   };

//ノードの初期化関数
void InitNodeL(SNode& updateNode,SNode& tmpNode,int numberblock){
   updateNode.pRoot=tmpNode.myAddr;						//子ノードの親のアドレス
   updateNode.myAddr=&updateNode;						//自アドレスにそのノードの実アドレスを格納
   for(int t=0;t<numberblock;t++){						//ノード内のブロック数分繰り返し
   	updateNode.block[t].addr="0";						//子ノードのアドレスの初期化
   	updateNode.block[t].label=0;						//子ノードのラベルの初期化
	updateNode.block[t].blockAddr=&updateNode.block[t];			//自アドレスにそのデータブロックの実アドレスを格納
	updateNode.hand = "left";						//左の手から生まれた子
   }
}
void InitNodeR(SNode& updateNode,SNode& tmpNode,int numberblock){
   updateNode.pRoot=tmpNode.myAddr;						//子ノードの親のアドレス
   updateNode.myAddr=&updateNode;						//自アドレスにそのノードの実アドレスを格納
   for(int t=0;t<numberblock;t++){						//ノード内のブロック数分繰り返し
   	updateNode.block[t].addr="0";						//子ノードのアドレスの初期化
   	updateNode.block[t].label=0;						//子ノードのラベルの初期化
	updateNode.block[t].blockAddr=&updateNode.block[t];			//自アドレスにそのデータブロックの実アドレスを格納
	updateNode.hand = "right";						//右の手から生まれた子
   }
}

// ２つの子を生成する関数
void AllocateChild(SNode* tmpNode,std::list<SNode> &child_list,int numberblock)
{
   //子バケツのメモリ領域の確保
   tmpNode->pChild[0] = new SNode;
   tmpNode->pChild[1] = new SNode;

   //子バケツ内のブロックのメモリ領域をnumberblock分、動的に確保
   tmpNode->pChild[0]->block = new Block[numberblock];
   tmpNode->pChild[1]->block = new Block[numberblock];

   std::cout<<"list挿入前"<<tmpNode->pChild[0]<<std::endl;
 
   SNode node1,node2,node3;

   InitNodeL(*tmpNode->pChild[0],*tmpNode,numberblock);				//初期化関数　L
   InitNodeR(*tmpNode->pChild[1],*tmpNode,numberblock);				//初期化関数　R
   //std::cout<<"init前"<<tmpNode->pChild[0]<<std::endl;
   //std::cout<<"init後"<<tmpNode->pChild[0]<<std::endl;

   child_list.push_back(*tmpNode->pChild[0]);					//子供リストの後ろに子供追加
   child_list.push_back(*tmpNode->pChild[1]);					//子供リストの後ろに子供追加
   child_list.pop_front();							//先頭の親を削除
   std::cout<<"list挿入後"<<&child_list.front()<<std::endl;
   std::cout<<"in Allocate "<<child_list.front().pRoot<<std::endl;
}

//パス読み込み関数 
void TraceToRoot(int leaflabel,std::list<Block> &stash_list,std::list<SNode> &child_list,int numberblock,std::string address,int NewLabel){	//本物のリクエストのアドレスはスタッシュに読み込む際、ラベルを新しいものに変える
   
   std::ofstream ofs(Fname, std::ios::app);					//ファイルオープン 
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
   SNode* tmpnode = itr->myAddr;        					//*tmpnodeに読み込みたいリーフノードのアドレスを渡している

   //スタッシュに繰り返しデータを入れるwhile文
   Block  tmpblock;								//ブロックの宣言
   int cleancounter=0;
   int stashin=0;								//スタッシュに格納した回数
   while(tmpnode != NULL){							//スタッシュに繰り返し
	for(int p = numberblock-1;0<=p;p--){
		tmpblock.addr = tmpnode->block[p].addr;				//tmpblockに今操作しているブロックのアドレス情報を渡す
		tmpblock.label = tmpnode->block[p].label;			//tmpblockに今操作しているブロックのラベル情報を渡す

		if(tmpblock.addr==address){					//要求されたリクエストのアドレスをスタッシュに読み込む前に、ラベルを更新する
			tmpblock.label=NewLabel;
		}

		//アクセス中のパスを空っぽにするための操作
		tmpnode->block[p].addr="0";					//パス内のブロックのアドレスとラベルを０にしてブロックを空にする
		if(cleancounter!=0){							//葉ノード以外のノードのブロックのラベルは0にする
	      		tmpnode->block[p].label=0; 	
		}
         
		if(tmpblock.addr != "0"){					//ダミー以外のデータのパス読み込み（アドレスが0のものはダミーであることを明記する）	
		ofs <<tmpblock.addr<<" READ"<<std::endl;			//トレースファイルの出力
		stash_list.push_back(tmpblock);					//スタッシュにデータブロックを格納していく
		stashin++;
		}
   	}
	tmpnode = tmpnode->pRoot;						//親ノードに移動
	cleancounter++;	
   }

  auto itr1 = stash_list.begin();						//itr1をスタッシュの先頭に設置(確認用?)
  for(int v=0;v<stashin-1;v++){							//スタッシュに格納した分イテレータを移動している
	itr1++;
  }  
  ofs.close();									//ファイルクローズ
}

//パス書き込みで各ノードに格納可能な左側リーフラベルを調べる関数
int Leftchecker(SNode* checkNode){
   SNode *tmp_node1 = checkNode;			
   int mostleftlabel; 								//最も左にあるラベルを見つけるための宣言
   mostleftlabel = tmp_node1->block[0].label;
   while(tmp_node1->pChild[0] != NULL){
      tmp_node1 = tmp_node1->pChild[0];
      mostleftlabel=tmp_node1->block[0].label;					//ここでブロックは0でも1でもよい  ０のほうが無難
   }
	return mostleftlabel;
}

//パス書き込みで各ノードに格納可能な右側リーフラベルを調べる関数
int Rightchecker(SNode* checkNode){
   SNode *tmp_node2 = checkNode;
   int mostrightlabel;								//最も右にあるラベルを見つけるための宣言
   mostrightlabel = tmp_node2->block[0].label;
   while(tmp_node2->pChild[1] != NULL){
      tmp_node2 = tmp_node2->pChild[1];
      mostrightlabel=tmp_node2->block[0].label;					//ここでブロックは0でも1でもよい
   }
   	return mostrightlabel;
}

//初パス書き込み
void PathWrite(std::string address,int leaflabel,std::list<SNode> &child_list,int numberblock){	

   auto itr = child_list.begin();						//leaflabelのノードにアクセスするためにイテレータの移動
   for(int s=1;s<leaflabel;s++){
	++itr;
   }
   //リーフノードから各パスの空いている部分にデータを格納
   SNode* tmpnode = itr->myAddr; 						//*tmpnodeに現在のリーフノードのアドレスを渡している
   
   int p=0;									//ブロック番号
   int s=0;									//
   while(tmpnode != NULL){
	for(p = numberblock-1;0<=p;p--){
		std::cout<<"as"<<std::endl;
		if(tmpnode->block[p].addr=="0"){
			tmpnode->block[p].addr = address;
			tmpnode->block[p].label = leaflabel; 
			s=p;
			std::cout<<s<<std::endl;
        		std::cout<<"[]"<<tmpnode->block[s].addr<<std::endl;
			break;							//ちっちゃいfor文抜ける
		}
	}
   	if(tmpnode->block[s].addr==address){
		break;
  	}
	std::cout<<"upnode"<<std::endl;
	tmpnode = tmpnode->pRoot;						//ひとつ上のノードに移動
   }
}


//パス更新									//パスツリーに詰め直す関数
void PathUpdate(int leaflabel,int numberblock,int GB,std::list<SNode> &child_list,std::list<Block> &stash_list){
   auto itr = child_list.begin();						//leaflabelのノードにアクセスするためにイテレータの移動
   for(int s=1;s<leaflabel;s++){
	++itr;
   }
   int tmpblocknum=numberblock-1;						//書くノードのブロック番号
   SNode* tmpnode = itr->myAddr;        					//*tmpnodeに現在のリーフノードのアドレスを渡している
   while(tmpnode != NULL){							//各ノードにデータブロックを詰め込んでいる
   	int leftside = Leftchecker(tmpnode);					//現在のノードのブロックに格納できるラベルを調べる
    	int rightside = Rightchecker(tmpnode);					//現在のノードのブロックに格納できるラベルを調べる
	std::cout<<"leftside"<<leftside<<std::endl;
	std::cout<<"rightside"<<rightside<<std::endl;

    	////スタッシュの先頭からあるラベルのブロックを抽出して詰める
    	std::list<Block> possibleblock_list;					//抽出されたブロックを格納するリスト
	std::cout<<"スタッシュサイズ"<<stash_list.size()<<std::endl;
   	for(auto itr1 = stash_list.begin();itr1 != stash_list.end();++itr1){      
		std::cout<<"stashにあるやつ"<<itr1->addr<<std::endl;
		if(leftside<=itr1->label && itr1->label<=rightside && tmpnode->block[tmpblocknum].addr=="0"){				//stash_listから取り出したブロックのラベルがleftside以上rightside以下であるか否か
			std::cout<<"doko"<<&itr1->addr<<std::endl;
			tmpnode->block[tmpblocknum].addr = itr1->addr;		//現在のブロックにスタッシュから格納
			tmpnode->block[tmpblocknum].label = itr1->label;	//現在のブロックにスタッシュから格納
			itr1=stash_list.erase(itr1);				//スタッシュから削除して,itr1は次の要素を指す
			--itr1;							//itr1が正しく動くように
			
			if(tmpblocknum==0){					//現在のブロックの番号が0ならfor文から抜ける
				std::cout<<"このノードにはもう格納できません"<<std::endl;
				break;
			}
			else{
			tmpblocknum--;
			}
		}
	}
  	if(tmpnode == NULL){
		break;
   	}
	std::cout<<"上に参りまーす"<<std::endl;
	tmpnode = tmpnode->pRoot;						//現在のノードの親ノードに移る
   	tmpblocknum =numberblock-1;						//上のノードに上がるのでブロックナンバーを初期化
        std::cout<<tmpblocknum<<std::endl;
  	if(tmpnode == NULL){
		break;
   	}
   }
//***********************空ダミーを作成するフェーズ************////////
	//空ブロックの検索
   	auto itr3 = child_list.begin();						//leaflabelのノードにアクセスするためにイテレータの移動
   	for(int rt=1;rt<leaflabel;rt++){
		++itr3;
   	}
   	SNode* tmpnodeE = itr3->myAddr;        					//*tmpnodeに現在のリーフノードのアドレスを渡している
	while(tmpnodeE != NULL){
		for(int as=numberblock-1;0<=as;as--){				//現在のブロックが空で
			if(tmpnodeE->block[as].addr == "0" ){			//このブロックは空であるか否か(アドレスが0であるかいなかで空かどうか判断している)
				if(GB==4){					//4GB
					std::string random;			//ランダム変数
				 	tmpnodeE->block[as].addr= "0x";
					std::random_device rnd;
					std::mt19937 mt(rnd());
					std::uniform_int_distribution<> randLabel(0,15);
					for(int addr_size=0;addr_size<8;addr_size++){		//アドレス空間の大きさによってyuは変化する
						//変数randomの乱数生成
						int num_random = randLabel(mt);
						if(num_random==0){
							random = "0";
						}
                                                else if(num_random==1){
							random = "1";
						}
                                                else if(num_random==2){
							random = "2";
						}
                                                else if(num_random==3){
							random = "3";
						}
                                                else if(num_random==4){
							random = "4";
						}
                                                else if(num_random==5){
							random = "5";
						}
                                                else if(num_random==6){
							random = "6";
						}
                                                else if(num_random==7){
							random = "7";
						}
                                                else if(num_random==8){
							random = "8";
						}
                                                else if(num_random==9){
							random = "9";
						}
                                                else if(num_random==10){
							random = "A";
						}
                                                else if(num_random==11){
							random = "B";
						}
                                                else if(num_random==12){
							random = "C";
						}
                                                else if(num_random==13){
							random = "D";
						}
                                                else if(num_random==14){
							random = "E";
						}
                                                else if(num_random==15){
							random = "F";
						}
						tmpnodeE->block[as].addr=tmpnodeE->block[as].addr+random;
					}
				}						//if(GB==4)
			else if(GB==2){						//2GB
			}							
		else{}
		}								 
	}									
	tmpnodeE =tmpnodeE->pRoot;						//現在のノードを親にする
   }
}

//ツリーの作成
void Pathmake(int depth,std::list<SNode> &child_list,int numberblock){
   auto itr =child_list.begin(); 
   for(int level=0;level<depth;level++){					//２分木の深さ
  	int numparent = child_list.size();       				//世代の親の数
	std::cout<<"世代"<<numparent<<std::endl;
     	for(int o=0;o<numparent;o++){             				 //現レベルごとの子供を産む
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

int main() {
   // Initialize tree
   int num = 0;
   int numberblock = 2;								//root.blockのnewする数に対応
   int depth=4;
   SNode root;
   SNode *tmpNode;
   root.block= new Block[2];
   
   std::list<SNode> child_list;
   std::list<Block> stash_list;

   root.block[1].addr="10";
   root.block[1].label = 5;
   root.block[0].addr="12";
   root.block[0].label = 3;
   root.block[0].blockAddr=&root.block[0];
   root.block[1].blockAddr=&root.block[1];
   root.pRoot = nullptr;
   root.myAddr = &root;
   child_list.push_back(root);
/////*******二分木のもとを作成******///////
 
   Pathmake(depth,child_list,numberblock);
   //２分木テスト（６番目の葉っぱに書き込みと読み込み）
   PathWrite("dada",1,child_list,numberblock);
   PathWrite("dame",1,child_list,numberblock); 
   PathWrite("daga",1,child_list,numberblock); 
   
   TraceToRoot(2,stash_list,child_list,2,"daga",4); 
   auto itr1 = stash_list.begin();
   for(int v=0;v<0;v++){
	itr1++;
   }
   int dpp=3;
   dpp = Leftchecker(child_list.back().myAddr);
   PathUpdate(2,2,4,child_list,stash_list);
   auto itr2 = child_list.begin();
   std::cout<<"dagaきてくれ"<<itr2->myAddr->pRoot->pRoot->pRoot->pRoot->block[0].addr<<std::endl;
   std::cout<<"dagaきてくれ"<<itr2->myAddr->pRoot->pRoot->block[1].addr<<std::endl;
   std::cout<<"dagaきてくれ"<<itr2->myAddr->pRoot->pRoot->pRoot->block[0].addr<<std::endl;
   
   return 0;
   // Add value
}
