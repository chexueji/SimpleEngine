#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include "stdafx.h"

class Texture
{
public:
	Texture()
	{
		dwImageDataSize=0;
		pImageData=NULL;
	}
	Texture(const Texture &tex)
	{
		bfh=tex.bfh;
		bih=tex.bih;
		dwImageDataSize=tex.dwImageDataSize;
		pImageData=new DWORD[tex.dwImageDataSize>>2];
	}
	~Texture()
	{
		if (pImageData!=NULL)
		{
			delete pImageData;
			pImageData=NULL;		
		}
		dwImageDataSize=0;
	}
	Texture &operator= (const Texture &tex)
	{
		if (this == &tex)
		{
			return *this;
		}
		if(pImageData!=NULL)
		{
			delete []pImageData;
			pImageData=NULL;
		}	
		pImageData=new DWORD[tex.dwImageDataSize>>2];

		bfh=tex.bfh;
		bih=tex.bih;
		dwImageDataSize=tex.dwImageDataSize;
		memcpy(pImageData,tex.pImageData,sizeof(DWORD)*(tex.dwImageDataSize>>2));
		return *this;
	}
public:
	BITMAPFILEHEADER bfh;//original bitmap file info
	BITMAPINFOHEADER bih;//original bitmap file info
	DWORD dwImageDataSize;//new 32 bits bitmap file info
	DWORD *pImageData;//new 32 bits bitmap file info
};
#endif //_TEXTUREINFO_H_
