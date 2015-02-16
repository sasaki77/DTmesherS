#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

using namespace std;

class Material {
    private:
        string name;
    
    public:
        Material(){
        };

        void setMaterial( string _name ){
            name = _name;
        }

        string getMaterial(){
            return name;
        }
};
    
#endif /* end of include guard */
