#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

struct Point
{
    int x;
    int y;
    Point(int x, int y) : x(x), y(y)
    {
    }
    bool operator==(const Point &other) const
    {
        return other.x == x && other.y == y;
    }
};


std::vector<Point> readPoints(std::string fileName)
{
    Point ul = Point(0, 0);

    std::ifstream in(fileName);
    std::vector<Point> points;
    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string sx, sy;
        std::getline(ss, sy, ',');
        std::getline(ss, sx);

        int x = std::stol(sx);
        int y = std::stol(sy);
        points.push_back(std::move(Point(x, y)));
    }
    return points;
}

bool rectangleValid(const Point &p1, const Point &p2, const std::vector<Point> &points)
{
    for (int i = 0; i < points.size(); ++i)
    {
        Point e1 = points[i];
        Point e2 = points[(i + 1) % points.size()]; // Wrap around to first point
        if (std::min(p1.x, p2.x) < std::max(e1.x, e2.x) && std::max(p1.x, p2.x) > std::min(e1.x, e2.x) && std::min(p1.y, p2.y) < std::max(e1.y, e2.y) && std::max(p1.y, p2.y) > std::min(e1.y, e2.y))
            return false;
    }
    return true;
}

// Using two points as corners find the rectangle with the maximum area
void getMaximumRectangleByArea(const std::vector<Point> &points)
{
    uint64_t maxAreaP1 = 0;
    uint64_t maxAreaP2 = 0;
    for (const auto &p1 : points)
    {
        for (const auto &p2 : points)
        {
            if (p1.x == p2.x || p1.y == p2.y)
                continue;
            // This ensures all intermediate steps use 64-bit arithmetic
            auto width = std::abs(static_cast<long>(p2.x - p1.x)) + 1;
            auto height = std::abs(static_cast<long>(p2.y - p1.y)) + 1;
            uint64_t area = width * height;
            maxAreaP1 = std::max(area, maxAreaP1);

            if (rectangleValid(p1, p2, points))
            {
                maxAreaP2 = std::max(area, maxAreaP2);
            }
        }
    }
    std::cout << "Max area rectangle area : " << maxAreaP1 << std::endl;
    std::cout << "Max area rectangle area P2: " << maxAreaP2 << std::endl;
}

int main()
{
    std::vector<Point> points = readPoints("input");
    getMaximumRectangleByArea(points);

    return 0;
}

