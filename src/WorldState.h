#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#include "Model.h"
#include "TrackBall.h"

#define NUM_TRACKED_FRAMES 10

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	float currentTime;
	bool running;
	Model model;
	TrackBall trackball;
	
	glm::mat4 translateToOrigin;
	glm::mat4 translateFromInput;
	glm::mat4 rotationFromInput;
	glm::mat4 rotationSpinStep;
	glm::mat4 currentModelTransform;
	
	void spinCube()
	{
		//
		//
		// TODO Put your code for an incremental rotation here.
		//
		//
		rotationFromInput = rotationFromInput * rotationSpinStep;
		currentModelTransform = translateFromInput * rotationFromInput *  translateToOrigin;
	}

public:
	WorldState()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		
		running = true;
		model = Model();
		
		//
		// Done figure out how to get the cube to the origin
		//
		
        translateToOrigin = glm::translate(glm::mat4(1.0f), -model.getCentroid());
		translateFromInput = glm::mat4(1.0f);
		rotationFromInput = glm::mat4(1.0f);
		rotationSpinStep = glm::mat4(1.0f);
        currentModelTransform = translateToOrigin;

	}
	
	void updateFrameTime(float timeAsSeconds)
	{
		for(size_t i=1; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = frameTimes[i-1];
		frameTimes[0] = timeAsSeconds;
	}
	
	void printFPS() const
	{
		float sum = 0.0f;
		float avg = 0.0f;
		float fps = 0.0f;
		
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			sum += frameTimes[i];
		
		avg = sum / NUM_TRACKED_FRAMES;
		fps = 1.0f / avg;
		printf("fps %f\n", fps);
	}
	
	Model const & getModel() const
	{ return model; }
	
	void setRunning(bool r)
	{ running = r; }

	bool isRunning() const
	{ return running; }

	float getCurrentTime() const
	{ return this->currentTime; }

	void timeStep(float t)
	{
		float elapsed = t - this->currentTime;
		this->updateFrameTime(elapsed);
		
		spinCube();
		
		this->currentTime = t;
	}
	
	void updateRotate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		float phi;
		glm::vec3 axis;
		#define ROT_SENSITIVITY 2.0f // might be helpful to scale rotation angle
		
		//computes the appropriate rotation data and stores in phi and axis
		trackball.getRotation(phi, axis, oldPos, newPos);
		
		//
		//
		// TODO Put your code for a rotation of phi about axis here.
		//
		//
        
		glm::vec4 newAxis = glm::inverse(rotationFromInput) * glm::vec4(axis.x, axis.y, axis.z, 1);
		axis = glm::vec3(newAxis.r, newAxis.g, newAxis.b);
        rotationSpinStep = glm::rotate(glm::mat4(1.0f), phi*ROT_SENSITIVITY, axis);
	}
	
	void updateXYTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		#define XY_SENSITIVITY 0.01f // might be helpful to scale translations in x and y
		
		//
		//
		// Done Put your code for a translation in the x,y direction here.
		//
		//*
        glm::vec3 xyDifference = glm::vec3((newPos.x - oldPos.x)*XY_SENSITIVITY, -(newPos.y - oldPos.y)*XY_SENSITIVITY, 0);
        translateFromInput *= glm::translate(glm::mat4(1.0f), xyDifference);
	}
	
	void updateZTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		#define Z_SENSITIVITY 0.02f // might be helpful to scale translations in z
		
		//
		//
		// Done Put your code for a translation in the z direction here.
		//
		//
        glm::vec3 xyDifference = glm::vec3(0,0,(oldPos[0] - newPos[0])*Z_SENSITIVITY);
        translateFromInput *= glm::translate(glm::mat4(1.0f), xyDifference);
	}
	
	void setSize(unsigned int x, unsigned int y)
	{
		trackball.setSize(x, y);
	}
	
	Model & getModel()
	{ return model; }
	
	glm::mat4 getCurrentModelTransform() const
	{ return currentModelTransform; }
};

#endif
