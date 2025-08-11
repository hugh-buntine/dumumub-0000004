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
        if (borderImage.isNull())
        {
            Logger::writeToLog("Failed to load image");
        }

    }

    ~CanvasBorder() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.drawImageAt(borderImage, 0, 0);


    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CanvasBorder)
    Image borderImage;
};
