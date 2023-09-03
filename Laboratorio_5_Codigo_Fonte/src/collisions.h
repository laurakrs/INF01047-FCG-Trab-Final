#pragma once

#include "globals.h"
#include "SceneInformation.h"
#include "Triangle.h"

bool TestRayOBBIntersection(
	glm::vec3 ray_origin,        // Ray origin, in world space
	glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
	glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
	glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
	glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
);

bool RayIntersectsSphere(
    glm::vec3 rayOrigin,
    glm::vec3 rayDirection,
    glm::vec4 originalSphereCenter,
    float originalSphereRadius,
    glm::mat4 modelMatrix,
    float& intersectionDistance);

bool RayIntersectsTriangle(
    const glm::vec3 rayOrigin,
    const glm::vec3 rayDirection,
    const Triangle triangle,
    const glm::mat4 modelMatrix,
    float& intersectionDistance);