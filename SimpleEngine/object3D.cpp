#include "stdafx.h"
#include "object3D.h"

bool Object3D::InitTexture(Texture* pTexture)
{
	if(pTexture != NULL)
	{
		for (int triIndex=0;triIndex<tri_num;triIndex++)
		{
			pTriList[triIndex].texture=*pTexture;
		}
		//currently support cube texture mapping
		if (pTexture->bih.biHeight==pTexture->bih.biWidth)
		{	
			int  mapCoord=pTexture->bih.biWidth-1;
			pTriList[0].uv[0]=Vec2f(0,mapCoord);    
			pTriList[0].uv[1]=Vec2f(0,0);      
			pTriList[0].uv[2]=Vec2f(mapCoord,0);    

			pTriList[1].uv[0]=Vec2f(mapCoord,mapCoord);  
			pTriList[1].uv[1]=Vec2f(0,mapCoord);    
			pTriList[1].uv[2]=Vec2f(mapCoord,0);    

			pTriList[2].uv[0]=Vec2f(mapCoord,mapCoord);      
			pTriList[2].uv[1]=Vec2f(0,mapCoord);    
			pTriList[2].uv[2]=Vec2f(mapCoord,0);    

			pTriList[3].uv[0]=Vec2f(0,0);  
			pTriList[3].uv[1]=Vec2f(mapCoord,0);    
			pTriList[3].uv[2]=Vec2f(0,mapCoord);    

			pTriList[4].uv[0]=Vec2f(0,mapCoord);    
			pTriList[4].uv[1]=Vec2f(0,0);      
			pTriList[4].uv[2]=Vec2f(mapCoord,mapCoord);  

			pTriList[5].uv[0]=Vec2f(0,0);      
			pTriList[5].uv[1]=Vec2f(mapCoord,0);    
			pTriList[5].uv[2]=Vec2f(mapCoord,mapCoord);  

			pTriList[6].uv[0]=Vec2f(0,mapCoord);    
			pTriList[6].uv[1]=Vec2f(mapCoord,0);    
			pTriList[6].uv[2]=Vec2f(mapCoord,mapCoord);  

			pTriList[7].uv[0]=Vec2f(0,0);      
			pTriList[7].uv[1]=Vec2f(mapCoord,0);    
			pTriList[7].uv[2]=Vec2f(0,mapCoord);    

			pTriList[8].uv[0]=Vec2f(mapCoord,0);    
			pTriList[8].uv[1]=Vec2f(mapCoord,mapCoord);  
			pTriList[8].uv[2]=Vec2f(0,mapCoord);    

			pTriList[9].uv[0]=Vec2f(0,0);      
			pTriList[9].uv[1]=Vec2f(mapCoord,0);    
			pTriList[9].uv[2]=Vec2f(0,mapCoord);    

			pTriList[10].uv[0]=Vec2f(0,0);     
			pTriList[10].uv[1]=Vec2f(mapCoord,0);   
			pTriList[10].uv[2]=Vec2f(mapCoord,mapCoord); 

			pTriList[11].uv[0]=Vec2f(0,0);     
			pTriList[11].uv[1]=Vec2f(mapCoord,mapCoord); 
			pTriList[11].uv[2]=Vec2f(0,mapCoord);   
		}else
		{
			for (int triIndex=0;triIndex<tri_num;triIndex++)
			{
				for (int vertexIdx=0;vertexIdx<3;vertexIdx++)
				{
					pTriList[triIndex].uv[vertexIdx]=Vec2f(0,0);
				}
			}
		}
		return 1;
	}else return 0;
}