/*
  ==============================================================================

    FreqDisplay.h
    Created: 15 Mar 2025 2:31:11pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FreqBar.h"

//==============================================================================
/**
 * Real-time frequency spectrum display component.
 * 
 * Visualizes audio frequency content using 1024 individual frequency bars.
 * Updates at 30Hz to show live spectrum analysis from the audio processor's
 * FFT data. Each bar represents a frequency bin and displays magnitude.
 */
class FreqDisplay : public juce::Component, private juce::Timer
{
public:
    /** Constructor - requires reference to audio processor for spectrum data */
    FreqDisplay(Dumumub0000004AudioProcessor& p);
    
    /** Destructor */
    ~FreqDisplay() override;

    // Component overrides
    void paint(juce::Graphics&) override;
    void resized() override;

    /** Updates frequency bar index mappings after bin remapping */
    void updateBarIndexs();

private:
    /** Timer callback for 30Hz display updates */
    void timerCallback() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FreqDisplay)
    
    Dumumub0000004AudioProcessor& audioProcessor;        ///< Reference to audio processor
    std::vector<std::unique_ptr<FreqBar>> freqBars;      ///< Array of 1024 frequency bars
};
