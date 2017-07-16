#ifndef _ENGINE_WEB_DOCUMENT_HPP_
#define _ENGINE_WEB_DOCUMENT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Asset/Asset.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class WebDocument : public Asset
	{
	public:
		WebDocument();
		~WebDocument();
		ASSET_DECLARE(Gorilla::Engine::WebDocument);

	protected:
		virtual void Initialize	(StreamReader* _pStream) override;
		virtual void Release	() override;

	public:
		inline const String&	GetUrl() const { return m_sUrl; }

	private:
		String m_sUrl;
	};
}}	// Gorilla::Asset

					

#endif