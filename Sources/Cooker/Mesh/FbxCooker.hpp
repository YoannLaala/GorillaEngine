#ifndef _COOKER_FBX_COOKER_HPP_
#define _COOKER_FBX_COOKER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Helper/AssetCooker.hpp>
#include <fbx/include/fbxsdk.h>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla
{
	struct Geometry;
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class FbxCooker : public AssetCooker
	{
	private:
		virtual bool	Serialize			(const char* _szInputPath, FileWriter& _kOutputStream) override;

		void			GenerateGeometry	(FbxNode* _pNode, Geometry* _pGeometry, float32 _fScale);
		void			GenerateVertices	(FbxMesh* _pMesh, Geometry* _pGeometry, float32 _fScale);
	};
}

#endif
