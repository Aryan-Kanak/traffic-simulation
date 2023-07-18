#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream> // for debugging

#include "Car.h"
#include "Road.h"
#include "Shader.h"

const float Car::height = 0.6f;
const float Car::length = 1.0f;
const float Car::width = 0.5f;
const float Car::globalMaxVelocity = 0.1f;
const float Car::slowVelocity = 0.05f;

Car::Car(Road *road) : shader{Shader("vertex.glsl", "fragment.glsl")}, road{road}, model{glm::mat4(1.0f)}, position{0.0f}
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
        -0.3f, 0.4f, 0.25f, 1.0f, 0.9f, 0.0f,
        0.3f, 0.4f, 0.25f, 1.0f, 0.9f, 0.0f,
        0.3f, 0.0f, 0.25f, 1.0f, 0.9f, 0.0f,
        -0.3f, 0.0f, 0.25f, 1.0f, 0.9f, 0.0f,
        -0.3f, 0.4f, -0.25f, 1.0f, 0.9f, 0.0f,
        0.3f, 0.4f, -0.25f, 1.0f, 0.9f, 0.0f,
        0.3f, 0.0f, -0.25f, 1.0f, 0.9f, 0.0f,
        -0.3f, 0.0f, -0.25f, 1.0f, 0.9f, 0.0f,
        -0.5f, 0.0f, 0.25f, 1.0f, 0.9f, 0.0f,
        -0.5f, 0.0f, -0.25f, 1.0f, 0.9f, 0.0f,
        0.5f, 0.0f, 0.25f, 1.0f, 0.9f, 0.0f,
        0.5f, 0.0f, -0.25f, 1.0f, 0.9f, 0.0f,
        -0.4f, 0.0f, 0.25f, 1.0f, 0.9f, 0.0f,
        0.4f, 0.0f, 0.25f, 1.0f, 0.9f, 0.0f,
        0.4f, -0.2f, 0.25f, 1.0f, 0.9f, 0.0f,
        -0.4f, -0.2f, 0.25f, 1.0f, 0.9f, 0.0f,
        -0.4f, 0.0f, -0.25f, 1.0f, 0.9f, 0.0f,
        0.4f, 0.0f, -0.25f, 1.0f, 0.9f, 0.0f,
        0.4f, -0.2f, -0.25f, 1.0f, 0.9f, 0.0f,
        -0.4f, -0.2f, -0.25f, 1.0f, 0.9f, 0.0f,
        -0.5f, 0.0f, -0.15f, 1.0f, 0.9f, 0.0f,
        -0.5f, 0.0f, 0.15f, 1.0f, 0.9f, 0.0f,
        -0.5f, -0.2f, 0.15f, 1.0f, 0.9f, 0.0f,
        -0.5f, -0.2f, -0.15f, 1.0f, 0.9f, 0.0f,
        0.5f, 0.0f, -0.15f, 1.0f, 0.9f, 0.0f,
        0.5f, 0.0f, 0.15f, 1.0f, 0.9f, 0.0f,
        0.5f, -0.2f, 0.15f, 1.0f, 0.9f, 0.0f,
        0.5f, -0.2f, -0.15f, 1.0f, 0.9f, 0.0f
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

    unsigned int indices[] = {
        // main body
        7, 6, 5,
        5, 4, 7,
        3, 2, 1,
        1, 0, 3,
        4, 5, 1,
        1, 0, 4,
        // front window
        0, 4, 9,
        0, 9, 8,
        0, 3, 8,
        4, 7, 9,
        // back window
        1, 5, 11,
        1, 11, 10,
        1, 2, 10,
        5, 6, 11,
        // bottom part
        12, 13, 14,
        12, 14, 15,
        16, 17, 18,
        16, 18, 19,
        14, 15, 19,
        14, 18, 19,
        20, 21, 22,
        20, 22, 23,
        24, 25, 26,
        24, 26, 27,
        22, 23, 27,
        22, 26, 27
    };

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

    glGenVertexArrays(1, &body_vao);
    glBindVertexArray(body_vao);

    glGenBuffers(1, &body_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, body_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &body_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, body_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &lights_vao);
    glBindVertexArray(lights_vao);

    glGenBuffers(1, &lights_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, lights_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &lights_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lights_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightIndices), lightIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    glm::vec2 roadDirection = glm::normalize(glm::vec2(x2 -x1, y2 - y1));
    glm::vec2 carDirection = glm::vec2(-1.0f, 0.0f);
    float angle = acos(glm::dot(roadDirection, carDirection));
    glm::vec2 increment = roadDirection * (float) position;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((x1 + x2) / 2 + increment.x, -3.7f, y1 + increment.y));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Car::render(const glm::mat4& view, const glm::mat4& proj)
{
    updateModelMatrix();
    shader.use();

    shader.setUniformMatrix4fv("model", model);
    shader.setUniformMatrix4fv("projection", proj);
    shader.setUniformMatrix4fv("view", view);

    glBindVertexArray(body_vao);
    glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_INT, 0);
    glBindVertexArray(lights_vao);
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

const float Car::getHeight()
{
    return height;
}

const float Car::getLength()
{
    return length;
}

const float Car::getWidth()
{
    return width;
}

const float Car::getPosition()
{
    return position;
}

const float Car::getVelocity()
{
    return velocity;
}

const float Car::getAcceleration()
{
    return acceleration;
}
