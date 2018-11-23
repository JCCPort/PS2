#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

class Sphere {
public :
    /**
     *
     * @param d. Diameter of the sphere, the variable radius is initialised within this method's scope as it is more
     * commonly used than diameter in calculations.
     * @param rho. Density of the sphere.
     */
    void set(double d = 0, double rho = 0) {
        diameter = d;
        radius = diameter / 2.0;
        density = rho;
    };

    /**
     *
     * @return Mass of the sphere, given the radius and density initialised in "set".
     */
    double GetMass() {
        return (4.0 / 3.0) * M_PI * pow((radius), 3.0) * density;
    }

    /**
     *
     * @return Surface area of the sphere, given the radius initialised in "set".
     */
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
    /**
     *
     * @param l. Length of the each side of the cube.
     * @param rho. The density of the cube.
     */
    void set(double l = 0, double rho = 0) {
        length = l;
        density = rho;
    };

    /**
     *
     * @return Mass of the cube, given the length and density initialised in "set".
     */
    double GetMass() {
        return pow(length, 3.0) * density;
    };

    /**
     *
     * @return Surface area of the cube, given the length initialised in "set".
     */
    double GetArea() {
        return 6.0 * pow(length, 2.0);
    };

private :
    double length;
    double density;
};



class Cylinder {
public :
    /**
     *
     * @param d. Diameter of the cylinder, the variable radius is initialised within this method's scope as it is more
     * commonly used than diameter in calculations.
     * @param h. Height of the cylinder.
     * @param rho. Density of the cylinder.
     */
    void set(double d = 0, double h = 0,
                      double rho = 0) {
        diameter = d;
        radius = diameter / 2.0;
        height = h;
        density = rho;
    };

    /**
     *
     * @return Mass of the cylinder, given the radius, height, and density initialised in "set".
     */
    double GetMass() {
        return M_PI * pow(radius, 2.0) * height * density;
    }

    /**
     *
     * @return Surface area of the cube, given the radius and height initialised in "set".
     */
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
    /**
     *
     * @param d. Diameter of the cone, the variable radius is initialised within this method's scope as it is more
     * commonly used than diameter in calculations.
     * @param h. Height of the cone, measured from base to vertex.
     * @param rho. Density of the cone.
     */
    void set(double d = 0, double h = 0,
                  double rho = 0) {
        diameter = d;
        radius = diameter / 2.0;
        height = h;
        density = rho;
    };

    /**
     *
     * @return Mass of the cone, given the radius, height, and density initialised in "set".
     */
    double GetMass() {
        return M_PI * pow((radius), 2.0) * (height / 3.0) * density;
    }

    /**
     *
     * @return Surface area of the cone, given the radius and height initialised in "set".
     */
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
    /**
     * Initialising instances of the classes (shapes).
     */
    Sphere sphere{};
    Cube cube{};
    Cylinder cylinder{};
    Cone cone{};
    static const float densityIron = 7.874;
    /**
     * Using the method "set" to assign values to the private variables of the class instances.
     */
    sphere.set(2.0, densityIron);
    cube.set(2.0, densityIron);
    cylinder.set(2.0, 2.0, densityIron);
    cone.set(2.0, 2.0, densityIron);

    static const double sphereRatio = sphere.GetArea()/sphere.GetMass();
    static const double cubeRatio = cube.GetArea()/cube.GetMass();
    static const double cylinderRatio = cylinder.GetArea()/cylinder.GetMass();
    static const double coneRatio = cone.GetArea()/cone.GetMass();
    double ratios[] = {sphereRatio, cubeRatio, cylinderRatio, coneRatio};
    static const double biggestRatio = *std::max_element(ratios, ratios+4);
    std::cout << "\nShape\t\t\tSurface Area/Mass (cm^2 g^-1)" << std::endl;
    std::cout << "Sphere\t\t\t" << sphereRatio << std::endl;
    std::cout << "Cube\t\t\t" << cubeRatio << std::endl;
    std::cout << "Cylinder\t\t" << cylinderRatio << std::endl;
    std::cout << "Cone\t\t\t" << coneRatio << std::endl;
    std::cout << "\nBiggest ratio\t\t" << biggestRatio << "\n\n" << std::endl;


    return 0;
}
