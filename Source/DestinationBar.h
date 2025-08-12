/*
  ==============================================================================

    DestinationBar.h
    Created: 21 Mar 2025 3:12:35pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Destination bar component for frequency bin remapping.
 * 
 * Displays target location for frequency content selected by SelectionBar.
 * Shows "to here" text and handles drag interactions to position the
 * destination for frequency bin manipulation operations.
 */
class DestinationBar : public juce::Component
{
public:
    /** Constructor */
    DestinationBar();
    
    /** Destructor */
    ~DestinationBar() override;

    // Component overrides
    void paint(juce::Graphics&) override;
    void resized() override;

    // Mouse interaction handling
    void mouseDown(const MouseEvent& event) override;
    void mouseDrag(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    /** @returns Left boundary position relative to frequency display */
    int getLeftBound() { return getX() - 38; }
    
    /** @returns Right boundary position relative to frequency display */
    int getRightBound() { return getX() + getWidth() - 38; }

    /** Notifies audio processor of position changes */
    void updateProcessorOfBars();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DestinationBar)
    
    ComponentDragger dragger;  ///< Handles drag operations
};
