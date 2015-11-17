#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "GLHelper.h"
#include "WorldState.h"

#define TEX_TYPE GL_TEXTURE_2D
//#define TEX_TYPE GL_TEXTURE_RECTANGLE

class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
	}

	~RenderEngine()
	{
		if(initialized)
		{
			// Clean up the buffers
			//glDeleteBuffers(1, &positionBuffer);
			//glDeleteBuffers(1, &colorBuffer);
		}
	}

	void init(WorldState & state)
	{
		initialized = true;
		
		glEnable(GL_DEPTH_TEST);
		
		//glCullFace(GL_BACK);
		//glEnable(GL_CULL_FACE);
		
		setupShader();
		//setupBuffers(state.getModel());
	}

	void display(WorldState & state)
	{
		size_t sid;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region create first shadowtexture
		//????????????????????????????????????????????????????????????????????/
		//????????????????????????????????????????????????????????????????????/
		///// light view /////
		glViewport(0, 0, mapSizeX, mapSizeY);
		//activate our new framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBufferOne);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shader
		sid = 2;
		glUseProgram(shaderProg[sid]);
		//uploadUniforms(shaderProg[sid], state, 0);

		//draw
		for (size_t i = 0; i<NUM_OBJECTS; i++) {
			uploadUniforms(shaderProg[sid], state, i, 0);
			state.getModel(i).draw(shaderProg[sid]);
		}

		checkGLError("light view");

		//????????????????????????????????????????????????????????????????????/
		//????????????????????????????????????????????????????????????????????/
#pragma endregion

#pragma region create second shadowtexture

		//????????????????????????????????????????????????????????????????????/
		//????????????????????????????????????????????????????????????????????/
		///// light view /////
		glViewport(0, 0, mapSizeX, mapSizeY);
		//activate our new framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBufferTwo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shader
		sid = 2;
		glUseProgram(shaderProg[sid]);
		//uploadUniforms(shaderProg[sid], state, 1);

		//draw
		for (size_t i = 0; i<NUM_OBJECTS; i++) {
			uploadUniforms(shaderProg[sid], state, i, 1);
			state.getModel(i).draw(shaderProg[sid]);
		}
		checkGLError("light view");

		//????????????????????????????????????????????????????????????????????/
		//????????????????????????????????????????????????????????????????????/
#pragma endregion

#pragma region create third shadowtexture

		//????????????????????????????????????????????????????????????????????/
		//????????????????????????????????????????????????????????????????????/
		///// light view /////
		glViewport(0, 0, mapSizeX, mapSizeY);
		//activate our new framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBufferThree);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shader
		sid = 2;
		glUseProgram(shaderProg[sid]);
		//uploadUniforms(shaderProg[sid], state, 2);

		//draw
		for (size_t i = 0; i<NUM_OBJECTS; i++) {
			uploadUniforms(shaderProg[sid], state, i, 2);
			state.getModel(i).draw(shaderProg[sid]);
		}
		checkGLError("light view");

		//????????????????????????????????????????????????????????????????????/
		//????????????????????????????????????????????????????????????????????/
#pragma endregion

#pragma region draw light point

		glViewport(0, 0, state.currentRes[0], state.currentRes[1]);
		///// light point /////
		//default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sid = 1;
		glUseProgram(shaderProg[sid]);
		//uploadUniforms(shaderProg[sid], state, 0);

		for (size_t i = 0; i<NUM_OBJECTS; i++) {
			uploadUniforms(shaderProg[sid], state, i, 0);
			state.getModel(i).draw(shaderProg[sid]);
		}
		checkGLError("light point");
#pragma endregion	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		GLuint texId = 0;
#pragma region activate and bind shadowRenderTextureOne
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0 + texId);
		glBindTexture(TEX_TYPE, shadowRenderTextureOne);
		glUniform1i(glGetUniformLocation(shaderProg[sid], "texIdOne"), texId);
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////  
#pragma endregion

#pragma region activate and bind shadowRenderTextureTwo		
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0 + texId + 1);
		glBindTexture(TEX_TYPE, shadowRenderTextureTwo);
		glUniform1i(glGetUniformLocation(shaderProg[sid], "texIdTwo"), texId + 1);
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region activate and bind shadowRenderTextureThree	
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0 + texId + 2);
		glBindTexture(TEX_TYPE, shadowRenderTextureThree);
		glUniform1i(glGetUniformLocation(shaderProg[sid], "texIdThree"), texId + 2);
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





		sid = 0;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProg[sid]);
		//uploadUniforms(shaderProg[shaderId], state);
		
		//draw
		for(size_t i=0; i<NUM_OBJECTS; i++) {
			uploadUniforms(shaderProg[sid], state, i,0);
			state.getModel(i).draw(shaderProg[sid]);
			//glm::mat4 M = glm::mat4(1);
			//glUniformMatrix4fv(glGetUniformLocation(shaderId, "M"), 1, GL_FALSE, &M[0][0]);
		}

		glUseProgram(0);
		checkGLError("model");
	}
	void buildRenderBuffers(size_t xSize, size_t ySize)
	{
		mapSizeX = 2048; // xSize;
		mapSizeY = 2048;//ySize;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region buffer cleanup

		if (shadowRenderTextureOne != 0) {
			glDeleteTextures(1, &shadowRenderTextureOne);
			glDeleteRenderbuffers(1, &shadowRenderBufferOne);
		}
		if (shadowRenderTextureTwo != 0) {
			glDeleteTextures(1, &shadowRenderTextureTwo);
			glDeleteRenderbuffers(1, &shadowRenderBufferTwo);
		}
		if (shadowRenderTextureThree != 0) {
			glDeleteTextures(1, &shadowRenderTextureThree);
			glDeleteRenderbuffers(1, &shadowRenderBufferThree);
		}
#pragma endregion

#pragma region Shadowbufferone setup
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//setup texture
		glGenTextures(1, &shadowRenderTextureOne);
		glBindTexture(TEX_TYPE, shadowRenderTextureOne);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//framebuffer
		glGenFramebuffers(1, &shadowFrameBufferOne); //generate framebuffer object name
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBufferOne); //bind a frame buffer to a framebuffer target

		//make renderbuffer for depth, attach to FB, must have the z depth
		glGenRenderbuffers(1, &shadowRenderBufferOne);
		glBindRenderbuffer(GL_RENDERBUFFER, shadowRenderBufferOne);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mapSizeX, mapSizeY);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shadowRenderBufferOne);

		//make texture, attach to FB
		glTexImage2D(TEX_TYPE, 0, GL_RGBA, mapSizeX, mapSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowRenderTextureOne, 0);
		GLenum colorBufferOne = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &colorBufferOne);

		checkGLError("shadow buffer");
		checkGLFrameBuffer(GL_FRAMEBUFFER, "shadow buffer");
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////  
#pragma endregion

#pragma region ShadowbufferTwo setup
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//setup texture
		glGenTextures(1, &shadowRenderTextureTwo);
		glBindTexture(TEX_TYPE, shadowRenderTextureTwo);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//framebuffer
		glGenFramebuffers(1, &shadowFrameBufferTwo); //generate framebuffer object name
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBufferTwo); //bind a frame buffer to a framebuffer target

		//make renderbuffer for depth, attach to FB, must have the z depth
		glGenRenderbuffers(1, &shadowRenderBufferTwo);
		glBindRenderbuffer(GL_RENDERBUFFER, shadowRenderBufferTwo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mapSizeX, mapSizeY);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shadowRenderBufferTwo);

		//make texture, attach to FB
		glTexImage2D(TEX_TYPE, 0, GL_RGBA, mapSizeX, mapSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowRenderTextureTwo, 0);
		GLenum colorBufferTwo = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &colorBufferTwo);

		checkGLError("shadow buffer");
		checkGLFrameBuffer(GL_FRAMEBUFFER, "shadow buffer");
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////  
#pragma endregion

#pragma region ShadowbufferThree setup
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//setup texture
		glGenTextures(1, &shadowRenderTextureThree);
		glBindTexture(TEX_TYPE, shadowRenderTextureThree);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(TEX_TYPE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//framebuffer
		glGenFramebuffers(1, &shadowFrameBufferThree); //generate framebuffer object name
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBufferThree); //bind a frame buffer to a framebuffer target

		//make renderbuffer for depth, attach to FB, must have the z depth
		glGenRenderbuffers(1, &shadowRenderBufferThree);
		glBindRenderbuffer(GL_RENDERBUFFER, shadowRenderBufferThree);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mapSizeX, mapSizeY);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shadowRenderBufferThree);

		//make texture, attach to FB
		glTexImage2D(TEX_TYPE, 0, GL_RGBA, mapSizeX, mapSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowRenderTextureThree, 0);
		GLenum colorBufferThree = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &colorBufferThree);

		checkGLError("shadow buffer");
		checkGLFrameBuffer(GL_FRAMEBUFFER, "shadow buffer");
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////  
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

private:
	bool initialized;
	GLuint shaderProg[4];
	GLuint vertexArray;
	GLuint quadVertexArray;
	GLuint lightArray;
	GLuint frameBuffer;
	GLuint renderTexture;
	GLuint renderBuffer;
	size_t mapSizeX;
	size_t mapSizeY;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	GLuint shadowFrameBufferOne;
	GLuint shadowFrameBufferTwo;
	GLuint shadowFrameBufferThree;

	GLuint shadowRenderTextureOne;
	GLuint shadowRenderTextureTwo;
	GLuint shadowRenderTextureThree;

	GLuint shadowRenderBufferOne;
	GLuint shadowRenderBufferTwo;
	GLuint shadowRenderBufferThree;

	//GLuint shadowFrameBuffers[ARRAY_SIZE];
	//GLuint shadowRenderTextures[ARRAY_SIZE];
	//GLuint shadowRenderBuffers[ARRAY_SIZE];

	//GLuint shadowFrameBuffers[ARRAY_SIZE] = { shadowFrameBufferOne, shadowFrameBufferTwo, shadowFrameBufferThree };
	//GLuint shadowRenderTextures[ARRAY_SIZE] = { shadowRenderTextureOne, shadowRenderTextureTwo, shadowRenderTextureThree };
	//GLuint shadowRenderBuffers[ARRAY_SIZE] = { shadowRenderBufferOne, shadowRenderBufferTwo, shadowRenderBufferThree };
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glm::vec2 size;
	
	float _near;
	float _far;
	float fov;

	void setupShader()
	{
		char const * vertPath = "resources/reflectance.vert";
		char const * fragPath = "resources/reflectance.frag";
		shaderProg[0] = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);

		char const * lightVPath = "resources/lightPos.vert";
		char const * lightFPath = "resources/lightPos.frag";
		shaderProg[1] = ShaderManager::shaderFromFile(&lightVPath, &lightFPath, 1, 1);

		char const * shdVPath = "resources/shadowMap.vert";
		char const * shdFPath = "resources/shadowMap.frag";
		shaderProg[2] = ShaderManager::shaderFromFile(&shdVPath, &shdFPath, 1, 1);

		char const * postVPath = "resources/texture.vert";
		char const * postFPath = "resources/texture.frag";
		shaderProg[3] = ShaderManager::shaderFromFile(&postVPath, &postFPath, 1, 1);

		checkGLError("shader");
	}

	void setupBuffers(Model & model)
	{



		checkGLError("setup");
	}

	void uploadUniforms(GLuint shaderId, WorldState const & state, int modelId, GLint lightID)
	{
		glm::vec3 dim = state.getModel(0).getDimension();
		float maxDim = std::max(dim[0], std::max(dim[1], dim[2]));
		_near = maxDim*0.1f;
		_far  = maxDim*50.0f;
		fov = 0.9f;
		
		glm::mat4 P = glm::perspective(1.0f, fov, _near, _far);
		glm::mat4 mT = state.getModelTranslate(modelId);
		glm::mat4 mR = state.getModelRotate(modelId);
		glm::mat4 C = state.getCameraMatrix();
		glm::mat4 M = C*mR*mT;
		glm::mat3 N = glm::inverseTranspose(glm::mat3(M));
		glm::vec4 camPos = state.getCameraPos();
		glm::mat4 Lp;
		Lp = glm::perspective(1.0f, fov, _near, _far);
		Lp = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 50.0f);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		glm::vec4 lightPositions[ARRAY_SIZE];
		glm::mat4 Lvs[ARRAY_SIZE];
		glm::mat4 Lrs[ARRAY_SIZE];
		//glm::vec4 Lis[ARRAY_SIZE];

		for (int i = 0; i < ARRAY_SIZE; i++){
			lightPositions[i] = state.getLightPos(i);
			Lvs[i] = state.getLightView(i);
			Lrs[i] = state.getLightRotation(i);
		}

		glm::vec4 lightPos = lightPositions[lightID];
		glm::mat4 Lr = Lrs[lightID];
		glm::mat4 Lv = Lvs[lightID];

		glm::vec4 lightPosOne = lightPositions[0];
		glm::vec4 lightPosTwo = lightPositions[1];
		glm::vec4 lightPosThree = lightPositions[2];
		glm::mat4 LrOne = Lrs[0];
		glm::mat4 LrTwo = Lrs[1];
		glm::mat4 LrThree = Lrs[2];
		glm::mat4 LvOne = Lvs[0];
		glm::mat4 LvTwo = Lvs[1];
		glm::mat4 LvThree = Lvs[2];
		glm::vec4 LiOne = state.getLightIntensity(0);
		glm::vec4 LiTwo = state.getLightIntensity(1);
		glm::vec4 LiThree = state.getLightIntensity(2);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//hacky light source size change
		GLfloat maxDis = state.getModel(0).getDimension()[2] * 3;
		GLfloat distScale = 1.0f / (glm::length(state.getLightRotation(0)*state.getLightPos(0) - camPos) / maxDis);
		glPointSize(glm::mix(1.0f, 10.0f, distScale));

		//printf("cam %f %f %f\n", camPos[0], camPos[1], camPos[2]);
		//printf("light %f %f %f\n", lightPos[0], lightPos[1], lightPos[2]);
		
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "Lp"), 1, GL_FALSE, &Lp[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "C"), 1, GL_FALSE, &C[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "mR"), 1, GL_FALSE, &mR[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "mT"), 1, GL_FALSE, &mT[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "M"), 1, GL_FALSE, &M[0][0]);
		glUniformMatrix3fv(glGetUniformLocation(shaderId, "N"), 1, GL_FALSE, &N[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(shaderId, "Lr"), 1, GL_FALSE, &Lr[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(shaderId, "Lv"), 1, GL_FALSE, &Lv[0][0]);
		//glUniform4fv(glGetUniformLocation(shaderId, "Li"), 1, &Li[0]);
		//glUniform4fv(glGetUniformLocation(shaderId, "lightPos"), 1, &lightPos[0]);
		glUniform4fv(glGetUniformLocation(shaderId, "camPos"), 1, &camPos[0]);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "Lr"), 1, GL_FALSE, &Lr[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "Lv"), 1, GL_FALSE, &Lv[0][0]);
		glUniform4fv(glGetUniformLocation(shaderId, "lightPos"), 1, &lightPos[0]);

		glUniformMatrix4fv(glGetUniformLocation(shaderId, "LrOne"), 1, GL_FALSE, &LrOne[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "LvOne"), 1, GL_FALSE, &LvOne[0][0]);
		glUniform4fv(glGetUniformLocation(shaderId, "lightPosOne"), 1, &lightPosOne[0]);

		glUniformMatrix4fv(glGetUniformLocation(shaderId, "LrTwo"), 1, GL_FALSE, &LrTwo[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "LvTwo"), 1, GL_FALSE, &LvTwo[0][0]);
		glUniform4fv(glGetUniformLocation(shaderId, "lightPosTwo"), 1, &lightPosTwo[0]);

		glUniformMatrix4fv(glGetUniformLocation(shaderId, "LrThree"), 1, GL_FALSE, &LrThree[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "LvThree"), 1, GL_FALSE, &LvThree[0][0]);
		glUniform4fv(glGetUniformLocation(shaderId, "lightPosThree"), 1, &lightPosThree[0]);

		glUniform4fv(glGetUniformLocation(shaderId, "LiOne"), 1, &LiOne[0]);
		glUniform4fv(glGetUniformLocation(shaderId, "LiTwo"), 1, &LiTwo[0]);
		glUniform4fv(glGetUniformLocation(shaderId, "LiThree"), 1, &LiThree[0]);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//		glUniform1f(glGetUniformLocation(shaderId, "elapsedTime"), state.currentTime);
//		glUniform1f(glGetUniformLocation(shaderId, "near"), _near);
//		glUniform1f(glGetUniformLocation(shaderId, "far"), _far);
//		glUniform1f(glGetUniformLocation(shaderId, "fov"), fov);
//		glUniform1f(glGetUniformLocation(shaderId, "cursorScrollAmount"), state.cursorScrollAmount);
		glUniform2f(glGetUniformLocation(shaderId, "resolution"), state.currentRes[0], state.currentRes[1]);
//		glUniform3f(glGetUniformLocation(shaderId, "modelCenter"),  state.center[0], state.center[1], state.center[2]);
//		glUniform3f(glGetUniformLocation(shaderId, "lookAtPos"),  state.cameraLook[0], state.cameraLook[1], state.cameraLook[2]);
//		glUniform3f(glGetUniformLocation(shaderId, "cameraUp"),  state.cameraUp[0], state.cameraUp[1], state.cameraUp[2]);
//		glUniform2f(glGetUniformLocation(shaderId, "cursorAbsolutePos"), state.cursorAbsolutePos[0], state.cursorAbsolutePos[1]);
//		glUniform2f(glGetUniformLocation(shaderId, "cursorDragAmount"), state.cursorDragAmount[0], state.cursorDragAmount[1]);
//		glUniform2f(glGetUniformLocation(shaderId, "lastClickPos"), state.lastClickPos[0], state.lastClickPos[1]);
//		glUniform2f(glGetUniformLocation(shaderId, "lastFrameDragPos"), state.lastFrameDragPos[0], state.lastFrameDragPos[1]);
//		glUniform1i(glGetUniformLocation(shaderId, "mouseButtonDown"), state.mouseButtonDown);
	}

};

#endif