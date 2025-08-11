/*
  ==============================================================================

    FreqBar.h
    Created: 15 Mar 2025 2:44:52pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FreqBar  : public juce::Component
{
public:
    FreqBar(int i);
    ~FreqBar() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setHeight(int h) 
    { 
      height = h; 
      repaint();
    }

    void setIsHighlighted(bool b) 
    { 
      isHighlighted = b; 
      repaint();
    }

    void calculateColour();

    void setCurrentIndex(int i) { currentIndex = i; }
    int getCurrentIndex() { return currentIndex; }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreqBar)
    int height;
    int homeIndex;
    int currentIndex;
    Colour colour;
    bool isHighlighted;
};
