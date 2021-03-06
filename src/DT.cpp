#include <stdio.h>
#include <time.h>
#include <iostream>

#include <cfloat>
#include <cmath>

#include "DT.h"
#include "add_node_oblateness.h"

using namespace std;

// static private member
vector< Node* >     DT::nodes;
vector< Edge* >     DT::edges;
vector< Triangle* > DT::triangles;

static double getDet( double,double ,double ,double ,double ,double );
static double isSamePoint( Point*, Point* );
static bool   isCrossed( StraightEdge, StraightEdge);
static double distance( double, double, double, double );

bool DT::startDT() 
{
	cout << "start DT" << endl;
	inputParam();
	cout << "form = " << form << endl;

	for (int i = 0; i < nodes.size(); i++) {
		cout << i << ":(" << nodes[i]->getX() << "," << nodes[i]->getY() << ")" << endl;
		cout << "is_on_boundary = " << nodes[i]->getIsOnBoundary() << endl;
	}

	for (int i = 0; i < edges.size(); i++) {
		cout << i << ":length = "  << edges[i]->getLength() << ", bc = " << edges[i]->bc.getBC() << "," << edges[i]->bc.getVal()  << endl;
		cout << "material = " << edges[i]->material.getMaterial() << endl;
	}

	cout << "node normarize" << endl << endl;
    nodesNormalize();

	for (int i = 0; i < nodes.size(); i++) {
		cout << i << ":(" << nodes[i]->getX() << "," << nodes[i]->getY() << ")" << endl;
	}


	for (int i = 0; i < nodes.size(); i++) {
		cout << i << ":(" << nodes[i]->getX() << "," << nodes[i]->getY() << ")" << endl;
	}

	cout << "set SuperTriangle" << endl << endl;
    setSuperTriangle();

	cout << "generate Boundary start" << endl << endl;
	generateBoundary();
	for (int i = 0; i < nodes.size(); i++) {
		cout << i << ":(" << nodes[i]->getX() << "," << nodes[i]->getY() << ")" << endl;
	}
    for (int i=0; i < triangles.size(); i++) {
        for( int j=0; j<3; j++ ){
            Node* p = triangles[i]->getNode(j);
            cout << "p" << j << " = (" << p->getNum() << "," << p->getX() << "," << p->getY() << ")" << endl;
        }
        cout << "exist: " << triangles[i]->getExist() << endl;
    }

	cout << "remove illegal triangles start" << endl << endl;
    removeIllegalTriangles();

	generateFineMesh();

	cout << "node denormalize start" << endl << endl;
    nodesDenormalize();

	cout << "remove illegal triangles start" << endl << endl;
    removeIllegalTriangles();

	cout << endl << "laplace start" << endl;
	laplace();

	cout << "output result start" << endl << endl;
    outputResult();

	return true;
}


void DT::nodesNormalize()
{
    cout << "start normalize" << endl;

    max_point.set( -DBL_MAX, -DBL_MAX );
    min_point.set( DBL_MAX, DBL_MAX );

    for( unsigned int i=0; i<nodes.size(); i++ ){
        double x = nodes[i]->getX();
        double y = nodes[i]->getY();
        if( min_point.getX() > x ) min_point.setX( x );
        if( min_point.getY() > y ) min_point.setY( y );
        if( max_point.getX() < x ) max_point.setX( x );
        if( max_point.getY() < y ) max_point.setY( y );
    }

    double x_dis = max_point.getX() - min_point.getX();
    double y_dis = max_point.getY() - min_point.getY();
    max_distance = (x_dis > y_dis) ? x_dis:y_dis ;

    for( unsigned int i=0; i<nodes.size(); i++ ){
        nodes[i]->setX( ( nodes[i]->getX() - min_point.getX() ) / max_distance );
        nodes[i]->setY( ( nodes[i]->getY() - min_point.getY() ) / max_distance );
    }

    //for(unsigned int i=0; i<edge.size(); i++){
    //    if( typeid(*edge[i]) == typeid(CircleEdge) ){
    //        CircleEdge *c       = dynamic_cast<CircleEdge*>(edge[i]);
    //        c->center.x  = (c->center.x-minx) / square;
    //        c->center.y  = (c->center.y-miny) / square;
    //        c->r        /= square;
    //    }
    //}
}

void DT::nodesDenormalize()
{
    cout << "start denormalize" << endl;
    for( unsigned int i=0; i<nodes.size(); i++ ){
        nodes[i]->setX( nodes[i]->getX() * max_distance + min_point.getX() );
        nodes[i]->setY( nodes[i]->getY() * max_distance + min_point.getY() );
    }

    //for(unsigned int i=0; i<edge.size(); i++){
    //    if( typeid(*edge[i]) == typeid(CircleEdge) ){
    //        CircleEdge *c       = dynamic_cast<CircleEdge*>(edge[i]);
    //        c->center.x  = c->center.x*square + minx;
    //        c->center.y  = c->center.y*square + miny;
    //        c->r        *= square;
    //    }
    //}
}

void DT::setSuperTriangle()
{
    SuperNode *p1 = new SuperNode( -1.23, -0.50 );
    SuperNode *p2 = new SuperNode(  2.23, -0.50 );
    SuperNode *p3 = new SuperNode(  0.50,  2.50 );

    nodes.push_back( p1 );
    nodes.push_back( p2 );
    nodes.push_back( p3 );

    Triangle *t = new Triangle();
    t->setNodes( p1, p2, p3 );
    t->setExist( true );
    triangles.push_back( t );
}

void DT::generateBoundary()
{
	cout << "generateBoundary" << endl;
	Triangle* tBase = triangles[0];

	for( int i=0; i<nodes.size(); i++ ){
		if( nodes[i]->isSuperNode() )
			continue;

		tBase = searchTriangle(tBase,nodes[i]);
		vector< Triangle* > t = divideTriInto3(tBase,nodes[i]);

		stack< Triangle* > triStack;
		for(int j=0; j<3; j++) {
			triStack.push(t[j]);
		}
		swappingAlg( &triStack, nodes[i] );
	}
}

void DT::removeIllegalTriangles()
{
    Triangle *triangle;

    for( unsigned int i=0; i<triangles.size(); i++ ){
        triangle = triangles[i];
        triangle->setExist( true );

        // 仮想三角形の頂点に接する三角形は除去
        if( triangle->getNode(0)->isSuperNode() ||
            triangle->getNode(1)->isSuperNode() ||
            triangle->getNode(2)->isSuperNode() 
          ){
            triangle->setExist( false );
            continue;
        }
    }
}

void DT::generateFineMesh()
{
	// 追加節点数の設定
	int addnum; // 追加節点数
	cout << " number of additional nodes : " << endl;
	cout << " >>" << flush ;
	cin >> addnum;

	for( unsigned int i=0; i<addnum; i++ ){
		AddNodeInterface *ani = new AddNodeOblateness();
		Node* new_node = ani->getAdditionalNode();

		Triangle* t = searchTriangle( triangles[0], new_node );

		int p_index = -1;
		for( int j=0; j<3; j++ ){
			double det = getDet(new_node->getX(), new_node->getY(),
					            t->getNode(j)->getX(), t->getNode(j)->getY(),
					            t->getNode( (j+1)%3 )->getX(), t->getNode( (j+1)%3 )->getY() );
			if( fabs(det) < kCalcEps ){
				p_index = j;
			}
		}

		vector< Triangle* > tv;
		if( p_index < 0 ){
			tv = divideTriInto3( t, new_node );
		}else{
            StraightEdge e( t->getNode( p_index ), t->getNode( (p_index+1)%3 ) );
			for( unsigned int j=0; j<edges.size(); j++ ){
				if( e.isEqual( *edges[j] ) ){
					Edge* new_edge = edges[j]->divideInto2( new_node );
					edges.push_back( new_edge );
					new_node->setIsOnBoundary( true );
				}
			}
			tv = divideTriInto4( t, new_node, p_index );
			for (int j=0; j < tv.size(); j++) {
				for( int k=0; k<3; k++ ){
					Node* p = tv[j]->getNode(k);
				}
			}
		}

		stack< Triangle* > triStack;
		for(int j=0; j<tv.size(); j++) {
			triStack.push(tv[j]);
		}
		swappingAlg( &triStack, new_node );

		nodes.push_back( new_node );
	}
}

void DT::laplace()
{
	const double kEpsLaplace = 1e-8;
	vector< vector<Triangle*> > shared_triangles;

	shared_triangles.resize( nodes.size() );
	// nodes[i]を含む三角形shared_triangles[i]のベクタを用意
	for( unsigned int ni=0; ni<nodes.size(); ni++ ){
		if( nodes[ni]->getIsOnBoundary() || nodes[ni]->isSuperNode() ){
			continue;
		}
		for( unsigned int ti=0; ti<triangles.size(); ti++ ){
			if( !triangles[ti]->getExist() ){
				continue;
			}
			for( int pi=0; pi<3; pi++ ){
				if( nodes[ni] == triangles[ti]->getNode( pi ) ){
					shared_triangles[ni].push_back( triangles[ti] );
				}
			}
		}
	}

	double max_dr = DBL_MAX;
	vector< Node* > poly_node;

	while( max_dr > kEpsLaplace ){
		max_dr = 0;
		for( unsigned int ni=0; ni<nodes.size(); ni++ ){
			if( nodes[ni]->getIsOnBoundary() || nodes[ni]->isSuperNode() ){
				continue;
			}

			// 多角形の構成
			poly_node.clear();
			for( unsigned int ti=0; ti<shared_triangles[ni].size(); ti++ ){
				for( int pi=0; pi<3; pi++){
					if( shared_triangles[ni][ti]->getNode(pi) == nodes[ni] ){
						Node* n = shared_triangles[ni][ti]->getNode( (pi+2)%3 );
						poly_node.push_back( n );
						break;
					}
				}
			}

			// nodes[ni]の移動
			Point sump( 0, 0 );
			Point prep( nodes[ni]->getX(), nodes[ni]->getY() );

			for(unsigned int pi=0; pi<poly_node.size(); pi++){
				sump.setX( sump.getX() + poly_node[pi]->getX() );
				sump.setY( sump.getY() + poly_node[pi]->getY() );
			}

			nodes[ni]->setX( sump.getX() / poly_node.size() );
			nodes[ni]->setY( sump.getY() / poly_node.size() );
			
			// 移動後の位置が許されるか判定
			bool canSet = true;
			for( unsigned int ti=0; ti<shared_triangles[ni].size(); ti++ ){
				Triangle *tBase     = shared_triangles[ni][ti];
				Triangle *tBaseNei;
				int tBcntrInd  = -1;
				int tBNcntrInd = -1;

				for(int pi=0; pi<3; pi++){
					if( tBase->getNode(pi) == nodes[ni] ){
						tBaseNei  = tBase->getNeighborTri(pi);
						tBcntrInd = pi;
						break;
					}
				}
				for(int pi=0; pi<3; pi++){
					if( tBaseNei->getNode(pi) == nodes[ni] ){
						tBNcntrInd = pi;
						break;
					}
				}

				StraightEdge e1( tBase->getNode((tBcntrInd+1)%3), tBase->getNode((tBcntrInd+2)%3) );
				StraightEdge e2( tBaseNei->getNode(tBNcntrInd), tBaseNei->getNode((tBNcntrInd+1)%3) );
				if(isCrossed(e1,e2)) {
					canSet = false;
				}
			}
			if( !canSet ){
				nodes[ni]->set( prep.getX(), prep.getY() );
			}

			double dr = distance( prep.getX(), prep.getY(), nodes[ni]->getX(), nodes[ni]->getY() );
			if( dr > max_dr ){
				max_dr = dr;
			}
		}
	}
}

vector<Triangle*> DT::divideTriInto3( Triangle* triBase, Node* node )
{
    Triangle temp_tri = *triBase;

    vector<Triangle*> t;
    t.push_back( triBase );
    t.push_back( new Triangle() );
    t.push_back( new Triangle() );

    t[0]->setNodes( temp_tri.getNode(0), temp_tri.getNode(1), node );
    t[1]->setNodes( temp_tri.getNode(1), temp_tri.getNode(2), node );
    t[2]->setNodes( temp_tri.getNode(2), temp_tri.getNode(0), node );

    t[0]->setNeighborTri( temp_tri.getNeighborTri(0), t[1], t[2] );
    t[1]->setNeighborTri( temp_tri.getNeighborTri(1), t[2], t[0] );
    t[2]->setNeighborTri( temp_tri.getNeighborTri(2), t[0], t[1] );

    // 外部の三角形と小三角形の関係の更新
    for( int i=0; i<3; i++ ){
        if( t[i]->getNeighborTri(0) == NULL ) continue;

        for( int j=0; j<3; j++ ){
            if( t[i]->getNeighborTri(0)->getNeighborTri(j) == triBase ){
                t[i]->getNeighborTri(0)->setNeighborTriOne( j, t[i] );
            }
        }
    }

	bool is_exist = temp_tri.getExist();
	for( int i=0; i<3; i++ ){
		t[i]->setExist( is_exist );
	}

    triangles.push_back( t[1] );
    triangles.push_back( t[2] );

    return t;
}

vector<Triangle*> DT::divideTriInto4( Triangle* triBase, Node* node, int index_e )
{
    Triangle  temp_base = *triBase;
	Triangle* triNei    = triBase->getNeighborTri(index_e);
	Triangle  temp_nei  = *(triNei);

	int index_en;
	for(int i=0;i<3;i++){
		if( temp_nei.getNode(i) == temp_base.getNode((index_e+1)%3) ){
			index_en = i;
		}
	}

    vector<Triangle*> t;
    t.push_back( triBase );
    t.push_back( new Triangle() );
    t.push_back( triNei );
    t.push_back( new Triangle() );

    t[0]->setNodes( temp_base.getNode((index_e+1)%3), temp_base.getNode((index_e+2)%3), node );
    t[1]->setNodes( temp_base.getNode((index_e+2)%3), temp_base.getNode(index_e),       node );
    t[2]->setNodes( temp_nei.getNode((index_en+1)%3), temp_nei.getNode((index_en+2)%3), node );
    t[3]->setNodes( temp_nei.getNode((index_en+2)%3), temp_nei.getNode(index_en),       node );

    t[0]->setNeighborTri( temp_base.getNeighborTri( (index_e+1)%3 ), t[1], t[3] );
    t[1]->setNeighborTri( temp_base.getNeighborTri( (index_e+2)%3 ), t[2], t[0] );
    t[2]->setNeighborTri( temp_nei.getNeighborTri( (index_en+1)%3 ), t[3], t[1] );
    t[3]->setNeighborTri( temp_nei.getNeighborTri( (index_en+2)%3 ), t[0], t[2] );

    // 外部の三角形と小三角形の関係の更新
    for( int i=0; i<2; i++ ){
        if( t[i]->getNeighborTri(0) == NULL ) continue;

        for( int j=0; j<3; j++ ){
            if( t[i]->getNeighborTri(0)->getNeighborTri(j) == triBase ){
                t[i]->getNeighborTri(0)->setNeighborTriOne( j, t[i] );
            }
        }
    }
    for( int i=2; i<4; i++ ){
        if( t[i]->getNeighborTri(0) == NULL ) continue;

        for( int j=0; j<3; j++ ){
            if( t[i]->getNeighborTri(0)->getNeighborTri(j) == triNei ){
                t[i]->getNeighborTri(0)->setNeighborTriOne( j, t[i] );
            }
        }
    }

	t[1]->setExist( temp_base.getExist() );
	t[3]->setExist( temp_nei.getExist() );

    triangles.push_back( t[1] );
    triangles.push_back( t[3] );

    return t;
}

Triangle* DT::searchTriangle( Triangle* tBase, Node* node )
{
	double det;
	double _x = node->getX(), _y = node->getY();
	double dx,dy;

	srand((unsigned)time(NULL));

	for( int i=0; i<3; i++ ){
		det = getDet(_x,_y,
				     tBase->getNode(i)->getX(), tBase->getNode(i)->getY(),
				     tBase->getNode( (i+1)%3 )->getX(), tBase->getNode( (i+1)%3 )->getY() );

		// 二点の座標が同じ->三角形として矛盾，終了する
		if( isSamePoint( tBase->getNode(i), tBase->getNode((i+1)%3) ) ){
			//cout << "tBase = "   << *tBase      << endl;
			//cout << " tBase->p[" << i << "] = " << endl << *(tBase->p[i]) << endl;
			//cout << " tBase->p[" << (i+1)%3 << "] = " << endl << *(tBase->p[(i+1)%3]) << endl;
			//dt->disp();
			exit(EXIT_FAILURE);
		}

		if( fabs(det) < kCalcEps ){
			// 直線上に節点が存在する時
			// 節点をわずかに移動させる
			dx  = kOutEps * (rand()%10+1);
			dy  = kOutEps * (rand()%10+1);
			_x -= 10*kOutEps/2;
			_x += dx;
			_y -= 10*kOutEps/2;
			_y += dy;
			i--;
			continue;
		}else if( det < 0 ){
			// 設置点が辺の右側にあれば移動
			tBase = tBase->getNeighborTri(i);
			i = -1;
		}
	}

	return tBase;
}

void DT::swapTriangles( Triangle* tA, Triangle* tB, int diagPtA, int diagPtB )
{
	Triangle tmpA = *tA,tmpB = *tB;

	tmpA.setNodes( tA->getNode(diagPtA), 
				   tB->getNode(diagPtB), 
				   tB->getNode((diagPtB+1)%3) );

	tmpB.setNodes( tA->getNode(diagPtA), 
			   	   tA->getNode((diagPtA+1)%3), 
				   tB->getNode(diagPtB) ); 

	tmpA.setNeighborTri( tB, 
						 tB->getNeighborTri(diagPtB),
						 tA->getNeighborTri((diagPtA+2)%3) );

	tmpB.setNeighborTri( tA->getNeighborTri(diagPtA),
						 tB->getNeighborTri((diagPtB+2)%3),
						 tA);

	// 外部の三角形と変換した三角形の隣接関係を更新
	Triangle* diagNeiTriA = tA->getNeighborTri( diagPtA );
	if( diagNeiTriA != NULL ){
		Triangle* diagNeiNeiTriA;
		for(int i=0; i<3; i++) {
			diagNeiNeiTriA = diagNeiTriA->getNeighborTri(i);
			if( diagNeiNeiTriA != NULL && diagNeiNeiTriA == tA ) {
				diagNeiTriA->setNeighborTriOne( i, tB );
			}
		}
	}

	Triangle* diagNeiTriB = tB->getNeighborTri( diagPtB );
	if( diagNeiTriB != NULL ){
		Triangle* diagNeiNeiTriB;
		for(int i=0; i<3; i++) {
			diagNeiNeiTriB = diagNeiTriB->getNeighborTri(i);
			if( diagNeiNeiTriB != NULL && diagNeiNeiTriB == tB ) {
				diagNeiTriB->setNeighborTriOne( i, tA );
			}
		}
	}

	*tA = tmpA;
	*tB = tmpB;
}


vector< Triangle* > DT::swappingAlg( stack< Triangle* > *triStack, Node *p )
{
	vector< Triangle* > triSharePt;
	Triangle  *tA;                     // スタックから取り出した三角形
	Triangle  *tB;                     // tAに隣接する三角形(pの対辺側)
	StraightEdge  diagLine,shareLine;  // 対角線，共有線

	while( !triStack->empty() ){

		tA = triStack->top();
		triStack->pop();

		bool haventA = true;

		// tAは節点pに接する三角形である．もし三角形群triSharePtがtAを持っていなければ
		// tAをtriSharePtに追加する．
		for(unsigned int i=0; i<triSharePt.size(); i++)
			if( tA == triSharePt[i] ) haventA = false;

		if(haventA) triSharePt.push_back( tA );

		// 対角線のtA側の端点を得る．
		int diagPtA = 0;
		for(int j=0; j<3; j++){
			if( tA->getNode(j)->isEqual(*p) ){
				diagPtA = j;
				break;
			}
		}

		// tBを得る．tBはpの対辺側に位置し，tAに隣接する三角形である．
		if( tA->getNeighborTri((diagPtA+1)%3) != NULL )
			tB = tA->getNeighborTri((diagPtA+1)%3);
		else continue;

		// 対角線のtB側の端点を得る．
		int diagPtB = 0;
		for(int j=0; j<3; j++){
			if( tB->getNeighborTri(j) != NULL && tB->getNeighborTri(j) == tA ){
				diagPtB = (j+2)%3;
				break;
			}
		}

		// 対角線およびtA.tBが共有する(接する)線を生成する．
		diagLine.setNodes(  tA->getNode(diagPtA)      , tB->getNode(diagPtB)       );
		shareLine.setNodes( tA->getNode((diagPtA+1)%3), tA->getNode((diagPtA+2)%3) );
		if( !isCrossed( diagLine, shareLine ) ) continue;


		// 対角線，共有線が制約線分か否か判定する．
		bool shareIsCond = false;
		bool diagIsCond  = false;
		for(unsigned int j=0; j<edges.size(); j++){
			//if( shareLine == *edges[j] ){
			if( shareLine.isEqual(*edges[j]) ){
				shareIsCond = true;
				break;
			}
			//if( diagLine == *edges[j] ){
			if( diagLine.isEqual(*edges[j]) ){
				diagIsCond = true;
				break;
			}
		}

		if( shareIsCond ){
			// 共有線が制約線分であれば分割は行わない．
			continue;
		}else if(diagIsCond || shareLine.getLength() > diagLine.getLength() ){
			// 対角線が制約線分であるか，delaunay条件を満たすならばスワッピングを実行
			swapTriangles( tA, tB, diagPtA, diagPtB );

			// スワッピングの結果，tBはpに接する三角形となるので，triSharePtが
			// tBを既に格納していなければtBをtriSharePtに追加する．
			bool haventB = true;
			for(unsigned int i=0; i<triSharePt.size(); i++)
				if( tB == triSharePt[i] ) haventB = false;

			if( haventB )
				triSharePt.push_back( tB );

			triStack->push(tA);
			triStack->push(tB);
		}
	}  

	return triSharePt;
}

static double getDet( double x0,double y0,double x1,double y1,double x2,double y2 )
{
	return ((x1-x0)*(y2-y0) - (y1-y0)*(x2-x0));
}

static double isSamePoint( Point* p1, Point* p2 )
{
	double dx = fabs( fabs( p1->getX() ) - fabs( p2->getX() ) );
	double dy = fabs( fabs( p1->getY() ) - fabs( p2->getY() ) );

	if( dx < DT::kCalcEps && dy < DT::kCalcEps ){
		return true;
	}

	return false;
}

static bool isCrossed( StraightEdge e1, StraightEdge e2 )
{
	double x1,x2,x3,x4;
	double y1,y2,y3,y4;
	double ksi, eta, delta;
	double ramda, mu;

	x1 = e1.getNode(0)->getX();  y1 = e1.getNode(0)->getY();
	x2 = e1.getNode(1)->getX();  y2 = e1.getNode(1)->getY();
	x3 = e2.getNode(0)->getX();  y3 = e2.getNode(0)->getY();
	x4 = e2.getNode(1)->getX();  y4 = e2.getNode(1)->getY();

	ksi   = ( y4 - y3 )*( x4 - x1 ) - ( x4 - x3 )*( y4 - y1 );
	eta   = ( x2 - x1 )*( y4 - y1 ) - ( y2 - y1 )*( x4 - x1 );
	delta = ( x2 - x1 )*( y4 - y3 ) - ( y2 - y1 )*( x4 - x3 );
	ramda = ksi / delta;
	mu    = eta / delta;
	if( ramda > 0 && ramda < 1 && mu > 0 && mu < 1 ) {
		return true;
	}
	return false;
}

static double distance( double x1, double y1, double x2, double y2)
{
  double dx = fabs( x1 - x2 );
  double dy = fabs( y1 - y2 );

  return ( sqrt( dx*dx + dy*dy ) );
}
