/******************************************************************************
**	Includes
******************************************************************************/
#include "Effect.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-10-18
	Effect::Effect()
		: m_pRenderState(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-10-18
	Effect::~Effect()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-10-18
	void Effect::Initialize(RenderState* _pRenderState, ETopology::Type _eTopology)
	{
		memset(m_aShader, 0, sizeof(Shader*) * EShader::Count * ETechnique::Count);
		m_pRenderState = _pRenderState;
		m_eTopology = _eTopology;
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void Effect::Release()
	{
		
	}
}}