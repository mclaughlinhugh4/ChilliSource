//
//  HighlightButton.cpp
//  Chilli Source
//  Created by Scott Downie on 26/04/2011.
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

#include <ChilliSource/GUI/Button/HighlightButton.h>
#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

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
		DEFINE_META_CLASS(HighlightButton)

		DEFINE_PROPERTY(NormalTexture);
		DEFINE_PROPERTY(HighlightTexture);
		DEFINE_PROPERTY(NormalTextureAtlas);
		DEFINE_PROPERTY(HighlightTextureAtlas);

		DEFINE_PROPERTY(HighlightColour);

		DEFINE_PROPERTY(NormalTextureAtlasID);
		DEFINE_PROPERTY(HighlightTextureAtlasID);

		DEFINE_PROPERTY(SizeFromImage);
		DEFINE_PROPERTY(HeightMaintain);
		DEFINE_PROPERTY(WidthMaintain);
        //-----------------------------------------------------------
        /// Constructor
        ///
        /// Create the widget by adding the background image
        //-----------------------------------------------------------
        HighlightButton::HighlightButton() 
        : mpBackgroundImage(new ImageView()), HighlightColour(0.7f, 0.7f, 0.7f, 1.0f),
        msDefaultUVs(0.0f, 0.0f, 1.0f, 1.0f),
        msHighlightUVs(0.0f, 0.0f, 1.0f, 1.0f),
        mbSelected(false), SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), WidthFromImage(false), HeightFromImage(false), mbFillMaintain(false), mbFitMaintain(false)
        {
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonDeselect));
        }
        //------------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------------
        HighlightButton::HighlightButton(const Core::ParamDictionary& insParams) 
        : Button(insParams), mpBackgroundImage(new ImageView()),
        msDefaultUVs(0.0f, 0.0f, 1.0f, 1.0f),
        msHighlightUVs(0.0f, 0.0f, 1.0f, 1.0f),
        mbSelected(false), HighlightColour(0.7f, 0.7f, 0.7f, 1.0f),
		SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), WidthFromImage(false), HeightFromImage(false), mbFillMaintain(false), mbFitMaintain(false)
        {
            std::string strValue;

            //---Default
            Core::StorageLocation eNormalTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("NormalTextureLocation", strValue))
            {
                eNormalTextureLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetNormalImage(resourcePool->LoadResource<Rendering::Texture>(eNormalTextureLocation, strValue));
            }
            //---Highlight
            Core::StorageLocation eHighlightTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("HighlightTextureLocation", strValue))
            {
                eHighlightTextureLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetHighlightImage(resourcePool->LoadResource<Rendering::Texture>(eHighlightTextureLocation, strValue));
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
				SetHighlightTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eHighlightTextureAtlasLocation, strValue));
            }
			//---Default index ID
			if(insParams.TryGetValue("NormalTextureAtlasID", strValue))
			{
				SetNormalTextureAtlasID(strValue);
			}
			//---Highlight index ID
			if(insParams.TryGetValue("HighlightTextureAtlasID", strValue))
			{
				SetHighlightTextureAtlasID(strValue);
			}
            //---Highlight Colour
            if(insParams.TryGetValue("HighlightColour", strValue))
            {
                SetHighlightColour(Core::ParseColour(strValue));
            }
			//---Size from image
            if(insParams.TryGetValue("SizeFromImage", strValue))
            {
                SizeFromImage = Core::ParseBool(strValue);
            }
            //---Width from image
            if(insParams.TryGetValue("WidthFromImage", strValue))
            {
                EnableWidthFromImage(Core::ParseBool(strValue));
            }
            //---Height from image
            if(insParams.TryGetValue("HeightFromImage", strValue))
            {
                EnableHeightFromImage(Core::ParseBool(strValue));
            }
			//---Maintain Width
			if(insParams.TryGetValue("HeightMaintain", strValue))
			{
				HeightMaintain = Core::ParseBool(strValue);
			}
			//---Maintain Height
			if(insParams.TryGetValue("WidthMaintain", strValue))
			{
				WidthMaintain = Core::ParseBool(strValue);
			}
			//---Set Maintain Height
			if(insParams.TryGetValue("SetHeightMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				HeightMaintain = true;
				SetHeightMaintainingAspect(vSize.x, vSize.y);
			}
			//---Set Maintain Width
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				WidthMaintain = true;
				SetWidthMaintainingAspect(vSize.x, vSize.y);
			}
            //---Set Fill Maintain
            if(insParams.TryGetValue("SetFillMaintain", strValue))
            {
                Core::Vector4 vSize = Core::ParseVector4(strValue);
                SetFillMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
            }
            //---Set Fit Maintain
            if(insParams.TryGetValue("SetFitMaintain", strValue))
            {
                Core::Vector4 vSize = Core::ParseVector4(strValue);
                SetFitMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
            }
            
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonDeselect));
        }
        //-----------------------------------------------------------
        /// Get Background Image View
        ///
        /// @return Image view
        //-----------------------------------------------------------
        const ImageViewSPtr& HighlightButton::GetBackgroundImageView() const
        {
            return mpBackgroundImage;
        }
        //-----------------------------------------------------------
        /// Set Default Image
        ///
        /// Image for unselected button
        ///
        /// @param Texture shared pointer
        //-----------------------------------------------------------
        void HighlightButton::SetNormalImage(const Rendering::TextureCSPtr& inpTexture)
        {
            NormalTexture = inpTexture;
			mpBackgroundImage->SetTexture(inpTexture);
            mpBackgroundImage->SetUVs(msDefaultUVs);
            
            if(!HighlightTexture)
            {
                SetHighlightImage(inpTexture);
            }
        }
		//-----------------------------------------------------------
		/// Get Default Image
		///
		/// @return Texture for non-selected button
		//-----------------------------------------------------------
		const Rendering::TextureCSPtr& HighlightButton::GetNormalImage() const
		{
			return NormalTexture;
		}
        //-----------------------------------------------------------
        /// Set Highlight Image
        ///
        /// Image for selected button
        ///
        /// @param Texture shared pointer
        //-----------------------------------------------------------
        void HighlightButton::SetHighlightImage(const Rendering::TextureCSPtr& inpTexture)
        {
            HighlightTexture = inpTexture;
            
            if(!NormalTexture)
            {
                SetNormalImage(inpTexture);
            }
        }
		//-----------------------------------------------------------
		/// Get Highlight Image
		///
		/// @return Texture for selected button
		//-----------------------------------------------------------
		const Rendering::TextureCSPtr& HighlightButton::GetHighlightImage() const
		{
			return HighlightTexture;
		}
        //-----------------------------------------------------------

        /// Set Default UVs
        ///
        /// Sets the UV coordinates for the default texture
        ///
        /// @param Rectangle for the UVs
        //-----------------------------------------------------------
        void HighlightButton::SetDefaultUVs(Rendering::UVs insUVs)
        {
            msDefaultUVs = insUVs;
            
            if(mpBackgroundImage->GetTexture() == NormalTexture)
                mpBackgroundImage->SetUVs(msDefaultUVs);
        }
        //-----------------------------------------------------------
        /// Set Highlight UVs
        ///
        /// Sets the UV coordinates for the Highlight texture
        ///
        /// @param Rectangle for the UVs
        //-----------------------------------------------------------
        void HighlightButton::SetHighlightUVs(Rendering::UVs insUVs)
        {
            msHighlightUVs = insUVs;
            
            if(mpBackgroundImage->GetTexture() == HighlightTexture)
                mpBackgroundImage->SetUVs(msHighlightUVs);
        }

        //-----------------------------------------------------------
        /// Set Normal Sprite Sheet
        ///
        /// @param Sprite sheet with default image
        //-----------------------------------------------------------
        void HighlightButton::SetNormalTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            NormalTextureAtlas = inpTextureAtlas;
            mpBackgroundImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!HighlightTextureAtlas && NormalTextureAtlas)
            {
                SetHighlightTextureAtlas(inpTextureAtlas);
            }
        }
		//-----------------------------------------------------------
		/// Get Normal Sprite Sheet
		///
		/// @return Sprite sheet with default image
		//-----------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& HighlightButton::GetNormalTextureAtlas() const
		{ 
			return NormalTextureAtlas; 
		}
		//-----------------------------------------------------------
		/// Set Highlight Sprite Sheet
		///
		/// @param Sprite sheet with highlight image
		//-----------------------------------------------------------
		void HighlightButton::SetHighlightTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
		{
			HighlightTextureAtlas = inpTextureAtlas;
			mpBackgroundImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!NormalTextureAtlas && HighlightTextureAtlas)
            {
                SetNormalTextureAtlas(inpTextureAtlas);
            }
		}
		//-----------------------------------------------------------
		/// Get Highlight Sprite Sheet
		///
		/// @return Sprite sheet with highlight image
		//-----------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& HighlightButton::GetHighlightTextureAtlas() const 
		{ 
			return HighlightTextureAtlas; 
		}
		//-----------------------------------------------------------
		/// Set Normal Sprite Sheet Index
		///
		/// @param Index of default state on sprite sheet
		//-----------------------------------------------------------
		void HighlightButton::SetNormalTextureAtlasID(const std::string& instrID)
		{
			NormalTextureAtlasID = instrID;
            mpBackgroundImage->SetTextureAtlasID(instrID);
		}
		//-----------------------------------------------------------
		/// Set Highlight Sprite Sheet Index ID
		///
		/// @param Index ID of highlight state on sprite sheet
		//-----------------------------------------------------------
		void HighlightButton::SetHighlightTextureAtlasID(const std::string& instrID)
		{
			HighlightTextureAtlasID = instrID;
		}
		//-----------------------------------------------------------
		/// Get Normal Sprite Sheet Index ID
		///
		/// @return Index ID of default state on sprite sheet
		//-----------------------------------------------------------
		const std::string& HighlightButton::GetNormalTextureAtlasID() const
		{
			return NormalTextureAtlasID;
		}
		//-----------------------------------------------------------
		/// Get Highlight Sprite Sheet Index ID
		///
		/// @return Index ID of highlight state on sprite sheet
		//-----------------------------------------------------------
		const std::string& HighlightButton::GetHighlightTextureAtlasID() const
		{
			return HighlightTextureAtlasID;
		}
		//--------------------------------------------------------
        /// Enable Size From Image
        ///
        /// When this is enabled the image view's size will be 
        /// based on the size of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void HighlightButton::EnableSizeFromImage(bool inbEnable)
        {
            SizeFromImage = inbEnable;
        }
		//--------------------------------------------------------
		/// Is Size From Image Enabled
		///
		/// When this is enabled the image view's size will be 
		/// based on the size of the image
		///
		/// @return Whether the image view's size will be 
		/// based on the size of the image
		//--------------------------------------------------------
		bool HighlightButton::IsSizeFromImageEnabled() const
		{
			return SizeFromImage;
		}
		//--------------------------------------------------------
		/// Set Highlight Colour
		///
		/// @param Tint colour to apply when the button is
		/// selected
		//--------------------------------------------------------
		void HighlightButton::SetHighlightColour(const Core::Colour & inValue)
		{
			HighlightColour = inValue;
		}
		//--------------------------------------------------------
		/// Get Highlight Colour
		///
		/// @return Tint colour to apply when the button is
		/// selected
		//--------------------------------------------------------
		const Core::Colour & HighlightButton::GetHighlightColour() const
		{
			return HighlightColour;
		}
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void HighlightButton::OnButtonSelect(GUIView* in_button, const Input::Pointer& in_pointer)
        {
			if(!mbSelected)
			{
                mvSelectedPos = in_pointer.GetPosition();
                
				mbSelected = true;
                
                mpBackgroundImage->SetTexture(HighlightTexture);
                
                if(HighlightTextureAtlas)
				{
					bool bUniqueHighlight = HighlightTextureAtlasID.empty() == false && HighlightTextureAtlasID != NormalTextureAtlasID;
					
					if (bUniqueHighlight && HighlightTextureAtlas)
					{
						mpBackgroundImage->SetTextureAtlas(HighlightTextureAtlas);
						mpBackgroundImage->SetTextureAtlasID(HighlightTextureAtlasID);
					} 
					else 
					{
						mpBackgroundImage->SetColour(HighlightColour);
					}

				}
                else
                {
                    mpBackgroundImage->SetUVs(msHighlightUVs);
                    mpBackgroundImage->SetColour(HighlightColour);
                }
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void HighlightButton::OnButtonDeselect(GUIView* in_button, const Input::Pointer& in_pointer)
        {
			if(mbSelected)
			{
                mpBackgroundImage->SetTexture(NormalTexture);
				

                if(NormalTextureAtlas)
				{
					mpBackgroundImage->SetColour(Core::Colour::k_white);
					mpBackgroundImage->SetTextureAtlasID(NormalTextureAtlasID);
					mpBackgroundImage->SetTextureAtlas(NormalTextureAtlas);
				}
                else
                {
                    mpBackgroundImage->SetUVs(msDefaultUVs);
                    mpBackgroundImage->SetColour(Core::Colour::k_white);
                }
				
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void HighlightButton::OnButtonDeselectThreshold(GUIView* in_button, const Input::Pointer& in_pointer)
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
        //-----------------------------------------------------------
        void HighlightButton::OnButtonActivated(GUIView* in_button, const Input::Pointer& in_pointer)
        {
			if(mbSelected)
			{
				OnButtonDeselect(in_button, in_pointer);
				mOnButtonActivatedEvent.NotifyConnections(this);
			}
        }
		//-----------------------------------------------------------
		/// Draw
		///
		/// Overloaded draw method
		///
		/// @param Canvas renderer
		//-----------------------------------------------------------
		void HighlightButton::Draw(Rendering::CanvasRenderer* inpCanvasRenderer)
		{
			SetSizeFromImage();
			GUIView::Draw(inpCanvasRenderer);
		}
		
		void HighlightButton::SetSizeFromImage()
		{
			if(SizeFromImage)
			{
				Core::Vector2 vImageSize = mpBackgroundImage->GetSizeFromImage();
				SetSize(0.0f, 0.0f, vImageSize.x, vImageSize.y);
			}
            else if(HeightFromImage)
            {
                SetHeightFromImage();
            }
            else if(WidthFromImage)
            {
                SetWidthFromImage();
            }
			else
            {
                UpdateSizeMaintainingAspect();
            }
		}
        //--------------------------------------------------------
        /// Update Size Maintaining Aspect
        ///
        /// Image view can be sized from assets etc. This function
        /// calculates the size based on these factors
        //--------------------------------------------------------
        void HighlightButton::UpdateSizeMaintainingAspect()
        {
            if(mbFillMaintain)
            {
                SetFillMaintainingAspect(mvFillMaintainTarget.GetRelative().x, mvFillMaintainTarget.GetRelative().y, mvFillMaintainTarget.GetAbsolute().x, mvFillMaintainTarget.GetAbsolute().y);
            }
            else if(mbFitMaintain)
            {
                SetFitMaintainingAspect(mvFitMaintainTarget.GetRelative().x, mvFitMaintainTarget.GetRelative().y, mvFitMaintainTarget.GetAbsolute().x, mvFitMaintainTarget.GetAbsolute().y);
            }
            else if(HeightMaintain)
            {
                Core::UnifiedVector2 uvSize = GetSize();
                SetHeightMaintainingAspect(uvSize.GetRelative().y, uvSize.GetAbsolute().y);
            }
            else if(WidthMaintain)
            {
                Core::UnifiedVector2 uvSize = GetSize();
                SetWidthMaintainingAspect(uvSize.GetRelative().x, uvSize.GetAbsolute().x);
            }
        }
		
        //--------------------------------------------------------
        /// Enable Width From Image
        ///
        /// When this is enabled the image view's width will be
        /// based on the width of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void HighlightButton::EnableWidthFromImage(bool inbEnable)
        {
            WidthFromImage = inbEnable;
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Is Width From Image Enabled
        ///
        /// When this is enabled the image view's width will be
        /// based on the width of the image
        ///
        /// @return Whether the image view's width will be
        /// based on the width of the image
        //--------------------------------------------------------
        bool HighlightButton::IsWidthFromImageEnabled() const
        {
            return WidthFromImage;
        }
        //--------------------------------------------------------
        /// Set Width From Image
        ///
        /// Sets the of the image view to the width of the image
        /// without requiring a draw to call. Useful for working
        /// with relative sizes and aspect ratios.
        //--------------------------------------------------------
        void HighlightButton::SetWidthFromImage()
        {
            SetSize(0.0f, GetSize().GetRelative().y, mpBackgroundImage->GetSizeFromImage().x, GetSize().GetAbsolute().y);
        }
        //--------------------------------------------------------
        /// Enable Height From Image
        ///
        /// When this is enabled the image view's height will be
        /// based on the height of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void HighlightButton::EnableHeightFromImage(bool inbEnable)
        {
            HeightFromImage = inbEnable;
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Is Height From Image Enabled
        ///
        /// When this is enabled the image view's height will be
        /// based on the height of the image
        ///
        /// @return Whether the image view's height will be
        /// based on the height of the image
        //--------------------------------------------------------
        bool HighlightButton::IsHeightFromImageEnabled() const
        {
            return HeightFromImage;
        }
        //--------------------------------------------------------
        /// Set Height From Image
        ///
        /// Sets the of the image view to the height of the image
        /// without requiring a draw to call. Useful for working
        /// with relative sizes and aspect ratios.
        //--------------------------------------------------------
        void HighlightButton::SetHeightFromImage()
        {
            SetSize(GetSize().GetRelative().x, 0.0f, GetSize().GetAbsolute().x, mpBackgroundImage->GetSizeFromImage().y);
        }
        //--------------------------------------------------------
        /// Layout Content
        ///
        /// Called when the view is able to retrieve an absolute
        /// value.
        //--------------------------------------------------------
		void HighlightButton::LayoutContent()
		{
            SetSizeFromImage();
            GUIView::LayoutContent();
		}
        
		//--------------------------------------------------------
		/// Get Size From Image
		///
		/// @return Absolute size of the image
		//--------------------------------------------------------
		Core::Vector2 HighlightButton::GetSizeFromImage() const
		{
			return GetBackgroundImageView()->GetSizeFromImage();
		}
        //--------------------------------------------------------
        /// Set Width Maintaining Aspect
        ///
        /// Change the width of the image and resize the height
        /// to maintain the aspect ratio
        ///
        /// @param Unified width
        //--------------------------------------------------------
        void HighlightButton::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
        {
            WidthMaintain = true;
            
            SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
            
            Core::Vector2 vCurrentSize = GetSizeFromImage();
            f32 fAspectRatio = vCurrentSize.y / vCurrentSize.x;
            
            vCurrentSize = GetAbsoluteSize();
            f32 fAbsHeight = (fAspectRatio * vCurrentSize.x) / fScaleY;
            SetSize(infRelWidth, 0.0f, infAbsWidth, fAbsHeight);
        }
        //--------------------------------------------------------
        /// Set Height Maintaining Aspect
        ///
        /// Change the height of the image and resize the width
        /// to maintain the aspect ratio
        ///
        /// @param Unified height
        //--------------------------------------------------------
        void HighlightButton::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
        {
            HeightMaintain = true;
            
            SetSize(0.0f, infRelHeight, 0.0f, infAbsHeight);
			
			f32 fScaleX = GetAbsoluteScale().x;
			if(fScaleX == 0.0f)
				return;
            
            Core::Vector2 vCurrentSize = GetSizeFromImage();
            f32 fAspectRatio = vCurrentSize.x / vCurrentSize.y;
            
            vCurrentSize = GetAbsoluteSize();
            f32 fAbsWidth = (fAspectRatio * vCurrentSize.y) / fScaleX;
            SetSize(0.0f, infRelHeight, fAbsWidth, infAbsHeight);
        }
        //--------------------------------------------------------
        /// Enable Height Maintaining Aspect
        ///
        /// Enables auto scaling of the height to maintain the aspect ratio
        ///
        /// @param boolean to disable or enable
        //--------------------------------------------------------
        void HighlightButton::EnableHeightMaintainingAspect(bool inbEnabled)
        {
			HeightMaintain = inbEnabled;
        }
        //--------------------------------------------------------
        /// Enable Width Maintaining Aspect
        ///
        /// Enables auto scaling of the height to maintain the aspect ratio
        ///
        /// @param boolean to disable or enable
        //--------------------------------------------------------
        void HighlightButton::EnableWidthMaintainingAspect(bool inbEnabled)
        {
			WidthMaintain = inbEnabled;
        }
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the width to maintain the aspect ratio
		//--------------------------------------------------------
		bool HighlightButton::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
		}
		//--------------------------------------------------------
		/// Is Height Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the height to maintain the aspect ratio
		//--------------------------------------------------------
		bool HighlightButton::IsHeightMaintainingAspectEnabled() const
		{
			return HeightMaintain;
		}
        //--------------------------------------------------------
        /// Set Fill Maintaining Aspect
        ///
        /// Based on the size of the image and the target size
        /// resize the image view to be the minimum of the target
        /// size while maintaining the aspect ratio
        ///
        /// @param Unified size
        //--------------------------------------------------------
        void HighlightButton::SetFillMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight)
        {
            mbFillMaintain = true;
            mvFillMaintainTarget.vRelative.x = infRelWidth;
            mvFillMaintainTarget.vRelative.y = infRelHeight;
            mvFillMaintainTarget.vAbsolute.x = infAbsWidth;
            mvFillMaintainTarget.vAbsolute.y = infAbsHeight;
            
            if(GetParentViewPtr() == nullptr)
                return;
            
            Core::Vector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::Vector2 vImageSize(GetSizeFromImage());
            
            f32 fParentRatio = (vParentSize.x * infRelWidth) / (vParentSize.y * infRelHeight);
            f32 fImageRatio = vImageSize.x / vImageSize.y;
            
            if(fImageRatio <= fParentRatio)
            {
                SetWidthMaintainingAspect(infRelWidth, infAbsWidth);
            }
            else
            {
                SetHeightMaintainingAspect(infRelHeight, infAbsHeight);
            }
        }
        //--------------------------------------------------------
        /// Set Fit Maintaining Aspect
        ///
        /// Based on the size of the image and the target size
        /// resize the image view to be the maximum of the target
        /// size while maintaining the aspect ratio
        ///
        /// @param Unified size
        //--------------------------------------------------------
        void HighlightButton::SetFitMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight)
        {
            mbFitMaintain = true;
            mvFitMaintainTarget.vRelative.x = infRelWidth;
            mvFitMaintainTarget.vRelative.y = infRelHeight;
            mvFitMaintainTarget.vAbsolute.x = infAbsWidth;
            mvFitMaintainTarget.vAbsolute.y = infAbsHeight;
            
            if(GetParentViewPtr() == nullptr)
                return;
            
            Core::Vector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::Vector2 vImageSize(GetSizeFromImage());
            
            f32 fParentRatio = (vParentSize.x * infRelWidth) / (vParentSize.y * infRelHeight);
            f32 fImageRatio = vImageSize.x / vImageSize.y;
            
            if(fImageRatio > fParentRatio)
            {
                SetWidthMaintainingAspect(infRelWidth, infAbsWidth);
            }
            else
            {
                SetHeightMaintainingAspect(infRelHeight, infAbsHeight);
            }
        }
        //----------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------
        HighlightButton::~HighlightButton()
        {

        }
    }
}
