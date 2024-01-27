#include "Simulation.h"

#include "Settings/MainSettings.h"

Simulation::Simulation(int width, int height, unsigned int seed) :
	width(width), height(height), seed(seed) { }

void Simulation::Initialize()
{
	Initialize(width, height, seed);
}

void Simulation::Initialize(int width, int height, unsigned int seed)
{
	this->width = width;
	this->height = height;
	this->seed = seed;
}

void Simulation::Restart(int width, int height, unsigned int seed)
{
	if (this->width != width || this->height != height || this->seed != seed)
		Initialize(width, height, seed);
	else
		Restart();
}

int Simulation::GetWidth() const
{
	return width;
}

int Simulation::GetHeight() const
{
	return height;
}

unsigned int Simulation::GetSeed() const
{
	return seed;
}
