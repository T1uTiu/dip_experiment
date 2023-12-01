#include "stdafx.h"
#include<math.h>
#include<vector>
#include<queue>
#include<algorithm>

#define PI 3.1415926535897931

bool LoadBmpFile(char* path, BITMAPINFO*& lpbmi, byte*& lpBits){
	FILE* file = fopen(path, "rb");
	if(!file) return false;
	
    fseek(file, 14, SEEK_SET);
    BITMAPINFOHEADER bmpInfoHeader;
    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    int imgWidth = bmpInfoHeader.biWidth;
    int imgHeight = bmpInfoHeader.biHeight;
    int imgBitCount = bmpInfoHeader.biBitCount;
	
	int paletteSize = 0;
    if(imgBitCount != 24){
        paletteSize = bmpInfoHeader.biClrUsed == 0 ? (int)pow(2, imgBitCount) : bmpInfoHeader.biClrUsed;
    }
	bmpInfoHeader.biClrUsed = paletteSize;
	
	lpbmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+paletteSize*sizeof(RGBQUAD));
    lpbmi->bmiHeader = bmpInfoHeader;
    fread(lpbmi->bmiColors, sizeof(RGBQUAD), paletteSize, file);
	
    int lineByte = (imgWidth * imgBitCount + 31)/32 * 4;
    int biSize = lineByte*imgHeight;
    lpBits = (byte*)malloc(biSize);
    fread(lpBits, sizeof(byte), biSize, file);
    fclose(file);
	return true;
}
bool SetRGBValue(BITMAPINFO* &bmpInfo, byte* &imgData, int i, int j, int r, int g, int b){
	if(bmpInfo==NULL || imgData==NULL) return false;

	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	if(i<0 || i>=imgHeight || j<0 || j>=imgWidth) return false;

	int bitCount = bmpInfo->bmiHeader.biBitCount;
	RGBQUAD* palette = bmpInfo->bmiColors;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;
	switch(bitCount){
		case 1:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+j/8;
			int idx = (*pixel) >> (7-(j%8)) &1;
			palette[idx].rgbRed = r;
			palette[idx].rgbGreen = g;
			palette[idx].rgbBlue = b;
			break;
		}
		case 4:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+j/2;
			int idx = j%2 == 0 ? (*pixel) >> 4 : (*pixel) & 15;
			palette[idx].rgbRed = r;
			palette[idx].rgbGreen = g;
			palette[idx].rgbBlue = b;
			break;
		}
		case 8:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+j;
			palette[*pixel].rgbRed = r;
			palette[*pixel].rgbGreen = g;
			palette[*pixel].rgbBlue= b; 
			break;
		}
		case 24:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+3*j;
			*pixel = b;
			*(pixel+1)= g;
			*(pixel+2) = r;
			break;
		}
	}
	return true;

}
bool GetRGBValue(BITMAPINFO* bmpInfo, byte* imgData,int i, int j, int&r, int&g, int&b){
	if(bmpInfo==NULL || imgData==NULL) return false;

	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	if(i<0 || i>=imgHeight || j<0 || j>=imgWidth) return false;

	int bitCount = bmpInfo->bmiHeader.biBitCount;
	RGBQUAD* palette = bmpInfo->bmiColors;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;
	switch(bitCount){
		case 1:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+j/8;
			int idx = (*pixel) >> (7-(j%8)) &1;
			r = g = b = idx ? 255 : 0;
			break;
		}
		case 4:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+j/2;
			int idx = j%2 == 0 ? (*pixel) >> 4 : (*pixel) & 15;
			r = palette[idx].rgbRed;
			g = palette[idx].rgbGreen;
			b = palette[idx].rgbBlue;
			break;
		}
		case 8:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+j;
			r = palette[*pixel].rgbRed;
			g = palette[*pixel].rgbGreen;
			b = palette[*pixel].rgbBlue; 
			break;
		}
		case 24:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+3*j;
			b = *pixel;
			g = *(pixel+1);
			r = *(pixel+2);
			break;
		}
	}
	return true;
}
bool GetRGBValue(BITMAPINFO* bmpInfo, byte* imgData,int i, int j,char* &val){
	int r,g,b;
	if(!GetRGBValue(bmpInfo, imgData, i,j, r,g,b)) return false;
	switch(bmpInfo->bmiHeader.biBitCount){
		case 1:
			if(r == 255) sprintf(val, "前景点");
			else sprintf(val, "背景点");
			break;
		case 8:
			if(r == g && g == b) sprintf(val, "亮度值:%d", r);
			else sprintf(val, "RGB(%d,%d,%d)", r,g,b);
			break;
		default:
			sprintf(val, "RGB(%d,%d,%d)", r,g,b);
			break;
	}
	return true;
		
}
bool Gray(BITMAPINFO* originBmpInfo, byte* originImgData, BITMAPINFO* &grayBmpInfo, byte* &grayImgData){
	if(originBmpInfo == NULL || originImgData==NULL) return false;

	grayBmpInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
	if(!grayBmpInfo) return false;
	memcpy(&(grayBmpInfo->bmiHeader), &(originBmpInfo->bmiHeader),sizeof(BITMAPINFOHEADER));
	grayBmpInfo->bmiHeader.biBitCount = 8;
	grayBmpInfo->bmiHeader.biClrUsed = 256;

	int imgWidth = originBmpInfo->bmiHeader.biWidth;
	int imgHeight = originBmpInfo->bmiHeader.biHeight;
	int oriLineByte = (imgWidth * originBmpInfo->bmiHeader.biBitCount + 31)/32 * 4;
	int grayLineByte = (imgWidth * 8 + 31)/32 * 4;

	grayImgData = (byte*)malloc(grayLineByte*imgHeight);
	if(!grayImgData) return false;

	for(int i = 0; i < imgHeight; i++){
		for(int j = 0; j < imgWidth; j++){
			int r,g,b;
			GetRGBValue(originBmpInfo, originImgData, i, j, r, g ,b);
			int gray = (r+g+b)/3;
			
			grayImgData[(imgHeight-i-1)*grayLineByte+j] = gray;
			grayBmpInfo->bmiColors[gray].rgbRed=grayBmpInfo->bmiColors[gray].rgbBlue=grayBmpInfo->bmiColors[gray].rgbGreen=gray;
		}
	}
	return true;
}
bool GetGrayHistogram(BITMAPINFO* bmpInfo, byte* imgData,int* &grayHistogram){
	if(bmpInfo==NULL || imgData==NULL) return false;

	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;

	grayHistogram = (int*)malloc(sizeof(int)*256*3); memset(grayHistogram, 0 , 256*sizeof(int)*3);

	for(int i = 0; i < imgHeight; i++){
		for(int j = 0; j < imgWidth; j++){
			int r,g,b;
			if(! GetRGBValue(bmpInfo, imgData, i, j, r, g ,b) ) return false;
			grayHistogram[0+r]++;
			grayHistogram[256+g]++;
			grayHistogram[512+b]++;

		}
	}
	return true;
}

bool EqualHistogram(BITMAPINFO* &bmpInfo, byte* &imgData, int* grayHistogram){
	if(bmpInfo==NULL || imgData==NULL || grayHistogram==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int imgArea = imgWidth * imgHeight;
	
	int i, j;
	// 灰度直方图前缀和
	for(i = 1; i < 256; i++){
		grayHistogram[0+i] += grayHistogram[0+i-1];
		grayHistogram[256+i] += grayHistogram[256+i-1];
		grayHistogram[512+i] += grayHistogram[512+i-1];
	}

	int palleteSize = bmpInfo->bmiHeader.biClrUsed, bitCount = bmpInfo->bmiHeader.biBitCount;
	RGBQUAD* pallete = bmpInfo->bmiColors;
	
	if(bitCount == 24){
		for(i = 0; i < imgHeight; i++){
			for(j = 0; j < imgWidth; j++){
				int r,g,b;
				if(! GetRGBValue(bmpInfo, imgData, i, j, r, g ,b) ) return false;
				int equalR = 255*grayHistogram[0+r]/imgArea, equalG=255*grayHistogram[256+g]/imgArea, equalB=255*grayHistogram[512+b]/imgArea;
				SetRGBValue(bmpInfo, imgData, i, j, equalR, equalG, equalB);
			}
		}
	}else{
		for(i = 0; i < palleteSize; i++){
			int r = pallete[i].rgbRed, g = pallete[i].rgbGreen, b = pallete[i].rgbBlue;
			int equalR = 255*grayHistogram[0+r]/imgArea, equalG=255*grayHistogram[256+g]/imgArea, equalB=255*grayHistogram[512+b]/imgArea;
			pallete[i].rgbRed = equalR;
			pallete[i].rgbGreen = equalG;
			pallete[i].rgbBlue = equalB;
		}
	}
	
	
	return true;
}
bool LinearPointCalculate(BITMAPINFO* &bmpInfo, byte* &imgData, int a1, int a0){
	if(bmpInfo==NULL || imgData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;

	int palleteSize = bmpInfo->bmiHeader.biClrUsed, bitCount = bmpInfo->bmiHeader.biBitCount;
	RGBQUAD* pallete = bmpInfo->bmiColors;
	
	int i,j;
	if(bitCount == 24){
		for(i = 0; i < imgHeight; i++){
			for(j = 0; j < imgWidth; j++){
				int r,g,b;
				if(! GetRGBValue(bmpInfo, imgData, i, j, r, g ,b) ) return false;
				int newR = min(255, max(0, a1*r+a0)), newG= min(255, max(0, a1*g+a0)) , newB=min(255, max(0, a1*b+a0));
				SetRGBValue(bmpInfo, imgData, i, j, newR, newG, newB);
			}
		}
	}else{
		for(i = 0; i < palleteSize; i++){
			int r = pallete[i].rgbRed, g = pallete[i].rgbGreen, b = pallete[i].rgbBlue;
			int newR = min(255, max(0, a1*r+a0)), newG= min(255, max(0, a1*g+a0)) , newB=min(255, max(0, a1*b+a0));
			pallete[i].rgbRed = newR;
			pallete[i].rgbGreen = newG;
			pallete[i].rgbBlue = newB;
		}
	}
	return true;
}

void FT(complex<double>* imgData, complex<double>* const &ftData, int length, int stride){
	for(int u = 0; u < length; u++){
		complex<double> sum = 0;
		for(int x = 0; x < length; x++){
			double theta = -2*PI*u*x/length;
			sum += imgData[x*stride] * complex<double>(cos(theta), sin(theta));
		}
		sum /= length;
		ftData[u*stride] = sum;
	}
}

bool Fourier(BITMAPINFO* bmpInfo, byte* imgData, complex<double>* &res){
	if(bmpInfo==NULL || imgData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;
	int i,j	;
	
	// 空域图像转成复数形式
	res = new complex<double>[imgWidth*imgHeight];
	
	for(i = 0; i < imgHeight; i++){
		for(j = 0; j < imgWidth; j++){
			res[i*imgWidth + j] = complex<double>(imgData[(imgHeight-1-i)*lineByte + j] * pow(-1, i+j), 0);
		}
	}
	
	complex<double>* tmp = new complex<double>[imgWidth*imgHeight];
	// 对每一行进行傅里叶变换
	for(i = 0; i < imgHeight; i++){
		FT(res+i*imgWidth, tmp+i*imgWidth, imgWidth, 1);
	}
	// 对每一列进行傅里叶变换
	for(j = 0; j < imgWidth; j++){
		FT(tmp+j, res+j, imgHeight, imgWidth);
	}
	delete[] tmp;

	return true;
}

bool GetFTSpectrum(BITMAPINFO* bmpInfo, complex<double>* ftData, byte*& res){
	if(bmpInfo==NULL || ftData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;
	
	res = (byte*) malloc(lineByte*imgHeight);
	for(int i = 0; i < imgHeight; i++){
		for(int j = 0; j < imgWidth; j++){
			complex<double> fp = ftData[i*imgWidth+j];
			double value = sqrt(pow(fp.real(), 2)+pow(fp.imag(), 2));
			res[(imgHeight-1-i)*lineByte+j] = (byte)min(255, (value*2000));
		}
	}
	return true;
}
void IFT(complex<double>* ftData, complex<double>* const &imgData, int length, int stride){
	for(int u = 0; u < length; u++){
		complex<double> sum = 0;
		for(int x = 0; x < length; x++){
			double theta = 2*PI*u*x/length;
			sum += ftData[x*stride] * complex<double>(cos(theta), sin(theta));
		}
		imgData[u*stride] = sum;
	}
}
bool IFourier(BITMAPINFO* bmpInfo, complex<double>* ftData, byte* &res){
	if(bmpInfo==NULL || ftData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;
	int i,j;
	
	complex<double>* tmp = new complex<double>[imgWidth*imgHeight];
	// 对每一行进行傅里叶变换
	for(i = 0; i < imgHeight; i++){
		IFT(ftData+i*imgWidth, tmp+i*imgWidth, imgWidth, 1);
	}
	
	// 对每一列进行傅里叶变换
	for(j = 0; j < imgWidth; j++){
		IFT(tmp+j, ftData+j, imgHeight, imgWidth);
	}
	res = (byte*) malloc(lineByte*imgHeight);
	for(i = 0; i < imgHeight; i++){
		for(j = 0; j < imgWidth; j++){
			complex<double> fp = ftData[i*imgWidth+j];
			double val = fp.real() / pow(-1, i+j);
			val = max(0, min(255, val));
			res[(imgHeight-1-i)*lineByte+j] = (byte)val;
		}
	}
	delete[] tmp;
	return true;
}

void FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// 计算付立叶变换点数
	LONG count = 1 << r;
	// 计算加权系数
	int i;
	double angle;
	complex<double>* W = new complex<double>[count / 2];
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	// 将时域点写入X1
	complex<double>* X1 = new complex<double>[count];
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 采用蝶形算法进行快速付立叶变换，输出为频域值X2
	complex<double>* X2 = new complex<double>[count]; 

	int k,j,p,size;
	complex<double>* temp;
	for (k = 0; k < r; k++)
	{
		for (j = 0; j < 1 << k; j++)
		{
			size = 1 << (r-k);
			for (i = 0; i < size/2; i++)
			{
				p = j * size;
				X2[i + p] = X1[i + p] + X1[i + p + size/2];
				X2[i + p + size/2] = (X1[i + p] - X1[i + p + size/2]) * W[i * (1<<k)];
			}
		}
		temp  = X1;
		X1 = X2;
		X2 = temp;
	}
	
	// 重新排序（码位倒序排列）
	for (j = 0; j < count; j++)
	{
		p = 0;
		for (i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				p += 1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
		FD[j] /= count;
	}
	
	// 释放内存
	delete W;
	delete X1;
	delete X2;
}

bool FFourier(BITMAPINFO* bmpInfo, byte* imgData, complex<double>* &res)
{
	if(bmpInfo==NULL || imgData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= imgWidth)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= imgHeight)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	res = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	BYTE* pixel;
	
	for(i = 0; i < FFT_h; i++)  // 行
	{
		for(j = 0; j < FFT_w; j++)  // 列
		{
			// 指向DIB第i行，第j个象素的指针
			pixel = imgData + lineByte * (imgHeight - 1 - i) + j;

			// 给时域赋值
			TD[j + FFT_w * i] = complex<double>(*pixel * pow(-1,i+j), 0);
		}
	}
	
	for(i = 0; i < FFT_h; i++)
	{
		// 对y方向进行快速付立叶变换
		FFT(&TD[FFT_w * i], &res[FFT_w * i], wp);
	}
	
	// 转置
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			TD[i + FFT_h * j] = res[j + FFT_w * i];
		}
	}
	
	for(i = 0; i < FFT_w; i++)
	{
		// 对x方向进行快速付立叶变换
		FFT(&TD[i * FFT_h], &res[i * FFT_h], hp);
	}

	// 转置
	for(i = 0; i < FFT_h; i++){
		for(j = i; j < FFT_w; j++){
			complex<double> tmp = res[i*FFT_w + j];
			res[i*FFT_w + j] = res[j*FFT_w + i];
			res[j*FFT_w + i] = tmp;
		}
	}
	delete TD;
	return true;
}




void IFFT(complex<double>* FD, complex<double>* TD, int r){
	long count = 1 << r;
	
	// 频域点的共轭点
	complex<double> * X = new complex<double>[count];
	memcpy(X, FD, sizeof(complex<double>)*count);
	
	int i;
	for(i = 0; i < count; i++){
		X[i] = complex<double>(X[i].real(), -X[i].imag());
	}

	FFT(X, TD, r);

	for(i = 0 ; i < count; i++){
		TD[i] = complex<double>(TD[i].real()*count, -TD[i].imag()*count);
	}

	delete X;
}

bool IFFTourier(BITMAPINFO* bmpInfo, complex<double>* ftData, byte* &res){
	if(bmpInfo==NULL || ftData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;
	
	int FFT_w = 1, wp = 0; // 宽度，宽度的2的次幂数
	while(FFT_w*2 <= imgWidth){
		FFT_w *= 2; 
		wp++;
	}
	int FFT_h = 1, hp = 0; // 高度，高度的2的次幂数
	while(FFT_h*2 <= imgHeight){
		FFT_h *= 2; 
		hp++;
	}
	int i,j;
	complex<double> * TD = new complex<double>[FFT_w * FFT_h];
	complex<double> * FD = new complex<double>[FFT_w * FFT_h];

	for(i = 0; i < FFT_h; i++){
		for(j = 0; j < FFT_w; j++){
			FD[i* FFT_w + j] = ftData[i* FFT_w + j];
		}
	}

	// 水平方向
	for(i = 0; i < FFT_h; i++){
		IFFT(&FD[FFT_w*i], &TD[FFT_w*i], wp);
	}
	// 转置
	for(i = 0; i < FFT_h; i++){
		for(j = 0; j < FFT_w; j++){
			FD[FFT_h * j + i] = TD[FFT_w * i + j];
		}
	}
	// 垂直方向
	for(i = 0; i < FFT_w; i++){
		IFFT(&FD[FFT_h*i], &TD[FFT_h*i], hp);
	}

	res = (byte*)malloc(lineByte * imgHeight);
	for(i = 0; i < FFT_h; i++){
		for(j = 0; j < FFT_w; j++){
			double val = TD[j*FFT_h + i].real() / pow(-1, i+j);
			val = max(0, min(255, val));
			res[lineByte*(imgHeight-i-1)+j] = (byte) val;
		}
	}
	delete FD;
	delete TD;

	return true;
}


bool Conv2D(BITMAPINFO* bmpInfo, byte*& imgData, int* kernel, int kernelSize){
	if(bmpInfo==NULL || imgData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;

	int i,j,x,y;

	int factor = 0;
	for(i = 0; i < kernelSize*kernelSize; i++) factor += kernel[i];
	int k2 = kernelSize / 2;

	byte* tmp = (byte*) malloc(lineByte*imgHeight);
	for(i = 0; i < imgHeight; i++){
		for(j = 0; j < imgWidth; j++){
			if(i < k2 || i >= imgHeight-k2 || j < k2 || j >= imgWidth-k2) tmp[(imgHeight-1-i)*lineByte+j] = imgData[(imgHeight-1-i)*lineByte+j];
			int sum = 0;
			for(y = i-k2; y <= i+k2; y++){
				for(x = j-k2; x <= j+k2; x++){
					int y0 = y-(i-k2), x0 = x-(j-k2);
					sum += imgData[(imgHeight-1-y)*lineByte+x] * kernel[(kernelSize-1-y0)*kernelSize+x0];
				}
			}
			tmp[(imgHeight-1-i)*lineByte+j] = sum / factor;

		}
	}
	for(i = 0; i < imgHeight; i++){
		for(j = 0; j < imgWidth; j++){
			imgData[(imgHeight-1-i)*lineByte+j] = tmp[(imgHeight-1-i)*lineByte+j];
		}
	}
	free(tmp);
	return true;
}

bool MidPool2D(BITMAPINFO* bmpInfo, byte*& imgData, int kernelSize){
	if(bmpInfo==NULL || imgData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;

	int i,j,x,y;

	int k2 = kernelSize / 2;

	byte* tmp = (byte*) malloc(lineByte*imgHeight);
	for(i = 0; i < imgHeight; i++){
		for(j = 0; j < imgWidth; j++){
			if(i < k2 || i >= imgHeight-k2 || j < k2 || j >= imgWidth-k2) tmp[(imgHeight-1-i)*lineByte+j] = imgData[(imgHeight-1-i)*lineByte+j];
			vector<byte> arr;
			
			for(y = i-k2; y <= i+k2; y++){
				for(x = j-k2; x <= j+k2; x++){
					arr.push_back(imgData[(imgHeight-1-y)*lineByte+x]);
				}
			}
			sort(arr.begin(), arr.end());
			tmp[(imgHeight-1-i)*lineByte+j] = arr[kernelSize*kernelSize/2];

		}
	}
	for(i = 0; i < imgHeight; i++){
		for(j = 0; j < imgWidth; j++){
			imgData[(imgHeight-1-i)*lineByte+j] = tmp[(imgHeight-1-i)*lineByte+j];
		}
	}
	free(tmp);
	return true;
}
bool GradientSharpen(BITMAPINFO* bmpInfo, byte*& imgData){
	if(bmpInfo==NULL || imgData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;
	
	int i,j;
	byte* tmp = (byte*) malloc(lineByte*imgHeight);
	for(i = 0; i < imgHeight; i++){
		for(j = 0; j < imgWidth; j++){
			if(i == imgHeight-1 || j == imgWidth - 1) continue;
			byte val = imgData[(imgHeight-1-i)*lineByte+j];
			byte right = imgData[(imgHeight-1-i)*lineByte+j+1], btm = imgData[(imgHeight-i)*lineByte+j];
			byte gradient = abs(val-right) + abs(val-btm);
			tmp[(imgHeight-1-i)*lineByte+j] = gradient;
		}
	}
	for(i = 0; i < imgHeight; i++){
		for(j = 0; j < imgWidth; j++){
			imgData[(imgHeight-1-i)*lineByte+j] = tmp[(imgHeight-1-i)*lineByte+j];
		}
	}
	free(tmp);

	return true;
}
double GetDistance(int x1, int y1, int x2, int y2){
	return sqrt((x1-x2)*(x1-x2)+ (y1-y2)*(y1-y2));
}
bool IdealFilter(BITMAPINFO* bmpInfo, complex<double>* &ftData, int d0, int method=0){
	if(bmpInfo==NULL || ftData==NULL) return false;
	int width = bmpInfo->bmiHeader.biWidth, height = bmpInfo->bmiHeader.biHeight;
	int centerX = width/2, centerY = height/2;
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			double d = GetDistance(j,i,centerX, centerY);
			if(method == 0 && d > d0 || method==1 && d <= d0) ftData[i*width+j] = 0;
			
		}
	}
	return true;
}
bool ButterworthFilter(BITMAPINFO* bmpInfo, complex<double>* &ftData, int d0, int n=2, int method=0){
	if(bmpInfo==NULL || ftData==NULL) return false;
	int width = bmpInfo->bmiHeader.biWidth, height = bmpInfo->bmiHeader.biHeight;
	int centerX = width/2, centerY = height/2;
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			double d = GetDistance(j,i,centerX, centerY);
			double r = method==0 ? d/d0 : d0/d;
			double h = 1 / (1+pow(r, 2*n));
			ftData[i*width+j] *= h;
			
		}
	}
	return true;
}