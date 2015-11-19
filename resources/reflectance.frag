#version 330

uniform mat4 P;   //projection matrix
uniform mat4 C;   //camera matrix
uniform mat4 M;   //modelview matrix: M = C * mR * mT
uniform mat3 N;   //inverse transpose of upperleft 3x3 of M

//uniform vec4 Li; //light intensity vector


uniform vec4 lightPosOne;
uniform vec4 lightPosTwo;
uniform vec4 lightPosThree;
uniform mat4 LrOne;
uniform mat4 LrTwo;
uniform mat4 LrThree;

uniform vec4 LiOne;
uniform vec4 LiTwo;
uniform vec4 LiThree;

uniform vec4 lightPositions;
uniform mat4 Lvs;
uniform mat4 Lrs;

uniform vec4 camPos;      //camera position

uniform sampler2D texIdOne;
uniform sampler2D texIdTwo;
uniform sampler2D texIdThree;
uniform sampler2D wnTexId;

in vec2 texMapping;
in vec4 smoothColor;
in vec3 smoothPos;
in vec3 smoothNorm;
in vec4 shadowPosOne;
in vec4 shadowPosTwo;
in vec4 shadowPosThree;

layout(location = 0) out vec4 fragColor;

vec4 reflectance(in vec3 pos, in vec3 norm, in vec3 colorIn, in float visibilityFactor, vec4 lightPos, mat4 Lr, vec4 Li)
{
	float alpha = 10;
	vec4 p = vec4(pos, 1);
	vec4 lp = lightPos;
	vec4 n = normalize(vec4(norm,0));
	vec4 c = vec4(0);
	vec4 ka = vec4(0.05);//ambient
	//vec4 kd = vec4(0.2, 0.2, 0.8, 1);
	vec4 kd = vec4(colorIn.xyz,1);//smoothColor;
	vec4 ks = vec4(.8);

	lp = C*Lr*lp;
	p = M*p;
	n = vec4(N*n.xyz, 0);

	// diffuse coefficent
	vec4 l = normalize(lp - p);
	float d = clamp(dot(l,n), 0, 1) * visibilityFactor;////////

	// specular coefficient
	vec4 v = normalize(c-p);
	vec4 r = normalize(reflect(-l,n));
	float s = clamp(dot(v,r), 0, 1);
	s = pow(s, alpha) * visibilityFactor;
	
	return ka*Li + kd*d*Li + ks*s*Li;
}

float scaleToRange(float v, vec2 bounds) {
	float range = bounds[1] - bounds[0];
	return (v - bounds[0]) / range;
}

vec4 redBlueMap(float v, vec3 bounds) {
	float m;
	if(v < bounds[1])
	{
		m = scaleToRange(v, bounds.xy);
		return mix (vec4(1,0,0,0), vec4(1,1,1,1), m);
	}
	
	m = scaleToRange(v, bounds.yz);
	return mix(vec4(1,1,1,1), vec4(0,0,1,0), m);
}

float getBias()
{
	return 0.004;
	
	//TODO: could do more complex stuff related to normals
}

vec4 getShadowPoint(vec4 shadowPos)
{
	//TODO: adjust for the perspective divide and bias
	vec4 shp = shadowPos;
	shp = shp/shp.w;
	shp.z = shp.z - getBias();
	return shp;
}

float getVisibility(sampler2D texId,vec4 shadowPos)
{
	float visibilityFactor = 1.0;
	vec4 shadowPoint = getShadowPoint(shadowPos);
	//TODO lookup the distance in the shapow map, get surface and occlusion distance

	vec4 shadowTexel = texture(texId, shadowPoint.xy);
	float surfaceDistance = shadowPoint.z;
	float occlusionDistance = shadowTexel.z;
	
	float diff = occlusionDistance - surfaceDistance;

	fragColor = redBlueMap(diff, vec3(-0.1, 0, 0.1));
	

	if(diff < 0){
		return  0.0;
	}

	
	return 1.0;
}

void main()
{
	
	vec4 texColor = texture(wnTexId, texMapping);
	
	float visibilityFactorOne = getVisibility(texIdOne, shadowPosOne);
	float visibilityFactorTwo = getVisibility(texIdTwo, shadowPosTwo);
	float visibilityFactorThree = getVisibility(texIdThree, shadowPosThree);

	float visibilityFactor = (visibilityFactorOne + visibilityFactorTwo + visibilityFactorThree)/3;
	
	vec3 gray = vec3(1,1,1);
	vec4 reflectanceOne = reflectance(smoothPos, smoothNorm, texColor.xyz, visibilityFactorOne, lightPosOne, LrOne, LiOne);//smoothColor.xyz
	vec4 reflectanceTwo = reflectance(smoothPos, smoothNorm, texColor.xyz, visibilityFactorTwo, lightPosTwo, LrTwo, LiTwo);
	vec4 reflectanceThree = reflectance(smoothPos, smoothNorm, texColor.xyz, visibilityFactorThree, lightPosThree, LrThree, LiThree);


	fragColor = (reflectanceOne + reflectanceTwo + reflectanceThree)/3;
}