#ifndef RAYTRACING_GEOMETRY_H
#define RAYTRACING_GEOMETRY_H

#include <double_equal.h>

#include <cmath>
#include <cassert>

template <typename T>
class Vector3 {
public:
    T x = 0;
    T y = 0;
    T z = 0;

    Vector3() = default;

    Vector3(const T& x, const T& y, const T& z):
            x(x),
            y(y),
            z(z) {}

    Vector3 operator+(const Vector3& vec) const {
        return Vector3(x + vec.x, y + vec.y, z + vec.z);
    }

    T operator*(const Vector3& vec) const {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    Vector3 operator*(const T scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator-(const Vector3& vec) const {
        return Vector3(x - vec.x, y - vec.y, z - vec.z);
    }

    Vector3 operator-() const {
        return *this * (-1);
    }

    Vector3 normalized() const {
        return Vector3(x / len(), y / len(), z / len());
    }

    Vector3& operator-() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    double len() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    double square_len() const {
        return x*x + y*y + z*z;
    }

    double operator^(const Vector3& vec) const {
        return std::acos((*this * vec) / (len() * vec.len()));
    }

    double cos(const Vector3& vec) const {
        return (*this * vec) / (len() * vec.len());
    }
    double sin(const Vector3& vec) const {
        double cosinus = cos(vec);

        return sqrt(1 - pow(cosinus, 2));
    }

    double parallelogram_area(const Vector3& vec) const {
        return len() * vec.len() * sin(vec);
    }

    Vector3 reflect_with(const Vector3& vec) const {
        return *this - vec.normalized() * 2 * len() * cos(vec);
    }

    Vector3& operator=(const Vector3& color) {
        x = color.x;
        y = color.y;
        z = color.z;

        return *this;
    }

    // refract_index is ratio of external refract index to internal refract index
    static Vector3<T> refract(const Vector3<T>& vec, const Vector3<T>& normal, double refract_index) {
        assert(Double_equal::is_equal(1, vec.len()));
        assert(Double_equal::is_equal(1, normal.len()));

        Vector3 true_normal = normal;

        double vec_normal_cos = -vec * normal;
        if (vec_normal_cos < 0) {
            vec_normal_cos = -vec_normal_cos;
            true_normal = -normal;
            refract_index = 1 / refract_index;
        }

        double ref_vec_normal_cos = sqrt(1 - refract_index * (1 - vec_normal_cos * vec_normal_cos));

        return vec * refract_index + true_normal * (refract_index * vec_normal_cos - ref_vec_normal_cos);
    }
};

class Color: public Vector3<unsigned char> {
public:
    Color(): Vector3<unsigned char>() {}

    Color(unsigned char r, unsigned char g, unsigned char b):
            Vector3<unsigned char>(r, g, b) {}

    explicit Color(const Vector3<unsigned char>& vec):
            Vector3<unsigned char>(vec) {}

    explicit Color(u_int32_t hex): Color((hex >> 16) & 0xff, (hex >> 8) & 0xff, hex & 0xff) {}

    Color operator*(const Color& color) {
        return Color(x * color.x / 255, y * color.y / 255, z * color.z / 255);
    }

    Color operator+(const Color& color) {
        Color newColor;
        newColor.x = std::min(255, x + color.x);
        newColor.y = std::min(255, y + color.y);
        newColor.z = std::min(255, z + color.z);

        return newColor;
    }

//    Color& operator=(const Color& color) {
//        x = color.x;
//        y = color.y;
//        z = color.z;
//
//        return *this;//    }

//    sf::Color operator*() const {
//        return sf::Color(x, y, z);
//    }

    Color operator*(const double intensity) {
        return Color(std::min(255., x * intensity), std::min(255., y * intensity), std::min(255., z * intensity));
    }
};

//Color operator*(double intensity, const Color& color) {
//    return Color(color.x * intensity, color.y * intensity, color.z * intensity);
//}


template <typename T>
class CoordinateSystem3 {
private:
    Vector3<T> zeroPoint;

public:
    CoordinateSystem3(T x, T y, T z):
            zeroPoint(x, y, z) {}

    CoordinateSystem3(const CoordinateSystem3& coordSys):
            zeroPoint(coordSys.zeroPoint.x, coordSys.zeroPoint.y, coordSys.zeroPoint.z) {}

    explicit CoordinateSystem3(const Vector3<T>& vec):
            zeroPoint(vec) {}

    Vector3<T> makeVec(T x, T y, T z) const {
        return Vector3(x - zeroPoint.x, zeroPoint.y - y, z - zeroPoint.z);
    }

    Vector3<T> makeVec(const Vector3<T>& vec) const {
        return Vector3(vec.x - zeroPoint.x, zeroPoint.y - vec.y, vec.z - zeroPoint.z);
    }

    Vector3<T> gridPos(const Vector3<T>& vec) const {
        return Vector3(vec.x + zeroPoint.x, zeroPoint.y - vec.y, vec.z + zeroPoint.z);
    }
};

#endif //RAYTRACING_GEOMETRY_H
