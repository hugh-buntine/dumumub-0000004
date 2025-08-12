/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FreqDisplay.h"
#include "SelectionBar.h"
#include "DestinationBar.h"
#include "CanvasBorder.h"
#include "Help.h"

//==============================================================================
/**
*/
class Dumumub0000004AudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            public juce::Button::Listener
{
public:
    Dumumub0000004AudioProcessorEditor (Dumumub0000004AudioProcessor&);
    ~Dumumub0000004AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(Button* button) override;
    int getSelectionBarLeftBound() { return selectionBar.getLeftBound(); }
    int getSelectionBarRightBound() { return selectionBar.getRightBound(); }

    void updateProcessorOfBars();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Dumumub0000004AudioProcessor& audioProcessor;

    FreqDisplay freqDisplay;

    ImageButton moveButton;
    ImageButton undoButton;
    ImageButton titleButton;

    Image titleImageUnpressed;
    Image titleImagePressed;
    Image titleImageHover;
    Image undoImageUnpressed;
    Image undoImagePressed;
    Image undoImageHover;
    Image moveImageUnpressed;
    Image moveImagePressed;
    Image moveImageHover;
    Image background;

    Help help;


    SelectionBar selectionBar;
    DestinationBar destinationBar;

    CanvasBorder borderImage;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dumumub0000004AudioProcessorEditor)
};
