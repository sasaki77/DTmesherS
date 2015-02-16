#ifndef BC_H
#define BC_H

enum BOUNDARY_COND {
    COND_DIRICHLET,
    COND_NEUMANN,
};

class BoundaryCondition {
    private:
        BOUNDARY_COND bc;
        double        val;

    public:
        BoundaryCondition(){
            bc  = COND_DIRICHLET;
            val = 0;
        }

        BoundaryCondition( BOUNDARY_COND _bc, double _val){
            bc  = _bc;
            val = _val;
        }

        void setBCandVal( BOUNDARY_COND _bc, double _val ){
            bc  = _bc;
            val = _val;
        }

        void setBC( BOUNDARY_COND _bc ){
            bc = _bc;
        }

        void setVal( double _val ){
            val = _val;
        }

        BOUNDARY_COND getBC(){
            return bc;
        }

        double getVal(){
            return val;
        }
};

#endif /* end of include guard */
