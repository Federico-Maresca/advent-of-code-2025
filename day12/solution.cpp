#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <cassert>
#include <algorithm>

struct Shape {
    std::array<bool, 9> grid;

    bool at(uint8_t x, uint8_t y) const {
        assert(x < 3 && y < 3);
        return grid[y * 3 + x];
    }
};

struct PackingArea {
    int width;
    int height;
    std::vector<int> numberOfShapes;
};


void readFromFile(const std::string& filename, 
                  std::vector<std::vector<Shape>>& shapeIterations, 
                  std::vector<PackingArea>& areas) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line.find(':') != std::string::npos && line.find('x') == std::string::npos) {
            Shape baseShape;
            for (int y = 0; y < 3; ++y) {
                std::getline(file, line);
                for (int x = 0; x < 3; ++x) {
                    baseShape.grid[y * 3 + x] = (line[x] == '#');
                }
            }

            std::vector<Shape> rotations;
            rotations.push_back(baseShape);            // 0 degrees
            shapeIterations.push_back(rotations);
        }
        
        else if (line.find('x') != std::string::npos) {
            PackingArea area;
            std::replace(line.begin(), line.end(), 'x', ' ');
            std::replace(line.begin(), line.end(), ':', ' ');

            std::stringstream ss(line);
            ss >> area.width >> area.height;

            int count;
            while (ss >> count) {
                area.numberOfShapes.push_back(count);
            }
            areas.push_back(area);
        }
    }
}

int main(int argc, char* argv[]) {
     std::vector<std::vector<Shape>> shapes;
    std::vector<PackingArea> areas;
    readFromFile("input",shapes, areas);

    int numOfSolutions = 0;
    for(const auto& area : areas)
    {
        int totalAreaReq = 0;
        for(const auto & shapecount : area.numberOfShapes)
        {
            totalAreaReq += shapecount * 3 * 3;
        }
        if(totalAreaReq <= area.width * area.height)
            numOfSolutions++;
    }
    std::cout << "Naive solution, that apparently works: " << numOfSolutions << "\n";
    
    return 0;
}