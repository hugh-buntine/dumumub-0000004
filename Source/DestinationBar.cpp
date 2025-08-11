/*
  ==============================================================================

    DragBar.cpp
    Created: 21 Mar 2025 3:12:35pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DestinationBar.h"
#include "PluginEditor.h"

//==============================================================================
DestinationBar::DestinationBar()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    updateProcessorOfBars();
}

DestinationBar::~DestinationBar()
{
}

void DestinationBar::paint (juce::Graphics& g)
{
    g.setColour(juce::Colour::fromRGBA(20, 20, 0, 140)); // #141400
    //g.drawLine(0, 0, getWidth(), 0, 2.0f); // top line
    //g.drawLine(0, 0, 0, getHeight(), 2.0f); // left line
    //g.drawLine(getWidth(), 0, getWidth(), getHeight(), 2.0f); // right line

    // Load the Duru Sans font
    static juce::Font duruSansFont = juce::Font(juce::Typeface::createSystemTypefaceFor(
        BinaryData::DuruSansRegular_ttf, BinaryData::DuruSansRegular_ttfSize));
    g.setFont(duruSansFont.withHeight(14.0f)); // Set the font with the desired size

    // Text to draw
    juce::String text = "to here";

    // Calculate spacing and positions
    float letterSpacing = (0.9f * getWidth()) / (text.length() - 1); // Adjust this value to control spacing between letters
    float x = 0.05f * getWidth(); // Start drawing from the left edge
    float y = (getHeight() - g.getCurrentFont().getHeight()) / 2.0f; // Vertically center the text

    // Draw each letter one by one
    for (int i = 0; i < text.length(); ++i)
    {
        // Explicitly convert the character to a juce::String
        juce::String letter = juce::String::charToString(text[i]);
        g.drawText(letter, x - (0.025f * getWidth()), y, g.getCurrentFont().getStringWidth(letter), g.getCurrentFont().getHeight(), juce::Justification::left);
        x += letterSpacing; // Move to the next position
    }
}


void DestinationBar::resized()
{
    // Enforce resizing constraints
    int rightBound = 1061;

    // Prevent resizing beyond the right boundary. grows left if on the right edge
    if (getRight() > rightBound) {
        setBounds(getX() - (getRight() - rightBound), getY(), getWidth(), getHeight());
    }
    updateProcessorOfBars();
}

void DestinationBar::mouseDown(const MouseEvent& event)
{
    dragger.startDraggingComponent(this, event);
}

void DestinationBar::mouseDrag(const MouseEvent& event)
{
    int leftBound = 38;
    int rightBound = 1061;
    int yPos = this->getY(); // lock y position

    dragger.dragComponent(this, event, nullptr);
    
    if (this->getX() < leftBound) { // cant go too far left
        this->setBounds(leftBound, yPos, this->getWidth(), this->getHeight());
    }
    else if (this->getRight() > rightBound) { // cant go too far right
        this->setBounds(rightBound - this->getWidth(), yPos, this->getWidth(), this->getHeight());
    }
    else {
        this->setBounds(this->getX(), yPos, this->getWidth(), this->getHeight());
    }
    updateProcessorOfBars();
}

void DestinationBar::mouseUp(const MouseEvent& event)
{
}

void DestinationBar::updateProcessorOfBars()
{
    auto* parentEditor = findParentComponentOfClass<Dumumub0000004AudioProcessorEditor>();
    if (parentEditor != nullptr)
    {
        parentEditor->updateProcessorOfBars();
    }
}