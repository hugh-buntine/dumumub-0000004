/*
  ==============================================================================

    CanvasBorder.h
    Created: 24 Mar 2025 1:14:49pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CanvasBorder  : public juce::Component
{
public:
    CanvasBorder()
    {
        borderImage = ImageCache::getFromMemory(BinaryData::CANVAS_BORDER_png, BinaryData::CANVAS_BORDER_pngSize);
    }

    ~CanvasBorder() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.drawImage(borderImage, 0, 0, getWidth(), getHeight(),
                    0, 0, borderImage.getWidth(), borderImage.getHeight());


    }

    void resized() override
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CanvasBorder)
    Image borderImage;
};
