/*
  ==============================================================================

    Help.h
    Created: 12 Aug 2025 10:51:09am
    Author:  Hugh Buntine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Help  : public juce::Component
{
public:
    Help();
    ~Help() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setHelpVisible() { isOn = !isOn; repaint(); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Help)

    Image helpImage;
    bool isOn;
};
