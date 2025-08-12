/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Dumumub0000004AudioProcessorEditor::Dumumub0000004AudioProcessorEditor(Dumumub0000004AudioProcessor& p)
    : AudioProcessorEditor(&p), 
      audioProcessor(p),
      freqDisplay(p),
      selectionBar(destinationBar)
{
    // Set plugin window dimensions
    setSize(1100, 700);

    // Load background image from binary resources
    background = ImageCache::getFromMemory(BinaryData::BACKGROUND_png, BinaryData::BACKGROUND_pngSize);

    // Initialize main frequency spectrum display component
    freqDisplay.setBounds(38, 162, 1024, 500);
    addAndMakeVisible(freqDisplay);

    // Configure title button with state-based images
    titleImageUnpressed = ImageCache::getFromMemory(BinaryData::TITLE_png, BinaryData::TITLE_pngSize);
    titleImagePressed = ImageCache::getFromMemory(BinaryData::TITLE_PRESS_png, BinaryData::TITLE_PRESS_pngSize);
    titleImageHover = ImageCache::getFromMemory(BinaryData::TITLE_HOVER_png, BinaryData::TITLE_HOVER_pngSize);
    titleButton.setImages(false, true, false, titleImageUnpressed, 1.0f, Colours::transparentBlack, 
                         titleImageHover, 1.0f, Colours::transparentBlack, titleImagePressed, 1.0f, Colours::transparentBlack);

    // Configure undo button with state-based images
    undoImageUnpressed = ImageCache::getFromMemory(BinaryData::UNDO_png, BinaryData::UNDO_pngSize);
    undoImagePressed = ImageCache::getFromMemory(BinaryData::UNDO_PRESS_png, BinaryData::UNDO_PRESS_pngSize);
    undoImageHover = ImageCache::getFromMemory(BinaryData::UNDO_HOVER_png, BinaryData::UNDO_HOVER_pngSize);
    undoButton.setImages(false, true, false, undoImageUnpressed, 1.0f, Colours::transparentBlack, 
                        undoImageHover, 1.0f, Colours::transparentBlack, undoImagePressed, 1.0f, Colours::transparentBlack);

    // Configure move button with state-based images
    moveImageUnpressed = ImageCache::getFromMemory(BinaryData::MOVE_png, BinaryData::MOVE_pngSize);
    moveImagePressed = ImageCache::getFromMemory(BinaryData::MOVE_PRESS_png, BinaryData::MOVE_PRESS_pngSize);
    moveImageHover = ImageCache::getFromMemory(BinaryData::MOVE_HOVER_png, BinaryData::MOVE_HOVER_pngSize);
    moveButton.setImages(false, true, false, moveImageUnpressed, 1.0f, Colours::transparentBlack, 
                        moveImageHover, 1.0f, Colours::transparentBlack, moveImagePressed, 1.0f, Colours::transparentBlack);

    // Position and configure move button (applies frequency bin remapping)
    moveButton.setBounds(620, 10, 200, 80);
    addAndMakeVisible(moveButton);
    moveButton.addListener(this);

    // Position and configure undo button (reverts last mapping change)
    undoButton.setBounds(860, 10, 200, 80);
    addAndMakeVisible(undoButton);
    undoButton.addListener(this);

    // Position and configure title button (shows/hides help overlay)
    titleButton.setBounds(80, 10, 500, 80);
    addAndMakeVisible(titleButton);
    titleButton.addListener(this);

    // Add spectrum display border component
    borderImage.setBounds(0, 124, 1100, 576);
    addAndMakeVisible(borderImage);
    borderImage.toFront(true);

    // Initialize destination frequency range selector
    destinationBar.setBounds(762, 120, 100, 20);
    destinationBar.toFront(true);
    addAndMakeVisible(destinationBar);

    // Initialize source frequency range selector
    selectionBar.setBounds(238, 100, 100, 20);
    selectionBar.toFront(true);
    addAndMakeVisible(selectionBar);

    // Add help overlay component (always on top, click-through enabled)
    help.setBounds(0, 0, 1100, 700);
    addAndMakeVisible(help);

    // Synchronize selection bar positions with audio processor
    updateProcessorOfBars();
}

Dumumub0000004AudioProcessorEditor::~Dumumub0000004AudioProcessorEditor()
{
}

//==============================================================================
void Dumumub0000004AudioProcessorEditor::paint(juce::Graphics& g)
{
    // Draw main background image
    g.drawImage(background, 0, 0, getWidth(), getHeight(),
                0, 0, background.getWidth(), background.getHeight());

    // Draw solid lines for source frequency selection range
    g.setColour(juce::Colour::fromRGB(20, 20, 0)); // Dark green-brown
    g.drawLine(selectionBar.getLeftBound() + 38 + 1, 162, selectionBar.getLeftBound() + 38 + 1, 662, 1);
    g.drawLine(selectionBar.getRightBound() + 38, 162, selectionBar.getRightBound() + 38, 662, 1);

    // Draw dashed lines for destination frequency range
    Line<float> line1 = Line<float>(destinationBar.getLeftBound() + 38 + 1, 162, destinationBar.getLeftBound() + 38 + 1, 662);
    Line<float> line2 = Line<float>(destinationBar.getRightBound() + 38, 162, destinationBar.getRightBound() + 38, 662);
    const float dashArray[] = { 5.0f, 5.0f };
    g.drawDashedLine(line1, dashArray, 2, 1.0f, 0);
    g.drawDashedLine(line2, dashArray, 2, 1.0f, 0);
}

void Dumumub0000004AudioProcessorEditor::resized()
{
    // No dynamic component resizing needed - all components use fixed positioning
}

void Dumumub0000004AudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &moveButton)
    {
        // Apply frequency bin remapping based on current selection and destination ranges
        audioProcessor.editBinMap();
        freqDisplay.updateBarIndexs();
    }
    else if (button == &undoButton)
    {
        // Revert to previous bin mapping state
        audioProcessor.undoBinMap();
        freqDisplay.updateBarIndexs();
    }
    else if (button == &titleButton)
    {
        // Toggle help overlay visibility
        help.setHelpVisible();
    }
}

void Dumumub0000004AudioProcessorEditor::updateProcessorOfBars()
{
    // Synchronize selection bar bounds with audio processor for bin mapping calculations
    audioProcessor.setSelectionBarLeft(selectionBar.getLeftBound());
    audioProcessor.setSelectionBarRight(selectionBar.getRightBound());
    audioProcessor.setDestinationBarLeft(destinationBar.getLeftBound());
    audioProcessor.setDestinationBarRight(destinationBar.getRightBound()); 
}

