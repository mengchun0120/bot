#include <cassert>
#include <cmath>
#include "bot_collide.h"

bool floatEqual(float x, float y)
{
	return abs(x - y) < 1e-9f;
}

void testCollideTrue()
{
	float newDelta, delta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1;
	float x2, y2, collideBreathX2, collideBreathY2;

	x1 = 400.0f;
	y1 = 400.0f;
	collideBreathX1 = 50.0f;
	collideBreathY1 = 50.0f;
	speedX1 = -100.0f;
	speedY1 = 200.0f;
	x2 = 290.0f;
	y2 = 520.0f;
	collideBreathX2 = 50.0f;
	collideBreathY2 = 50.0f;
	delta = 0.2f;

	bool collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
									      x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(collide);

	speedX1 = 400.0f;
	speedY1 = 50.0f;
	x2 = 520.0f;
	y2 = 510.0f;
	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
							         x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(collide);

	speedX1 = -50.0f;
	speedY1 = -100.0f;
	x2 = 280.0f;
	y2 = 290.0f;
	delta = 0.5f;
	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                             x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(collide);

	speedX1 = 100.0f;
	speedY1 = -400.0f;
	x2 = 510.0f;
	y2 = 280.0f;
	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                             x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(collide);

	speedX1 = 50.0f;
	speedY1 = 200.0f;
	x2 = 330.0f;
	y2 = 520.0f;

	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                             x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(collide);

	speedX1 = 100.0f;
	speedY1 = 200.0f;
	x2 = 520.0f;
	y2 = 470.0f;

	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                             x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(collide);

	speedX1 = 0.0f;
	speedY1 = -200.0f;
	x2 = 310.0f;
	y2 = 280.0f;

	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                             x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(collide);

	speedX1 = -200.0f;
	speedY1 = -50.0f;
	x2 = 280.0f;
	y2 = 480.0f;
	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                             x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(collide);
}

void testCollideFalse()
{
	float newDelta, delta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1;
	float x2, y2, collideBreathX2, collideBreathY2;

	x1 = 400.0f;
	y1 = 400.0f;
	collideBreathX1 = 50.0f;
	collideBreathY1 = 50.0f;
	speedX1 = 100.0f;
	speedY1 = 200.0f;
	x2 = 290.0f;
	y2 = 520.0f;
	collideBreathX2 = 50.0f;
	collideBreathY2 = 50.0f;
	delta = 0.2f;

	bool collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                                  x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(!collide);

	speedX1 = 10.0f;
	speedY1 = 10.0f;
	x2 = 520.0f;
	y2 = 510.0f;
	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                             x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(!collide);

	speedX1 = 100.0f;
	speedY1 = -4000.0f;
	x2 = 510.0f;
	y2 = 280.0f;
	delta = 0.5f;
	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                             x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(!collide);

	speedX1 = 0.0f;
	speedY1 = -200.0f;
	x2 = 310.0f;
	y2 = 280.0f;
	collide = bot::checkObjCollision(newDelta, x1, y1, collideBreathX1, collideBreathY1, speedX1, speedY1,
		                             x2, y2, collideBreathX2, collideBreathY2, delta);
	assert(!collide);
}