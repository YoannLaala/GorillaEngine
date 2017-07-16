/******************************************************************************
**	Includes
******************************************************************************/
#include "WebDocument.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	ASSET_REGISTER(Gorilla::Engine::WebDocument);

	//!	@brief		Constructor
	//!	@date		2015-11-11
	WebDocument::WebDocument()
	{
		// Nothong to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-11
	WebDocument::~WebDocument()
	{
		// Nothong to do
	}

	//!	@brief		Deserialize
	//!	@date		2015-10-18
	void WebDocument::Initialize(StreamReader* /*_pStream*/)
	{
		m_sUrl = GetFilePath();
	}

	//!	@brief		Release
	//!	@date		2015-10-18
	void WebDocument::Release()
	{
		// Nothing to do
	}

}}	// Gorilla::Asset
