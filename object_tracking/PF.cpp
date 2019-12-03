#include "PF.h"

Particle::Particle()
{
	x=0;
	y=0;
	vx=0;
	vy=0;
	hx=0;
	hy=0;
	a=0;
	weight = 0;


	hist.create(3, NUMBINS, CV_32F);
	hist.setTo(0);
}

void PF::Init(Mat img)
{

}

void PF::LoadImage(Mat img)
{

}
