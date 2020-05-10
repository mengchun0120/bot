#version 120

uniform vec2 viewportSize;
uniform vec2 viewportOrigin;
uniform float curTime;
uniform float acceleration;
uniform float particleSize;

attribute vec2 position;
attribute vec4 color;
attribute vec2 direction;
attribute float initSpeed;
attribute float startTime;

varying vec4 vColor;
varying float alpha;

void main()
{
	vColor = color;
	
	float elapsedTime = curTime - startTime;

	vec2 pos = position + direction * (initSpeed * elapsedTime - acceleration * elapsedTime * elapsedTime / 2.0);
	pos -= viewportOrigin;
	gl_Position = vec4(pos * 2.0 / viewportSize, 0.0, 1.0);
	
	alpha = 1.0 - acceleration * elapsedTime / initSpeed;
	if (alpha < 0.0)
	{
		alpha = 0.0;
	}

	gl_PointSize = particleSize;
}