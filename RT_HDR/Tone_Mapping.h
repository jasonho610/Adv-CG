#include <iostream>
#include <cmath>
using namespace std;
#define HDR_dmax 50.0
#define HDR_b 0.85 
#define HDR_gamma 2.2



void rgb2Yxy( const float * src, float * dst, int size){
		for(int i = 0; i < size; ++i) {
		float a, b, c;
		a = (0.412453) * src[i*3 + 0]+
			(0.357580) * src[i*3 + 1]+
			(0.180423) * src[i*3 + 2];

		b = (0.212671) * src[i*3 + 0]+
			(0.715160) * src[i*3 + 1]+
			(0.072169) * src[i*3 + 2];

		c = (0.019334) * src[i*3 + 0]+
			(0.119193) * src[i*3 + 1]+
			(0.950227) * src[i*3 + 2];
		dst[i*3 + 0] = b;
		dst[i*3 + 1] = a / (a+b+c);
		dst[i*3 + 2] = b / (a+b+c);
	}
} 
void Yxy2rgb(const float * src, float * dst, int size){
	for(int i = 0; i < size; ++i) {
		float a, b, c, newW;
		newW = src[i*3 + 0] / src[i*3 + 2];
		a = src[i*3 + 0];
		b = newW * src[i*3 + 1];
		c = newW - b - a;

		dst[i*3 + 0]  =  float(3.240479)*b;
		dst[i*3 + 0] += -float(1.537150)*a;
		dst[i*3 + 0] += -float(0.498535)*c;

		dst[i*3 + 1]  = -float(0.969256)*b;
		dst[i*3 + 1] +=  float(1.875992)*a;
		dst[i*3 + 1] +=  float(0.041556)*c;

		dst[i*3 + 2]  =  float(0.055648)*b;
		dst[i*3 + 2] += -float(0.204043)*a;
		dst[i*3 + 2] +=  float(1.057311)*c;
	}
} 
void globalToneMapping(float * dst, int size, float dmax=HDR_dmax, float b=HDR_b){
	constexpr int dim = 3; 
	constexpr int rgb = 0; 
	
	float maxLum = dst[rgb];
	for(unsigned i = 1; i < size; ++i) {
		if(dst[i*dim+rgb] > maxLum)
			maxLum = dst[i*dim+rgb];
	}

	float logSum = 0.0;
	for(int i = 0; i < size; ++i) 
		logSum += log(dst[i*dim+rgb]);
	
	const float logAvgLum = logSum/size;
	const float avgLum = exp(logAvgLum);
	const float maxLumW = (maxLum / avgLum);
	const float coeff = (dmax*float(0.01)) / log10(maxLumW+1.0);

	for(int i = 0; i < size; ++i) {
		auto& p = dst[i*dim+rgb];
		p /= avgLum;
		p = log(p+1.0) / log(2.0 + pow((p/maxLumW),(log(b)/log(0.5)))*8.0);
		p *= coeff;
	}
} 
void gama_fix(float* dst, int size, float gam) {
	const float fgamma = (0.45/gam)*2.0;
	float slope = 4.5;
	float start = 0.018;
	// 判定係數
	if(gam >= float(2.1)) {
		start /= ((gam - 2) * float(7.5));
		slope *= ((gam - 2) * float(7.5));
	} else if(gam <= float(1.9)) {
		start *= ((2 - gam) * float(7.5));
		slope /= ((2 - gam) * float(7.5));
	}
	// 校正像素
	for (int i = 0; i < size*3; i++) {
		if(dst[i] <= start) {
			dst[i] = dst[i]*slope;
		} else {
			dst[i] = float(1.099)*pow(dst[i], fgamma) - float(0.099);
		}
	}
}


