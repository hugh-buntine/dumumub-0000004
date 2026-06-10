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
/**
 * Decorative border component for the frequency display canvas.
 * 
 * Simple image component that draws a border around the frequency spectrum
 * visualization area. Provides visual frame for the interactive elements.
 */
class CanvasBorder : public juce::Component
{
public:
    /** Constructor - loads border image from binary data */
    CanvasBorder()
    {
        borderImage = ImageCache::getFromMemory(BinaryData::CANVAS_BORDER_png, 
                                              BinaryData::CANVAS_BORDER_pngSize);
    }

    /** Destructor */
    ~CanvasBorder() override
    {
    }

    /** Draws the border image scaled to component bounds */
    void paint(juce::Graphics& g) override
    {
        g.drawImage(borderImage, 0, 0, getWidth(), getHeight(),
                    0, 0, borderImage.getWidth(), borderImage.getHeight());
    }

    /** Resize handler - no action needed */
    void resized() override
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CanvasBorder)
    
    Image borderImage;  ///< Border graphic image
};
