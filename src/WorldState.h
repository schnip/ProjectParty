#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#include "Model.h"

#define NUM_TRACKED_FRAMES 10
#define NUM_OBJECTS 2
const int ARRAY_SIZE = 3;

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	bool running;
	Model model[NUM_OBJECTS];

public:
	int lightStatus;
	float beatTracker;
	float secondsPerBeat;
	float currentTime;
	
	glm::vec3 cameraPos;
	glm::vec3 cameraLook;
	glm::vec3 cameraUp;
	
	glm::vec4 lightPos;
	glm::mat4 lightRotate;
	glm::mat4 lightIncrement;

	///////////////////
	glm::vec4 lightPositions[ARRAY_SIZE];
	glm::vec3 lightIntensities[ARRAY_SIZE];
	glm::mat4 lightRotations[ARRAY_SIZE];
	glm::mat4 lightIncrements[ARRAY_SIZE];
	glm::mat4 lightViews[ARRAY_SIZE];
	///////////////////

	glm::vec4 lightIntensity;
	glm::vec4 baseLightIntensity;
	
	glm::mat4 modelRotate[NUM_OBJECTS];
	glm::mat4 modelIncrement[NUM_OBJECTS];
	glm::mat4 modelTranslate[NUM_OBJECTS];
	glm::mat4 cameraMatrix;
	glm::mat4 lightView;
	
	bool lightRotating;
	bool modelRotating;

	float cursorScrollAmount;
	float center[3];
	float lastClickPos[2];
	float lastFrameDragPos[2];
	float cursorDragAmount[2];
	float cursorAbsolutePos[2];
	int currentRes[2];
	bool mouseButtonDown;

public:
	WorldState()
	{ }

	void init()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		
		running = true;

		for(size_t i=0; i<NUM_OBJECTS; i++) {
			model[i] = Model();
		}
		model[0].init("resources/material_test.obj");
		model[1].init("resources/teapot.obj");
		for(size_t i=0; i<NUM_OBJECTS; i++) {
			model[i].setupAttributeBuffers();
		}
		
		lightStatus = 1;
		secondsPerBeat = 60.0 / 120.0;
		beatTracker = 0;
		glm::vec3 center = model[0].getCentroid();
		glm::vec3 max = model[0].getMaxBound();
		glm::vec3 min = model[0].getMinBound();
		glm::vec3 dim = model[0].getDimension();
		glm::vec3 toMax = max-center;
		printf("model loaded\n");
		printf("min [%.2f %.2f %.2f]\n", min[0], min[1], min[2]);
		printf("max [%.2f %.2f %.2f]\n", max[0], max[1], max[2]);
		printf("cen [%.2f %.2f %.2f]\n", center[0], center[1], center[2]);
		printf("dim [%.2f %.2f %.2f]\n", dim[0], dim[1], dim[2]);
		float camDistance = std::max(dim[0], dim[1]);
		cameraPos = glm::vec3(0,(center+(toMax*5.0f))[1],camDistance);
		cameraLook = glm::vec3(0,0,0);
		cameraUp = glm::vec3(0,1,0);
		
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < ARRAY_SIZE; i++){
			lightPositions[i] = glm::vec4((center + (toMax*2.0f)), 1);
			lightPositions[i][1] = (center + (toMax*6.0f))[1];
		}

		//lightIntensities[0] = glm::vec3(1, 1, 1);
		//lightIntensities[1] = glm::vec3(1, 1, 1);
		//lightIntensities[2] = glm::vec3(1, 1, 1);
		for (int i = 0; i < ARRAY_SIZE; i++){
			lightIntensities[i] = glm::vec3(1, 1, 1);
		}

		for (int i = 0; i < ARRAY_SIZE; i++){
			lightRotations[i] = glm::mat4(1);
		}

		lightIncrements[0] = glm::rotate(glm::mat4(1), -.0314159f, glm::vec3(0, 1, 0));//.05
		lightIncrements[1] = glm::rotate(glm::mat4(1), -.0161803f, glm::vec3(0, 1, 0));//.01
		lightIncrements[2] = glm::rotate(glm::mat4(1), -.0271828f, glm::vec3(0, 1, 0));//.03

		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////

		lightPos = glm::vec4((center+(toMax*2.0f)), 1);
		lightPos[1] = (center+(toMax*6.0f))[1];
		baseLightIntensity = glm::vec4(1, 0, 0, 0);
		lightIntensity = baseLightIntensity;

		lightRotate = glm::mat4(1);
		lightIncrement = glm::rotate(glm::mat4(1), -0.05f, glm::vec3(0,1,0));
		
		for(size_t i=0; i<NUM_OBJECTS; i++) {
			modelRotate[i] = glm::mat4(1);
			modelIncrement[i] = glm::rotate(glm::mat4(1), 0.02f, glm::vec3(0,1,0));
			modelTranslate[i] = glm::translate(glm::mat4(1), -model[i].getCentroid());
		}

		lightRotating = false;
		modelRotating = false;
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
	
	Model const & getModel(int i) const
	{ return model[i]; }
	
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
		
		//Light chaning stuff
		beatTracker = beatTracker + elapsed;
		if (beatTracker >= secondsPerBeat) {
			//printf("Changed Light\n");
			beatTracker = 0;
			if (lightStatus == 0) {
				lightStatus++;
				lightIntensity = glm::vec4(0, 1, 0, 0);
			}
			else if (lightStatus == 1) {
				lightIntensity = glm::vec4(0,0,1,0);
				lightStatus++;
			}
			else if (lightStatus == 2) {
				lightIntensity = glm::vec4(1, 0, 0, 0);
				lightStatus = 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		//spin light
		if (lightRotating){
			for (int i = 0; i < ARRAY_SIZE; i++){
				lightRotations[i] = lightIncrements[i] * lightRotations[i];
			}
		}
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		// spin model
		if(modelRotating) {
			for (int i = 0; i<NUM_OBJECTS; i++) {
				//modelRotate[i] = modelIncrement[i] * modelRotate[i];
				if (i>0) {
					modelRotate[i] = modelIncrement[i] * modelRotate[i];
				}
			}
		}

		// Translate second object
		//if (modelRotating)
		//modelTranslate[1] = glm::translate(glm::mat4(1), glm::vec3(elapsed,0,0))*modelTranslate[1];
		
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		glm::vec3 currentLightPos;
		for (int i = 0; i < ARRAY_SIZE; i++){
			currentLightPos = glm::vec3(lightRotations[i] * lightPositions[i]);
			lightViews[i] = glm::lookAt(currentLightPos, cameraLook, cameraUp);
		}
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		
		this->currentTime = t;
	}
	
	Model & getModel(int i)
	{ return model[i]; }
	
	glm::mat4 getModelTranslate(int i) const
	{ return modelTranslate[i]; }
	
	glm::mat4 getModelRotate(int i) const
	{ return modelRotate[i]; }
	
	glm::mat4 getLightRotate() const
	{ return lightRotate; }
	
	glm::vec4 getLightPos() const
	{ return this->lightPos; }
	
	glm::vec4 getLightIntensity() const
	{ return this->lightIntensity; }
	
	glm::mat4 getCameraMatrix() const
	{ return glm::lookAt(cameraPos, cameraLook, cameraUp); }
	
	glm::mat4 getLightViewMatrix() const
	{ return lightView; }
	
	glm::vec4 getCameraPos() const
	{ return glm::vec4(this->cameraPos, 1); }
	
	void toggleModelRotate()
	{ modelRotating = !modelRotating; }
	
	void toggleLightRotate()
	{ lightRotating = !lightRotating; }
	
	void zoomCamera(int delta)
	{
		float d = pow(0.95, delta);
		printf("%f\n", d);
		cameraPos = cameraPos * d;
	}

	///////////////////////////////////////
	///////////////////////////////////////
	glm::mat4 getLightRotation(int i) const
	{
		return lightRotations[i];
	}


	glm::vec4 getLightPos(int i) const
	{
		return lightPositions[i];
	}


	glm::vec3 getLightIntensity(int i)const
	{
		return lightIntensities[i];
	}


	glm::mat4 getLightView(int i) const
	{
		return lightViews[i];
	}
	///////////////////////////////////////
	///////////////////////////////////////
};

#endif
