#ifndef _COMPONENT_WEB_VIEW_HPP_
#define _COMPONENT_WEB_VIEW_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/Renderable.hpp>

#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Gui/WebDocument.hpp>

#include <Web/WebBrowser.hpp>
#include <Web/WebPage.hpp>
#include <Web/WebFunction.hpp>
#include <Web/WebValue.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Texture2D;
}}

namespace Gorilla { namespace Web
{
	class WebBrowser;
	class WebFunction;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class WebView : public Renderable
	{
	public:
		WebView();
		~WebView();
		COMPONENT_DECLARE(Gorilla::Component::WebView);

	protected:
		//virtual bool Serialize		(FileWriter& /*_kStream*/) override;
		//virtual bool Deserialize	(FileReader& /*_kStream*/) override;

		virtual void Start			() override;
		virtual void Update			() override;
		virtual void Stop			() override;

		// Renderale
		virtual void PushCommand(Renderer::RenderBuffer* _pBuffer) override;

	private:
		Web::WebBrowser* GetBrowser();

		SIGNAL_RECEIVER(WebView, SizeChanged, uint32 /*_uiWidth*/, uint32 /*_uiHeight*/);
		void OnSizeChanged(uint32 _uiWidth, uint32 _uiHeight);


	public:
		Engine::AssetHandle<Engine::WebDocument>	Document;
		Renderer::Texture2D*						RenderTexture;
		bool										Transparent;

		inline Web::WebPage*						GetPage() { return m_pInstance; }

	private:
		Web::WebPage*				m_pInstance;
		Renderer::Texture2D*		m_pTexture;
	};
}}

#endif