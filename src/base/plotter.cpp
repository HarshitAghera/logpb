#include "plotter.h"

Curve::Curve(const Plotter* x, const Plotter* y) : x_data{x}, y_data{y} {}

Points Curve::get_series() const {
    return Points{.x_data = x_data->container().data(),
                  .y_data = y_data->container().data(),
                  .size = std::min(x_data->container().size(),
                                   y_data->container().size())};
}

Curve_Serializer::Curve_Serializer(int x, int y) : x_index{x}, y_index{y} {}

Curve Curve_Serializer::deserialize(
    const std::vector<std::unique_ptr<Plotter>>& plotters) {
    return Curve{plotters[x_index].get(), plotters[y_index].get()};
}
