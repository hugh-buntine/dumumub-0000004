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
/*
*/
class DestinationBar  : public juce::Component
{
public:
    DestinationBar();
    ~DestinationBar() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDown(const MouseEvent& event) override;
    void mouseDrag(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    int getLeftBound() { return getX() - 38; } // made it relative
    int getRightBound() { return getX() + getWidth() - 38; } // made it relative

    void updateProcessorOfBars();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DestinationBar)
    ComponentDragger dragger;

};
