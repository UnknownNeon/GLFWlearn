#include "obj_loader.h"
#include <sstream>

void object_loader::loadObject(std::string& path)
{
    std::string sx, si, sj, sk;
    Vertex v;
 /*   v_indices vi;*/

    std::ifstream file(path);

    if (file) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            ss >> sx >> si >> sj >> sk;

            if (line[0] == 'v' && line[1] == ' ') {

                

                v.position.x = stof(si);
                v.position.y = stof(sj);
                v.position.z = stof(sk);
                //1 face :
                v.color.x = 1.0f;
                v.color.y = 1.0f;
                v.color.z = 1.0f;

                this->Object.push_back(v);
            }

            if (sx == "f" || sx == "F") {

                int i = stoi(si);
                int j = stoi(sj);
                int k = stoi(sk);


                this->vertex_indices.push_back(i);
                this->vertex_indices.push_back(j);
                this->vertex_indices.push_back(k);
            }
        }
    }
    else {
        std::cout << "Error reading Obj File" << std::endl;
    }
    file.close();

    //mixed feelings 

    /*bool visited_f = false;

    std::ifstream file2(path);
    if (file2) {
        std::string line;

        while (std::getline(file2, line)) {
            std::istringstream ss(line);

            ss >> sx >> si >> sj >> sk;
            if (sx == "f" || sx == "F") {

                visited_f = true;

                int i = stoi(si);
                int j = stoi(sj);
                int k = stoi(sk);


                vi.vertex_indices.push_back(i);
                vi.vertex_indices.push_back(j);
                vi.vertex_indices.push_back(k);
            }
            else if((sx != "f" || sx != "F") && visited_f == true) {
                this->v_i_data.push_back(vi);
                vi.vertex_indices.clear();
                visited_f = false;
            }
            
        }
        if (visited_f == true) {
            this->v_i_data.push_back(vi);
            vi.vertex_indices.clear();
            visited_f = false;
        }
       
    }
    else {
        std::cout << "Cant open file for Indexing " << std::endl;
    }

    file2.close();*/

    std::cout << "Loaded Vertices:" << this->Object.size() << std::endl;
    std::cout << "Loaded Indices Count :" << this->vertex_indices.size() << std::endl;
}
