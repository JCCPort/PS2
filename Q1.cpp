#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <math.h>


class Sphere {
public :
    void set(double d = 0, double rho = 0) {
        diameter = d;
        radius = diameter / 2.0;
        density = rho;
    };

    double GetMass() {
        return (4.0 / 3.0) * M_PI * pow((radius), 3.0) * density;
    }

    double GetArea() {
        return 4.0 * M_PI * pow((radius), 2.0);
    }

private :
    double radius;
    double diameter;
    double density;
};



class Cube {
public :
    void set(double l = 0, double rho = 0) {
        length = l;
        density = rho;
    };

    double GetMass() {
        return pow(length, 3.0) * density;
    };

    double GetArea() {
        return 6.0 * pow(length, 2.0);
    };

private :
    double length;
    double density;
};



class Cylinder {
public :
    void set(double d = 0, double h = 0,
                      double rho = 0) {
        diameter = d;
        radius = diameter / 2.0;
        height = h;
        density = rho;
    };

    double GetMass() {
        return M_PI * pow(radius, 2.0) * height * density;
    }

    double GetArea() {
        return 2.0 * M_PI * radius * height + 2.0 * M_PI * pow((radius), 2.0);
    }

private :
    double radius;
    double diameter;
    double height;
    double density;
};



class Cone {
public :
    void set(double d = 0, double h = 0,
                  double rho = 0) {
        diameter = d;
        radius = diameter / 2.0;
        height = h;
        density = rho;
    };

    double GetMass() {
        return M_PI * pow((radius), 2.0) * (height / 3.0) * density;
    }

    double GetArea() {
        return M_PI * (radius) * (radius + pow(pow(height, 2.0) + pow(radius, 2.0), 1.0 / 2.0));
    }

private :
    double radius;
    double diameter;
    double height;
    double density;
};



int main() {
    Sphere sphere{};
    Cube cube{};
    Cylinder cylinder{};
    Cone cone{};
    float densityIron = 7.874;
    sphere.set(2.0, densityIron);
    cube.set(2.0, densityIron);
    cylinder.set(2.0, 2.0, densityIron);
    cone.set(2.0, 2.0, densityIron);

    std::cout << "\nShape\t\t\tSurface Area/Mass (cm^2 g^-1)" << std::endl;
    std::cout << "Sphere\t\t\t" << sphere.GetArea()/sphere.GetMass() << std::endl;
    std::cout << "Cube\t\t\t" << cube.GetArea()/cube.GetMass() << std::endl;
    std::cout << "Cylinder\t\t" << cylinder.GetArea()/cylinder.GetMass() << std::endl;
    std::cout << "Cone\t\t\t" << cone.GetArea()/cone.GetMass() << "\n\n" << std::endl;
    return 0;
}