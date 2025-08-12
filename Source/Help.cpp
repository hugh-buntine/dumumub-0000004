/*
  ==============================================================================

    Help.cpp
    Created: 12 Aug 2025 10:51:09am
    Author:  Hugh Buntine

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Help.h"

//==============================================================================
Help::Help()
{
    // Load help image from binary data
    helpImage = ImageCache::getFromMemory(BinaryData::HELP_png, BinaryData::HELP_pngSize);
    isOn = false;
    
    // Configure as overlay: always on top and click-through
    setAlwaysOnTop(true);
    setInterceptsMouseClicks(false, false);
}

Help::~Help()
{
}

void Help::paint(juce::Graphics& g)
{
    if (isOn)
    {
        // Draw help image scaled to component bounds
        g.drawImage(helpImage, 0, 0, getWidth(), getHeight(),
                    0, 0, helpImage.getWidth(), helpImage.getHeight());
    }
    else
    {
        // Clear the area when help is hidden
        g.fillAll(juce::Colour::fromRGBA(0, 0, 0, 0));
    }
}

void Help::resized()
{
    // No child components to resize
}
