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

void FreqBar::paint(juce::Graphics& g)
{
    // Determine opacity based on selection state
    Colour currentColour;
    if (isHighlighted) {
        currentColour = Colour::fromRGBA(colour.getRed(), colour.getGreen(), colour.getBlue(), 255);
    } else {
        currentColour = Colour::fromRGBA(colour.getRed(), colour.getGreen(), colour.getBlue(), 60);
    }

    // Draw vertical frequency bar
    g.setColour(currentColour);
    g.fillRect(currentIndex, 500 - height, 1, height);  
}

void FreqBar::resized()
{
    // No child components to resize
}

void FreqBar::calculateColour()
{
    // Calculate position ratio for color mapping (0.0 to 1.0)
    float ratio = static_cast<float>(homeIndex) / 1023.0f;

    uint8_t red, green, blue;

    // Color mapping based on frequency ranges:
    // Red: 0-100Hz, Red->Green: 100-2000Hz, Green: 2000-3000Hz, 
    // Green->Blue: 3000-9500Hz, Blue: 9500-10000Hz
    
    if (ratio < 0.01f) { // 0Hz-100Hz: Pure red
        red = 255;
        green = 0;
        blue = 0;
    } else if (ratio < 0.2f) { // 100Hz-2000Hz: Red to green transition
        float subRatio = (ratio - 0.01f) / (0.2f - 0.01f);
        red = static_cast<uint8_t>((1.0f - subRatio) * 255);
        green = static_cast<uint8_t>(subRatio * 255);
        blue = 0;
    } else if (ratio < 0.3f) { // 2000Hz-3000Hz: Pure green
        red = 0;
        green = 255;
        blue = 0;
    } else if (ratio < 0.95f) { // 3000Hz-9500Hz: Green to blue transition
        float subRatio = (ratio - 0.3f) / (0.95f - 0.3f);
        red = 0;
        green = static_cast<uint8_t>((1.0f - subRatio) * 255);
        blue = static_cast<uint8_t>(subRatio * 255);
    } else { // 9500Hz-10000Hz: Pure blue
        red = 0;
        green = 0;
        blue = 255;
    }

    colour = Colour::fromRGB(red, green, blue);
}


