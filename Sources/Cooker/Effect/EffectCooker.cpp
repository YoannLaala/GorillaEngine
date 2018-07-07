/******************************************************************************
**	Includes
******************************************************************************/
#include "EffectCooker.hpp"
#include <Core/File/FileManager.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <Renderer/Resource/RenderState.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Serialize
	//!	@date		2015-11-22
	bool EffectCooker::Serialize(const char* _szInputPath, FileWriter& _kOutputStream)
	{
		// Read effect file
		Dictionary dEffect;
		if(!dEffect.Read<DictionaryStreamJson>(_szInputPath))
		{
			return false;
		}

		// Write Topology
		uint8 eTopology = Helper::GetEnumValue(dEffect["Topology"].GetString(), Renderer::ETopology::Value, Renderer::ETopology::Name, Renderer::ETopology::Count);
		_kOutputStream.Write(eTopology);

		// Render State
		Renderer::RenderState::Key kRenderState;
		Node kRenderStateNode = dEffect["RenderState"];
		kRenderState.WireFrame = kRenderStateNode["WireFrame"].GetBool();
		kRenderState.Cull = kRenderStateNode["Cull"].GetBool();
		kRenderState.Clockwise = kRenderStateNode["Clockwise"].GetBool();
		kRenderState.Scissor = kRenderStateNode["Scissor"].GetBool();
		kRenderState.MultiSample = kRenderStateNode["MultiSample"].GetBool();

		// Depth Stenil
		Node kDepthNode = kRenderStateNode["Depth"];
		kRenderState.Depth = kDepthNode["Enabled"].GetBool();
		kRenderState.DepthWrite = kDepthNode["Write"].GetBool();
		kRenderState.DepthComparaison = Helper::GetEnumValue(kDepthNode["Comparaison"].GetString(), Renderer::EDepthComparaison::Value, Renderer::EDepthComparaison::Name, Renderer::EDepthComparaison::Count);
		kRenderState.Stencil = kRenderStateNode["Stencil"].GetBool();
	
		// Blend
		Node kBlendNode = kRenderStateNode["Blend"];
		kRenderState.Blend = kBlendNode["Enabled"].GetBool();

		Node kBlendColorNode =  kBlendNode["Color"];
		kRenderState.BlendSource = Helper::GetEnumValue(kBlendColorNode["Source"].GetString(), Renderer::EBlend::Value, Renderer::EBlend::Name, Renderer::EBlend::Count);
		kRenderState.BlendOperation = Helper::GetEnumValue(kBlendColorNode["Operation"].GetString(), Renderer::EBlendOperation::Value, Renderer::EBlendOperation::Name, Renderer::EBlendOperation::Count);
		kRenderState.BlendDestination = Helper::GetEnumValue(kBlendColorNode["Destination"].GetString(), Renderer::EBlend::Value, Renderer::EBlend::Name, Renderer::EBlend::Count);

		// Write RenderState
		_kOutputStream.Write(&kRenderState, sizeof(Renderer::RenderState::Key));

		// Write number of technique and shader for this effect
		uint8 uiShaderCount = 0;
		uint32 uiStreamPosition = _kOutputStream.GetPosition();
		_kOutputStream.Write(uiShaderCount);

		String sShaderFile, sShaderEntry, sShaderParam, sShaderContent;
		for(uint8 eShader = 0; eShader < Renderer::EShader::Count; ++eShader)
		{
			sShaderFile.Set(GetInputPath().GetDirectory()).Append(GetInputPath().GetFileName()).Append(".", 1).Append(Renderer::EShader::Extension[eShader]);
			
			// Check if the file exist
			if(FileManager::IsFileExist(sShaderFile.GetBuffer()))
			{
				FileReader kStream(sShaderFile.GetBuffer());
				uint32 uiSize = kStream.GetSize();
				sShaderContent.Resize(uiSize);
				kStream.Read(&sShaderContent[0], uiSize);

				// Search all technique available for this shader
				sShaderFile.Remove(m_sProjectPath.GetBuffer());	// Make relative path
				for(uint8 eTechnique = 0; eTechnique < Renderer::ETechnique::Count; ++eTechnique)
				{
					sShaderEntry.Set(Renderer::EShader::Name[eShader]).Append(Renderer::ETechnique::Name[eTechnique]);
					if(sShaderContent.Find(sShaderEntry.GetBuffer()) != (uint32)-1)
					{	
						sShaderParam.Set("");
						if(eTechnique != Renderer::ETechnique::Default) sShaderParam.Set("--technique ").Append(Renderer::ETechnique::Name[eTechnique]);
						++uiShaderCount;

						_kOutputStream.Write(eTechnique);
						_kOutputStream.Write(eShader);
						WriteDependency(sShaderFile, sShaderParam, _kOutputStream);
					}
				}
			}
		}

		// Write proper technique count
		_kOutputStream.Seek(uiStreamPosition);
		_kOutputStream.Write(uiShaderCount);

		return true;
	}
}
