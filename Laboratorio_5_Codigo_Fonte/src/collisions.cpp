// Arquivo separado para os testes de colisao
#pragma once

#include "collisions.h"

//FONTE: http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
// Com adaptações para suportar escala de objetos (as quais não estavam inclusas na fonte original)
bool TestRayOBBIntersection(
	glm::vec3 ray_origin,        // Ray origin, in world space
	glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
	glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
	glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
	glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
)
{
	// Intersection method from Real-Time Rendering and Essential Mathematics for Games
	
	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

	glm::vec3 delta = OBBposition_worldspace - ray_origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
        glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
        // Alteração do código original para comportar escala
        float scaleX = glm::length(xaxis);
        xaxis /= scaleX;

		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray_direction, xaxis);

		if ( fabs(f) > 0.001f ){ // Standard case
            // Alteração do código original para comportar escala
			// float t1 = (e+aabb_min.x)/f; // Intersection with the "left" plane
			// float t2 = (e+aabb_max.x)/f; // Intersection with the "right" plane
			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections
            float t1 = (e+aabb_min.x*scaleX)/f;
            float t2 = (e+aabb_max.x*scaleX)/f;

			// We want t1 to represent the nearest intersection, 
			// so if it's not the case, invert t1 and t2
			if (t1>t2){
				float w=t1;t1=t2;t2=w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if ( t2 < tMax )
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if ( t1 > tMin )
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin )
				return false;

		}else{ // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if(-e+aabb_min.x > 0.0f || -e+aabb_max.x < 0.0f)
				return false;
		}
	}

	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
        // Alteração do código original para comportar escala
        float scaleY = glm::length(yaxis);
        yaxis /= scaleY;

		float e = glm::dot(yaxis, delta);
		float f = glm::dot(ray_direction, yaxis);

		if ( fabs(f) > 0.001f ){
            
            // Alteração do código original para comportar escala
			// float t1 = (e+aabb_min.y)/f;
			// float t2 = (e+aabb_max.y)/f;
            float t1 = (e+aabb_min.y*scaleY)/f;
            float t2 = (e+aabb_max.y*scaleY)/f;

			if (t1>t2){float w=t1;t1=t2;t2=w;}

			if ( t2 < tMax )
				tMax = t2;
			if ( t1 > tMin )
				tMin = t1;
			if (tMin > tMax)
				return false;

		}else{
			if(-e+aabb_min.y > 0.0f || -e+aabb_max.y < 0.0f)
				return false;
		}
	}

	{
		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
        // Alteração do código original para comportar escala
        float scaleZ = glm::length(zaxis);
        zaxis /= scaleZ;

		float e = glm::dot(zaxis, delta);
		float f = glm::dot(ray_direction, zaxis);

		if ( fabs(f) > 0.001f ){
            
            // Alteração do código original para comportar escala
			// float t1 = (e+aabb_min.z)/f;
			// float t2 = (e+aabb_max.z)/f;
            float t1 = (e+aabb_min.z*scaleZ)/f;
            float t2 = (e+aabb_max.z*scaleZ)/f;

			if (t1>t2){float w=t1;t1=t2;t2=w;}

			if ( t2 < tMax )
				tMax = t2;
			if ( t1 > tMin )
				tMin = t1;
			if (tMin > tMax)
				return false;

		}else{
			if(-e+aabb_min.z > 0.0f || -e+aabb_max.z < 0.0f)
				return false;
		}
	}

	intersection_distance = tMin;
	return true;

}

bool RayIntersectsSphere(
    glm::vec3 rayOrigin, 
    glm::vec3 rayDirection, 
    glm::vec3 sphereCenter, 
    float sphereRadius, 
    glm::mat4 modelMatrix, 
    float& intersectionDistance)
{
    // Extract translation from the model matrix
    glm::vec3 translation = glm::vec3(modelMatrix[3]);

    // Adjust the sphere's center using the translation
    sphereCenter += translation;

    // Extract scale factors from the model matrix
    glm::vec3 scaleFactors = glm::vec3(glm::length(modelMatrix[0]), glm::length(modelMatrix[1]), glm::length(modelMatrix[2]));

    // Use the largest scale factor for the sphere's radius to ensure intersection
    float maxScale = glm::max(scaleFactors.x, glm::max(scaleFactors.y, scaleFactors.z));
    sphereRadius *= maxScale;

    // Now test for intersection with the transformed sphere
    glm::vec3 toSphere = sphereCenter - rayOrigin;
    float t = glm::dot(toSphere, rayDirection);
    glm::vec3 closestPoint = rayOrigin + rayDirection * t;

    float distSquared = glm::dot(toSphere, toSphere) - t * t;
    float radiusSquared = sphereRadius * sphereRadius;

    if (distSquared > radiusSquared)
        return false; // No intersection

    float dt = sqrt(radiusSquared - distSquared);
    float t1 = t - dt;
    float t2 = t + dt;

    if (t2 < 0)
        return false; // Object is behind the camera

    intersectionDistance = (t1 < 0) ? t2 : t1; // Get the nearest intersection point in front of the camera
    return true;
}

