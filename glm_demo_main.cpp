#include <glm/glm.hpp>
#include <iostream>

#include "util/glm_helper.h"

using namespace std;

int main() {
    //glm vector datatypes
    //vectors of float:
    glm::vec3 position;
    glm::vec4 color;

    //vectors of int
    glm::ivec2 gridpos;
    glm::ivec3 color_rgb;

    //set values one by one, using xyz
    position.x = -10.0f;
    position.y = 10.0f;
    position.z = 20.0f;

    //set values one by one, using rgba  (good for colors: Red Green Blue Alpha)
    color.r = 0.5f;
    color.g = 0.5f;
    color.b = 0.0f;
    color.a = 1.0f;

    //set values one by one, using array indexing
    gridpos[0] = 4;
    gridpos[1] = 5;

    //set values one by one, using mixed accessors
    color_rgb.r = 255;   //equivalent to color_rgb[0] = 255 and color_rgb.x = 255
    color_rgb[1] = 128;  //equivalent to color_rgb.x = 128  and color_rgb.g = 128
    color_rgb.z = 0;     //equivalent to color_rgb[2] = 0   and color_rgb.b = 0

    cout << "position = " << position << endl;
    cout << "color = " << color << endl;
    cout << "gridpos = " << gridpos << endl;
    cout << "color_rgb = " << color_rgb << endl;
    cout << endl;

    //access in different equivalent ways
    cout << "gridpos.x  = " << gridpos.x << "    gridpos.y  = " << gridpos.y << "   (xyzw access)" << endl;
    cout << "gridpos.r  = " << gridpos.r << "    gridpos.g  = " << gridpos.g << "   (rgba access)" << endl;
    cout << "gridpos.s  = " << gridpos.s << "    gridpos.t  = " << gridpos.t << "   (stpq access)" << endl;
    cout << "gridpos[0] = " << gridpos[0] << "    gridpos[1] = " << gridpos[1] << "   (array index access)" << endl;
    cout << endl;

    //set values all at once
    position = glm::vec3(-10.0f, 10.0f, 20.0f);
    color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    gridpos = glm::ivec2(4, 5);
    color_rgb = glm::ivec3(255, 128, 0);

    cout << "position = " << position << endl;
    cout << "color = " << color << endl;
    cout << "gridpos = " << gridpos << endl;
    cout << "color_rgb = " << color_rgb << endl;
    cout << endl;

    //set values when initialising
    glm::vec3 ex1(1.0f, 2.0f, 3.0f);
    glm::vec3 ex2(1.0f, 0.0f, 1.0f);
    cout << "ex1 = " << ex1 << endl;
    cout << "ex2 = " << ex2 << endl;
    cout << endl;

    //other helpers needed later
    glm::vec3 position2(1.0f, 50.0f, 100.0f);
    glm::ivec2 gridpos2 (-9, 80);
    glm::ivec3 color_rgb2(128, 255, 255);

    //assign values from 1 vector to another
    position = position2;
    gridpos = gridpos2;

    cout << "position  = " << position << endl;
    cout << "position2 = " << position2 << endl;
    cout << "gridpos  = " << gridpos << endl;
    cout << "gridpos2 = " << gridpos2 << endl;
    cout << endl;

    //check equality
    cout << "color_rgb2 = " << color_rgb2 << endl;
    cout << endl;

    cout << "position == position2 -> " << ((position == position2) ? "true" : "false") << endl;
    cout << "gridpos == gridpos2 -> " << ((gridpos == gridpos2) ? "true" : "false") << endl;
    cout << "color_rgb == color_rgb2 -> " << ((color_rgb == color_rgb2) ? "true" : "false") << endl;
    cout << endl;

    //addition, substraction, scaler and element multiplication
    cout << "ex1 = " << ex1 << endl;
    cout << "ex2 = " << ex2 << endl;
    cout << "ex1 + ex2  = " << (ex1 + ex2) << endl;
    cout << "ex1 - ex2  = " << (ex1 - ex2) << endl;
    cout << "ex1 * 3.0f = " << (ex1 * 3.0f) << endl;
    cout << "ex1 * ex2  = " << (ex1 * ex2) << endl;
    cout << endl;

    //glm supports a ton of extra functionality: matrixes, dot and cross product, normalisation, inverse, transpose, ...
    //see https://glm.g-truc.net/0.9.9/index.html
    return 0;
}
