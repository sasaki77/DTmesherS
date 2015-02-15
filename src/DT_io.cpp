#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>

#include "DT.h"

#define BUFF_SIZE 1024

enum READ_STATE{
	STATE_DEFAULT,
	STATE_PARAMS,
	STATE_NODE,
	STATE_EDGE,
	STATE_BND,
};

static bool isCommentLine( char *);
static vector<string> parseStringLine( char *);

using namespace std;


bool DT::inputParam() {
	cout << "input stage" << endl;

	ifstream ifs( ( infile_name + ".in" ).c_str() );
	if( ifs == NULL ){
		cout << "cannot open \"" << ( infile_name+".in" ) << "\"" << endl;
		exit(0);
	}else{
		cout << "open \"" << ( infile_name+".in" ) << "\"" << endl;
	}

	char line_buff[BUFF_SIZE];
	READ_STATE read_state = STATE_DEFAULT;

	while( ifs.getline( line_buff,BUFF_SIZE ) ){
		if( isCommentLine( line_buff ) ){
			continue;
		}

		// 入力情報の種類選別
		if(      (string)line_buff == "$begin_params"   ) read_state = STATE_PARAMS;
		else if( (string)line_buff == "$begin_node"     ) read_state = STATE_NODE;
		else if( (string)line_buff == "$begin_edge"     ) read_state = STATE_EDGE;
		else if( (string)line_buff == "$begin_boundary" ) read_state = STATE_BND;

		if( read_state == STATE_DEFAULT ) continue;

		// 入力情報の種類毎での情報の取得
		while( ifs.getline(line_buff,BUFF_SIZE) ){
			if( isCommentLine( line_buff ) ){
				continue;
			}

			vector< string > vs = parseStringLine( line_buff );

			if( ( (string) line_buff ).substr(0,4) == "$end") break;

			vector< string > varg; // デリミタ','による分割結果を格納する変数
			list<Node*>      p;    // 新規ノード
			//Boundary        *newb; // 新規境界
			static double    scale = 1;

			switch( read_state ){
				case STATE_PARAMS: 
					// 事前に必要なパラメータ情報の入力
					if(vs[0] == "$form"){
						form = atoi( vs[1].c_str() );
						//if(vs.size() > 2 && vs[2] == "-obl")
						//	// 偏平率分割が指定されている場合
						//	useOblDivide = true;
					}
					if(vs[0] == "$scale")    scale    = atof( vs[1].c_str() );
					//if(vs[0] == "$interval") interval = atof( vs[1].c_str() ) * scale;
					break;

				case STATE_NODE:
					// ノード情報の入力
					nodes.push_back( new Node(scale * atof(vs[1].c_str()),
								scale * atof(vs[2].c_str() ) ));
	
					//nodes.at( node.size()-1 )->isOnBnd = true;
					break;
			}
		}
	}

	return true;
}

static bool isCommentLine(char* line )
{
	if( line[0] == '#'  || line[0] == '\n' ||
			line[0] == '\r' || line[0] == '\0'){
		cout << line << endl;
		return true;
	}

	return false;
}

static vector<string> parseStringLine( char* line)
{
	// line = "This is Test"
	// vs   = ["This","is","Test"]
	
	stringstream     ss;
	vector< string > vs;
	char             minibuff[BUFF_SIZE];

	ss << (string) line;
	while( ss.getline(minibuff,BUFF_SIZE,' '))
		if( minibuff[0] != '\0')
			vs.push_back( (string) minibuff );

	return vs;
}