#include "PF.h"

Particle::Particle()
{
	vx=0;
	vy=0;
	//scale=0;

	weight = 0;
	sum_weight = 0;

	hist.create(3, NUMBINS, CV_32F);
	hist.setTo(1e-10);
}

PF::PF()
{
	rnd_e.seed(0);
}

void PF::Init(Mat img)
{
	src1 = img.clone();
	src1_rect = src1(rect_1);
	for (int i = 0; i < N_PARTICLES; ++i)
	{
		Particle p;
		p.rect = rect_1;
		p.weight = 1.0 / N_PARTICLES;
		particles.push_back(p);
	}
	predict[0] = 0;
	predict[1] = 0;
}


void PF::LoadImage(Mat img)
{

}

void PF::TraslateParticls()
{
	for (auto p : particles)
	{
		p.rect.x += predict[0];
		p.rect.y += predict[1];
		//p.rect.height *= p.scale;
		//p.rect.width *= p.scale;

		normal_distribution<float> rand_pnd(0, POS_VAR);
		normal_distribution<float> rand_snd(1, SCALE_VAR);

		p.rect.x += rand_pnd(rnd_e);
		p.rect.y += rand_pnd(rnd_e);

		float scale_h = rand_snd(rnd_e);
		float scale_w = rand_snd(rnd_e);

		p.rect.height = scale_h* p.rect.height;
		p.rect.width = scale_w* p.rect.width;


	}	
}

void PF::Updatepredict()
{
	predict[0] = (1 - BETA) * (predict[0] - pre_rect.x) + BETA * (rect_1.x - pre_rect.x);
	predict[1] = (1 - BETA) * (predict[1] - pre_rect.y) + BETA * (rect_1.y - pre_rect.y);
}
