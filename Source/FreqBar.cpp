/*
  ==============================================================================

    FreqBar.cpp
    Created: 15 Mar 2025 2:44:52pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FreqBar.h"

//==============================================================================
FreqBar::FreqBar(int i) : homeIndex(i)
{
  calculateColour();
}

FreqBar::~FreqBar()
{
}

void FreqBar::paint (juce::Graphics& g)
{
    Colour currentColour;
    if (isHighlighted) {
        currentColour = Colour::fromRGBA(colour.getRed(), colour.getGreen(), colour.getBlue(), 255); // Full opacity
    } else {
        currentColour = Colour::fromRGBA(colour.getRed(), colour.getGreen(), colour.getBlue(), 60); // Semi-transparent
    }

    g.setColour (currentColour);
    g.fillRect(currentIndex, 500 - height, 1, height);  
}

void FreqBar::resized()
{

}

void FreqBar::calculateColour()
{
    float ratio = static_cast<float>(homeIndex) / 1023.0f; // Use 1023 to include the last index

    uint8_t red, green, blue;

    if (ratio < 0.01f) { // 0Hz-100Hz
        // Red (255, 0, 0)
        red = 255;
        green = 0;
        blue = 0;
    } else if (ratio < 0.2f) { // 100Hz-2000Hz
        // Interpolate from red (255, 0, 0) to green (0, 255, 0)
        float subRatio = (ratio - 0.01f) / (0.2f - 0.01f);
        red = static_cast<uint8_t>((1.0f - subRatio) * 255);
        green = static_cast<uint8_t>(subRatio * 255);
        blue = 0;
    } else if (ratio < 0.3f) { // 2000Hz-3000Hz
        // Green (0, 255, 0)
        red = 0;
        green = 255;
        blue = 0;
    } else if (ratio < 0.95f) { // 3000Hz-9500Hz
        // Interpolate from green (0, 255, 0) to blue (0, 0, 255)
        float subRatio = (ratio - 0.3f) / (0.95f - 0.3f);
        red = 0;
        green = static_cast<uint8_t>((1.0f - subRatio) * 255);
        blue = static_cast<uint8_t>(subRatio * 255);
    } else { // 9500Hz-10000Hz
        // Blue (0, 0, 255)
        red = 0;
        green = 0;
        blue = 255;
    }

    colour = Colour::fromRGB(red, green, blue);
}


