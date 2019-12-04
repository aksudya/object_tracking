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

	pre_rect = rect_1;

	ref_hist = CaculateHist(src1_rect);
}


void PF::LoadImage(Mat img)
{
	src2 = img.clone();
	src2_rect = src2(rect_1);
	Updatepredict();
	TraslateParticls();
	Updateweight();



	UpdateHist();
	
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

		p.img = src2(p.rect);
		p.hist = CaculateHist(p.img);
	}	
}

void PF::Updatepredict()
{
	predict[0] = (1 - BETA) * (predict[0] - pre_rect.x) + BETA * (rect_1.x - pre_rect.x);
	predict[1] = (1 - BETA) * (predict[1] - pre_rect.y) + BETA * (rect_1.y - pre_rect.y);
}

void PF::Updateweight()
{
	float sum=0;
	float max = 1e-10;
	float min = FLT_MAX;
	for (auto p : particles)
	{
		p.weight= exp(-100 * compareHist(ref_hist, p.hist, HISTCMP_BHATTACHARYYA));

		if (p.weight > max)
		{
			max = p.weight;
		}
		if (p.weight < min)
		{
			min = p.weight;
		}
	}
	
	for (auto p : particles)
	{
		p.weight -= min;
		p.weight /= (max - min)+1e-10;

		sum += p.weight;
		p.sum_weight = sum;
	}

}

void PF::UpdateHist()
{
	Mat histnow = CaculateHist(src2_rect);

	for (int i = 0; i < NUMBINS; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ref_hist.at<float>(j, i) = (1 - ALPHA) * ref_hist.at<float>(j, i) + ALPHA * histnow.at<float>(j, i);
		}
	}
}

void PF::UpdateRect()
{
	vector<Particle> newparticles;
	normal_distribution<float> rand_pnd(0, 1);


}



Mat PF::CaculateHist(Mat img)
{
	Mat kernal = CaculatKernal(img.rows, img.cols).clone();

	Mat hist(3, NUMBINS, CV_32F);
	hist.setTo(1e-10);

	for (int i = 0; i < src1_rect.rows; ++i)
	{
		for (int j = 0; j < src1_rect.cols; ++j)
		{
			int index1 = img.at<Vec3b>[0](i, j) / (COLORWITH / NUMBINS);
			int index2 = img.at<Vec3b>[1](i, j) / (COLORWITH / NUMBINS);
			int index3 = img.at<Vec3b>[2](i, j) / (COLORWITH / NUMBINS);
			
			hist.at<float>(0, index1) += kernal.at<float>(i, j);
			hist.at<float>(1, index1) += kernal.at<float>(i, j);
			hist.at<float>(2, index1) += kernal.at<float>(i, j);
		}
	}

	return hist;
}

Mat PF::CaculatKernal(int rows, int cols)
{
	int h = rows;
	int w = cols;
	Mat kernal(rows, cols, CV_32F);
	float epanechnikov_cd = 0.1 * PI * h * w;
	float kernel_sum = 0.0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			float x = i - h / 2.0;
			float  y = j - w / 2.0;
			float norm_x = x * x / (h * h / 4.0) + y * y / (w * w / 4.0);
			float result = norm_x < 1 ? (epanechnikov_cd * (1.0 - norm_x)) : 0;
			kernal.at<float>(i, j) = result;
			kernel_sum += result;
		}
	}
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			kernal.at<float>(i, j) /= kernel_sum;
		}
	}
	return kernal;
}


