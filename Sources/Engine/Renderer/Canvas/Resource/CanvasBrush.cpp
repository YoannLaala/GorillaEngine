/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasBrush.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	  //!    @brief        Constructor
    //!    @date        2015-11-07
    CanvasBrush::CanvasBrush()
        : m_pHandle(NULL)
    {
        // Nothing to do
    }

    //!    @brief        Destructor
    //!    @date        2015-11-07
    CanvasBrush::~CanvasBrush()
    {
        // Nothing to do
    }

    //!    @brief        SetPosition
    //!    @date        2015-04-04
    void CanvasBrush::Initialize(void* _pHandle)
    {
        m_pHandle = _pHandle;
    }

    //!    @brief        Translate
    //!    @date        2015-04-04
    void CanvasBrush::Release()
    {
        
    }

}}