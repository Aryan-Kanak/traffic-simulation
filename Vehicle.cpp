#include "Vehicle.h"

const float Vehicle::globalMaxVelocity = 0.1f;
const float Vehicle::slowVelocity = 0.05f;

void Vehicle::Stop()
{
    stopping = true;
}

void Vehicle::Unstop()
{
    stopping = false;
}

void Vehicle::Slow()
{
    maxVelocity = slowVelocity;
}

void Vehicle::Unslow()
{
    maxVelocity = globalMaxVelocity;
}

const float Vehicle::getHeight()
{
    return height;
}

const float Vehicle::getLength()
{
    return length;
}

const float Vehicle::getWidth()
{
    return width;
}

const float Vehicle::getPosition()
{
    return position;
}

const float Vehicle::getVelocity()
{
    return velocity;
}

const float Vehicle::getAcceleration()
{
    return acceleration;
}
