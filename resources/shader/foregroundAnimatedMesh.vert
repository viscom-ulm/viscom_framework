#version 330 core

#define MAX_BONES 128

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in uvec4 indices;
layout(location = 4) in vec4 weights;

uniform mat4 modelMatrix;
uniform mat4 invNodeMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 skinningMatrices[MAX_BONES];

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoords;

vec4 skin_position(vec4 pos)
{
    float sumWeights = weights[0] + weights[1] + weights[2] + weights[3];
    vec4 result = (1.0f - sumWeights) * pos;
    result = result + weights[0] * skinningMatrices[indices[0]] * pos;
    result = result + weights[1] * skinningMatrices[indices[1]] * pos;
    result = result + weights[2] * skinningMatrices[indices[2]] * pos;
    result = result + weights[3] * skinningMatrices[indices[3]] * pos;

    if (sumWeights > 0.0001f) result = invNodeMatrix * result;
    return result;
}

void main()
{
    vec4 skinned_position = skin_position(vec4(position, 1.0));

    vec4 posV4 = modelMatrix * skinned_position;
    vPosition = vec3(posV4);
    vNormal = normalize(normalMatrix * normal);
    vTexCoords = texCoords;
    
    gl_Position = viewProjectionMatrix * posV4;
}

