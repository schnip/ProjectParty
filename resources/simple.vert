#version 330

uniform mat4 P;  //projection matrix
uniform mat4 C;  //camera matrix
uniform mat4 mT; //model transform
uniform mat4 mR; //model rotation
uniform mat4 M;  //modelview matrix: M = C * mR * mT
uniform mat4 N;  //inverse transpose of M
uniform mat4 L;  //light rotation matrix
uniform vec4 lightPos;
uniform vec4 camPos;
uniform int shadingMode;

// N is incorrect, C*mR

in vec3 pos;
in vec3 colorIn;

smooth out vec4 smoothColor;
smooth out vec3 smoothPos;

vec4 justColor()
{
	vec3 ret = colorIn;
	ret[0] = (ret[0]+1)/2;
	ret[1] = (ret[1]+1)/2;
	ret[2] = (ret[2]+1)/2;
	return vec4(ret, 1);
}

vec4 gouraud()
{
	// Set whether to use colors or white
	vec4 trueColor = justColor();
	//vec4 trueColor = vec4(1,1,1,1);

	// Ambiant reflection
	vec4 retAmb = vec4(.1*trueColor[0],.1*trueColor[1],.1*trueColor[2],0);

	// Diffuse reflectance
	float temp = dot(normalize(L*lightPos-vec4(pos,0)), mR*vec4(colorIn, 1));
	temp = clamp(temp, 0, 1);
	vec4 retDiff = vec4(temp*trueColor[0], temp*trueColor[1], temp*trueColor[2], 1);


	// Specular reflection
	vec4 t = L*lightPos-mR*vec4(pos,0);
	t = normalize(t);
	vec4 n = mR*vec4(colorIn, 0);
	//t = reflect(t, n);
	vec4 r = 2*dot(n, t)*n - t;
	//r = clamp(r, 0, 1);
	//r = r*.5;
	r = normalize(r);
	vec4 v = camPos-mR*vec4(pos,1);
	v = normalize(v);
	temp = pow(1*dot(r, v),10);
	//temp = .2 * temp;
	temp = clamp(temp, 0, 1);
	temp = temp*10;
	vec4 retSpec = vec4(temp, temp, temp, 0);
	//vec4 retSpec = vec4(0,0,0,0);


	//vec4 ret = vec4(temp*temp, 0, 2*temp, 1);
	//ret = normalize(ret);
	return  retDiff + retAmb + retSpec;
	//return abs(vec4(1) - vec4(colorIn, 1));
}

vec4 phong()
{
	return vec4(colorIn, 1);
}

void main()
{
	//TODO add gouraud and phong shading support
	smoothPos = pos;

	smoothColor = justColor();
	return;
	
	vec4 pos = vec4(pos, 1);
	gl_Position = P*M*pos;
	
	if(shadingMode == 0)
		smoothColor = justColor();
	else if (shadingMode == 1)
		smoothColor = gouraud();
	else
		smoothColor = phong();
}
