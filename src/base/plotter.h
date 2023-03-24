#pragma once
#include <vector>

namespace logpb {
class Plotter {
public :
    void insert(const float val) { data.push_back(val); };
    std::vector<float>& container() { return data; };

private :
    std::vector<float> data;
};

struct Graph {
    Plotter x_data;
    std::vector<Plotter> y_data;

    // Plotter& x_data() { return xd; };
    // std::vector<Plotter> y_data() { return yd; };
};
}
