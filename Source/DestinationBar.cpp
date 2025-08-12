/*
  ==============================================================================

    DestinationBar.cpp
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
    // Notify processor of initial position
    updateProcessorOfBars();
}

DestinationBar::~DestinationBar()
{
}

void DestinationBar::paint(juce::Graphics& g)
{
    // Semi-transparent background color
    g.setColour(juce::Colour::fromRGBA(20, 20, 0, 255));

    // Load and configure Duru Sans font
    static juce::Font duruSansFont = juce::Font(juce::Typeface::createSystemTypefaceFor(
        BinaryData::DuruSansRegular_ttf, BinaryData::DuruSansRegular_ttfSize));
    g.setFont(duruSansFont.withHeight(14.0f));

    // Text to display
    juce::String text = "to here";

    // Calculate letter spacing and positions for distributed text
    float letterSpacing = (0.9f * getWidth()) / (text.length() - 1);
    float x = 0.05f * getWidth(); // Start drawing from left edge
    float y = (getHeight() - g.getCurrentFont().getHeight()) / 2.0f; // Vertically center

    // Draw each letter individually with custom spacing
    for (int i = 0; i < text.length(); ++i)
    {
        juce::String letter = juce::String::charToString(text[i]);
        g.drawText(letter, x - (0.025f * getWidth()), y, 
                  g.getCurrentFont().getStringWidth(letter), 
                  g.getCurrentFont().getHeight(), 
                  juce::Justification::left);
        x += letterSpacing; // Move to next position
    }
}


void DestinationBar::resized()
{
    // Constrain to frequency display bounds
    int rightBound = 1061;

    // Prevent extending beyond right boundary
    if (getRight() > rightBound) {
        setBounds(getX() - (getRight() - rightBound), getY(), getWidth(), getHeight());
    }
    updateProcessorOfBars();
}

void DestinationBar::mouseDown(const MouseEvent& event)
{
    // Initialize dragging operation
    dragger.startDraggingComponent(this, event);
}

void DestinationBar::mouseDrag(const MouseEvent& event)
{
    // Define movement constraints
    int leftBound = 38;
    int rightBound = 1061;
    int yPos = this->getY(); // Lock vertical position

    // Apply drag movement
    dragger.dragComponent(this, event, nullptr);
    
    // Enforce horizontal bounds
    if (this->getX() < leftBound) {
        this->setBounds(leftBound, yPos, this->getWidth(), this->getHeight());
    }
    else if (this->getRight() > rightBound) {
        this->setBounds(rightBound - this->getWidth(), yPos, this->getWidth(), this->getHeight());
    }
    else {
        this->setBounds(this->getX(), yPos, this->getWidth(), this->getHeight());
    }
    updateProcessorOfBars();
}

void DestinationBar::mouseUp(const MouseEvent& event)
{
    // Drag operation complete - no additional action needed
}

void DestinationBar::updateProcessorOfBars()
{
    // Find parent editor and update processor with current position
    auto* parentEditor = findParentComponentOfClass<Dumumub0000004AudioProcessorEditor>();
    if (parentEditor != nullptr)
    {
        parentEditor->updateProcessorOfBars();
    }
}