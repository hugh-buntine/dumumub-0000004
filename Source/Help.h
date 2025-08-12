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
/**
 * Overlay help component for displaying user instructions.
 * 
 * Shows/hides help image overlay when toggled. Positioned always on top
 * and click-through to allow interaction with underlying components while
 * help is displayed.
 */
class Help : public juce::Component
{
public:
    /** Constructor - loads help image and configures overlay behavior */
    Help();
    
    /** Destructor */
    ~Help() override;

    // Component overrides
    void paint(juce::Graphics&) override;
    void resized() override;

    /** Toggles help visibility state */
    void setHelpVisible() { isOn = !isOn; repaint(); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Help)

    Image helpImage;  ///< Help overlay image
    bool isOn;        ///< Visibility state flag
};
