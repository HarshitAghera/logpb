#pragma once
#include <string>
#include <vector>
#include <memory>

class Plotter {
public:
    void append(const float val) { data.push_back(val); };
    const std::vector<float>& container() const { return data; };

private:
    std::vector<float> data;
};

struct Points {
    const float* x_data;
    const float* y_data;
    size_t size;
};

class Curve {
public:
    Points get_series() const;
    Curve(const Plotter* x, const Plotter* y);

private:
    const Plotter* x_data;
    const Plotter* y_data;
};

class Curve_Serializer {
public:
    Curve_Serializer(int x, int y);

    void set_x(int x);
    void set_y(int y);

    int get_x() const { return x_index; };
    int get_y() const { return y_index; };

    Curve deserialize(const std::vector<std::unique_ptr<Plotter>>& plotters);

private:
    int x_index;
    int y_index;
};

struct Plot_Info {
    struct Curve_Info {
        std::string x_field;
        std::string y_field;
        const Curve* curve;
        int curve_index;
    };

    std::vector<Curve_Info> curves;
};
