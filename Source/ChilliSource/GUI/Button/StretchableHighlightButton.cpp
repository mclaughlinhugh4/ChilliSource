//
//  StretchableHighlightButton.cpp
//  Chilli Source
//  Created by Ian Copland on 03/04/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/GUI/Button/StretchableHighlightButton.h>
#include <ChilliSource/GUI/Image/StretchableImage.h>

#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <cmath>

namespace ChilliSource
{
    namespace GUI
    {
        DEFINE_META_CLASS(StretchableHighlightButton);
        
        DEFINE_PROPERTY(NormalTextureAtlas);
        DEFINE_PROPERTY(HighlightTextureAtlas);
        DEFINE_PROPERTY(BaseNormalTextureAtlasID);
        DEFINE_PROPERTY(BaseHighlightTextureAtlasID);
        DEFINE_PROPERTY(HighlightColour);
        
		//-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        StretchableHighlightButton::StretchableHighlightButton()
        :   HighlightColour(0.7f, 0.7f, 0.7f, 1.0f), mbSelected(false), mCurrentColour(Core::Colour::k_white)
        {
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonDeselect));
            
            m_image = StretchableImageSPtr(new StretchableImage());
            m_image->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            m_image->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            m_image->EnableUserInteraction(false);
            AddSubview(m_image);
        }
        //-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        StretchableHighlightButton::StretchableHighlightButton(const Core::ParamDictionary& insParams)
        : Button(insParams), HighlightColour(0.7f, 0.7f, 0.7f, 1.0f), mbSelected(false)
        {
            std::string strValue;
            
            //---Texture
            Core::StorageLocation eNormalTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("NormalTextureLocation", strValue))
            {
                eNormalTextureLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetNormalTexture(resourcePool->LoadResource<Rendering::Texture>(eNormalTextureLocation, strValue));
            }
            //---Texture
            Core::StorageLocation eHighlightTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("HighlightTextureLocation", strValue))
            {
                eHighlightTextureLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetHighlightTexture(resourcePool->LoadResource<Rendering::Texture>(eHighlightTextureLocation, strValue));
            }
            
            //---Sprite sheet
            Core::StorageLocation eNormalTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("NormalTextureAtlasLocation", strValue))
            {
                eNormalTextureAtlasLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalTextureAtlas", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetNormalTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eNormalTextureAtlasLocation, strValue));
            }
            //---Sprite sheet
            Core::StorageLocation eHighlightTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("HighlightTextureAtlasLocation", strValue))
            {
                eHighlightTextureAtlasLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightTextureAtlas", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetNormalTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eHighlightTextureAtlasLocation, strValue));
            }
			//---Default index ID
			if(insParams.TryGetValue("BaseNormalTextureAtlasID", strValue))
			{
				SetBaseNormalTextureAtlasID(strValue);
			}
			//---Highlight index ID
			if(insParams.TryGetValue("BaseHighlightTextureAtlasID", strValue))
			{
				SetBaseHighlightTextureAtlasID(strValue);
			}
            //---Highlight Colour
            if(insParams.TryGetValue("HighlightColour", strValue))
            {
                SetHighlightColour(Core::ParseColour(strValue));
            }
            
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonDeselect));
            
            m_image = StretchableImageSPtr(new StretchableImage());
            m_image->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            m_image->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            m_image->EnableUserInteraction(false);
            AddSubview(m_image);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetNormalTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            NormalTexture = inpTexture;
            mCurrentTexture = NormalTexture.get();
            
            if(!HighlightTexture)
            {
                SetHighlightTexture(inpTexture);
            }
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetHighlightTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            HighlightTexture = inpTexture;
            
            if(!NormalTexture)
            {
                SetNormalTexture(inpTexture);
            }
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        const Rendering::TextureCSPtr& StretchableHighlightButton::GetNormalTexture() const
        {
            return NormalTexture;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        const Rendering::TextureCSPtr& StretchableHighlightButton::GetHighlightTexture() const
        {
            return HighlightTexture;
        }
        //-----------------------------------------------------------
        /// Set Normal Sprite Sheet
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetNormalTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            NormalTextureAtlas = inpTextureAtlas;
            mCurrentTextureAtlas = NormalTextureAtlas.get();
            
            if(!HighlightTextureAtlas)
            {
                SetHighlightTextureAtlas(inpTextureAtlas);
            }
        }
        //-----------------------------------------------------------
        /// Set Highlight Sprite Sheet
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetHighlightTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            HighlightTextureAtlas = inpTextureAtlas;
            
            if(!NormalTextureAtlas)
            {
                SetNormalTextureAtlas(inpTextureAtlas);
            }
        }
        //-----------------------------------------------------------
        /// Get Normal Sprite Sheet
        //-----------------------------------------------------------
        const Rendering::TextureAtlasCSPtr& StretchableHighlightButton::GetNormalTextureAtlas() const
        {
            return NormalTextureAtlas; 
        }
        //-----------------------------------------------------------
        /// Get Highlight Sprite Sheet
        //-----------------------------------------------------------
        const Rendering::TextureAtlasCSPtr& StretchableHighlightButton::GetHighlightTextureAtlas() const
        {
            return HighlightTextureAtlas; 
        }
        //-----------------------------------------------------------
        /// Set Base Normal Sprite Sheet Index ID
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetBaseNormalTextureAtlasID(const std::string& instrID)
        {
            m_image->SetBaseTextureAtlasID(instrID);
            BaseNormalTextureAtlasID = instrID;
        }
        //-----------------------------------------------------------
        /// Set Base Highlight Sprite Sheet Index ID
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetBaseHighlightTextureAtlasID(const std::string& instrID)
        {
            BaseHighlightTextureAtlasID = instrID;
        }
        //-----------------------------------------------------------
        /// Get Base Normal Sprite Sheet Index ID
        //-----------------------------------------------------------
        const std::string& StretchableHighlightButton::GetBaseNormalTextureAtlasID() const
        {
            return BaseNormalTextureAtlasID;
        }
        //-----------------------------------------------------------
        /// Get Base Highlight Sprite Sheet Index ID
        //-----------------------------------------------------------
        const std::string& StretchableHighlightButton::GetBaseHighlightTextureAtlasID() const
        {
            return BaseHighlightTextureAtlasID;
        }
        //--------------------------------------------------------
        /// Set Highlight Colour
        //--------------------------------------------------------
        void StretchableHighlightButton::SetHighlightColour(const Core::Colour & inValue)
        {
            HighlightColour = inValue;
        }
        //--------------------------------------------------------
        /// Get Highlight Colour
        //--------------------------------------------------------
        const Core::Colour & StretchableHighlightButton::GetHighlightColour() const
        {
            return HighlightColour;
        }
        //-----------------------------------------------------------
        /// Draw
        //-----------------------------------------------------------
        void StretchableHighlightButton::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            GUIView::Draw(inpCanvas);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonSelect(GUIView* in_button, const Input::Pointer& in_pointer)
        {
            if(!mbSelected)
			{
                mvSelectedPos = in_pointer.GetPosition();
                
				mbSelected = true;
				
                m_image->SetTexture(HighlightTexture);
                
                if(HighlightTextureAtlas)
				{
                    bool bUniqueHighlight = BaseHighlightTextureAtlasID.empty() == false && BaseHighlightTextureAtlasID != BaseNormalTextureAtlasID;
					
					if (bUniqueHighlight && HighlightTextureAtlas)
					{
						m_image->SetTextureAtlas(HighlightTextureAtlas);
						m_image->SetBaseTextureAtlasID(BaseHighlightTextureAtlasID);
					}
					else
					{
						m_image->SetColour(HighlightColour);
					}
                    
				}
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonActivated(GUIView* in_button, const Input::Pointer& in_pointer)
        {
            if(mbSelected)
			{
				OnButtonDeselect(in_button, in_pointer);
				mOnButtonActivatedEvent.NotifyConnections(this);
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonDeselect(GUIView* in_button, const Input::Pointer& in_pointer)
        {
            if(mbSelected)
			{
                m_image->SetTexture(NormalTexture);
				
                
                if(NormalTextureAtlas)
				{
					m_image->SetColour(Core::Colour::k_white);
					m_image->SetBaseTextureAtlasID(BaseNormalTextureAtlasID);
					m_image->SetTextureAtlas(NormalTextureAtlas);
				}
				
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonDeselectThreshold(GUIView* in_button, const Input::Pointer& in_pointer)
        {
            const f32 kfThreshold = (f32)(GetScreen()->GetResolution().x) * 0.02f;
            f32 fDistX = std::abs(in_pointer.GetPosition().x - mvSelectedPos.x);
            f32 fDisty = std::abs(in_pointer.GetPosition().y - mvSelectedPos.y);
			if(fDistX >= kfThreshold || fDisty >= kfThreshold)
			{
                OnButtonDeselect(in_button, in_pointer);
			}
        }
        //-----------------------------------------------------------
        /// Destructor
        //-----------------------------------------------------------
        StretchableHighlightButton::~StretchableHighlightButton()
        {

        }
    }
}
