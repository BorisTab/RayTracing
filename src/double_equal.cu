#include <double_equal.h>

namespace Double_equal {
    double double_accuracy = 1e-5;

    bool is_equal(double a, double b) {
        double diff = a - b;
        return diff <= double_accuracy && diff >= -double_accuracy;
    }
}
