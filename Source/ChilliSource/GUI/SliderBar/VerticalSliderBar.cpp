//
//  VerticalSliderBar.cpp
//  Chilli Source
//  Created by Scott Downie on 09/06/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/GUI/SliderBar/VerticalSliderBar.h>

#include <ChilliSource/GUI/Image/ImageView.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(VerticalSliderBar)

		DEFINE_PROPERTY(BarTextureAtlas);
		DEFINE_PROPERTY(SliderTextureAtlas);
		DEFINE_PROPERTY(BarTextureAtlasID);
		DEFINE_PROPERTY(SliderTextureAtlasID);
		DEFINE_PROPERTY(UnifiedSliderSize);

        //------------------------------------------------------
        /// Constructor
        ///
        /// Create the subviews that make up the container
        //------------------------------------------------------
        VerticalSliderBar::VerticalSliderBar() 
		: mpBackgroundImage(new ImageView()), mpSliderImage(new ImageView()), UnifiedSliderSize(1.5f, 0.15f, 0.0f, 0.0f)
        {
			SetSize(0.1f, 0.8f, 0.0f, 0.0f);
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
            
            mpSliderImage->SetSize(UnifiedSliderSize);
            mpSliderImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.0f), Core::Vector2(0, 0)));
            mpSliderImage->EnableUserInteraction(false);
            mpBackgroundImage->AddSubview(mpSliderImage);
            
            m_sliderMovedConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &VerticalSliderBar::OnSliderMoved));
        }
        //------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------
        VerticalSliderBar::VerticalSliderBar(const Core::ParamDictionary& insParams) 
		: SliderBar(insParams), mpBackgroundImage(new ImageView()), mpSliderImage(new ImageView()), UnifiedSliderSize(1.5f, 0.15f, 0.0f, 0.0f)
        {
			SetSize(0.1f, 0.8f, 0.0f, 0.0f);
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
            
            mpSliderImage->SetSize(UnifiedSliderSize);
            mpSliderImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.0f), Core::Vector2(0, 0)));
            mpSliderImage->EnableUserInteraction(false);
			mpSliderImage->EnableAcceptTouchesOutsideOfBounds(true);
            mpBackgroundImage->AddSubview(mpSliderImage);
            
            std::string strValue;
            
            //---Bar Texture
            Core::StorageLocation eBarTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BarTextureLocation", strValue))
            {
                eBarTextureLocation = Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BarTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetBarTexture(resourcePool->LoadResource<Rendering::Texture>(eBarTextureLocation, strValue));
            }
			//---Slider Texture
            Core::StorageLocation eSliderTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SliderTextureLocation", strValue))
            {
                eSliderTextureLocation = Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("SliderTexture", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetSliderTexture(resourcePool->LoadResource<Rendering::Texture>(eSliderTextureLocation, strValue));
			}
            
            //---Bar Sprite sheet
            Core::StorageLocation eBarTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BarTextureAtlasLocation", strValue))
            {
                eBarTextureAtlasLocation = Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BarTextureAtlas", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetBarTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eBarTextureAtlasLocation, strValue));
            }
			//---Slider Sprite sheet
            Core::StorageLocation eSliderTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SliderTextureAtlasLocation", strValue))
            {
                eSliderTextureAtlasLocation = Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("SliderTextureAtlas", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetSliderTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eSliderTextureAtlasLocation, strValue));
			}
			//---Background ID
			if(insParams.TryGetValue("BarTextureAtlasID", strValue))
			{
				SetBarTextureAtlasID(strValue);
			}
			//---Slider ID
			if(insParams.TryGetValue("SliderTextureAtlasID", strValue))
			{
				SetSliderTextureAtlasID(strValue);
			}
            //---Slider size
            if(insParams.TryGetValue("UnifiedSliderSize", strValue))
            {
                Core::Vector4 vRawSize = Core::ParseVector4(strValue);
                SetSliderSize(Core::UnifiedVector2(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w));
            }
            
            m_sliderMovedConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &VerticalSliderBar::OnSliderMoved));
        }
        //--------------------------------------------------------
        /// Set Bar Texture
        ///
        /// @param Texture for the bar
        //--------------------------------------------------------
        void VerticalSliderBar::SetBarTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            BarTexture = inpTexture;
            mpBackgroundImage->SetTexture(inpTexture);
            
            if(!SliderTexture)
            {
                SetSliderTexture(inpTexture);
            }
        }
        //--------------------------------------------------------
        /// Get Bar Texture
        ///
        /// @return Texture for the bar
        //--------------------------------------------------------
        const Rendering::TextureCSPtr& VerticalSliderBar::GetBarTexture() const
        {
            return BarTexture;
        }
        //--------------------------------------------------------
        /// Set Bar Sprite Sheet
        ///
        /// @param Sprite sheet for the bar
        //--------------------------------------------------------
        void VerticalSliderBar::SetBarTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
			BarTextureAtlas = inpTextureAtlas;
            mpBackgroundImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!SliderTextureAtlas)
            {
                SetSliderTextureAtlas(inpTextureAtlas);
            }
        }
        //--------------------------------------------------------
        /// Set Slider Texture
        ///
        /// @param Texture for the slider
        //--------------------------------------------------------
        void VerticalSliderBar::SetSliderTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            SliderTexture = inpTexture;
            mpSliderImage->SetTexture(inpTexture);
            
            if(!BarTextureAtlas)
            {
                SetBarTexture(inpTexture);
            }
        }
        //--------------------------------------------------------
        /// Get Slider Texture
        ///
        /// @return Texture for the slider
        //--------------------------------------------------------
        const Rendering::TextureCSPtr& VerticalSliderBar::GetSliderTexture() const
        {
            return SliderTexture;
        }
		//--------------------------------------------------------
		/// Get Bar Sprite Sheet
		///
		/// @return Sprite sheet for the bar
		//--------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& VerticalSliderBar::GetBarTextureAtlas() const
		{
			return BarTextureAtlas;
		}
		//--------------------------------------------------------
		/// Set Slider Sprite Sheet
		///
		/// @param Sprite sheet for the slider
		//--------------------------------------------------------
		void VerticalSliderBar::SetSliderTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
		{
			SliderTextureAtlas = inpTextureAtlas;
			mpSliderImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!BarTextureAtlas)
            {
                SetBarTextureAtlas(inpTextureAtlas);
            }
		}
		//--------------------------------------------------------
		/// Get Slider Sprite Sheet
		///
		/// @return Sprite sheet for the slider
		//--------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& VerticalSliderBar::GetSliderTextureAtlas() const
		{
			return SliderTextureAtlas;
		}
		//--------------------------------------------------------
		/// Set Bar Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void VerticalSliderBar::SetBarTextureAtlasID(const std::string& instrID)
		{
			CS_ASSERT(BarTextureAtlas, "Cannot set sprite sheet index without setting sprite sheet");
			BarTextureAtlasID = instrID;
            mpBackgroundImage->SetTextureAtlasID(instrID);
		}
		//--------------------------------------------------------
		/// Set Slider Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void VerticalSliderBar::SetSliderTextureAtlasID(const std::string& instrID)
		{
			CS_ASSERT(SliderTextureAtlas, "Cannot set sprite sheet index without setting sprite sheet");
			SliderTextureAtlasID = instrID;
            mpSliderImage->SetTextureAtlasID(instrID);
		}
		//--------------------------------------------------------
		/// Get Bar Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& VerticalSliderBar::GetBarTextureAtlasID() const
		{
			return BarTextureAtlasID;
		}
		//--------------------------------------------------------
		/// Get Slider Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& VerticalSliderBar::GetSliderTextureAtlasID() const
		{
			return SliderTextureAtlasID;
		}
        //--------------------------------------------------------
        /// Set Slider Size
        ///
        /// @param The unified size of the slider
        //--------------------------------------------------------
        void VerticalSliderBar::SetSliderSize(const Core::UnifiedVector2& invUnifiedSize)
        {
			UnifiedSliderSize = invUnifiedSize;
            mpSliderImage->SetSize(invUnifiedSize);
        }
		//--------------------------------------------------------
		/// Get Slider Size
		///
		/// @return The unified size of the slider
		//--------------------------------------------------------
		const Core::UnifiedVector2& VerticalSliderBar::GetSliderSize() const
		{
			return UnifiedSliderSize;
		}
        //---Touch Delegates
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void VerticalSliderBar::OnSliderMoved(GUIView* inpView, const Input::Pointer& in_pointer)
        {
            mfSliderValue = ((in_pointer.GetPosition().y - GetAbsoluteScreenSpacePosition().y)/GetAbsoluteSize().y) + 0.5f;
            mpSliderImage->SetPosition(0.5f, mfSliderValue, 0.0f, 0.0f);
        }
		//-----------------------------------------------------------
		/// Update Slider Position
		/// 
		/// Reposition the slider image
		//-----------------------------------------------------------
		void VerticalSliderBar::UpdateSliderPosition()
		{
			mpSliderImage->SetPosition(0.5f, mfSliderValue, 0.0f, 0.0f);
		}
        //------------------------------------------------------
        /// Draw
        ///
        /// Draws a background box with the progress bar
        /// as subview
        ///
        /// @param Canvas renderer pointer
        //------------------------------------------------------
        void VerticalSliderBar::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            if(Visible)
            {
                GUIView::Draw(inpCanvas);
            }
        }
    }
}
