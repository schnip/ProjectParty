#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#include "Model.h"
#include "TrackBall.h"

#define NUM_TRACKED_FRAMES 10

#ifndef NUM_MODELS
#define NUM_MODELS 5
#endif

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	bool running;
	Model model[NUM_MODELS];
	TrackBall trackball;

public:
	
	float currentTime;
	
	glm::vec3 cameraPos;
	glm::vec3 cameraLook;
	glm::vec3 cameraUp;
	
	glm::vec4 lightPos;
	glm::vec3 lightIntensity;
	
	glm::mat4 lightRotate;
	glm::mat4 lightIncrement;
	glm::mat4 modelRotate;
	glm::mat4 modelIncrement;
	glm::mat4 modelTranslate;
	glm::mat4 cameraMatrix;
	glm::mat4 lightView;

	glm::mat4 translateToOrigin;
	glm::mat4 translateFromInput;
	glm::mat4 rotationFromInput;
	glm::mat4 rotationSpinStep;
	glm::mat4 currentModelTransform;
	
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
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		
		running = true;

		for (size_t i=0;i<NUM_MODELS; i++) {
			model[i] = Model();
			model[i].init();
		}
		
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
		
		lightPos = glm::vec4((center+(toMax*2.0f)), 1);
		lightPos[1] = (center+(toMax*6.0f))[1];
		//lightPos = glm::vec4(0,0,0,1); //new
		lightIntensity = glm::vec3(1,1,1);
		
		lightRotate = glm::mat4(1);
		lightIncrement = glm::rotate(glm::mat4(1), -0.025f, glm::vec3(0,1,0));
		
		modelRotate = glm::mat4(1);
		modelIncrement = glm::rotate(glm::mat4(1), .01f, glm::vec3(1,0,0));
		modelTranslate = glm::translate(glm::mat4(1), -model[0].getCentroid());
		
		lightRotating = false;
		modelRotating = false;

		translateToOrigin = glm::translate(glm::mat4(1.0f), -model[0].getCentroid());
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
	
	Model const & getModel(int ref) const
	{ return model[ref]; }
	
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
		
		//spin light
		if(lightRotating)
			lightRotate = lightIncrement * lightRotate;
		
		//spin model
		if(modelRotating)
			modelRotate = modelIncrement * modelRotate;
		
		glm::vec3 currentLightPos = glm::vec3(lightRotate*lightPos);
		lightView = glm::lookAt(currentLightPos, cameraLook, cameraUp);
		
		this->currentTime = t;
	}
	
	Model & getModel(int ref)
	{ return model[ref]; }
	
	glm::mat4 getModelTranslate() const
	{ return modelTranslate; }
	
	glm::mat4 getModelRotate() const
	{ return modelRotate; }
	
	glm::mat4 getLightRotate() const
	{ return lightRotate; }
	
	glm::vec4 getLightPos() const
	{ return this->lightPos; }
	
	glm::vec3 getLightIntensity() const
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

	void setSize(unsigned int x, unsigned int y)
	{
		trackball.setSize(x, y);
	}
	void updateRotate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		float phi;
		glm::vec3 axis;
		#define ROT_SENSITIVITY 2.0f // might be helpful to scale rotation angle
		
		//computes the appropriate rotation data and stores in phi and axis
		trackball.getRotation(phi, axis, oldPos, newPos);
		glm::vec4 tempaxis = glm::vec4(axis, 1);
		//tempaxis=rotationFromInput*tempaxis;
		rotationFromInput = glm::rotate(glm::mat4(1.0f), ROT_SENSITIVITY*phi, glm::vec3(tempaxis)) * rotationFromInput;
		rotationSpinStep = glm::rotate(glm::mat4(1.0f), phi, glm::vec3(tempaxis));
	}
	
	void updateXYTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		#define XY_SENSITIVITY 0.01f // might be helpful to scale translations in x and y
		translateFromInput = glm::translate(translateFromInput, glm::vec3(XY_SENSITIVITY*(newPos[0]-oldPos[0]),XY_SENSITIVITY*(oldPos[1]-newPos[1]),0));
		//glm::mat4 translateFromInput = glm::translate(translateFromInput, glm::vec3(4,6,0));
		

	}
	
	void updateZTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		#define Z_SENSITIVITY 0.02f // might be helpful to scale translations in z
		translateFromInput =  translateFromInput * glm::translate(glm::mat4(1.0f), glm::vec3(0,0,Z_SENSITIVITY*(oldPos[1]-newPos[1])));
	}
};

#endif
