#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#include "Model.h"
#include "TrackBall.h"

#define NUM_TRACKED_FRAMES 10

class UserState
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
		// DONE Put your code for an incremental rotation here.
		//
		//
		rotationFromInput=rotationSpinStep*rotationFromInput;

	}

public:
	UserState()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		
		running = true;
		model = Model();
		
		//
		// DONE figure out how to get the cube to the origin
		//

		translateToOrigin = glm::translate(glm::mat4(1.0f), -model.getCentroid());
		translateFromInput = glm::mat4(1.0f);
		rotationFromInput = glm::mat4(1.0f);
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
		// DONE Put your code for a rotation of phi about axis here.
		//
		//
		glm::vec4 tempaxis = glm::vec4(axis, 1);
		//tempaxis=rotationFromInput*tempaxis;
		rotationFromInput = glm::rotate(glm::mat4(1.0f), ROT_SENSITIVITY*phi, glm::vec3(tempaxis)) * rotationFromInput;
		rotationSpinStep = glm::rotate(glm::mat4(1.0f), phi, glm::vec3(tempaxis));
	}
	
	void updateXYTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		#define XY_SENSITIVITY 0.01f // might be helpful to scale translations in x and y
		
		//
		//
		// DONE Put your code for a translation in the x,y direction here.
		//
		//
		translateFromInput = glm::translate(translateFromInput, glm::vec3(XY_SENSITIVITY*(newPos[0]-oldPos[0]),XY_SENSITIVITY*(oldPos[1]-newPos[1]),0));
		//glm::mat4 translateFromInput = glm::translate(translateFromInput, glm::vec3(4,6,0));
		

	}
	
	void updateZTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		#define Z_SENSITIVITY 0.02f // might be helpful to scale translations in z
		
		//
		//
		// DONE Put your code for a translation in the z direction here.
		//
		//
		//printf("z");
		translateFromInput =  translateFromInput * glm::translate(glm::mat4(1.0f), glm::vec3(0,0,Z_SENSITIVITY*(oldPos[1]-newPos[1])));
	}
	
	void setSize(unsigned int x, unsigned int y)
	{
		trackball.setSize(x, y);
	}
	
	Model & getModel()
	{ return model; }
	
	glm::mat4 getCurrentModelTransform() const
	{ 
		//printf("test");
		return translateFromInput*rotationFromInput*currentModelTransform; }
		//return rotationFromInput*currentModelTransform; }
};

#endif
