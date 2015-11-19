#version 330
const int ARRAY_SIZE = 2;


uniform mat4 P;  //projection matrix
uniform mat4 Lp;  //light projection matrix
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 M;  //modelview matrix: M = C * mR * mT

uniform vec3 lightID;	//tells which light to use all idecies are the same

uniform vec4 lightPos;
uniform mat4 Lv; //light view matrix
uniform mat4 Lr;  //light rotation matrix
////
uniform vec4 lightPosOne;
uniform mat4 LvOne;
uniform mat4 LrOne;
uniform vec4 lightPosTwo;
uniform mat4 LvTwo; 
uniform mat4 LrTwo;
uniform vec4 lightPosThree;
uniform mat4 LvThree; 
uniform mat4 LrThree;

uniform vec4 lightPositions[ARRAY_SIZE];
uniform mat4 Lvs[ARRAY_SIZE];
uniform mat4 Lrs[ARRAY_SIZE];
////

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 colorIn;
layout(location = 2) in vec2 texCoord;

out vec3 smoothPos;
out vec3 smoothNorm;
out vec4 smoothColor;
out vec4 shadowPosOne;
out vec4 shadowPosTwo;
out vec4 shadowPosThree;
out vec2 texMapping;

vec4 positionInLightView(vec3 pos, mat4 LLvv)//LLVV is a standin for Lv names are hard
{
	mat4 offset =
	mat4(
		 0.5, 0.0, 0.0, 0.0,
		 0.0, 0.5, 0.0, 0.0,
		 0.0, 0.0, 0.5, 0.0,
		 0.5, 0.5, 0.5, 1.0);
	
	//TODO: move vertex, put in light view
	vec4 vertPos = mR * mT * vec4(pos,1);
	vertPos = offset * Lp * LLvv * vertPos;
	
	return vertPos;
}

float weirdRandomThing(vec2 co) {
	return fract(sin(dot(co, vec2(12.9898, 4.1414))) * 43758.5453);
}

void main()
{
	smoothPos = pos;
	float a = clamp(weirdRandomThing(pos.xy),0,1);
	float b = clamp(weirdRandomThing(pos.xy + vec2(0.1f)),0,1);
	texMapping = vec2(a,b);
	//texMapping = (noise2(pos.xy) + 1) / 2;
	smoothNorm = colorIn*2 - 1;
	smoothColor = vec4(colorIn, 1);
	shadowPosOne = positionInLightView(pos,LvOne);
	shadowPosTwo = positionInLightView(pos,LvTwo);
	shadowPosThree = positionInLightView(pos,LvThree);
	vec4 p = vec4(pos, 1);
	gl_Position = P*M*p;
}