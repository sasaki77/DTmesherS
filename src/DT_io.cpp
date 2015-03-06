#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>

#include "DT.h"

#define BUFF_SIZE (1024)

enum READ_STATE{
	STATE_DEFAULT,
	STATE_PARAMS,
	STATE_NODE,
	STATE_EDGE,
	STATE_BND,
};

static bool isCommentLine( char * );
static vector<string> parseStringLine( char * );
static Node* createNewNodeFromVs( const vector<string>& , double );
static Edge* createNewEdgeFromVs( const vector<string>& , double, const vector<Node*>& );

using namespace std;


bool DT::inputParam() 
{
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
                    {
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
                    }

                case STATE_NODE:
                    {
                        // ノード情報の入力
                        Node* new_node = createNewNodeFromVs( vs, scale );
                        nodes.push_back( new_node );
                        break;
                    }

                case STATE_EDGE:
                    {
                        Edge* new_edge = createNewEdgeFromVs( vs, scale, nodes );
                        edges.push_back( new_edge );
                    }
            }
        }
    }

	return true;
}

bool DT::outputResult()
{
    // 節点要素関係ファイル
    ofstream ofs_elem( ( outfile_name + ".elem" ).c_str() );

    ofs_elem << form << endl;

    // 全節点数の算出
    int nodenum = 0;
    for( unsigned int i=0; i<nodes.size(); i++ )
        if( !nodes[i]->isSuperNode() ) nodenum++;

    // 全要素数
    int elem_num = 0;
    for( unsigned int i=0; i<triangles.size(); i++ )
        if( triangles[i]->getExist() ) elem_num++;

    ofs_elem << nodenum  << endl;
    ofs_elem << elem_num << endl;

    // 節点情報の出力
    int supernode_num = 0;
    for( unsigned int i=0; i<nodes.size(); i++ ){
        if( nodes[i]->isSuperNode() ){
            supernode_num++;
            continue;
        }

        nodes[i]->setNum( nodes[i]->getNum() - supernode_num );
        ofs_elem << nodes[i]->getNum() << ' ';

        if( fabs(nodes[i]->getX() ) < DT::kOutEps ) {
            ofs_elem << 0 << ' ';
        }else{
            ofs_elem << nodes[i]->getX() << ' ';
        }

        if( fabs(nodes[i]->getY() ) < DT::kOutEps ){
            ofs_elem << 0 << endl;
        }else{
            ofs_elem << nodes[i]->getY() << endl;
        }
    }

    // 要素情報の出力
    int tn = 0;
    for(unsigned int i=0; i<triangles.size(); i++){
        if( triangles[i]->getExist() ){
            tn++;
            ofs_elem << tn << ' ' << triangles[i]->getNode( 0 )->getNum()
                           << ' ' << triangles[i]->getNode( 1 )->getNum()
                           << ' ' << triangles[i]->getNode( 2 )->getNum() << endl;
        }
    }

    ofstream ofs_bc((outfile_name+".bc").c_str());

    // 境界条件ファイル
    ofs_bc << "$begin_bc" << endl;
    //for( unsigned int i=0; i<edges.size(); i++){
    //    if( edges[i]->bc.getBC() != COND_NEUMANN ){
    //        edges[i]->p[0]->bc  = edges[i]->p[1]->bc  = edges[i]->bc;
    //        edges[i]->p[0]->val = edges[i]->p[1]->val = edges[i]->val;
    //    }
    //}

    //for(unsigned int i=0; i<nodes.size(); i++)
    //    if(nodes[i]->bc == 1)
    //        ofs_bc << nodes[i]->number << " " << nodes[i]->val << endl;
    ofs_bc << "$end" << endl << endl;

    // 材料ファイル
    //vector< StraightEdge > surf;
    //for( unsigned int i=0; i<edges.size(); i++ ){
    //    if( edges[i]->material.getMaterial() != "" && edges[i]->material.getMaterial() != "$none" ){
    //        StraightEdge e( edges[i]->p[0], edges[i]->p[1] );
    //        e.material = edges[i]->material.getMaterial();
    //        surf.push_back(e);
    //    }
    //}

    ofs_bc << "$begin_material" << endl;
    //ofs_bc << surf.size()       << endl;

    //for(unsigned int i=0;i<surf.size();i++){
    //    ofs_bc << surf[i].p[0]->number << " "
    //        << surf[i].p[1]->number << " " 
    //        << surf[i].material << endl;
    //}
    ofs_bc << "$end" << endl;
}

static bool isCommentLine( char* line )
{
    if( line[0] == '#'  || line[0] == '\n' ||
			line[0] == '\r' || line[0] == '\0'){
		return true;
	}

	return false;
}

static vector<string> parseStringLine( char* line )
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

static Node* createNewNodeFromVs( const vector<string>& vs, double scale )
{
    double px = scale * atof( vs[1].c_str() );
    double py = scale * atof( vs[2].c_str() );
	Node* new_node = new Node( px, py );
	new_node->setIsOnBoundary( true );
    return new_node;
}

static Edge* createNewEdgeFromVs( const vector<string>& vs, double scale, const vector<Node*>& nodes)
{
    Edge* new_edge;

    // 線の種類
    if(vs[3] == "$str"){
        Node* node1 = nodes[ atoi( vs[1].c_str() )-1 ];
        Node* node2 = nodes[ atoi( vs[2].c_str() )-1 ];
        new_edge = new StraightEdge( node1, node2 ) ;
    }

    //else if(vs[3] == "$cir"){
    //    parseStrWithComma( vs[4],varg );
    //    edge.push_back( new CircleEdge( node[atoi(vs[1].c_str())-1],
    //                node[atoi(vs[2].c_str())-1], 
    //                scale * atof(varg[1].c_str()),
    //                scale * atof(varg[2].c_str()),
    //                atof(varg[3].c_str())          ));
    //}

    // 境界条件
    if(vs[5] == "$dirichlet"){
        double val = atof(vs[6].c_str());
        new_edge->bc.setBCandVal( COND_DIRICHLET, val );
    }else if(vs[5] == "$neumann"){
        new_edge->bc.setBCandVal( COND_NEUMANN, 0 );
    }

    // 表面の材料
    if(vs[7] == "$Cu" || vs[7] == "$none"){
        new_edge->material.setMaterial( vs[7].c_str() );
    }

    return new_edge;
}
