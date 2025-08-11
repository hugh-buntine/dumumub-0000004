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
/*
*/
class FreqDisplay  : public juce::Component, private juce::Timer
{
public:
    FreqDisplay(Dumumub0000004AudioProcessor& p);
    ~FreqDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void updateBarIndexs();


private:
    void timerCallback() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreqDisplay)
    Dumumub0000004AudioProcessor& audioProcessor;
    std::vector<std::unique_ptr<FreqBar>> freqBars;
};
