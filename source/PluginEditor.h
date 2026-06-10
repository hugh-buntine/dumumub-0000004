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
 * Main plugin editor interface for frequency manipulation.
 * 
 * Provides graphical interface for real-time frequency spectrum visualization
 * and interactive frequency bin remapping. Includes selection tools, control
 * buttons, and help overlay system.
 */
class Dumumub0000004AudioProcessorEditor : public juce::AudioProcessorEditor,
                                          public juce::Button::Listener
{
public:
    /** Constructor - initializes all UI components and layout */
    Dumumub0000004AudioProcessorEditor(Dumumub0000004AudioProcessor&);
    
    /** Destructor */
    ~Dumumub0000004AudioProcessorEditor() override;

    //==============================================================================
    // Component interface
    void paint(juce::Graphics&) override;
    void resized() override;

    /** Handles button click events for move, undo, and title buttons */
    void buttonClicked(Button* button) override;
    
    /** @returns Selection bar left boundary for processor coordination */
    int getSelectionBarLeftBound() { return selectionBar.getLeftBound(); }
    
    /** @returns Selection bar right boundary for processor coordination */
    int getSelectionBarRightBound() { return selectionBar.getRightBound(); }

    /** Updates audio processor with current bar positions */
    void updateProcessorOfBars();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dumumub0000004AudioProcessorEditor)
    
    // Core references
    Dumumub0000004AudioProcessor& audioProcessor;  ///< Reference to audio processor

    // Main display components
    FreqDisplay freqDisplay;        ///< Real-time frequency spectrum display
    
    // Control buttons with multiple image states
    ImageButton moveButton;         ///< Move/apply frequency remapping button
    ImageButton undoButton;         ///< Undo last operation button  
    ImageButton titleButton;        ///< Title/help toggle button

    // Button image assets (normal, pressed, hover states)
    Image titleImageUnpressed;      ///< Title button normal state
    Image titleImagePressed;        ///< Title button pressed state
    Image titleImageHover;          ///< Title button hover state
    Image undoImageUnpressed;       ///< Undo button normal state
    Image undoImagePressed;         ///< Undo button pressed state
    Image undoImageHover;           ///< Undo button hover state
    Image moveImageUnpressed;       ///< Move button normal state
    Image moveImagePressed;         ///< Move button pressed state
    Image moveImageHover;           ///< Move button hover state
    Image background;               ///< Background image

    // Overlay and interaction components
    Help help;                      ///< Help overlay system
    SelectionBar selectionBar;      ///< Frequency range selection tool
    DestinationBar destinationBar;  ///< Frequency remapping destination tool
    CanvasBorder borderImage;       ///< Decorative border frame
};
