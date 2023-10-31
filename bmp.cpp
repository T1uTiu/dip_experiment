#include "stdafx.h"
#include<math.h>


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
bool GetGrayHistogram(BITMAPINFO* bmpInfo, byte* &imgData,int* &redGrayHistogram,int* &greenGrayHistogram,int* &blueGrayHistogram){
	if(bmpInfo==NULL || imgData==NULL) return false;

	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;

	redGrayHistogram = (int*)malloc(sizeof(int)*256); memset(redGrayHistogram, 0 , 256*4);
	greenGrayHistogram = (int*)malloc(sizeof(int)*256); memset(greenGrayHistogram, 0 , 256*4);
	blueGrayHistogram = (int*)malloc(sizeof(int)*256); memset(blueGrayHistogram, 0 , 256*4);

	for(int i = 0; i < imgHeight; i++){
		for(int j = 0; j < imgWidth; j++){
			int r,g,b;
			GetRGBValue(bmpInfo, imgData, i, j, r, g ,b);
			redGrayHistogram[r]++;
			greenGrayHistogram[g]++;
			blueGrayHistogram[b]++;

		}
	}
	return true;
}


bool Mirror(BITMAPINFO* bmpInfo, byte* &imgData, int direction){
	if(bmpInfo==NULL || imgData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;
	
	if(direction==0){
		for(int i = 0; i < imgHeight; i++){
			int l = 0, r = imgWidth-1;
			while(l<r){
				byte* pixel1 = imgData + lineByte* i + l;
				byte* pixel2 = imgData + lineByte * i+ r;
				byte tmp = *pixel1;
				*pixel1 = *pixel2;
				*pixel2 = tmp;
				l++;
				r--;
			}
		}
	}else{
		for(int i = 0; i < imgWidth; i++){
			int l = 0, r = imgHeight-1;
			while(l<r){
				byte* pixel1 = imgData + lineByte* l + i;
				byte* pixel2 = imgData + lineByte * r+ i;
				byte tmp = *pixel1;
				*pixel1 = *pixel2;
				*pixel2 = tmp;
				l++;
				r--;
			}
		}
	}
	return true;

}