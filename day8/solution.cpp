#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <memory>
#include <map>
#include <limits>
#include <algorithm>
#include <numeric>

struct UnionFind {
    std::vector<int> parent;
    std::vector<int> rank;
    std::vector<int> size;
    
    UnionFind(int n) : parent(n), rank(n, 0), size(n, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return false;
        
        if (rank[px] < rank[py]) std::swap(px, py);
        parent[py] = px;
        size[px] += size[py];
        if (rank[px] == rank[py]) rank[px]++;
        return true;
    }
    
    int getSize(int x) {
        return size[find(x)];
    }
};

struct Point
{
    int x;
    int y;
    int z;
    unsigned int circuitId;
    Point(int x, int y, int z, unsigned int circuitId) : x(x), y(y), z(z), circuitId(circuitId)
    {
    }

    double distance(Point two) const
    {
        return std::pow(x - two.x, 2) + std::pow(y - two.y, 2) + std::pow(z - two.z, 2);
    }
};

struct Connection
{
    unsigned int p1 = 0;
    unsigned int p2 = 0;
    double weight = std::numeric_limits<double>::infinity();
    Connection(unsigned int p1, unsigned int p2, double weight) : p1(p1), p2(p2), weight(weight) {}
    Connection() {}
};

typedef std::vector<Connection> connectionData;

std::vector<Point> readPoints(const std::string fileName)
{
    std::ifstream in(fileName);
    if (!in) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        exit(1);
    }
    std::vector<Point> points;
    std::string line;
    unsigned int pointId = 0;
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string sx, sy, sz;
        std::getline(ss, sx, ',');
        std::getline(ss, sy, ',');
        std::getline(ss, sz);

        int x = std::stoi(sx);
        int y = std::stoi(sy);
        int z = std::stoi(sz);
        points.push_back(std::move(Point(x, y, z, pointId)));
        pointId++;
    }
    return points;
}

connectionData buildConnectionData(std::vector<Point> &points)
{
    connectionData connectionsMap;
    Connection connection;
    // Create all connections and add them to a map ordered by weight

    for (int i = 0; i < points.size(); i++)
    {
        connection.weight = std::numeric_limits<double>::infinity();
        for (int j = i + 1; j < points.size(); j++)
        {
            if (i == j)
            {
                continue;
            }
            auto dist = points[i].distance(points[j]);
            connection.weight = dist;
            connection.p1 = i;
            connection.p2 = j;
            connectionsMap.push_back(connection);
        }
    }
    std::sort(connectionsMap.begin(), connectionsMap.end(), [](auto &a, auto &b)
              { return a.weight < b.weight; });
    return connectionsMap;
}

void parseConnections(std::vector<Point> &points, const connectionData &connectionsMap)
{
    UnionFind uf(points.size());
    int connectionsCount = 0;
    int checkpointAt1000 = 1000;
    
    // In ascending order start connecting junction boxes
    for (int i = 0; i < connectionsMap.size(); i++)
    {
        // Check at 1000 connections
        if (i == 999)
        {
            std::vector<int> componentSizes = uf.size;
            std::sort(componentSizes.rbegin(), componentSizes.rend());
            std::cout << "Size of 3 largest circuits, multiplied : " 
                      << (long)componentSizes[0]* componentSizes[1] * componentSizes[2] 
                      << std::endl;
        }
        auto &connection = connectionsMap.at(i);
        auto &p1 = points[connection.p1];
        auto &p2 = points[connection.p2];
        
        // Try to unite components
        if (!uf.unite(connection.p1, connection.p2))
        {
            continue;  // Already in same component
        }
        connectionsCount++;
        
        
        
        // Check if all connected
        if (connectionsCount == (int)points.size() - 1)
        {
            std::cout << "Found last connection : " << p1.x<< "," << p1.y<< ","<< p1.z 
                      << " | "<< p2.x<< "," << p2.y<< "," << p2.z << std::endl;
            std::cout << "Mult of X coords : " << (long)p1.x*p2.x << std::endl;
            break;
        }
    }
}
int main(int argc, char **argv)
{
    auto input = "input";//argv[1];
    std::vector<Point> points = readPoints(input);
    connectionData connectionsMap = buildConnectionData(points);
    parseConnections(points, connectionsMap);
    
    std::cout << points.size() << " points parsed\n";
    return 0;
}