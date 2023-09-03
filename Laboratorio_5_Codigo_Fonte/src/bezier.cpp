// Arquivo separado para a curva de bezier
#include "bezier.h"


// bezier curve
glm::vec3 bezierCurve(float t, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4){

    float t = t - floor(t);
    glm::vec3 point = pow(1.0f-t,3.0f)*p1 +
                    (3.0f * t * pow((1.0f-t),2.0f))*p2 +
                    ((3.0f*pow(t,2.0f) * (1.0f-t))*p3) +
                    pow(t,3.0f)*p4;

    return point;
}

/*
// Function to generate a circle in the XZ plane using a Bezier curve
std::vector<glm::vec3> generateCircle(float radius, float height, int numSegments) {
    std::vector<glm::vec3> points;

    // Calculate control points for the Bezier curve
    glm::vec3 p1(radius, 0.0f, 0.0f);
    glm::vec3 p2(radius, 0.0f, 0.0f);
    glm::vec3 p3(-radius, 0.0f, 0.0f);
    glm::vec3 p4(-radius, 0.0f, 0.0f);

    // Create the Bezier curve points
    for (int i = 0; i <= numSegments; i++) {
        float t = static_cast<float>(i) / numSegments;
        float angle = t * 2.0f * glm::pi<float>();
        glm::vec3 point(radius * glm::cos(angle), height, radius * glm::sin(angle));

        // Add the point to the vector
        points.push_back(point);
    }

    return points;
}
In this function:

radius is the radius of the circle.
height is the height at which the circle is placed on the Y-axis.
numSegments is the number of segments to approximate the circle. More segments will result in a smoother curve.
The function calculates control points for the Bezier curve that create a circular shape in the XZ plane and then generates points along this curve using the parametric equation for a circle. It returns a vector of glm::vec3 points representing the circle in 3D space.

You can call this function in your main program, specifying the radius, height, and numSegments values to generate the circle and use the resulting vector of points as needed in your rendering or game logic.




User
should I use these points in my bezierCurve function? 

// bezier curve
glm::vec4 bezierCurve(float t, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4){

   glm::vec4 point = pow(1.0f-t,3.0f)*p1 +
                    (3.0f * t * pow((1.0f-t),2.0f))*p2 +
                    ((3.0f*pow(t,2.0f) * (1.0f-t))*p3) +
                    pow(t,3.0f)*p4;
}
ChatGPT
The bezierCurve function you've provided appears to be a cubic Bezier curve function that takes four control points (p1, p2, p3, and p4) and calculates a point on the curve at a given parameter t. This function is suitable for generating points on a cubic Bezier curve but is not directly related to generating a circle in the XZ plane.

If you want to generate a circle in the XZ plane using the control points derived from the generateCircle function I provided earlier, you would typically use those circle points directly without involving the bezierCurve function. The bezierCurve function is used when you want to interpolate points along a Bezier curve defined by its control points.

Here's how you can use the generateCircle function:

cpp
Copy code
int main() {
    float radius = 5.0f;
    float height = 0.0f;
    int numSegments = 100; // Adjust the number of segments for smoother or coarser circle

    std::vector<glm::vec3> circlePoints = generateCircle(radius, height, numSegments);

    // Now, 'circlePoints' contains the points of the circle in the XZ plane.
    // You can use these points directly in your rendering or game logic.
    
    // Example usage: print the generated points
    for (const glm::vec3& point : circlePoints) {
        std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")\n";
    }

    return 0;
}




int main() {
    float radius = 5.0f;
    float height = 0.0f;
    int numSegments = 100; // Adjust the number of segments for smoother or coarser circle

    std::vector<glm::vec3> circleControlPoints = generateCircle(radius, height, numSegments);

    // Now, 'circleControlPoints' contains the control points of the circle in the XZ plane.
    // You can use these control points with your 'bezierCurve' function to interpolate points.

    int numInterpolatedPoints = 100; // Number of interpolated points along the curve
    std::vector<glm::vec3> interpolatedPoints;

    for (int i = 0; i <= numInterpolatedPoints; i++) {
        float t = static_cast<float>(i) / numInterpolatedPoints;
        glm::vec3 point = bezierCurve(t, circleControlPoints[0], circleControlPoints[1], circleControlPoints[2], circleControlPoints[3]);
        interpolatedPoints.push_back(point);
    }

    // Now, 'interpolatedPoints' contains the points interpolated along the Bezier curve.

    // Example usage: print the interpolated points
    for (const glm::vec3& point : interpolatedPoints) {
        std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")\n";
    }

    return 0;
}
In this code, we first generate the control points for the circle using generateCircle. Then, we interpolate points along the Bezier curve using the bezierCurve function with these control points. The interpolatedPoints vector will contain the points interpolated along the curve. You can use these points for rendering or other purposes as needed.




User
with the interpolatedPoints I can draw a circle?
ChatGPT
Yes, with the interpolatedPoints obtained from the Bezier curve, you can draw a circle by connecting these points sequentially. To visualize the circle, you can use a graphics library or framework like OpenGL, SDL, or a graphical user interface library like Qt or wxWidgets. Here's a simple example using OpenGL to draw the circle:

cpp
Copy code
#include <iostream>
#include <vector>
#include <GL/glut.h> // You'll need OpenGL/GLUT for this example

// bezier curve
glm::vec3 bezierCurve(float t, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {
    glm::vec3 point = pow(1.0f - t, 3.0f) * p1 +
                     3.0f * t * t * (1.0f - t) * p2 +
                     3.0f * t * (1.0f - t) * (1.0f - t) * p3 +
                     pow(t, 3.0f) * p4;

    return point;
}

void drawCircle(const std::vector<glm::vec3>& points) {
    glBegin(GL_LINE_LOOP);
    for (const glm::vec3& point : points) {
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
}


    float radius = 5.0f;
    float height = 0.0f;
    int numSegments = 100;
    int numInterpolatedPoints = 100;

    std::vector<glm::vec3> circleControlPoints = generateCircle(radius, height, numSegments);

    std::vector<glm::vec3> interpolatedPoints;
    for (int i = 0; i <= numInterpolatedPoints; i++) {
        float t = static_cast<float>(i) / numInterpolatedPoints;
        glm::vec3 point = bezierCurve(t, circleControlPoints[0], circleControlPoints[1], circleControlPoints[2], circleControlPoints[3]);
        interpolatedPoints.push_back(point);
    }

    // Set up OpenGL projection and modelview matrices here

    // Main rendering loop
    glutDisplayFunc([]() {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f); // White color

        drawCircle(interpolatedPoints);

        glFlush();
    });

    glutMainLoop();

    return 0;
}
This example initializes OpenGL, calculates the Bezier curve points with bezierCurve, and then uses the drawCircle function to render the circle by connecting the interpolated points. Make sure you have the necessary OpenGL/GLUT libraries installed and properly set up for this code to work.






    float radius = 5.0f;
    float height = 0.0f;
    int numSegments = 100; // Adjust the number of segments for smoother or coarser circle

    std::vector<glm::vec3> circleControlPoints = generateCircle(radius, height, numSegments);

    // Now, 'circleControlPoints' contains the control points of the circle in the XZ plane.
    // You can use these control points with your 'bezierCurve' function to interpolate points.

    int numInterpolatedPoints = 100; // Number of interpolated points along the curve
    std::vector<glm::vec3> interpolatedPoints;

    for (int i = 0; i <= numInterpolatedPoints; i++) {
        float t = static_cast<float>(i) / numInterpolatedPoints;
        glm::vec3 point = bezierCurve(t, circleControlPoints[0], circleControlPoints[1], circleControlPoints[2], circleControlPoints[3]);
        interpolatedPoints.push_back(point);
    }

    // Now, 'interpolatedPoints' contains the points interpolated along the Bezier curve.

    // Example usage: print the interpolated points
    for (const glm::vec3& point : interpolatedPoints) {
        std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")\n";
    }

    return 0;


#include <iostream>
#include <vector>
#include <GL/glut.h> // You'll need OpenGL/GLUT for this example

// bezier curve
glm::vec3 bezierCurve(float t, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {
    glm::vec3 point = pow(1.0f - t, 3.0f) * p1 +
                     3.0f * t * t * (1.0f - t) * p2 +
                     3.0f * t * (1.0f - t) * (1.0f - t) * p3 +
                     pow(t, 3.0f) * p4;

    return point;
}

void drawCircle(const std::vector<glm::vec3>& points) {
    glBegin(GL_LINE_LOOP);
    for (const glm::vec3& point : points) {
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
}

int main(int argc, char** argv) {
    // Initialize OpenGL/GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Bezier Circle");

    float radius = 5.0f;
    float height = 0.0f;
    int numSegments = 100;
    int numInterpolatedPoints = 100;

    std::vector<glm::vec3> circleControlPoints = generateCircle(radius, height, numSegments);

    std::vector<glm::vec3> interpolatedPoints;
    for (int i = 0; i <= numInterpolatedPoints; i++) {
        float t = static_cast<float>(i) / numInterpolatedPoints;
        glm::vec3 point = bezierCurve(t, circleControlPoints[0], circleControlPoints[1], circleControlPoints[2], circleControlPoints[3]);
        interpolatedPoints.push_back(point);
    }

    // Set up OpenGL projection and modelview matrices here

    // Main rendering loop
    glutDisplayFunc([]() {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f); // White color

        drawCircle(interpolatedPoints);

        glFlush();
    });

    glutMainLoop();

    return 0;
}
*/