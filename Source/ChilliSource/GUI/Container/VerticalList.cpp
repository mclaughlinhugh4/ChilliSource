//
//  VerticalList.cpp
//  Chilli Source
//  Created by Scott Downie on 01/07/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#include <ChilliSource/GUI/Container/VerticalList.h>

#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace GUI
    {
        DEFINE_META_CLASS(VerticalList);
        
        DEFINE_PROPERTY(AbsoluteSpacing);
        DEFINE_PROPERTY(RelativeSpacing);
        DEFINE_PROPERTY(Justifiction);
        DEFINE_PROPERTY(HorizontalJustifiction);
        
        //---------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //---------------------------------------------------------
        VerticalList::VerticalList()
        : VerticalJustifiction(ItemVerticalJustification::k_top), AbsoluteSpacing(0.0f), RelativeSpacing(0.0f), meHorizontalAlignmentAnchor(Rendering::AlignmentAnchor::k_topCentre)
        {
            
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// @param Dictionary of params
        //---------------------------------------------------------
        VerticalList::VerticalList(const Core::ParamDictionary& insParams)
        : GUIView(insParams), VerticalJustifiction(ItemVerticalJustification::k_top), AbsoluteSpacing(0.0f), meHorizontalAlignmentAnchor(Rendering::AlignmentAnchor::k_topCentre), RelativeSpacing(0.0f)
        {
            std::string strValue;
            
            //---Set Justification
            if(insParams.TryGetValue("VerticalJustification", strValue))
            {
                if(strValue == "Top")
                {
                    SetVerticalJustification(ItemVerticalJustification::k_top);
                }
                else if(strValue == "Middle")
                {
                    SetVerticalJustification(ItemVerticalJustification::k_middle);
                }
                else if(strValue == "Bottom")
                {
                    SetVerticalJustification(ItemVerticalJustification::k_bottom);
                }
            }
            
            //---Set Justification
            if(insParams.TryGetValue("HorizontalJustification", strValue))
            {
                if(strValue == "Centre")
                {
                    SetHorizontalJustification(ItemHorizontalJustification::k_centre);
                }
                else if(strValue == "Left")
                {
                    SetHorizontalJustification(ItemHorizontalJustification::k_left);
                }
                else if(strValue == "Right")
                {
                    SetHorizontalJustification(ItemHorizontalJustification::k_right);
                }
            }
            
            //---Set Spacing
            if(insParams.TryGetValue("AbsoluteSpacing", strValue))
            {
                AbsoluteSpacing = Core::ParseF32(strValue);
            }
            
            if(insParams.TryGetValue("RelativeSpacing", strValue))
            {
                RelativeSpacing = Core::ParseF32(strValue);
            }
        }
        //---------------------------------------------------------
        /// Set Relative Spacing
        ///
        /// @param Space between items in the list relative to
        /// the size of the container
        //---------------------------------------------------------
        void VerticalList::SetRelativeSpacing(f32 infSpacing)
        {
            RelativeSpacing = infSpacing;
        }
        //---------------------------------------------------------
        /// Set Absolute Spacing
        ///
        /// @param Space between items in the list
        //---------------------------------------------------------
        void VerticalList::SetAbsoluteSpacing(f32 infSpacing)
        {
            AbsoluteSpacing = infSpacing;
        }
        //---------------------------------------------------------
        /// Get Absolute Spacing
        ///
        /// @return Space between items in the list
        //---------------------------------------------------------
        f32 VerticalList::GetAbsoluteSpacing() const
        {
            return AbsoluteSpacing;
        }
        //---------------------------------------------------------
        /// Get Relative Spacing
        ///
        /// @return Space between items in the list relative to the
        /// size of the container
        //---------------------------------------------------------
        f32 VerticalList::GetRelativeSpacing() const
        {
            return RelativeSpacing;
        }
        //---------------------------------------------------------
        /// Set Vertical Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void VerticalList::SetVerticalJustification(ItemVerticalJustification ineJustification)
        {
            VerticalJustifiction = ineJustification;
        }
        //---------------------------------------------------------
        /// Get Vertical Justification
        ///
        /// @return Justification which will align the items within the list
        //---------------------------------------------------------
        VerticalList::ItemVerticalJustification VerticalList::GetVerticalJustification() const
        {
            return VerticalJustifiction;
        }
        //---------------------------------------------------------
        /// Set Horizontal Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void VerticalList::SetHorizontalJustification(ItemHorizontalJustification ineJustification)
        {
            HorizontalJustifiction = ineJustification;
            
            switch(HorizontalJustifiction)
            {
                case ItemHorizontalJustification::k_left:
                    meHorizontalAlignmentAnchor = Rendering::AlignmentAnchor::k_topLeft;
                    break;
                case ItemHorizontalJustification::k_centre:
                    meHorizontalAlignmentAnchor = Rendering::AlignmentAnchor::k_topCentre;
                    break;
                case ItemHorizontalJustification::k_right:
                    meHorizontalAlignmentAnchor = Rendering::AlignmentAnchor::k_topRight;
                    break;
            }
            
        }
        //---------------------------------------------------------
        /// Get Horizontal Justification
        ///
        /// @return Horizontal Justification which will align the items within the list
        //---------------------------------------------------------
        VerticalList::ItemHorizontalJustification VerticalList::GetHorizontalJustification() const
        {
            return HorizontalJustifiction;
        }
        //-----------------------------------------------------
        /// Add Subview
        ///
        /// Add a view to the hierarchy
        ///
        /// @param GUIView shared pointer
        //-----------------------------------------------------
        void VerticalList::AddSubview(const GUIViewSPtr& inpSubview)
        {
            GUIView::AddSubview(inpSubview);
            maListSubviews.push_back(inpSubview);
        }
        //-----------------------------------------------------
        /// Remove Subview (Internal)
        ///
        /// Remove a view from our hierarchy
        ///
        /// @param GUIView pointer
        //-----------------------------------------------------
        void VerticalList::RemoveSubview(GUIView* inpSubview)
        {
            GUIView::RemoveSubview(inpSubview);
            for(GUIView::Subviews::iterator it = maListSubviews.begin(); it != maListSubviews.end(); ++it)
            {
                if((*it).get() == inpSubview)
                {
                    maListSubviews.erase(it);
                    return;
                }
            }
            
        }
		//-----------------------------------------------------
		/// Remove Subview (Internal)
		///
		/// Remove a view from our hierarchy
		///
		/// @param GUIView pointer
		//-----------------------------------------------------			
		void VerticalList::RemoveAllSubviews()
		{
			GUIView::RemoveAllSubviews();
            maListSubviews.clear();
		}
        //-------------------------------------------------------
        /// Draw
        ///
        /// Draw all our subviews in neat rows and columns. Each
        /// cell is based on the size of the largest content 
        ///
        /// @param Canvas renderer pointer
        //-------------------------------------------------------
        void VerticalList::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            //This is a container view and has no renderables
            //We will need to render our subviews though
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
				
                //Arrange everything!
                LayoutContent();
				
                GUIView::Draw(inpCanvas);
            }
        }
        //-------------------------------------------------------
        /// Layout Content
        ///
        /// Loop through the subviews and position them correctly
        /// in the list 
        //-------------------------------------------------------
        void VerticalList::LayoutContent()
        {
            f32 fContainerHeight = GetAbsoluteSize().y;
            f32 fSpacing = (fContainerHeight * RelativeSpacing) + AbsoluteSpacing;
            
            f32 fY = 0.0f;
            for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
            {
                (*pView)->EnableAlignmentToParent(true);
                (*pView)->SetAlignmentToParent(meHorizontalAlignmentAnchor);
                (*pView)->SetLocalAlignment(meHorizontalAlignmentAnchor);
                
                (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, fY);
                fY -= ((*pView)->GetAbsoluteSize().y + fSpacing);
            }
            
            fY -= fSpacing;
            
            switch(VerticalJustifiction)
            {
                case ItemVerticalJustification::k_top:
                default:
                    break;
                case ItemVerticalJustification::k_bottom:
                {
                    f32 fOffset = -(GetAbsoluteSize().y + fY);
                    for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
                    {
                        (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, fOffset + (*pView)->GetOffsetFromParentAlignment().vAbsolute.y);
                    }
                    break;
                }
                case ItemVerticalJustification::k_middle:
                {
                    f32 fHalfOffset = (GetAbsoluteSize().y + fY) * -0.5f;
                    for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
                    {
                        (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, fHalfOffset + (*pView)->GetOffsetFromParentAlignment().vAbsolute.y);
                    }
                    break;
                }
            }
        }
    }
}

