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

bool TrueColor2Gray(BITMAPINFO* orilpbmi, byte* orilpBits,BITMAPINFO* &graylpbmi, byte* &graylpBits){
	int paletteSize = 256;
	graylpbmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+paletteSize*sizeof(RGBQUAD));
	if(!graylpbmi) return false;
	memcpy(&(graylpbmi->bmiHeader), &(orilpbmi->bmiHeader),sizeof(BITMAPINFOHEADER));
	graylpbmi->bmiHeader.biBitCount = 8;
	graylpbmi->bmiHeader.biClrUsed = 256;

	int imgWidth = orilpbmi->bmiHeader.biWidth;
	int imgHeight = orilpbmi->bmiHeader.biWidth;
	int oriLineByte = (imgWidth * 24 + 31)/32 * 4;
	int grayLineByte = (imgWidth * 8 + 31)/32 * 4;
	graylpBits = (byte*)malloc(grayLineByte*imgHeight);
	if(!graylpBits) return false;
	for(int i = 0; i < imgHeight; i++){
		for(int j = 0; j < imgWidth; j++){
			int b = orilpBits[oriLineByte*i+j*3];
			int g = orilpBits[oriLineByte*i+j*3+1];
			int r = orilpBits[oriLineByte*i+j*3+2];
			int gray = (r+g+b)/3;
			
			graylpBits[i*grayLineByte+j] = gray;
			graylpbmi->bmiColors[gray].rgbRed=graylpbmi->bmiColors[gray].rgbBlue=graylpbmi->bmiColors[gray].rgbGreen=gray;
		}
	}
	return true;
}
bool GetRGBValue(BITMAPINFO* bmpInfo, byte* imgData,int i, int j,int& r, int& g, int& b){
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
		}
		case 4:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+j/2;
			int idx = j%2 == 0 ? (*pixel) >> 4 : (*pixel) & 15;
			r = palette[idx].rgbRed;
			g = palette[idx].rgbGreen;
			b = palette[idx].rgbBlue;
		}
		case 8:{
			byte* pixel = imgData + lineByte*(imgHeight-i-1)+j;
			r = palette[*pixel].rgbRed;
			g = palette[*pixel].rgbGreen;
			b = palette[*pixel].rgbBlue; 	   
		}
		case 24:{
			byte* pixel = imgData + lineByte*(imgHeight-j-1)+3*i;
			r = *pixel;
			g = *(pixel+1);
			b = *(pixel+2);
		}
	}
	return true;
}
bool Mirror(BITMAPINFO* bmpInfo, byte* imgData, int direction, byte* &newImgData){
	if(bmpInfo==NULL || imgData==NULL) return false;
	int imgWidth = bmpInfo->bmiHeader.biWidth, imgHeight = bmpInfo->bmiHeader.biHeight;
	int bitCount = bmpInfo->bmiHeader.biBitCount;
	if(bitCount != 8) return false;
	int lineByte = (imgWidth * bitCount + 31)/32 * 4;
	
	newImgData = (byte*)malloc(lineByte*imgHeight);
	memcpy(newImgData, imgData, lineByte*imgHeight);
	if(direction==0){
		for(int i = 0; i < imgHeight; i++){
			int l = 0, r = imgWidth-1;
			while(l<r){
				byte* pixel1 = newImgData + lineByte* i + l;
				byte* pixel2 = newImgData + lineByte * i+ r;
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
				byte* pixel1 = newImgData + lineByte* l + i;
				byte* pixel2 = newImgData + lineByte * r+ i;
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