#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_
#include "triangle.h"
#include "texture.h"
#include "material.h"


class Object3D
{
public:
	Object3D()
		{ 
			vertex_num=0;tri_num=0;pVertexList=NULL;pTriList = NULL;/*pTriList_trans=NULL;*/bRemoved=false;
		}
	~Object3D()
		{ 
			if(pVertexList !=NULL)
			{
				delete []pVertexList;
				pVertexList=NULL;
			} 
			if(pTriList !=NULL) 
			{
				delete []pTriList;
				pTriList=NULL;
			}
			//if(pTriList_trans !=NULL) {delete pTriList_trans;pTriList_trans=NULL;}
		}
	bool InitTexture(Texture *pTexture);

public:
	int vertex_num;
	int tri_num;
	Vertex3D *pVertexList;
	Triangle *pTriList;
	//Triangle *pTriList_trans;
	Material material;
	bool bRemoved;
};

#endif