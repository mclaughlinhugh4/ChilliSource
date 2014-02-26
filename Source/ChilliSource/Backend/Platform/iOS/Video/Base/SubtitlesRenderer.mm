//
//  SubtitlesRenderer.mm
//  moFlow
//
//  Created by Ian Copland on 22/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Video/Base/SubtitlesRenderer.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/String/UTF8String.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Video/Base/VideoPlayerActivity.h>
#include <ChilliSource/Video/Base/Subtitles.h>

#import <QuartzCore/QuartzCore.h>

@implementation CSubtitlesRenderer
//--------------------------------------------------------
/// init With Video Player
//--------------------------------------------------------
-(id) InitWithVideoPlayer:(ChilliSource::iOS::CVideoPlayerActivity*)inpVideoPlayer View:(UIView*)inpView StorageLocation:(ChilliSource::Core::StorageLocation)ineStorageLocation Filename:(std::string)instrFilename
{
    if(!(self = [super init]))
	{
		return nil;
	}
    
    mpVideoPlayer = inpVideoPlayer;
    mpBaseView = inpView;
    
    //create the display link
    mpDisplayLink = [CADisplayLink displayLinkWithTarget: self selector: @selector(OnUpdate)];
    [mpDisplayLink addToRunLoop: [NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
    
    //load the subtitles
    mpSubtitles = LOAD_RESOURCE(ChilliSource::Video::CSubtitles, ineStorageLocation, instrFilename);
    
    return self;
}
//--------------------------------------------------------
/// On Update
//--------------------------------------------------------
-(void) OnUpdate
{
    //get the current subtitles
    f32 fPosition = mpVideoPlayer->GetTime();
    TimeIntervalMs currentTimeMS = (TimeIntervalMs)(fPosition * 1000.0f);
    
    if (mCurrentTimeMS != currentTimeMS)
    {
        mCurrentTimeMS = currentTimeMS;
        std::vector<ChilliSource::Video::CSubtitles::SubtitlePtr> pSubtitleArray = mpSubtitles->GetSubtitlesAtTime(mCurrentTimeMS);

        //add any new subtitles
        for (std::vector<ChilliSource::Video::CSubtitles::SubtitlePtr>::iterator it = pSubtitleArray.begin(); it != pSubtitleArray.end(); ++it)
        {
            
            std::unordered_map<ChilliSource::Video::CSubtitles::SubtitlePtr, UITextView*>::iterator mapEntry = maTextViewMap.find(*it);
            if (mapEntry == maTextViewMap.end())
            {
                [self AddTextView:*it];
            }
        }

        //update the current text views
        for (std::unordered_map<ChilliSource::Video::CSubtitles::SubtitlePtr, UITextView*>::iterator it = maTextViewMap.begin(); it != maTextViewMap.end(); ++it)
        {
            [self UpdateTextView:it->second Subtitle:it->first Time:mCurrentTimeMS];
        }

        //removes any text views that are no longer needed.
        for (std::vector<ChilliSource::Video::CSubtitles::SubtitlePtr>::iterator it = maSubtitlesToRemove.begin(); it != maSubtitlesToRemove.end(); ++it)
        {
            std::unordered_map<ChilliSource::Video::CSubtitles::SubtitlePtr, UITextView*>::iterator mapEntry = maTextViewMap.find(*it);
            if (mapEntry != maTextViewMap.end())
            {
                [mapEntry->second removeFromSuperview];
                [mapEntry->second release];
                maTextViewMap.erase(mapEntry);
            }
        }
        maSubtitlesToRemove.clear();
    }
}
//--------------------------------------------------------
/// Add Text View
//--------------------------------------------------------
-(void) AddTextView:(const ChilliSource::Video::CSubtitles::SubtitlePtr&)inpSubtitle
{
    //get the style
    ChilliSource::Video::CSubtitles::StylePtr pStyle = mpSubtitles->GetStyleWithName(inpSubtitle->strStyleName);
    if (pStyle == nullptr)
    {
        CS_ERROR_LOG("Cannot find style '" + inpSubtitle->strStyleName + "' in subtitles.");
        return;
    }
    
    //create the new text view
    UITextView* pNewTextView = [[UITextView alloc] initWithFrame: [self CalculateTextBoxRect:pStyle->Bounds]];
    [mpBaseView addSubview:pNewTextView];
    [mpBaseView bringSubviewToFront:pNewTextView];
    pNewTextView.backgroundColor = [UIColor clearColor];
    
    //setup the text.
    [pNewTextView setText: ChilliSource::Core::CStringUtils::UTF8StringToNSString(ChilliSource::Core::CLocalisedText::GetText(inpSubtitle->strTextID))];
    [pNewTextView setFont:[UIFont fontWithName: ChilliSource::Core::CStringUtils::StringToNSString(pStyle->strFontName) size: pStyle->udwFontSize]];
    [pNewTextView setTextColor:[UIColor colorWithRed:pStyle->Colour.r green:pStyle->Colour.g blue:pStyle->Colour.b alpha:0.0f]];
    [pNewTextView setEditable:NO];
    [pNewTextView setUserInteractionEnabled:NO];
    
    [self SetAlignment: pNewTextView WithAnchor: pStyle->eAlignment];
    maTextViewMap.insert(std::pair<ChilliSource::Video::CSubtitles::SubtitlePtr, UITextView*>(inpSubtitle, pNewTextView));
}
//--------------------------------------------------------
/// Update Text View
//--------------------------------------------------------
-(void) UpdateTextView:(UITextView*)inpTextView Subtitle:(const ChilliSource::Video::CSubtitles::SubtitlePtr&)inpSubtitle Time:(TimeIntervalMs)inTimeMS
{
    ChilliSource::Video::CSubtitles::StylePtr pStyle = mpSubtitles->GetStyleWithName(inpSubtitle->strStyleName);
    
    f32 fFade = 0.0f;
    s64 lwRelativeTime = ((s64)inTimeMS) - ((s64)inpSubtitle->StartTimeMS);
    s64 lwDisplayTime = ((s64)inpSubtitle->EndTimeMS) - ((s64)inpSubtitle->StartTimeMS);
    
    //subtitle should not be displayed yet so remove
    if (lwRelativeTime < 0)
    {
        [self RemoveTextView:inpSubtitle];
    }
    
    //fading in
    else if (lwRelativeTime < pStyle->FadeTimeMS)
    {
        fFade = ((f32)lwRelativeTime) / ((f32)pStyle->FadeTimeMS);
    }
    
    //active
    else if (lwRelativeTime < lwDisplayTime - pStyle->FadeTimeMS)
    {
        fFade = 1.0f;
    }
    
    //fading out
    else if (lwRelativeTime < lwDisplayTime)
    {
        fFade = 1.0f - (((f32)lwRelativeTime - (lwDisplayTime - pStyle->FadeTimeMS)) / ((f32)pStyle->FadeTimeMS));
    }
    
    //should no longer be displayed so remove
    else if (lwRelativeTime >= lwDisplayTime)
    {
        [self RemoveTextView:inpSubtitle];
    }
    
    inpTextView.textColor = [UIColor colorWithRed:pStyle->Colour.r green:pStyle->Colour.g blue:pStyle->Colour.b alpha:fFade * pStyle->Colour.a];
}
//--------------------------------------------------------
/// Remove Text View
//--------------------------------------------------------
-(void) RemoveTextView:(const ChilliSource::Video::CSubtitles::SubtitlePtr&)inpSubtitle
{
    maSubtitlesToRemove.push_back(inpSubtitle);
}
//--------------------------------------------------------
/// Set Alignment
//--------------------------------------------------------
-(void) SetAlignment:(UITextView*)inpView WithAnchor:(ChilliSource::Core::AlignmentAnchor)ineAnchor
{
    inpView.textAlignment = [self TextAlignmentFromAnchor: ineAnchor];
    
    switch (ineAnchor)
    {
        case ChilliSource::Core::AlignmentAnchor::k_topLeft:
        case ChilliSource::Core::AlignmentAnchor::k_topCentre:
        case ChilliSource::Core::AlignmentAnchor::k_topRight:
        {
            inpView.contentOffset = (CGPoint){.x = 0.0f, .y = 0.0f};
            break;
        }
        case ChilliSource::Core::AlignmentAnchor::k_middleLeft:
        case ChilliSource::Core::AlignmentAnchor::k_middleCentre:
        case ChilliSource::Core::AlignmentAnchor::k_middleRight:
        {
            f32 fBoxSize = [inpView bounds].size.height;
            f32 fContentSize = [inpView contentSize].height;
            f32 fOffset = (fBoxSize - fContentSize);
            if (fOffset < 0.0f)
            {
                fOffset = 0.0f;
            }
            inpView.contentOffset = (CGPoint){.x = 0.0f, .y = -fOffset / 2.0f};
            break;
            break;
        }
        case ChilliSource::Core::AlignmentAnchor::k_bottomLeft:
        case ChilliSource::Core::AlignmentAnchor::k_bottomCentre:
        case ChilliSource::Core::AlignmentAnchor::k_bottomRight:
        {
            f32 fBoxSize = [inpView bounds].size.height;
            f32 fContentSize = [inpView contentSize].height;
            f32 fOffset = (fBoxSize - fContentSize);
            if (fOffset < 0.0f)
            {
                fOffset = 0.0f;
            }
            inpView.contentOffset = (CGPoint){.x = 0.0f, .y = -fOffset};
            break;
        }
        default:
             CS_WARNING_LOG("Could not set vertical alignment.");
            break;
    }
}
//--------------------------------------------------------
/// Text Alignment From Anchor
//--------------------------------------------------------
-(NSTextAlignment) TextAlignmentFromAnchor:(ChilliSource::Core::AlignmentAnchor)ineAnchor
{
    switch (ineAnchor)
    {
        case ChilliSource::Core::AlignmentAnchor::k_topLeft:
        case ChilliSource::Core::AlignmentAnchor::k_middleLeft:
        case ChilliSource::Core::AlignmentAnchor::k_bottomLeft:
            return NSTextAlignmentLeft;
        case ChilliSource::Core::AlignmentAnchor::k_topCentre:
        case ChilliSource::Core::AlignmentAnchor::k_middleCentre:
        case ChilliSource::Core::AlignmentAnchor::k_bottomCentre:
            return NSTextAlignmentCenter;
        case ChilliSource::Core::AlignmentAnchor::k_topRight:
        case ChilliSource::Core::AlignmentAnchor::k_middleRight:
        case ChilliSource::Core::AlignmentAnchor::k_bottomRight:
            return NSTextAlignmentRight;
        default:
            CS_WARNING_LOG("Could not convert alignment anchor to NSTextAlignment.");
            return NSTextAlignmentLeft;
    }
}
//--------------------------------------------------------
/// Calculate Text Box Rect
///
/// @return the rectangle in which the text box should
/// appear on screen.
//--------------------------------------------------------
-(CGRect) CalculateTextBoxRect:(const ChilliSource::Core::Rectangle&)inRelativeBounds
{
    ChilliSource::Core::CVector2 vScreenDimensions(ChilliSource::Core::CScreen::GetOrientedWidth() * ChilliSource::Core::CScreen::GetInverseDensity(), ChilliSource::Core::CScreen::GetOrientedHeight() * ChilliSource::Core::CScreen::GetInverseDensity());
    ChilliSource::Core::CVector2 vVideoDimensions = mpVideoPlayer->GetVideoDimensions();
    float fScreenAspectRatio = vScreenDimensions.x / vScreenDimensions.y;
    float fVideoAspectRatio = vVideoDimensions.x / vVideoDimensions.y;
    
    ChilliSource::Core::CVector2 vVideoViewDimensions;
    if (fScreenAspectRatio < fVideoAspectRatio)
    {
        vVideoViewDimensions.x = vScreenDimensions.x;
        vVideoViewDimensions.y = vScreenDimensions.x * (vVideoDimensions.y / vVideoDimensions.x);
    }
    else
    {
        vVideoViewDimensions.x = vScreenDimensions.y * (vVideoDimensions.x / vVideoDimensions.y);
        vVideoViewDimensions.y = vScreenDimensions.y;
    }
    
    ChilliSource::Core::CVector2 vVideoViewTopLeft = (vScreenDimensions - vVideoViewDimensions) * 0.5f;
    return CGRectMake(vVideoViewTopLeft.x + inRelativeBounds.Left() * vVideoViewDimensions.x, vVideoViewTopLeft.y + inRelativeBounds.Top() * vVideoViewDimensions.y, inRelativeBounds.vSize.x * vVideoViewDimensions.x, inRelativeBounds.vSize.y * vVideoViewDimensions.y);
    
}

//--------------------------------------------------------
/// CleanUp
///
/// Cleans up any possible retainers, should be called before releasing
//--------------------------------------------------------
-(void) CleanUp
{
    [mpDisplayLink invalidate];
}

//--------------------------------------------------------
/// Dealloc
//--------------------------------------------------------
-(void) dealloc
{
    [super dealloc];
}
@end