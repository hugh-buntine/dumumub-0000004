/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Dumumub0000004AudioProcessorEditor::Dumumub0000004AudioProcessorEditor (Dumumub0000004AudioProcessor& p)
    : AudioProcessorEditor (&p), 
      audioProcessor (p),
      freqDisplay (p),
      selectionBar(destinationBar)
{
    Logger::writeToLog("===============================================================");
    Logger::writeToLog("EDITOR CONSTRUCTOR");
    setSize (1100, 700);
    Logger::writeToLog("set size to 1100, 700");

    // FREQ DISPLAY
    freqDisplay.setBounds(38, 162, 1024, 500);
    addAndMakeVisible(freqDisplay);
    Logger::writeToLog("added freqDisplay to editor");

    titleImageUnpressed = ImageCache::getFromMemory(BinaryData::TITLE_4_png, BinaryData::TITLE_4_pngSize);
    titleImagePressed = ImageCache::getFromMemory(BinaryData::TITLE_4_PRESS_png, BinaryData::TITLE_4_PRESS_pngSize);
    titleImageHover = ImageCache::getFromMemory(BinaryData::TITLE_4_HOVER_png, BinaryData::TITLE_4_HOVER_pngSize);
    titleButton.setImages(false, true, false, titleImageUnpressed, 1.0f, Colours::transparentBlack, titleImageHover, 1.0f, Colours::transparentBlack, titleImagePressed, 1.0f, Colours::transparentBlack);

    undoImageUnpressed = ImageCache::getFromMemory(BinaryData::UNDO_png, BinaryData::UNDO_pngSize);
    undoImagePressed = ImageCache::getFromMemory(BinaryData::UNDO_PRESS_png, BinaryData::UNDO_PRESS_pngSize);
    undoImageHover = ImageCache::getFromMemory(BinaryData::UNDO_HOVER_png, BinaryData::UNDO_HOVER_pngSize);
    undoButton.setImages(false, true, false, undoImageUnpressed, 1.0f, Colours::transparentBlack, undoImageHover, 1.0f, Colours::transparentBlack, undoImagePressed, 1.0f, Colours::transparentBlack);

    moveImageUnpressed = ImageCache::getFromMemory(BinaryData::MOVE_png, BinaryData::MOVE_pngSize);
    moveImagePressed = ImageCache::getFromMemory(BinaryData::MOVE_PRESS_png, BinaryData::MOVE_PRESS_pngSize);
    moveImageHover = ImageCache::getFromMemory(BinaryData::MOVE_HOVER_png, BinaryData::MOVE_HOVER_pngSize);
    moveButton.setImages(false, true, false, moveImageUnpressed, 1.0f, Colours::transparentBlack, moveImageHover, 1.0f, Colours::transparentBlack, moveImagePressed, 1.0f, Colours::transparentBlack);

    // MOVE BUTTON
    moveButton.setBounds(620, 10, 200, 80);
    addAndMakeVisible(moveButton);
    moveButton.addListener(this);
    Logger::writeToLog("added move button");

    // UNDO BUTTON
    undoButton.setBounds(860, 10, 200, 80);
    addAndMakeVisible(undoButton);
    undoButton.addListener(this);
    Logger::writeToLog("added undo button");

    // TITLE BUTTON
    titleButton.setBounds(40, 10, 540, 80);
    addAndMakeVisible(titleButton);
    titleButton.addListener(this);
    Logger::writeToLog("added title button");

    // BORDER IMAGE
    borderImage.setBounds(0, 124, 1100, 576);
    addAndMakeVisible(borderImage);
    borderImage.toFront(true);
    Logger::writeToLog("added border image");

    // DESTINATION BAR
    destinationBar.setBounds(762, 120, 100, 20);
    destinationBar.toFront(true);
    addAndMakeVisible(destinationBar);
    Logger::writeToLog("added destination bar");

    // SELECTION BAR
    selectionBar.setBounds(238, 100, 100, 20);
    selectionBar.toFront(true);
    addAndMakeVisible(selectionBar);
    Logger::writeToLog("added selection bar");

    // SET THE PROCESSOR
    updateProcessorOfBars();
    Logger::writeToLog("set processor of bars");
    

    



    Logger::writeToLog("===============================================================");
   
}

Dumumub0000004AudioProcessorEditor::~Dumumub0000004AudioProcessorEditor()
{
    Logger::writeToLog("===============================================================");
    Logger::writeToLog("EDITOR DESTRUCTOR");
    Logger::writeToLog("===============================================================");
}

//==============================================================================
void Dumumub0000004AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB(255, 255, 242)); // #FFFFF2

    // SELECTION BAR LINES
    g.setColour (juce::Colour::fromRGB(20, 20, 0)); // #141400
    g.drawLine(selectionBar.getLeftBound() + 38 + 1, 162, selectionBar.getLeftBound() + 38 + 1, 662, 1); // + 38 to account for the offset. + 1 to make it not stick to the side
    g.drawLine(selectionBar.getRightBound() + 38, 162, selectionBar.getRightBound() + 38, 662, 1); // + 38 to account for the offset.

    // DESTINATION BAR LINES
    Line<float> line1 = Line<float>(destinationBar.getLeftBound() + 38 + 1, 162, destinationBar.getLeftBound() + 38 + 1, 662);
    Line<float> line2 = Line<float>(destinationBar.getRightBound() + 38, 162, destinationBar.getRightBound() + 38, 662);
    const float dashArray[] = { 5.0f, 5.0f };
    g.drawDashedLine(line1, dashArray, 2, 1.0f, 0);
    g.drawDashedLine(line2, dashArray, 2, 1.0f, 0);
    

}

void Dumumub0000004AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void Dumumub0000004AudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &moveButton)
    {
        audioProcessor.editBinMap();
        freqDisplay.updateBarIndexs();
    }
    else if (button == &undoButton)
    {
        audioProcessor.undoBinMap();
        freqDisplay.updateBarIndexs();
    }
    else if (button == &titleButton)
    {
        Logger::writeToLog("Title button clicked");
    }
}

void Dumumub0000004AudioProcessorEditor::updateProcessorOfBars()
{
    audioProcessor.setSelectionBarLeft(selectionBar.getLeftBound());
    audioProcessor.setSelectionBarRight(selectionBar.getRightBound());
    audioProcessor.setDestinationBarLeft(destinationBar.getLeftBound());
    audioProcessor.setDestinationBarRight(destinationBar.getRightBound()); 


}

