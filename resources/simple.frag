#version 330

uniform mat4 P;
uniform mat4 C;
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 L;
uniform vec4 lightPos;
uniform vec4 camPos;
uniform int shadingMode;

smooth in vec4 smoothColor;
smooth in vec3 smoothPos;

out vec4 fragColor;

vec4 justColor()
{
	vec4 ret = smoothColor;
	ret[0] = (ret[0]+1)/2;
	ret[1] = (ret[1]+1)/2;
	ret[2] = (ret[2]+1)/2;
	return ret;
}

vec4 phong() {
	// Set whether to use colors or white
	vec4 trueColor = justColor();
	//vec4 trueColor = vec4(1,1,1,1);

	// Ambiant reflection
	vec4 retAmb = vec4(.1*trueColor[0],.1*trueColor[1],.1*trueColor[2],0);

	// Diffuse reflectance
	float temp = dot(normalize(L*lightPos-vec4(smoothPos,0)), mR*smoothColor);
	temp = clamp(temp, 0, 1);
	vec4 retDiff = vec4(temp*trueColor[0], temp*trueColor[1], temp*trueColor[2], 1);

	// Specular reflection
	vec4 t = L*lightPos-mR*vec4(smoothPos,0);
	t = normalize(t);
	vec4 n = smoothColor;
	n[3] = 0;
	n = mR*n;
	//t = reflect(t, n);
	vec4 r = 2*dot(n, t)*n - t;
	r = normalize(r);
	vec4 v = camPos-mR*vec4(smoothPos,1);
	v = normalize(v);
	temp = pow(1*dot(r, v),10);
	temp = clamp(temp, 0, 1);
	temp = temp*10;
	vec4 retSpec = vec4(temp, temp, temp, 0);

	return retAmb + retDiff + retSpec;
}

void main()
{
	fragColor = smoothColor;

	if(shadingMode == 0)
		fragColor = smoothColor;
	else if (shadingMode == 1)
		fragColor = smoothColor;
	else
		fragColor = phong();
    //TODO add gouraud and phong shading support
}
