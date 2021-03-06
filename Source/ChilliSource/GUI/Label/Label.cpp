//
//  Label.cpp
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

#include <ChilliSource/GUI/Label/Label.h>

#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(Label)

		//---Properties
		DEFINE_PROPERTY(Text);
		DEFINE_PROPERTY(LocalisedTextID);
		DEFINE_PROPERTY(MaxNumLines);
		DEFINE_PROPERTY(TextScale);
		DEFINE_PROPERTY(LineSpacing);
		DEFINE_PROPERTY(HorizontalJustification);
		DEFINE_PROPERTY(VerticalJustification);
		DEFINE_PROPERTY(Font);
		DEFINE_PROPERTY(TextColour);
		DEFINE_PROPERTY(Background);
		DEFINE_PROPERTY(Autosizing);
		DEFINE_PROPERTY(UnifiedMaxSize);
		DEFINE_PROPERTY(UnifiedMinSize);
        DEFINE_PROPERTY(TextOutlined);
        DEFINE_PROPERTY(TextOutlineColour);
        DEFINE_PROPERTY(FlipVertical);
        
        f32 Label::mfGlobalTextScale = 1.0f;
        
        //-------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //-------------------------------------------------------
        Label::Label() : MaxNumLines(0), TextScale(1.0f), LineSpacing(1.0f), HorizontalJustification(TextJustification::k_left),
		VerticalJustification(TextJustification::k_centre), Background(true), Autosizing(false), FlipVertical(false), mbLastDrawWasClipped(false), mbLastDrawHadInvalidCharacter(false)
        {
            SetColour(Core::Colour(0.18f, 0.3f, 0.4f, 0.6f));

            mpWhiteTex = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Texture>(Core::StorageLocation::k_chilliSource, "Textures/Blank.csimage");
            
            ConsumesTouches = false;
            
			UnifiedSize = Core::UnifiedVector2(0.2f, 0.05f, 0.0f, 0.0f);
            UnifiedMaxSize = GetSize();
            UnifiedMinSize = GetSize();
        }
        //-------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //-------------------------------------------------------
        Label::Label(const Core::ParamDictionary& insParams) 
        : GUIView(insParams), MaxNumLines(0), TextScale(1.0f), LineSpacing(1.0f), HorizontalJustification(TextJustification::k_left),
		VerticalJustification(TextJustification::k_centre), Background(true), Autosizing(false), FlipVertical(false)
        {
            std::string strValue;
            
            UnifiedMaxSize = GetSize();
            UnifiedMinSize = GetSize();
            
            Core::StorageLocation localisedTextLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("LocalisedTextLocation", strValue))
            {
                localisedTextLocation = Core::ParseStorageLocation(strValue);
            }
            //---Localised Text
            if(insParams.TryGetValue("LocalisedText", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                m_localisedText = resourcePool->LoadResource<Core::LocalisedText>(localisedTextLocation, strValue);
            }
            //---Localised Text ID
            if(insParams.TryGetValue("LocalisedTextID", strValue))
            {
                SetLocalisedTextID(strValue);
            }
            //---Text
            if(insParams.TryGetValue("Text", strValue))
            {
                SetText(strValue);
            }
            //---Number of lines
            if(insParams.TryGetValue("NumLines", strValue))
            {
                MaxNumLines = Core::ParseU32(strValue);
            }
            //---Text Scale
            if(insParams.TryGetValue("TextScale", strValue))
            {
                TextScale = Core::ParseF32(strValue);
            }
            //---Unified maximum size
            if(insParams.TryGetValue("UnifiedMaxSize", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::Vector4 vRawSize = Core::ParseVector4(strValue);
                SetMaximumSize(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w);
            }
            //---Unified maximum size
            if(insParams.TryGetValue("UnifiedMinSize", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::Vector4 vRawSize = Core::ParseVector4(strValue);
                SetMinimumSize(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w);
            }
            //---Line spacing
            if(insParams.TryGetValue("LineSpacing", strValue))
            {
                LineSpacing = Core::ParseF32(strValue);
            }
            //---Text justification
            if(insParams.TryGetValue("HorizontalJustification", strValue))
            {
                HorizontalJustification = JustificationFromString(strValue);
            }
			if(insParams.TryGetValue("VerticalJustification", strValue))
            {
                VerticalJustification = JustificationFromString(strValue);
            }
            //---Font
            Core::StorageLocation eFontLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("FontLocation", strValue))
            {
                eFontLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("Font", strValue))
            {
                Font = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Font>(eFontLocation, strValue);
            }
            //---Text Colour
            if(insParams.TryGetValue("TextColour", strValue))
            {
                TextColour = Core::ParseColour(strValue);
            }
            //---Enable Background Colour
            if(insParams.TryGetValue("EnableBackground", strValue))
            {
                Background = Core::ParseBool(strValue);
            }
            //---Enable Autosizing
            if(insParams.TryGetValue("EnableAutosizing", strValue))
            {
                Autosizing = Core::ParseBool(strValue);
            }
            //---UV Flipped Y
            if(insParams.TryGetValue("FlipVertical", strValue))
            {
                FlipVertical = true;
            }
            
            mpWhiteTex = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Texture>(Core::StorageLocation::k_chilliSource, "Textures/Blank.csimage");;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Label::SetText(const std::string& instrText)
        {
            Text = instrText;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const std::string& Label::GetText() const
        {
            return Text;
        }
        //-------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Localised text resource used in conjunction with
        /// id
        //-------------------------------------------------------
        void Label::SetLocalisedText(const Core::LocalisedTextCSPtr& in_text)
        {
            m_localisedText = in_text;
        }
        //-------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Localised text resource used in conjunction with
        /// id
        //-------------------------------------------------------
        const Core::LocalisedTextCSPtr& Label::GetLocalisedText() const
        {
            return m_localisedText;
        }
		//-------------------------------------------------------
		/// Set Text ID
		///
		/// @param Text string representing lookup ID
		//-------------------------------------------------------
		void Label::SetLocalisedTextID(const std::string& instrTextId)
		{
            CS_ASSERT(m_localisedText != nullptr, "Label must have a localised text resource set before setting the text Id");
            
			LocalisedTextID = instrTextId;

			if(!LocalisedTextID.empty())
			{
				SetText(m_localisedText->GetText(LocalisedTextID));
			}
			else
			{
				SetText("");
			}
		}
		//-------------------------------------------------------
		/// Get Text ID
		///
		/// @return Text string representing lookup ID
		//-------------------------------------------------------
		const std::string& Label::GetLocalisedTextID() const
		{
			return LocalisedTextID;
		}
        //-------------------------------------------------------
        /// Set Font
        ///
        /// @param Font used to display text
        //-------------------------------------------------------
        void Label::SetFont(const Rendering::FontCSPtr& inpFont)
        {
            Font = inpFont;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Get Font
        ///
        /// @return Font used to display text
        //-------------------------------------------------------
        const Rendering::FontCSPtr& Label::GetFont() const
        {
            return Font;
        }
        //-------------------------------------------------------
        /// Set Maximum Size
        ///
        /// The unified size to which this label will expand 
        /// after which the text will clip
        ///
        /// @param Unified vector
        //-------------------------------------------------------
        void Label::SetMaximumSize(const Core::UnifiedVector2& invSize)
        {
            UnifiedMaxSize = invSize;

			mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Set Minimum Size
        ///
        /// The unified size to which this label will shrink 
        ///
        /// @param Unified vector
        //-------------------------------------------------------
        void Label::SetMinimumSize(const Core::UnifiedVector2& invSize)
        {
            UnifiedMinSize = invSize;

			mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Set Maximum Size
        ///
        /// The unified size to which this label will expand 
        /// after which the text will clip
        ///
        /// @param Unified vector RX, RY, AX & AY
        //-------------------------------------------------------
        void Label::SetMaximumSize(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
        {
            UnifiedMaxSize.vRelative.x = infRx;
            UnifiedMaxSize.vRelative.y = infRy;
            UnifiedMaxSize.vAbsolute.x = infAx;
            UnifiedMaxSize.vAbsolute.y = infAy;

			mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Set Minimum Size
        ///
        /// The unified size to which this label will shrink 
        ///
        /// @param Unified vector RX, RY, AX & AY
        //-------------------------------------------------------
        void Label::SetMinimumSize(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
        {
            UnifiedMinSize.vRelative.x = infRx;
            UnifiedMinSize.vRelative.y = infRy;
            UnifiedMinSize.vAbsolute.x = infAx;
            UnifiedMinSize.vAbsolute.y = infAy;

			mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Minimum Size
		///
		/// The unified size to which this label will shrink 
		///
		/// @return Unified vector
		//-------------------------------------------------------
		const Core::UnifiedVector2& Label::GetMinimumSize() const
		{
			return UnifiedMinSize;
		}
		//-------------------------------------------------------
		/// Get Maximum Size
		///
		/// The unified size to which this label will grow 
		///
		/// @return Unified vector
		//-------------------------------------------------------
		const Core::UnifiedVector2& Label::GetMaximumSize() const
		{
			return UnifiedMaxSize;
		}
        //-------------------------------------------------------
        /// Enable Autosizing
        ///
        /// @param Whether the label should grow and shrink
        /// based on the text contents
        //-------------------------------------------------------
        void Label::EnableAutosizing(bool inbEnabled)
        {
            Autosizing = inbEnabled;

			mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Enable Autosizing
		///
		/// @return Whether the label should grow and shrink
		/// based on the text contents
		//-------------------------------------------------------
		bool Label::IsAutosizingEnabled() const
		{
			return Autosizing;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Label::OnScreenResolutionChanged()
		{
			mCachedChars.clear();

			GUIView::OnScreenResolutionChanged();
		}
        //-------------------------------------------------------
        /// Set Number of Lines
        ///
        /// The maximum number of lines for which the text will
        /// wrap. If this is zero then the number of lines is
        /// not fixed
        ///
        /// The text will wrap based on the width of the label
        ///
        /// @param Num lines
        //-------------------------------------------------------
        void Label::SetNumberOfLines(u32 inudwNumLines)
        {
            MaxNumLines = inudwNumLines;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Set Number of Lines
        ///
        /// The maximum number of lines for which the text will
        /// wrap. If this is zero then the number of lines is
        /// not fixed
        ///
        /// The text will wrap based on the width of the label
        ///
        /// @return Num lines
        //-------------------------------------------------------
        u32 Label::GetNumberOfLines() const
        {
            return MaxNumLines;
        }
        //-------------------------------------------------------
        /// Set Line Spacing
        ///
        /// Set the space left vertically between lines 
        ///
        /// @param Absolute Scalar
        //-------------------------------------------------------
        void Label::SetLineSpacing(f32 infSpacing)
        {
            LineSpacing = infSpacing;
            
            mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Line Spacing
		///
		/// @return The space left vertically between lines 
		//-------------------------------------------------------
		f32 Label::GetLineSpacing() const
		{
			return LineSpacing;
		}
        //-------------------------------------------------------
        /// Set Text Scale
        ///
        /// @param Absolute value by which the text is scaled
        //-------------------------------------------------------
        void Label::SetTextScale(f32 infScale)
        {
            TextScale = infScale;
            
            mCachedChars.clear();
        }
		//-------------------------------------------------------
		/// Get Text Scale
		///
		/// @return Absolute Scalar
		//-------------------------------------------------------
		f32 Label::GetTextScale() const
		{
			return TextScale;
		}
        //-------------------------------------------------------
        /// Set Text Colour
        ///
        /// @param Colour
        //-------------------------------------------------------
        void Label::SetTextColour(const Core::Colour& inColour)
        {
            TextColour = inColour;
        }
		//-------------------------------------------------------
		/// Get Text Colour
		///
		/// @return Colour
		//-------------------------------------------------------
		const Core::Colour& Label::GetTextColour() const
		{
			return TextColour;
		}
		//-------------------------------------------------------
		/// Set Horizontal Justification
		///
		/// @param Horizontal justification
		//-------------------------------------------------------
		void Label::SetHorizontalJustification(TextJustification ineHorizontalJustification)
		{
			HorizontalJustification = ineHorizontalJustification;

			mCachedChars.clear();
		}
        //-------------------------------------------------------
        /// Set Vertical Justification
        ///
        /// @param Vertical justification
        //-------------------------------------------------------
        void Label::SetVerticalJustification(TextJustification ineVerticalJustification)
        {
			VerticalJustification = ineVerticalJustification;
            
            mCachedChars.clear();
        }
        //-------------------------------------------------------
        /// Get Horizontal Justification
        ///
        /// @return Horizontal justification
        //-------------------------------------------------------
        TextJustification Label::GetHorizontalJustification() const
        {
            return HorizontalJustification;
        }
        //-------------------------------------------------------
        /// Get Vertical Justification
        ///
        /// @return Vertical justification
        //-------------------------------------------------------
        TextJustification Label::GetVerticalJustification() const
        {
            return VerticalJustification;
        }
        //-------------------------------------------------------
        /// Enable Background
        ///
        /// @param Whether the label has a coloured background
        //-------------------------------------------------------
        void Label::EnableBackground(bool inbEnabled)
        {
            Background = inbEnabled;
        }
		//-------------------------------------------------------
		/// Is Background Enabled
		///
		/// @return Whether the label has a coloured background
		//-------------------------------------------------------
		bool Label::IsBackgroundEnabled() const
		{
			return Background;
		}
        //-------------------------------------------------------
        /// Justification From String
        ///
        /// Convert the string to a justification enum. This is
        /// used when creating labels from script files
        ///
        /// @param Text representation of justification
        /// @return Justification enum
        //-------------------------------------------------------
        TextJustification Label::JustificationFromString(const std::string& instrJustification)
        {
            if(instrJustification == "Left")
            {
                return TextJustification::k_left;
            }
            else if(instrJustification == "Right")
            {
                return TextJustification::k_right;
            }
            else if(instrJustification == "Centre")
            {
                return TextJustification::k_centre;
            }
			else if(instrJustification == "Top")
            {
                return TextJustification::k_top;
            }
            else if(instrJustification == "Bottom")
            {
                return TextJustification::k_bottom;
            }
            
            CS_LOG_FATAL("No justification matches type");
            
            return TextJustification::k_left;
        }
        //-------------------------------------------------------
        /// Draw
        ///
        /// Draw the text using the canvas then draw any
        /// subviews
        ///
        /// @param Canvas renderer
        //-------------------------------------------------------
        void Label::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            CS_ASSERT(Font, "Cannot render label without font.");
            
            if(Visible)
            {
                //Check if this is on screen
                Core::Vector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
                Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
                
                if(vTopRight.y < 0 || vBottomLeft.y > GetScreen()->GetResolution().y || vTopRight.x < 0 || vBottomLeft.x > GetScreen()->GetResolution().x)
                {
                    //Offscreen
                    return;
                }
                
                DoAutosizing(inpCanvas);
                
                //Calculate the size of the label box
                Core::Vector2 vAbsoluteLabelSize = GetAbsoluteSize();
                Core::Colour AbsCol = GetAbsoluteColour();

                //Check if we force clip our children 
                if(ClipSubviews)
                {
                    inpCanvas->PushClipBounds(vBottomLeft, vAbsoluteLabelSize);
                }
                
                //Draw ourself
                if(Background)
                {
                    inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), Core::Vector2::k_zero, mpWhiteTex, Rendering::UVs(0.0f, 0.0f, 1.0f, 1.0f), AbsCol, Rendering::AlignmentAnchor::k_middleCentre);
                }
                
                if(mCachedChars.empty())
                {
                    f32 fAssetTextScale = GetGlobalTextScale();
                    mCachedChars = inpCanvas->BuildText(Text, Font, TextScale * fAssetTextScale, LineSpacing, vAbsoluteLabelSize, MaxNumLines, HorizontalJustification, VerticalJustification).m_characters;
                }
                
                Core::Colour sDrawColour = TextColour * GetAbsoluteColour();
                inpCanvas->DrawText(mCachedChars, GetTransform(), sDrawColour, Font->GetTexture());
                
                //Draw the kids
                for(GUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
                {
                    (*it)->Draw(inpCanvas);
                }
                
                if(ClipSubviews)
                {
                    inpCanvas->PopClipBounds();
                }
            }
        }
        
        //-------------------------------------------------------
        /// Do Autosizing
        ///
        /// based on the text contents
        //-------------------------------------------------------
        void Label::DoAutosizing(Rendering::CanvasRenderer* inpCanvas)
        {
            if(Autosizing && mCachedChars.empty())
            {
                //Get the size of the text contents and resize the label
                //to match. Clamp the bounds between the minimum and
                //maximum size
                
                //Convert min and max to absolute co-ordinates
                Core::Vector2 vAbsMaxSize = mpParentView ? (mpParentView->GetAbsoluteSize() * UnifiedMaxSize.GetRelative()) + UnifiedMaxSize.GetAbsolute() : UnifiedMaxSize.GetAbsolute();
                Core::Vector2 vAbsMinSize = mpParentView ? (mpParentView->GetAbsoluteSize() * UnifiedMinSize.GetRelative()) + UnifiedMinSize.GetAbsolute() : UnifiedMinSize.GetAbsolute();
                
                //Build the text for the biggest possible bounds
                Rendering::CanvasRenderer::BuiltText builtText = inpCanvas->BuildText(Text, Font, TextScale * mfGlobalTextScale, LineSpacing, vAbsMaxSize, MaxNumLines, HorizontalJustification, VerticalJustification);
                
                f32 fNewRelWidth = UnifiedMaxSize.vRelative.x;
                f32 fNewRelHeight = 0.0f;
                f32 fNewAbsWidth = UnifiedMaxSize.vAbsolute.x;
                f32 fNewAbsHeight = 0.0f;
                f32 fTextWidth = builtText.m_width;
                f32 fTextHeight = builtText.m_height;

                //If the size of the text would actually fit inside the min bounds then clamp to that
                if(fTextWidth < vAbsMinSize.x)
                {
                    fNewRelWidth = UnifiedMinSize.vRelative.x;
                    fNewAbsWidth = UnifiedMinSize.vAbsolute.x;
                    
                    //Now that we have calculated the width of the label we
                    //can use that to work out the height
                    builtText = inpCanvas->BuildText(Text, Font, TextScale * mfGlobalTextScale, LineSpacing, vAbsMinSize, MaxNumLines, HorizontalJustification, VerticalJustification);
                    fTextHeight = builtText.m_height;
                }
                //If the size of text is smaller than the max bounds then clamp to that
                else if(fTextWidth < vAbsMaxSize.x)
                {
                    fNewRelWidth = 0.0f;
                    fNewAbsWidth = fTextWidth;
                    
                    //Now that we have calculated the width of the label we
                    //can use that to work out the height
                    builtText = inpCanvas->BuildText(Text, Font, TextScale * mfGlobalTextScale, LineSpacing, Core::Vector2(fNewAbsWidth, vAbsMaxSize.y), MaxNumLines, HorizontalJustification, VerticalJustification);
                    fTextHeight = builtText.m_height;
                }
                
                if(fTextHeight > vAbsMaxSize.y)
                {
                    fNewRelHeight = UnifiedMaxSize.vRelative.y;
                    fNewAbsHeight = UnifiedMaxSize.vAbsolute.y;
                }
                else if(fTextHeight < vAbsMinSize.y)
                {
                    fNewRelHeight = UnifiedMinSize.vRelative.y;
                    fNewAbsHeight = UnifiedMinSize.vAbsolute.y;
                }
                else
                {
                    fNewAbsHeight = fTextHeight;
                }
                
                mCachedChars = std::move(builtText.m_characters);
                
                //Resize the label
                SetSize(fNewRelWidth, fNewRelHeight, fNewAbsWidth, fNewAbsHeight);
            }
        }
        //-----------------------------------------------------------
		/// Set Flipped Vertical
		///
		/// Flip the label about it's local y-axis
		///
		/// @param Whether to flip or not
		//-----------------------------------------------------------
		void Label::EnableVerticalFlip(bool inbValue)
		{
			FlipVertical = inbValue;
		}
		//-----------------------------------------------------------
		/// Get Flipped Vertical
		///
		/// @param Whether label is flipped about it's local y-axis
		//-----------------------------------------------------------
		bool Label::IsVerticalFlipEnabled() const
		{
			return FlipVertical;
		}
        //-------------------------------------------------------
        /// Set Global Text Scale
        ///
        /// This text scale is applied to every label along
        /// with the per label text scale. This can be
        /// used to size text relative to screen size
        ///
        /// @param scale
        //-------------------------------------------------------
        void Label::SetGlobalTextScale(f32 infScale)
        {
            mfGlobalTextScale = infScale;
        }
        //-------------------------------------------------------
        /// Get Global Text Scale
        //-------------------------------------------------------
        f32 Label::GetGlobalTextScale()
        {
            return mfGlobalTextScale;
        }
        //-------------------------------------------------------
        /// Set Text Outline Colour
        ///
        /// @param Text outline colour for scaleable fonts
        //-------------------------------------------------------
        void Label::SetTextOutlineColour(const Core::Colour& inColour)
        {
            TextOutlineColour = inColour;
        }
        //-------------------------------------------------------
        /// Get Text Outline Colour
        ///
        /// @return Colour for outline of scaleable fonts
        //-------------------------------------------------------
        const Core::Colour& Label::GetTextOutlineColour() const
        {
            return TextOutlineColour;
        }
        //-----------------------------------------------------
        /// On Transform Changed
        ///
        /// Dirty the transform and force it to be recalculated
        //-----------------------------------------------------
        void Label::OnTransformChanged(u32 inudwInvalidFlags)
        {
            GUIView::OnTransformChanged(inudwInvalidFlags);
            if ((inudwInvalidFlags & (u32)TransformCache::k_absSize) == (u32)TransformCache::k_absSize)
            {
                mCachedChars.clear();
            }
        }
    }
}
