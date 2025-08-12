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
/**
 * Individual frequency bar component for spectrum visualization.
 * 
 * Represents a single frequency bin in the spectrum display. Height reflects
 * magnitude, color indicates frequency displacement from original position,
 * and highlighting shows selection state.
 */
class FreqBar : public juce::Component
{
public:
    /** Constructor - sets initial home index for color calculation */
    FreqBar(int i);
    
    /** Destructor */
    ~FreqBar() override;

    // Component overrides
    void paint(juce::Graphics&) override;
    void resized() override;

    /** Sets bar height based on frequency magnitude */
    void setHeight(int h) 
    { 
        height = h; 
        repaint();
    }

    /** Sets selection highlight state */
    void setIsHighlighted(bool b) 
    { 
        isHighlighted = b; 
        repaint();
    }

    /** Calculates color based on frequency displacement */
    void calculateColour();

    /** Sets current frequency bin index after remapping */
    void setCurrentIndex(int i) { currentIndex = i; }
    
    /** @returns Current frequency bin index */
    int getCurrentIndex() { return currentIndex; }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FreqBar)
    
    int height;           ///< Visual height (magnitude)
    int homeIndex;        ///< Original frequency bin index
    int currentIndex;     ///< Current frequency bin index after remapping
    Colour colour;        ///< Display color based on displacement
    bool isHighlighted;   ///< Selection highlight state
};
