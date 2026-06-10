/*
  ==============================================================================

    SelectionBar.h
    Created: 21 Mar 2025 2:08:22pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DestinationBar.h"

//==============================================================================
/**
 * Interactive selection bar component for frequency range selection.
 * 
 * Allows users to select a frequency range by dragging the bar and resizing
 * its edges. Works in conjunction with DestinationBar to define source and
 * target ranges for frequency bin manipulation.
 */
class SelectionBar  : public juce::Component
{
public:
    /** Constructor - requires reference to destination bar for coordination */
    SelectionBar(DestinationBar& d);
    
    /** Destructor */
    ~SelectionBar() override;

    // Component overrides
    void paint (juce::Graphics&) override;
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SelectionBar)
    
    // Interaction components
    ComponentDragger dragger;                    ///< Handles drag operations
    ComponentBoundsConstrainer constrainer;      ///< Constrains movement bounds
    ResizableEdgeComponent leftEdgeComponent;    ///< Left edge resize handle
    ResizableEdgeComponent rightEdgeComponent;   ///< Right edge resize handle

    DestinationBar& destinationBar;              ///< Reference to destination bar
};
