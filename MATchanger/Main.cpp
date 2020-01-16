#include"main.h"


//ファイルを読み込む関数
void Perse::fileopen(std::string filename){
	
	std::ifstream ifs(filename);

	if(ifs.fail()){
		std::cout<<"ファイルオープンに失敗しました"<<std::endl;
	}
	else{
		std::cout<<"ここ入ってる"<<std::endl;
	}
	
//	return = file;
}



//行トレースをsplitしてまう関数
std::vector<std::string>split(std::string str, char del){
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



//入力トレースからアドレス、命令タイプ、時刻を切り出す関数
void Perse::cut(std::string access){
	
	//std::cout<<access<<std::endl;

	//分割するために1行トレースを調整
	if(access[15]!=' '){
		access.erase(access.begin() +17);//行トレースの17番目の文字を削除
	}
	else{
		access.erase(access.begin() +15);//行トレースの15番目を削除
	}
	del=' ';
	subStr = split(access,del);	
	//std::cout<<subStr.front()<<std::endl;
	for(auto itr = subStr.begin(); itr != subStr.end(); ++itr){
		//std::cout << *itr << '\n';
		a[itr - subStr.begin()] = *itr;
		std::string address = a[0];     //実際のアドレス
		std::string type = a[1];        //実際の命令タイプ
		std::string cycle = a[2];       //実際のサイクル時間
	}
}

//メモリアドレスのリーフラベルを取得する関数
/**
int checkleaf(std::string address){
	int OldLabel = mp[address];
}
**/


int main(){

	
//*****入力パラメータの宣言
	std::cout << "二分木の高さを入力してください"<< std::endl; 
	std::cin >> PathL;
	std::cout << "バケツ内のブロック数を入力してください"<< std::endl; 
	std::cin >> PathZ;
	Label =pow(2,PathL);	

//クラスのインスタンス化
	Perse perse;
	PosMap positionmap;	


//*****2分木を作成する関数
	

//*****入力トレースを読み込む関数
	std::ifstream ifs("mase.txt");// ファイルの読み込み
	if (ifs.fail())
	{
		std::cerr << "失敗" << std::endl;
		return -1;
	}
/////////////////////////////////ループ開始部////////////////////////////////////////////////////////////
	while(std::getline(ifs,trace))//1行ずつ読み込み
	{
		std::cout << "[" << trace << "]" << std::endl;//1行ずつ標準出力

//*****入力からアドレス、命令タイプ、時刻を切り出す関数	
	
		perse.cut(trace);

//perse.a[0] アドレス ,perse.a[1] 命令タイプ ,perse.a[2] サイクル数
//*****メモリアドレスのリーフラベルを取得する関数
		//std::map<std::string, int >mp;//文字列→整数の連想配列
		std::cout<<"["<<mp[perse.a[0]]<<"]"<<std::endl;
		positionmap.OldLabel= mp[perse.a[0]];//メモリアドレスのリーフラベルを取得する関数
		std::cout<<"OldLabelは"<<positionmap.OldLabel<<std::endl;

//*****アドレスのラベルを更新する関数
		
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> randLabel(1,Label);
		mp[perse.a[0]]=randLabel(mt); //アドレスにランダムにラベル登録
		
		std::cout <<mp[perse.a[0]]<<"\n";

	



	}
	return 0;

}

