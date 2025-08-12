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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    helpImage = ImageCache::getFromMemory(BinaryData::HELP_png, BinaryData::HELP_pngSize);
    isOn = false;
    
    setAlwaysOnTop(true); // This will keep the component always at the front
    setInterceptsMouseClicks(false, false); // Make it click-through
}

Help::~Help()
{
}

void Help::paint (juce::Graphics& g)
{
    if (isOn)
    {
        g.drawImage(helpImage, 0, 0, getWidth(), getHeight(),
                    0, 0, helpImage.getWidth(), helpImage.getHeight());
    }
    else
    {
        g.fillAll(juce::Colour::fromRGBA(0, 0, 0, 0)); // Clear the area if not on
    }

}

void Help::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
