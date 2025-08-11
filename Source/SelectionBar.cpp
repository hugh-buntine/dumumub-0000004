/*
  ==============================================================================

    SelectionBar.cpp
    Created: 21 Mar 2025 2:08:22pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SelectionBar.h"
#include "PluginEditor.h"

//==============================================================================
SelectionBar::SelectionBar(DestinationBar& d) : 
    leftEdgeComponent(this, &constrainer, ResizableEdgeComponent::Edge::leftEdge),
    rightEdgeComponent(this, &constrainer, ResizableEdgeComponent::Edge::rightEdge),
    destinationBar(d)

{
    constrainer.setMinimumWidth(1); // Minimum width for the SelectionBar
    constrainer.setMaximumWidth(1024); // Maximum width for the SelectionBar

    addAndMakeVisible(leftEdgeComponent);
    addAndMakeVisible(rightEdgeComponent);

    leftEdgeComponent.setAlwaysOnTop(true);
    rightEdgeComponent.setAlwaysOnTop(true);

    updateProcessorOfBars();


}

SelectionBar::~SelectionBar()
{
}

void SelectionBar::paint (juce::Graphics& g)
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
    juce::String text = "from here";

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

void SelectionBar::resized()
{
    // Set bounds for the edge components
    auto bounds = getLocalBounds();
    leftEdgeComponent.setBounds(bounds.removeFromLeft(10)); // 5px wide edge on the left
    rightEdgeComponent.setBounds(bounds.removeFromRight(10)); // 5px wide edge on the right

    // Enforce resizing constraints
    int leftBound = 38;
    int rightBound = 1061;

    // Prevent resizing beyond the left boundary
    if (getX() < leftBound) {
        setBounds(leftBound, getY(), getWidth() - (leftBound - getX()), getHeight());
    }

    // Prevent resizing beyond the right boundary
    if (getRight() > rightBound) {
        setBounds(getX(), getY(), getWidth() - (getRight() - rightBound), getHeight());
    }

    destinationBar.setSize(getWidth(), getHeight());

    updateProcessorOfBars();

}

void SelectionBar::mouseDown(const MouseEvent& event)
{
    dragger.startDraggingComponent(this, event);
}

void SelectionBar::mouseDrag(const MouseEvent& event)
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

void SelectionBar::mouseUp(const MouseEvent& event)
{
}

void SelectionBar::updateProcessorOfBars()
{
    auto* parentEditor = findParentComponentOfClass<Dumumub0000004AudioProcessorEditor>();
    if (parentEditor != nullptr)
    {
        parentEditor->updateProcessorOfBars();
    }
}

