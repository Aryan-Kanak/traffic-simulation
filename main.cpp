#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Car.h"
#include "Road.h"
#include "Skybox.h"
#include "Moonlight.h"
#include "Sphere.h"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 600;

Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window, Camera &camera) {
    bool isSprinting = false;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
        isSprinting = true;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveForward(deltaTime, isSprinting);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveBackward(deltaTime, isSprinting);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveLeft(deltaTime, isSprinting);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveRight(deltaTime, isSprinting);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    camera.handleMouseInput(xpos, ypos);
}

int main()
{
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Traffic Simulation", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    // sphere test
    Sphere sphere(5, -3.0f, -3.0f, 1.5f, 18, 36);

    // setup skybox
    std::vector<std::string> faces{
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
    Skybox skybox(faces);
    
    // create roads
    Road roads[] = {
        Road(10, 0, 10, 30),
        Road(11, 30, 11, 0),
        Road(20, 0, 20, 30),
        Road(21, 30, 21, 0),
        Road(30, 10, 0, 10),
        Road(0, 11, 30, 11),
        Road(30, 20, 0, 20),
        Road(0, 21, 30, 21)
    };
    const unsigned int roadsLen = 8;

    roads[0].spawnCar(5.0f);
    roads[0].spawnCar(0.0f);
    roads[1].spawnCar(5.0f);
    roads[1].spawnCar(0.0f);
    roads[2].spawnCar(5.0f);
    roads[2].spawnCar(0.0f);
    roads[3].spawnCar(5.0f);
    roads[3].spawnCar(0.0f);
    roads[4].spawnCar(5.0f);
    roads[4].spawnCar(0.0f);
    roads[5].spawnCar(5.0f);
    roads[5].spawnCar(0.0f);
    roads[6].spawnCar(5.0f);
    roads[6].spawnCar(0.0f);
    roads[7].spawnCar(5.0f);
    roads[7].spawnCar(0.0f);

    // add traffic lights
    roads[0].addTrafficLight(11.5f, State::red);
    roads[0].addTrafficLight(21.5f, State::red);
    roads[1].addTrafficLight(10.5f, State::red);
    roads[1].addTrafficLight(20.5f, State::red);
    roads[2].addTrafficLight(11.5f, State::red);
    roads[2].addTrafficLight(21.5f, State::red);
    roads[3].addTrafficLight(10.5f, State::red);
    roads[3].addTrafficLight(20.5f, State::red);
    roads[4].addTrafficLight(10.5f, State::green);
    roads[4].addTrafficLight(20.5f, State::green);
    roads[5].addTrafficLight(11.5f, State::green);
    roads[5].addTrafficLight(21.5f, State::green);
    roads[6].addTrafficLight(10.5f, State::green);
    roads[6].addTrafficLight(20.5f, State::green);
    roads[7].addTrafficLight(11.5f, State::green);
    roads[7].addTrafficLight(21.5f, State::green);

    // set up G-buffer
    unsigned int gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    unsigned int gPosition;
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    unsigned int gNormal;
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    unsigned int gAlbedo;
    glGenTextures(1, &gAlbedo);
    glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

    unsigned int gSpecular;
    glGenTextures(1, &gSpecular);
    glBindTexture(GL_TEXTURE_2D, gSpecular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gSpecular, 0);

    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);

    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Framebuffer not complete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // set up deferred shading pass
    Shader deferredShadingShader("deferredShading.vert", "deferredShading.frag");
    deferredShadingShader.use();
    deferredShadingShader.setUniform1i("gPosition", 0);
    deferredShadingShader.setUniform1i("gNormal", 1);
    deferredShadingShader.setUniform1i("gAlbedo", 2);
    deferredShadingShader.setUniform1i("gSpecular", 3);

    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    unsigned int quadVAO;
    unsigned int quadVBO;

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window, camera);

        // get positions of point lights
        glm::vec3 lightPositions[64];
        for (int i = 0; i < roadsLen; ++i) {
            glm::vec3 *curPositions = roads[i].getLightPositions();
            for (int j = 0; j < 8; ++j) {
                lightPositions[8 * i + j] = curPositions[j];
            }
            delete[] curPositions;
        }

        // render scene completely with forward rendering
        /*glm::mat4 view = camera.getViewMatrix();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
        for (int i = 0; i < roadsLen; ++i) {
            roads[i].render(view, projection, camera.getCameraPos(), lightPositions);
            roads[i].update(deltaTime);
        }
        skybox.render(skyboxView, projection);*/

        // bind and fill G-buffer
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
        for (int i = 0; i < roadsLen; ++i) {
            roads[i].fillGBuffer(view, projection);
            roads[i].update(deltaTime);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // calculate lighting
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        deferredShadingShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gSpecular);

        glm::vec3 viewPos = camera.getCameraPos();
        deferredShadingShader.setUniform3f("viewPos", viewPos.x, viewPos.y, viewPos.z);

        // set moonlight properties
        Moonlight::SetMoonlightUniforms(deferredShadingShader);

        // set point light properties
        for (int i = 0; i < 1; ++i) {
            std::string uniformName = "pointLights[" + std::to_string(i) + "].position";
            deferredShadingShader.setUniform3f(uniformName, lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
            uniformName = "pointLights[" + std::to_string(i) + "].constant";
            deferredShadingShader.setUniform1f(uniformName, 1.0f);
            uniformName = "pointLights[" + std::to_string(i) + "].linear";
            deferredShadingShader.setUniform1f(uniformName, 0.7f);
            uniformName = "pointLights[" + std::to_string(i) + "].quadratic";
            deferredShadingShader.setUniform1f(uniformName, 1.8f);
            if (i % 4 == 0 || i % 4 == 1) {
                // headlight
                uniformName = "pointLights[" + std::to_string(i) + "].ambient";
                deferredShadingShader.setUniform3f(uniformName, 0.2f, 0.2f, 0.2f);
                uniformName = "pointLights[" + std::to_string(i) + "].diffuse";
                deferredShadingShader.setUniform3f(uniformName, 0.5f, 0.5f, 0.5f);
                uniformName = "pointLights[" + std::to_string(i) + "].specular";
                deferredShadingShader.setUniform3f(uniformName, 1.0f, 1.0f, 1.0f);
            }
            else {
                // taillight
                uniformName = "pointLights[" + std::to_string(i) + "].ambient";
                deferredShadingShader.setUniform3f(uniformName, 0.2f, 0.0f, 0.0f);
                uniformName = "pointLights[" + std::to_string(i) + "].diffuse";
                deferredShadingShader.setUniform3f(uniformName, 0.5f, 0.0f, 0.0f);
                uniformName = "pointLights[" + std::to_string(i) + "].specular";
                deferredShadingShader.setUniform3f(uniformName, 1.0f, 0.0f, 0.0f);
            }
        }

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // copy depth buffer data
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // forward render
        for (int i = 0; i < roadsLen; ++i) {
            roads[i].forwardRender(view, projection);
        }
        //skybox.render(skyboxView, projection);
        sphere.render(view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
