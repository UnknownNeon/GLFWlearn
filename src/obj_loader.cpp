#include "obj_loader.h"
#include <sstream>

void object_loader::loadObject(std::string& path )
{
    std::string sx, si, sj, sk;

    Vertex v;
    std::ifstream file(path);

    int index_counter = 0;
    int index_counter_previous = 0;

    if (file) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);

            if (line[0] == 'v' && line[1] == ' ') {

               /* ss >> sx >> si >> sj >> sk;

                v.position.x = stoi(si);
                v.position.y = stoi(sj);
                v.position.z = stoi(sk);*/

                (void)sscanf_s(line.c_str(), "v  %f %f %f", &v.position[0], &v.position[1], &v.position[2]);

                    //1 face :
                    v.color.x = 1.0f;
                    v.color.y = 1.0f;
                    v.color.z = 1.0f;
      
                //std::cout << v.position.x << v.position.y << v.position.z <<std::endl;

                this->Object.push_back(v);

            }
            if ((line[0] == 'f' && line[1] == ' ') || line[0] == 'F') {

                
                ss >>sx >> si >> sj >> sk;                      
                int i = stoi(si);  
                int j = stoi(sj);
                int k = stoi(sk);  

                std::cout << i << " " << j << " " << k << " " << std::endl;

                vertex_indices.push_back(i);
                vertex_indices.push_back(j);
                vertex_indices.push_back(k);   

                
            }


        }
    }
    else {
        std::cout << "Error reading Obj File" << std::endl;
    }

    std::cout << "Loaded Vertices:" << this->Object.size() << std::endl;
    std::cout << "Loaded Indices:" << this->vertex_indices.size() << std::endl;
}
