#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream> // for debugging

#include "Car.h"
#include "Road.h"
#include "Shader.h"
#include "Moonlight.h"

const float Car::height = 0.6f;
const float Car::length = 1.0f;
const float Car::width = 0.5f;
const float Car::globalMaxVelocity = 0.1f;
const float Car::slowVelocity = 0.025f;

Car::Car(Road *road, float position) :
    bodyShader{Shader("lighting.vert", "lighting.frag")},
    lightsShader{Shader("default.vert", "default.frag")},
    gBufferShader{Shader("lighting.vert", "gBuffer.frag")},
    road{road}, model{glm::mat4(1.0f)}, position{position}
{
    safeDist = 1.0f;
    reactionTime = 1.0f;
    maxVelocity = globalMaxVelocity;
    maxAcceleration = 0.009f;
    deceleration = 0.028f;
    smoothness = 4.0f;
    velocity = maxVelocity;
    acceleration = 0.0f;
    stopping = false;

    float vertices[] = {
        // main body
        -0.3f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        0.3f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        0.3f, 0.4f, -0.25f, 0.0f, 0.0f, -1.0f,
        0.3f, 0.4f, -0.25f, 0.0f, 0.0f, -1.0f,
        -0.3f, 0.4f, -0.25f, 0.0f, 0.0f, -1.0f,
        -0.3f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        -0.3f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        0.3f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        0.3f, 0.4f, 0.25f, 0.0f, 0.0f, 1.0f,
        0.3f, 0.4f, 0.25f, 0.0f, 0.0f, 1.0f,
        -0.3f, 0.4f, 0.25f, 0.0f, 0.0f, 1.0f,
        -0.3f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        -0.3f, 0.4f, -0.25f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, -0.25f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, 0.25f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, 0.25f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, 0.25f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -0.25f, 0.0f, 1.0f, 0.0f,
        // front window
        -0.3f, 0.4f, 0.25f, -0.2f, 0.1f, 0.0f,
        -0.3f, 0.4f, -0.25f, -0.2f, 0.1f, 0.0f,
        -0.5f, 0.0f, -0.25f, -0.2f, 0.1f, 0.0f,
        -0.3f, 0.4f, 0.25f, -0.2f, 0.1f, 0.0f,
        -0.5f, 0.0f, -0.25f, -0.2f, 0.1f, 0.0f,
        -0.5f, 0.0f, 0.25f, -0.2f, 0.1f, 0.0f,
        -0.3f, 0.4f, 0.25f, 0.0f, 0.0f, 1.0f,
        -0.3f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        -0.3f, 0.4f, -0.25f, 0.0f, 0.0f, -1.0f,
        -0.3f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        // back window
        0.3f, 0.4f, 0.25f, 0.2f, 0.1f, 0.0f,
        0.3f, 0.4f, -0.25f, 0.2f, 0.1f, 0.0f,
        0.5f, 0.0f, -0.25f, 0.2f, 0.1f, 0.0f,
        0.3f, 0.4f, 0.25f, 0.2f, 0.1f, 0.0f,
        0.5f, 0.0f, -0.25f, 0.2f, 0.1f, 0.0f,
        0.5f, 0.0f, 0.25f, 0.2f, 0.1f, 0.0f,
        0.3f, 0.4f, 0.25f, 0.0f, 0.0f, 1.0f,
        0.3f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        0.3f, 0.4f, -0.25f, 0.0f, 0.0f, -1.0f,
        0.3f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        // bottom part
        -0.4f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        0.4f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        0.4f, -0.2f, 0.25f, 0.0f, 0.0f, 1.0f,
        -0.4f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,
        0.4f, -0.2f, 0.25f, 0.0f, 0.0f, 1.0f,
        -0.4f, -0.2f, 0.25f, 0.0f, 0.0f, 1.0f,
        -0.4f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        0.4f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        0.4f, -0.2f, -0.25f, 0.0f, 0.0f, -1.0f,
        -0.4f, 0.0f, -0.25f, 0.0f, 0.0f, -1.0f,
        0.4f, -0.2f, -0.25f, 0.0f, 0.0f, -1.0f,
        -0.4f, -0.2f, -0.25f, 0.0f, 0.0f, -1.0f,
        0.4f, -0.2f, 0.25f, 0.0f, -1.0f, 0.0f,
        -0.4f, -0.2f, 0.25f, 0.0f, -1.0f, 0.0f,
        -0.4f, -0.2f, -0.25f, 0.0f, -1.0f, 0.0f,
        0.4f, -0.2f, 0.25f, 0.0f, -1.0f, 0.0f,
        0.4f, -0.2f, -0.25f, 0.0f, -1.0f, 0.0f,
        -0.4f, -0.2f, -0.25f, 0.0f, -1.0f, 0.0f,
        -0.5f, 0.0f, -0.15f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.15f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.2f, 0.15f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.15f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.2f, 0.15f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.2f, -0.15f, -1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, -0.15f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.15f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.2f, 0.15f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, -0.15f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.2f, 0.15f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.2f, -0.15f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.2f, 0.15f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.2f, -0.15f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.2f, -0.15f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.2f, 0.15f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.2f, 0.15f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.2f, -0.15f, 0.0f, -1.0f, 0.0f,
    };

    float lightVertices[] = {
        -0.5f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f,
        -0.4f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f,
        -0.4f, -0.2f, 0.25f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.2f, 0.25f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.0f, 0.15f, 1.0f, 1.0f, 1.0f,
        -0.4f, 0.0f, 0.15f, 1.0f, 1.0f, 1.0f,
        -0.4f, -0.2f, 0.15f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.2f, 0.15f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.0f, -0.15f, 1.0f, 1.0f, 1.0f,
        -0.4f, 0.0f, -0.15f, 1.0f, 1.0f, 1.0f,
        -0.4f, -0.2f, -0.15f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.2f, -0.15f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.0f, -0.25f, 1.0f, 1.0f, 1.0f,
        -0.4f, 0.0f, -0.25f, 1.0f, 1.0f, 1.0f,
        -0.4f, -0.2f, -0.25f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.2f, -0.25f, 1.0f, 1.0f, 1.0f,
        0.4f, 0.0f, 0.25f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.25f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.2f, 0.25f, 1.0f, 0.0f, 0.0f,
        0.4f, -0.2f, 0.25f, 1.0f, 0.0f, 0.0f,
        0.4f, 0.0f, 0.15f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.15f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.2f, 0.15f, 1.0f, 0.0f, 0.0f,
        0.4f, -0.2f, 0.15f, 1.0f, 0.0f, 0.0f,
        0.4f, 0.0f, -0.15f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, -0.15f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.2f, -0.15f, 1.0f, 0.0f, 0.0f,
        0.4f, -0.2f, -0.15f, 1.0f, 0.0f, 0.0f,
        0.4f, 0.0f, -0.25f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, -0.25f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.2f, -0.25f, 1.0f, 0.0f, 0.0f,
        0.4f, -0.2f, -0.25f, 1.0f, 0.0f, 0.0f
    };

    unsigned int indices[78];
    for (int i = 0; i < 78; ++i) {
        indices[i] = i;
    }

    unsigned int lightIndices[] {
        // headlights
        0, 1, 2,
        0, 2, 3,
        0, 4, 3,
        4, 7, 3,
        7, 6, 3,
        6, 3, 2,
        12, 13, 14,
        12, 14, 15,
        8, 12, 11,
        12, 15, 11,
        15, 14, 11,
        14, 11, 10,
        // taillights
        16, 17, 18,
        16, 18, 19,
        17, 21, 18,
        18, 21, 22,
        23, 22, 19,
        22, 19, 18,
        25, 29, 26,
        26, 29, 30,
        28, 29, 30,
        28, 30, 31,
        31, 30, 27,
        30, 27, 26
    };

    bodyVertexData = VertexData(vertices, sizeof(vertices), indices, sizeof(indices));

    lightsVertexData = VertexData(lightVertices, sizeof(lightVertices), lightIndices, sizeof(lightIndices));

    updateModelMatrix();
}

void Car::updatePosition(Car *next)
{
    // update position and velocity
    float unitTimePerFrame = 1.0f / 60.0f;
    if (velocity + acceleration * unitTimePerFrame < 0) {
        position -= 0.5 * velocity * velocity / acceleration;
        velocity = 0;
    } else {
        velocity += acceleration * unitTimePerFrame;
        position += velocity * unitTimePerFrame + 0.5 * acceleration * unitTimePerFrame * unitTimePerFrame;
    }

    // car is stopping at light
    if (stopping) {
        acceleration = -1 * deceleration * velocity / maxVelocity;
        return;
    }

    // use intelligent driver model to update position
    float interactionAccFactor = 0;
    if (next != nullptr) {
        float deltaX = next->getPosition() - position - next->getLength();
        float deltaV = velocity - next->getVelocity();
        float desiredDist = safeDist;
        float extraDist = velocity * reactionTime + velocity * deltaV / glm::sqrt(2 * maxAcceleration * deceleration);
        if (extraDist > 0) {
            desiredDist += extraDist;
        }
        interactionAccFactor = glm::pow(desiredDist / deltaX, 2);
    }

    float freeRoadAccFactor = glm::pow(velocity / maxVelocity, smoothness);
    acceleration = maxAcceleration * (1 - freeRoadAccFactor - interactionAccFactor);
}

void Car::updateModelMatrix()
{
    float x1 = road->GetX1();
    float x2 = road->GetX2();
    float y1 = road->GetY1();
    float y2 = road->GetY2();
    glm::vec3 roadDirection = glm::normalize(glm::vec3(x2 - x1, 0.0f, y2 - y1));
    glm::vec3 carDirection = glm::vec3(-1.0f, 0.0f, 0.0f);

    glm::vec2 increment = glm::vec2(roadDirection.x, roadDirection.z) * (float) position;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x1 + increment.x, -3.7f, y1 + increment.y));

    float dotProduct = glm::dot(carDirection, roadDirection);
    float rotationAngle = acos(dotProduct);
    glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    if (glm::abs(dotProduct) != 1) {
        rotationAxis = glm::cross(carDirection, roadDirection);
    }
    model = glm::rotate(model, rotationAngle, rotationAxis);
}

void Car::render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3 &cameraPos)
{
    updateModelMatrix();

    // render body
    bodyShader.use();

    bodyShader.setUniformMatrix4fv("model", model);
    bodyShader.setUniformMatrix4fv("projection", proj);
    bodyShader.setUniformMatrix4fv("view", view);
    glm::mat3 normMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
    bodyShader.setUniformMatrix3fv("normMatrix", normMatrix);

    bodyShader.setUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

    // set material properties
    bodyShader.setUniform3f("material.ambient", 0.24725f, 0.1995f, 0.0745f);
    bodyShader.setUniform3f("material.diffuse", 0.75164f, 0.60648f, 0.22648f);
    bodyShader.setUniform3f("material.specular", 0.628281f, 0.555802f, 0.366065f);
    bodyShader.setUniform1f("material.shininess", 51.2f);

    // set moonlight properties
    Moonlight::SetMoonlightUniforms(bodyShader);

    glBindVertexArray(bodyVertexData.vao);
    glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_INT, 0);

    // render lights
    lightsShader.use();

    lightsShader.setUniformMatrix4fv("model", model);
    lightsShader.setUniformMatrix4fv("projection", proj);
    lightsShader.setUniformMatrix4fv("view", view);

    glBindVertexArray(lightsVertexData.vao);
    glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);
}

void Car::fillGBuffer(const glm::mat4& view, const glm::mat4& proj)
{
    updateModelMatrix();

    gBufferShader.use();

    gBufferShader.setUniformMatrix4fv("model", model);
    gBufferShader.setUniformMatrix4fv("projection", proj);
    gBufferShader.setUniformMatrix4fv("view", view);
    glm::mat3 normMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
    gBufferShader.setUniformMatrix3fv("normMatrix", normMatrix);

    gBufferShader.setUniform3f("material.ambient", 0.24725f, 0.1995f, 0.0745f);
    gBufferShader.setUniform3f("material.diffuse", 0.75164f, 0.60648f, 0.22648f);
    gBufferShader.setUniform3f("material.specular", 0.628281f, 0.555802f, 0.366065f);
    gBufferShader.setUniform1f("material.shininess", 51.2f);

    glBindVertexArray(bodyVertexData.vao);
    glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_INT, 0);
}

void Car::forwardRender(const glm::mat4& view, const glm::mat4& proj)
{
    lightsShader.use();

    lightsShader.setUniformMatrix4fv("model", model);
    lightsShader.setUniformMatrix4fv("projection", proj);
    lightsShader.setUniformMatrix4fv("view", view);

    glBindVertexArray(lightsVertexData.vao);
    glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);
}

void Car::Stop()
{
    stopping = true;
}

void Car::Unstop()
{
    stopping = false;
}

void Car::Slow()
{
    maxVelocity = slowVelocity;
}

void Car::Unslow()
{
    maxVelocity = globalMaxVelocity;
}

glm::vec3 *Car::getLightPositions() const
{
    glm::vec3* lightPositions = new glm::vec3[4];
    const glm::vec4 leftHeadLight = glm::vec4(-0.45f, -0.1f, 0.2f, 1.0f);
    const glm::vec4 rightHeadLight = glm::vec4(-0.45f, -0.1f, -0.2f, 1.0f);
    const glm::vec4 leftTailLight = glm::vec4(0.45f, -0.1f, 0.2f, 1.0f);
    const glm::vec4 rightTailLight = glm::vec4(0.45f, -0.1f, -0.2f, 1.0f);

    lightPositions[0] = glm::vec3(model * leftHeadLight);
    lightPositions[1] = glm::vec3(model * rightHeadLight);
    lightPositions[2] = glm::vec3(model * leftTailLight);
    lightPositions[3] = glm::vec3(model * rightTailLight);

    return lightPositions;
}

float Car::getHeight() const
{
    return height;
}

float Car::getLength() const
{
    return length;
}

float Car::getWidth() const
{
    return width;
}

float Car::getPosition() const
{
    return position;
}

float Car::getVelocity() const
{
    return velocity;
}

float Car::getAcceleration() const
{
    return acceleration;
}
