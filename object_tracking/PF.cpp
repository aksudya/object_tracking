#include "PF.h"

Particle::Particle()
{
	vx=0;
	vy=0;
	//scale=0;

	weight = 0;
	sum_weight = 0;

	hist.create(NUMBINS, 3, CV_32F);
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

	//for (auto pp : particles)
	//{
	//	cv::rectangle(src1, pp.rect, cv::Scalar(0, 255, 0), 2, 8, 0);  // »­¾ØÐÎ¿ò
	//}

	Updateweight();
	UpdateRect();


	UpdateHist();
	pre_rect = rect_1;
	src1 = src2.clone();
	src1_rect = src2_rect;
	
}

void PF::TraslateParticls()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].rect.x += predict[0];
		particles[i].rect.y += predict[1];
		//p.rect.height *= p.scale;
		//p.rect.width *= p.scale;

		normal_distribution<float> rand_pnd(0, POS_VAR);
		normal_distribution<float> rand_snd(1, SCALE_VAR);

		particles[i].rect.x += rand_pnd(rnd_e);
		particles[i].rect.y += rand_pnd(rnd_e);

		//float scale_h = rand_snd(rnd_e);
		//float scale_w = rand_snd(rnd_e);

		float scale_h = 1;
		float scale_w = 1;

		float hh = (scale_h * particles[i].rect.height - particles[i].rect.height)/2;
		float ww = (scale_w * particles[i].rect.width - particles[i].rect.width) / 2;

		particles[i].rect.x -= ww;
		particles[i].rect.y -= hh;

		particles[i].rect.height = scale_h* particles[i].rect.height;
		particles[i].rect.width = scale_w* particles[i].rect.width;



		if(particles[i].rect.height<1)
		{
			particles[i].rect.height = 1;
		}
		if (particles[i].rect.width < 1)
		{
			particles[i].rect.width = 1;
		}




		if (particles[i].rect.x <= 0)
		{
			particles[i].rect.x = 1;
		}
		if (particles[i].rect.y <= 0)
		{
			particles[i].rect.y = 1;
		}

		if (particles[i].rect.x >= src1.cols -1)
		{
			particles[i].rect.x = src1.cols -1;
		}
		if (particles[i].rect.y >= src1.rows-1)
		{
			particles[i].rect.y = src1.rows -1;
		}

		particles[i].img = src2(particles[i].rect);
		particles[i].hist = CaculateHist(particles[i].img);
	}	
}

void PF::Updatepredict()
{
	predict[0] = (1 - BETA) * (predict[0]) + BETA * (rect_1.x - pre_rect.x);
	predict[1] = (1 - BETA) * (predict[1]) + BETA * (rect_1.y - pre_rect.y);
}

void PF::Updateweight()
{
	float sum=0;
	float max = 1e-10;
	float min = FLT_MAX;
	for (int i=0;i<particles.size();i++)
	{
		particles[i].weight= exp(-100 * compareHist(ref_hist, particles[i].hist, HISTCMP_BHATTACHARYYA));
		sum += particles[i].weight;
		
	}
	float sum1 = 0;
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].weight /= sum;
		

		sum1 += particles[i].weight;
		particles[i].sum_weight = sum1;
	}

}

void PF::UpdateHist()
{
	Mat histnow = CaculateHist(src2_rect);

	for (int i = 0; i < NUMBINS; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ref_hist.at<float>(i, j) = (1 - ALPHA) * ref_hist.at<float>(i, j) + ALPHA * histnow.at<float>(i, j);
		}
	}
}

void PF::UpdateRect()
{
	vector<Particle> newparticles;
	uniform_real_distribution<float> rand_pnd(0, 1);
	float sum = 0;
	for (int i = 0; i < N_PARTICLES; ++i)
	{
		float rnd = rand_pnd(rnd_e);
		for (int i = 0; i < particles.size(); i++)
		{
			if(particles[i].sum_weight>=rnd)
			{
				newparticles.push_back(particles[i]);
				break;
			}
		}

		sum += newparticles[i].weight;

	}
	particles = newparticles;


	float sumx=0;
	float sumy=0;
	float sumh=0;
	float sumw=0;
	for (int i = 0; i < particles.size(); i++)
	{
		
		particles[i].weight /=sum;
		sumx += particles[i].rect.x * particles[i].weight;
		sumy += particles[i].rect.y * particles[i].weight;
		sumh += particles[i].rect.height * particles[i].weight;
		sumw += particles[i].rect.width * particles[i].weight;
	}

	rect_1.x = sumx;
	rect_1.y = sumy;
	rect_1.height = sumh;
	rect_1.width = sumw;



}



Mat PF::CaculateHist(Mat img)
{
	Mat kernal = CaculatKernal(img.rows, img.cols).clone();

	Mat hist(NUMBINS, 3, CV_32F);
	hist.setTo(1e-10);

	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j < img.cols; ++j)
		{
			int index1 = img.at<Vec3b>(i, j)[0] / (COLORWITH / NUMBINS);
			int index2 = img.at<Vec3b>(i, j)[1] / (COLORWITH / NUMBINS);
			int index3 = img.at<Vec3b>(i, j)[2] / (COLORWITH / NUMBINS);
			
			hist.at<float>(index1, 0) += kernal.at<float>(i, j);
			hist.at<float>(index2, 1) += kernal.at<float>(i, j);
			hist.at<float>(index3, 2) += kernal.at<float>(i, j);
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


