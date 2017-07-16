/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasGeometry.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!@brief        Constructor
    //!@date        2015-11-07
    CanvasGeometry::CanvasGeometry()
        : m_pHandle(NULL)
    {
        // Nothing to do
    }

    //!@brief       Destructor
    //!@date        2015-11-07
    CanvasGeometry::~CanvasGeometry()
    {
        // Nothing to do
    }

    //!@brief       SetPosition
    //!@date        2015-04-04
    void CanvasGeometry::Initialize(void* _pHandle)
    {
        m_pHandle = _pHandle;
    }

    //!@brief        Translate
    //!@date        2015-04-04
    void CanvasGeometry::Release()
    {
        
    }
}}