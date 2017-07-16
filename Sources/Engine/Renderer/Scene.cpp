/******************************************************************************
**	Includes
******************************************************************************/
#include "Scene.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/Tree/Octree.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	Scene::Scene()
		: m_pTree(NULL)
	{
		m_pTree = new Octree();
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	Scene::~Scene()
	{
		SAFE_DELETE(m_pTree)
	}

	//!	@brief		CreateCamera 
	//!	@date		2015-11-11
	Camera*	Scene::CreateCamera() 
	{ 
		Camera* pCamera = new Camera(); 
		if(m_vCamera.IsEmpty()) pCamera->SetViewport(Renderer::GetInstance()->GetViewport(0));
		m_vCamera.Add(pCamera); 

		return pCamera; 
	}

	//!	@brief		DestroyCamera 
	//!	@date		2015-11-11
	void Scene::DestroyCamera(Camera* _pCamera) 
	{ 
		m_vCamera.Remove(_pCamera); 
		SAFE_DELETE(_pCamera); 
	} 
}}