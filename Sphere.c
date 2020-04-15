#include "Sphere.h"


Sphere* CollisionSphere(Model* model)
{
    Sphere* collisionSphere = malloc(sizeof(Sphere));

    vec3 sum = { 0,0,0 };
    float farthest = 0;
    for (int i = 0; i < model->numVertices; i++)
    {
        sum.x += model->vertexArray[i * 3 + 0];
        sum.y += model->vertexArray[i * 3 + 1];
        sum.z += model->vertexArray[i * 3 + 2];
    }
    collisionSphere->center = ScalarMult(sum, 1.0 / (float)model->numVertices);

    for (int i = 0; i < model->numVertices; i++)
    {
        vec3 current = {
            model->vertexArray[i * 3 + 0],
            model->vertexArray[i * 3 + 1],
            model->vertexArray[i * 3 + 2],
        };
        float distance = Norm(VectorSub(collisionSphere->center, current));

        if (distance > farthest) farthest = distance;
    }
    collisionSphere->radius = farthest;

    // printf("%f %f %f\n", collisionSphere->center.x, collisionSphere->center.y, collisionSphere->center.z);
    // printf("%f\n", collisionSphere->radius);

    return collisionSphere;
}

bool areColliding(Sphere* a, Sphere* b)
{
    return Norm(VectorSub(a->center, b->center)) < a->radius + b->radius;
}