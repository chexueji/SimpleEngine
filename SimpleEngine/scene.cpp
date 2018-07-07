#include "stdafx.h"
#include <cmath>
#include "scene.h"
#include "stdio.h"

using namespace std;

Scene::Scene()
{
	shade_mode=SHADE_MODE_NONE;
	hWnd=NULL;
	scene_a_color=RGB(50,50,50);
	winMin=Vec2f(-300.0,-300.0);
	winMax=Vec2f(300.0,300.0);
}
Scene::~Scene()
{

}

Texture*  Scene:: LoadTexture(char *pTextureName)
{
	Texture* pBmp=NULL;
	FILE *pFile =NULL;
	fopen_s(&pFile,pTextureName,"rb");
	if (pFile==NULL)
	{
		return 0;
	}
	pBmp = new Texture();
	fread(&(pBmp->bfh),sizeof(BITMAPFILEHEADER),1,pFile);
	fread(&(pBmp->bih),sizeof(BITMAPINFOHEADER),1,pFile);

	RGBQUAD pPalette[256];
	if (pBmp->bih.biBitCount==8)
	{		
		fread(pPalette,sizeof(RGBQUAD),256,pFile);
	}
	int nPixelBytes_ori=pBmp->bih.biBitCount/8;
	int nWidthBytes_ori=nPixelBytes_ori*pBmp->bih.biWidth;
	nWidthBytes_ori=BYTESALIGN(nWidthBytes_ori);

	LPBYTE pImageData_ori = (LPBYTE)new BYTE[(nWidthBytes_ori*(pBmp->bih.biHeight))];
	if (pImageData_ori == NULL)
	{
		return 0;
	}
	fread(pImageData_ori,sizeof(BYTE),(nWidthBytes_ori*(pBmp->bih.biHeight)),pFile);

	int nPixelBytes_trans=PIXELBYTES/8;
	int nWidthBytes_trans=nPixelBytes_trans*pBmp->bih.biWidth;
	nWidthBytes_trans=BYTESALIGN(nWidthBytes_trans);

	pBmp->dwImageDataSize = nWidthBytes_trans*pBmp->bih.biHeight;
	pBmp->pImageData  = (DWORD  *)new DWORD[pBmp->dwImageDataSize>>2];

	// all in byte unit,convert the bitmap to 32bits 
	long nDstHorizontalOffset=0;
	long nDstVerticalOffset=0;
	long nDstTotalOffset=0;
	long nSrcHorizontalOffset=0;
	long nSrcVerticalOffset=0;
	long nSrcTotalOffset=0;
	for (long i=0; i<pBmp->bih.biHeight; i++)
	{
		nDstHorizontalOffset = 0;
		nSrcHorizontalOffset = 0;
		for (long j=0; j<pBmp->bih.biWidth; j++)
		{
			nDstTotalOffset = nDstVerticalOffset + nDstHorizontalOffset;
			nSrcTotalOffset = nSrcVerticalOffset + nSrcHorizontalOffset;
			switch (pBmp->bih.biBitCount)
			{
			case 8:
				{
					BYTE red = pPalette[pImageData_ori[nSrcTotalOffset]].rgbRed;
					BYTE green = pPalette[pImageData_ori[nSrcTotalOffset]].rgbGreen;
					BYTE blue = pPalette[pImageData_ori[nSrcTotalOffset]].rgbBlue;
					pBmp->pImageData[nDstTotalOffset>>2] = RGB(red, green, blue);
				}
				break;
			case 16:
				{
					//RGB555
					LPWORD pSrcData = (LPWORD)pImageData_ori;
					BYTE red = (pSrcData[nSrcTotalOffset>>1] & 0x7C00) >> 10;
					BYTE green = (pSrcData[nSrcTotalOffset>>1] & 0x03E0) >> 5;
					BYTE blue = (pSrcData[nSrcTotalOffset>>1] & 0x001F);
					pBmp->pImageData[nDstTotalOffset>>2] = RGB(red, green, blue);
				}
				break;
			case 24:
				{
					pBmp->pImageData[nDstTotalOffset>>2] = RGB(pImageData_ori[nSrcTotalOffset+2], pImageData_ori[nSrcTotalOffset+1], pImageData_ori[nSrcTotalOffset]);
				}
				break;
			case 32:
				{
					LPDWORD pSrcData = (LPDWORD)pImageData_ori;
					pBmp->pImageData[nDstTotalOffset>>2] = pSrcData[nSrcTotalOffset>>2];
				}
				break;
			default:
				break;
			}
			nDstHorizontalOffset += nPixelBytes_trans;
			nSrcHorizontalOffset += nPixelBytes_ori;
		}
		nDstVerticalOffset += nWidthBytes_trans;
		nSrcVerticalOffset += nWidthBytes_ori;
	}

	//fseek(pFile,pBmp->bfh.bfOffBits,SEEK_SET);
	//fread(pBmp->pImageData,sizeof(BYTE),pBmp->dwImageDataSize,pFile);


	fclose(pFile);
	if(pImageData_ori!=NULL)
	{
		delete []pImageData_ori;
	}
	return pBmp;
}

Object3D *Scene::LoadObject(char *pObjectName)
{
	char szText[512];
	FILE *pFile=NULL;
	Object3D *pObject3D = NULL;
	fopen_s(&pFile,pObjectName,"r");
	if(pFile != NULL)
	{
		if(fgets(szText,512,pFile)!=NULL)
		{
			pObject3D = new Object3D();
			sscanf_s(szText,"%d %d\r\n",&pObject3D->vertex_num,&pObject3D->tri_num);
		}
		pObject3D->pVertexList =new Vertex3D[pObject3D->vertex_num];
		for(int i=0;i<pObject3D->vertex_num;i++)
		{
			if(fgets(szText,512,pFile)!=NULL)
			{
				sscanf_s(szText,"%lf %lf %lf\r\n",&pObject3D->pVertexList[i].v[0],&pObject3D->pVertexList[i].v[1],&pObject3D->pVertexList[i].v[2]);
			}else
			{
				break;
			}
		}
		pObject3D->pTriList = new Triangle[pObject3D->tri_num];
		//pObject3D->pTriList_trans = new Triangle[pObject3D->tri_num];
		int color_r,color_g,color_b;
		for(int i=0;i<pObject3D->tri_num;i++)
		{
			if(fgets(szText,512,pFile)!=NULL)
			{
				sscanf_s(szText,"%d %d %d %d %d %d",&color_r,&color_g,&color_b,\
					&pObject3D->pTriList[i].VertexIdx[0],&pObject3D->pTriList[i].VertexIdx[1],&pObject3D->pTriList[i].VertexIdx[2]);
				pObject3D->pTriList[i].tri_color = RGB(color_r,color_g,color_b);
			}else
			{
				break;
			}
		}
		//´ý¶¨´¦Àí
		pObject3D->material.setAmbient(Vec3(color_r/255.0,color_g/255.0,color_b/255.0));
		pObject3D->material.setDiffuse(Vec3(0.2,0.2,0.2));
		pObject3D->material.setSpecular(Vec3(0.6,0.7,0.6));
		pObject3D->material.setShininess(7.0);

		for(int i=0;i<pObject3D->tri_num;i++)
		{
			int index0,index1,index2;
			Vec3 v1,v2,n;

			index0 = pObject3D->pTriList[i].VertexIdx[0];
			index1 = pObject3D->pTriList[i].VertexIdx[1];
			index2 = pObject3D->pTriList[i].VertexIdx[2];
			v1=Vec3(pObject3D->pVertexList[index1].v[0],pObject3D->pVertexList[index1].v[1],pObject3D->pVertexList[index1].v[2])-\
				Vec3(pObject3D->pVertexList[index0].v[0],pObject3D->pVertexList[index0].v[1],pObject3D->pVertexList[index0].v[2]);
			v1.normalize();
			v2=Vec3(pObject3D->pVertexList[index2].v[0],pObject3D->pVertexList[index2].v[1],pObject3D->pVertexList[index2].v[2])-\
				Vec3(pObject3D->pVertexList[index0].v[0],pObject3D->pVertexList[index0].v[1],pObject3D->pVertexList[index0].v[2]);
			v2.normalize();
			n=v1^v2;
			n.normalize();
			//the method below is not accurate enough
			pObject3D->pVertexList[index0].n+=n;
			//pObject3D->pVertexList[index0].n.normalize();
			pObject3D->pVertexList[index1].n+=n;
			//pObject3D->pVertexList[index1].n.normalize();
			pObject3D->pVertexList[index2].n+=n;
			//pObject3D->pVertexList[index2].n.normalize();

			pObject3D->pTriList[i].tri_normal=Vec4(n[0],n[1],n[2],1.0);

				
			//pObject3D->pTriList[i].pnt[0]=pObject3D->pVertexList[index0];
			//pObject3D->pTriList[i].pnt[1]=pObject3D->pVertexList[index1];
			//pObject3D->pTriList[i].pnt[2]=pObject3D->pVertexList[index2];			
		}

		for(int i=0;i<pObject3D->vertex_num;i++)
		{
			pObject3D->pVertexList[i].n.normalize();
		}
		for(int i=0;i<pObject3D->tri_num;i++)
		{
			int index0,index1,index2;
			index0 = pObject3D->pTriList[i].VertexIdx[0];
			index1 = pObject3D->pTriList[i].VertexIdx[1];
			index2 = pObject3D->pTriList[i].VertexIdx[2];

			pObject3D->pTriList[i].pnt_local[0]=pObject3D->pVertexList[index0];
			pObject3D->pTriList[i].pnt_local[1]=pObject3D->pVertexList[index1];
			pObject3D->pTriList[i].pnt_local[2]=pObject3D->pVertexList[index2];

			memcpy(pObject3D->pTriList[i].pnt_trans,pObject3D->pTriList[i].pnt_local,3*sizeof(Vertex3D));
		}

	}else
	{
		return 0;
	}
	return pObject3D;
}
void Scene::ResetObject3DInfo(Object3D *pObject3D)
{
	if(pObject3D!=NULL)
	{
		pObject3D->bRemoved = false;
		for(int i=0;i<pObject3D->tri_num;i++)
		{
			pObject3D->pTriList[i].bHiddenFace = false;
			memcpy(pObject3D->pTriList[i].pnt_trans,pObject3D->pTriList[i].pnt_local,3*sizeof(Vertex3D));
		}
		
	}
}
void Scene::RemoveObject3D(Object3D *pObject3D,Camera *pCamera)
{
	//the code is not completed
	int nNum_OutOfBoundary=0;
	int nNum_X=0;
	int nNum_Y=0;
	int nNum_Z=0;
	Vertex3D pnt[3];
	for(int i=0;i<pObject3D->tri_num;i++)
	{
		nNum_X=0;
		nNum_Y=0;
		nNum_Z=0;
		for(int j=0;j<3;j++)
		{
			pnt[j].v = pCamera->mat_WCToVC*pObject3D->pTriList[i].pnt_trans[j].v;
			pnt[j].v = pCamera->mat_VCToNC*pObject3D->pTriList[i].pnt_trans[j].v;
			pnt[j].v/=pnt[j].v[3];
		}
	}
}
void Scene::RemoveHidenfaces(Object3D *pObject3D,Camera *pCamera)
{
	if(pObject3D!=NULL && pCamera!=NULL)
	{
		for(int i=0;i<pObject3D->tri_num;i++)
		{
			if(pObject3D->pTriList[i].bHiddenFace == true)
					continue;
			Vec3 V1=Vec3(pObject3D->pTriList[i].pnt_trans[1].v[0],pObject3D->pTriList[i].pnt_trans[1].v[1],pObject3D->pTriList[i].pnt_trans[1].v[2])- \
							Vec3(pObject3D->pTriList[i].pnt_trans[0].v[0],pObject3D->pTriList[i].pnt_trans[0].v[1],pObject3D->pTriList[i].pnt_trans[0].v[2]);
			Vec3 V2=Vec3(pObject3D->pTriList[i].pnt_trans[2].v[0],pObject3D->pTriList[i].pnt_trans[2].v[1],pObject3D->pTriList[i].pnt_trans[2].v[2])- \
							Vec3(pObject3D->pTriList[i].pnt_trans[0].v[0],pObject3D->pTriList[i].pnt_trans[0].v[1],pObject3D->pTriList[i].pnt_trans[0].v[2]);
			Vec3 N=V1^V2;
			Vec3 Vview=pCamera->eye_pos-Vec3(pObject3D->pTriList[i].pnt_trans[0].v[0],pObject3D->pTriList[i].pnt_trans[0].v[1],pObject3D->pTriList[i].pnt_trans[0].v[2]); 
			float result = Vview*N;
			if(result <= 0.0f)
			{
				pObject3D->pTriList[i].bHiddenFace=true;
			}
		}
	}
}
void Scene::DrawObject3D(HDC hDC,Object3D *pObject3D,Camera *pCamera,Light *pLights,int nLightNum,float angle)
{

	if(hWnd != NULL)
	{	
		ResetObject3DInfo(pObject3D);
		Rotate3D(pObject3D,angle,0.0,1.0,0.0);
		//RemoveObject3D(pObject3D,pCamera);
		RemoveHidenfaces(pObject3D,pCamera);
		PhongShading(pObject3D,pCamera,pLights,nLightNum);
		
		if(pObject3D->pVertexList != NULL && pObject3D->pTriList != NULL)
		{
			for(int i=0;i<pObject3D->tri_num;i++)
			{
				if(pObject3D->pTriList[i].bHiddenFace == false)//reduce the calculation of hidden faces
				{
					for(int j=0;j<3;j++)
					{
						if(GetShadeMode() & SHADE_MODE_NORMAL)
						{				
							Vec4 n_end,n_start,n_vc;
							n_start=pObject3D->pTriList[i].pnt_trans[j].v;
							n_end=pObject3D->pTriList[i].pnt_trans[j].n+Vec3(n_start[0],n_start[1],n_start[2]);
							n_start=pCamera->mat_WCToVC*n_start;
							n_end=pCamera->mat_WCToVC*n_end;
							n_vc=n_end-n_start;n_vc[3]=1.0;
							n_vc.normalize();
							pObject3D->pTriList[i].pnt_trans[j].n=n_vc;
						}

						pObject3D->pTriList[i].pnt_trans[j].v = pCamera->mat_WCToVC*pObject3D->pTriList[i].pnt_trans[j].v;
						pObject3D->pTriList[i].z[j] = (pObject3D->pTriList[i].pnt_trans[j].v[2])/(pObject3D->pTriList[i].pnt_trans[j].v[3]);
						
						pObject3D->pTriList[i].pnt_trans[j].v = pCamera->mat_VCToNC*pObject3D->pTriList[i].pnt_trans[j].v;
						pObject3D->pTriList[i].pnt_trans[j].v = pCamera->mat_NCToVP*pObject3D->pTriList[i].pnt_trans[j].v;
						pObject3D->pTriList[i].pnt_trans[j].v/= pObject3D->pTriList[i].pnt_trans[j].v[3];


					}

					if(shade_mode == SHADE_MODE_NONE)					
					{
						pObject3D->pTriList[i].draw_triangle_wireframe(winMin,winMax);
					}else if(shade_mode==SHADE_MODE_CONSTANT)
					{
						pObject3D->pTriList[i].draw_triangle_gouraud(winMin,winMax);			
					}else if(shade_mode==SHADE_MODE_FLAT)
					{
						pObject3D->pTriList[i].draw_triangle_gouraud(winMin,winMax);
					}else if(shade_mode==SHADE_MODE_GOURAUD)
					{
						pObject3D->pTriList[i].draw_triangle_gouraud(winMin,winMax);
					}else if(shade_mode==SHADE_MODE_NORMAL)
					{
						pObject3D->pTriList[i].draw_triangle_normal(&(pObject3D->material),pCamera,pLights,nLightNum,winMin,winMax);
					}
					else if (shade_mode==SHADE_MODE_TEXTURE)
					{
						pObject3D->pTriList[i].draw_triangle_texture(winMin,winMax);
					}
				}
			}
		}
	}

}

void Scene::PhongShading(Object3D *pObject3D,Camera *pCamera,Light *pLights,int nLightNum)
{

	for(int triIndex=0;triIndex<pObject3D->tri_num;triIndex++)
	{
		
		Triangle *curTri=&(pObject3D->pTriList[triIndex]);
		if(curTri->bHiddenFace == false)
		{
			if(shade_mode & SHADE_MODE_NONE)
			{

			}else if(shade_mode & SHADE_MODE_CONSTANT)
			{
				for(int i=0;i<3;i++)
				{
					curTri->vertex_color[i]=curTri->tri_color;
				}

			}else if(shade_mode & SHADE_MODE_FLAT)
			{
				float scene_a_R,scene_a_G,scene_a_B;
				Vec3 sumRGB(0,0,0);
				scene_a_R = GetR(scene_a_color);
				scene_a_G = GetG(scene_a_color);
				scene_a_B = GetB(scene_a_color);
				Vec3 ambientFactorRGB = pObject3D->material.getAmbient();
				Vec3 diffuseFactorRGB = pObject3D->material.getDiffuse();
				Vec3 specularFactorRGB = pObject3D->material.getSpecular();
				float shininessFactor = pObject3D->material.getShininess();
				
			//	Vec3 BPhongModel_a = Vec3((float)GetR(scene_a_color),(float)GetG(scene_a_color),(float)GetB(scene_a_color))*ambientFactorRGB;
				Vec3 BPhongModel_a = Vec3(scene_a_R*ambientFactorRGB[0],scene_a_G*ambientFactorRGB[1],scene_a_B*ambientFactorRGB[2]);
				Vec3 L,V,N,H;
				float distance=0;
				sumRGB+=BPhongModel_a;
				for(int lightIndex=0;lightIndex<nLightNum;lightIndex++)
				{

					if(pLights[lightIndex].bActive)
					{
						float lightR,lightG,lightB;
						lightR=GetR(pLights[lightIndex].getLightColor());
						lightG=GetG(pLights[lightIndex].getLightColor());
						lightB=GetB(pLights[lightIndex].getLightColor());

						if(pLights[lightIndex].type & LIGHT_TYPE_POINT)
						{
							L=Vec3(pLights[lightIndex].light_pos[0],pLights[lightIndex].light_pos[1],pLights[lightIndex].light_pos[2])-\
								Vec3(curTri->pnt_trans[0].v[0],curTri->pnt_trans[0].v[1],curTri->pnt_trans[0].v[2]);
							distance=L.length();
							L.normalize();

							V=Vec3(pCamera->eye_pos[0],pCamera->eye_pos[1],pCamera->eye_pos[2])-\
								Vec3(curTri->pnt_trans[0].v[0],curTri->pnt_trans[0].v[1],curTri->pnt_trans[0].v[2]);
							V.normalize();

							H=(L+V);
							H.normalize();

							//N=Vec3(curTri->pnt[0].n[0],curTri->pnt[0].n[1],curTri->pnt[0].n[2]);
							N=Vec3(curTri->tri_normal[0],curTri->tri_normal[1],curTri->tri_normal[2]);
							N.normalize();

							float dotLN=L*N;
							float dotHN=H*N;
							float attenuation=0.0;
							//float common_factor =(pLights[lightIndex].K_d*dotLN + pLights[lightIndex].K_s *pow(dotHN,pLights[lightIndex].N_s));
							Vec3 factorRGB;
							if(dotLN>0)
							{
								factorRGB=(diffuseFactorRGB*max(dotLN,0.0) + specularFactorRGB *pow(max(dotHN,0.0),shininessFactor));

								attenuation=min(1.0,1.0/(pLights[lightIndex].kc + pLights[lightIndex].kl*distance + pLights[lightIndex].kq*distance*distance));
								//attenuation=1.0;
								factorRGB*=attenuation;
							}

							sumRGB[0]+=factorRGB[0]*lightR;
							sumRGB[1]+=factorRGB[1]*lightG;
							sumRGB[2]+=factorRGB[2]*lightB;

				
						}
					}
				}
				float finalR =  max(0.0f, min(255.0f, sumRGB[0]));
				float finalG = max(0.0f, min(255.0f, sumRGB[1]));
				float finalB = max(0.0f, min(255.0f, sumRGB[2]));
				curTri->tri_flat_color=RGB(finalR,finalG,finalB);
				for(int vIdx = 0;vIdx<3;vIdx++)
				{
					curTri->vertex_color[vIdx]=curTri->tri_flat_color;
				}
							
			}else if(shade_mode & SHADE_MODE_GOURAUD)
			{
				float scene_a_R,scene_a_G,scene_a_B;
				Vec3 sumRGB[3] = {Vec3(0,0,0),Vec3(0,0,0),Vec3(0,0,0)};
	
				scene_a_R = GetR(scene_a_color);
				scene_a_G = GetG(scene_a_color);
				scene_a_B = GetB(scene_a_color);
				Vec3 ambientFactorRGB = pObject3D->material.getAmbient();
				Vec3 diffuseFactorRGB = pObject3D->material.getDiffuse();
				Vec3 specularFactorRGB = pObject3D->material.getSpecular();
				float shininessFactor = pObject3D->material.getShininess();

				//	Vec3 BPhongModel_a = Vec3((float)GetR(scene_a_color),(float)GetG(scene_a_color),(float)GetB(scene_a_color))*ambientFactorRGB;
				Vec3 BPhongModel_a = Vec3(scene_a_R*ambientFactorRGB[0],scene_a_G*ambientFactorRGB[1],scene_a_B*ambientFactorRGB[2]);
				Vec3 L,V,N,H;
				float distance=0;
				for(int vIdx = 0;vIdx<3;vIdx++)
				{
					sumRGB[vIdx]+=BPhongModel_a;
				}
				for(int lightIndex=0;lightIndex<nLightNum;lightIndex++)
				{

					if(pLights[lightIndex].bActive)
					{
						float lightR,lightG,lightB;
						lightR=GetR(pLights[lightIndex].light_color);
						lightG=GetG(pLights[lightIndex].light_color);
						lightB=GetB(pLights[lightIndex].light_color);

						if(pLights[lightIndex].type & LIGHT_TYPE_POINT)
						{
							for(int vIdx=0;vIdx<3;vIdx++)
							{
									L=Vec3(pLights[lightIndex].light_pos[0],pLights[lightIndex].light_pos[1],pLights[lightIndex].light_pos[2])-\
										Vec3(curTri->pnt_trans[vIdx].v[0],curTri->pnt_trans[vIdx].v[1],curTri->pnt_trans[vIdx].v[2]);
									distance=L.length();
									L.normalize();

									V=Vec3(pCamera->eye_pos[0],pCamera->eye_pos[1],pCamera->eye_pos[2])-\
										Vec3(curTri->pnt_trans[vIdx].v[0],curTri->pnt_trans[vIdx].v[1],curTri->pnt_trans[vIdx].v[2]);
									V.normalize();

									H=(L+V);
									H.normalize();

									N=Vec3(curTri->pnt_trans[vIdx].n[0],curTri->pnt_trans[vIdx].n[1],curTri->pnt_trans[vIdx].n[2]);
									N.normalize();

									float dotLN=L*N;
									float dotHN=H*N;
									float attenuation=0.0;
									//float common_factor =(pLights[lightIndex].K_d*dotLN + pLights[lightIndex].K_s *pow(dotHN,pLights[lightIndex].N_s));
									Vec3 factorRGB;
									if(dotLN>0)
									{
										factorRGB=(diffuseFactorRGB*max(dotLN,0.0) + specularFactorRGB *pow(max(dotHN,0.0),shininessFactor));
										attenuation=min(1.0,1.0/(pLights[lightIndex].kc + pLights[lightIndex].kl*distance + pLights[lightIndex].kq*distance*distance));
										//attenuation=1.0;
										factorRGB*=attenuation;
									}

									sumRGB[vIdx][0]+=factorRGB[0]*lightR;
									sumRGB[vIdx][1]+=factorRGB[1]*lightG;
									sumRGB[vIdx][2]+=factorRGB[2]*lightB;
							}

						}
					}
				}
				for(int vIdx = 0;vIdx<3;vIdx++)
				{
					float finalR =  max(0.0f, min(255.0f, sumRGB[vIdx][0]));
					float finalG = max(0.0f, min(255.0f, sumRGB[vIdx][1]));
					float finalB = max(0.0f, min(255.0f, sumRGB[vIdx][2]));
					curTri->vertex_color[vIdx]=RGB(finalR,finalG,finalB);
				}
				
				
			}else if(shade_mode & SHADE_MODE_TEXTURE)
			{
				
			}	
		}	
	}
}
void Scene::SetWinWidthAndHeight(double vp_centerX,double vp_centerY,double vpWidth,double vpHeight)
{
	double xvmax,yvmax;
	double xvmin,yvmin;
	xvmin=vp_centerX-vpWidth/2.0;
	yvmin=vp_centerY-vpHeight/2.0;
	xvmax=vp_centerX+vpWidth/2.0;
	yvmax=vp_centerY+vpHeight/2.0;
	winMin=Vec2f(xvmin,yvmin);
	winMax=Vec2f(xvmax,yvmax);
}
Mat4 Scene::Translate3D(float tx,float ty,float tz)
{
	Mat4 translateMat;
	translateMat[0][3] = tx;
	translateMat[1][3] = ty;
	translateMat[2][3] = tz;
	return translateMat;;
}
Mat4 Scene::Scale3D(float sx,float sy,float sz)
{
	Mat4 scaleMat;
	scaleMat[0][0] = sx;
	scaleMat[1][1] = sy;
	scaleMat[2][2] = sz;
	return scaleMat;
}
bool Scene::Rotate3D(Object3D *pObject3D,float theta,float x,float y,float z)
{
	if (pObject3D == NULL)
	{
		return 0;
	}
	float angle = theta/ (180.0 / PI);
	float c = cos( angle );
	float s = sin( angle );
	float t = 1.0 - c;

	Vec3 a(x, y, z);
	a.normalize();
	Mat4 matRatate = Mat4 (
		t*a[0]*a[0]+c, t*a[0]*a[1]-s*a[2], t*a[0]*a[2]+s*a[1], 0.0,
		t*a[0]*a[1]+s*a[2], t*a[1]*a[1]+c, t*a[1]*a[2]-s*a[0], 0.0,
		t*a[0]*a[2]-s*a[1], t*a[1]*a[2]+s*a[0], t*a[2]*a[2]+c, 0.0,
		0.0, 0.0, 0.0, 1.0);

	Vec4 n_end,n_start,n_vc;
	for (int triIdx=0;triIdx<pObject3D->tri_num;triIdx++)
	{
		for(int vIdx =0;vIdx<3;vIdx++)
		{
			//rotate the normals
			n_start=pObject3D->pTriList[triIdx].pnt_trans[vIdx].v;
			n_end=pObject3D->pTriList[triIdx].pnt_trans[vIdx].n+Vec3(n_start[0],n_start[1],n_start[2]);
			n_start=matRatate*n_start;
			n_end=matRatate*n_end;
			n_vc=n_end-n_start;n_vc[3]=1.0;
			n_vc.normalize();
			pObject3D->pTriList[triIdx].pnt_trans[vIdx].n=n_vc;
			//rotate the vertexes
			pObject3D->pTriList[triIdx].pnt_trans[vIdx].v=matRatate*(pObject3D->pTriList[triIdx].pnt_trans[vIdx].v);
		}	
	}
	return 1;
}

void Scene::Destroy_Camera(Camera *pCamera)
{
	if(pCamera != NULL)
	{
		delete pCamera;
		pCamera = NULL;
	}
}
void Scene::Destroy_Texture(Texture * pTextureInfo)
{
	if (pTextureInfo!=NULL)
	{
		delete pTextureInfo;
		pTextureInfo=NULL;
	}
}
void Scene::Destroy_Object3D(Object3D * pObject3D)
{
	if (pObject3D!=NULL)
	{
		delete pObject3D;
		pObject3D=NULL;
	}
}