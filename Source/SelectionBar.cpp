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
    // Configure size constraints for source frequency selection
    constrainer.setMinimumWidth(1);    // Minimum selection width (1 frequency bin)
    constrainer.setMaximumWidth(1024); // Maximum selection width (full spectrum)

    // Add resize handles to left and right edges
    addAndMakeVisible(leftEdgeComponent);
    addAndMakeVisible(rightEdgeComponent);

    // Ensure resize handles stay on top for interaction
    leftEdgeComponent.setAlwaysOnTop(true);
    rightEdgeComponent.setAlwaysOnTop(true);

    // Initialize processor communication
    updateProcessorOfBars();
}

SelectionBar::~SelectionBar()
{
}

void SelectionBar::paint(juce::Graphics& g)
{
    // Set color for "from here" text label
    g.setColour(juce::Colour::fromRGBA(20, 20, 0, 255)); // Dark green-brown

    // Load custom Duru Sans font from binary resources
    static juce::Font duruSansFont = juce::Font(juce::Typeface::createSystemTypefaceFor(
        BinaryData::DuruSansRegular_ttf, BinaryData::DuruSansRegular_ttfSize));
    g.setFont(duruSansFont.withHeight(14.0f));

    // Render "from here" label with custom letter spacing
    juce::String text = "from here";
    float letterSpacing = (0.9f * getWidth()) / (text.length() - 1);
    float x = 0.05f * getWidth();
    float y = (getHeight() - g.getCurrentFont().getHeight()) / 2.0f;

    // Draw each character individually for precise spacing control
    for (int i = 0; i < text.length(); ++i)
    {
        juce::String letter = juce::String::charToString(text[i]);
        g.drawText(letter, x - (0.025f * getWidth()), y, 
                  g.getCurrentFont().getStringWidth(letter), 
                  g.getCurrentFont().getHeight(), 
                  juce::Justification::left);
        x += letterSpacing;
    }
}

void SelectionBar::resized()
{
    // Position resize handles at component edges
    auto bounds = getLocalBounds();
    leftEdgeComponent.setBounds(bounds.removeFromLeft(10));   // 10px wide left handle
    rightEdgeComponent.setBounds(bounds.removeFromRight(10)); // 10px wide right handle

    // Enforce spectrum display boundaries (38px offset for UI margins)
    int leftBound = 38;    // Left edge of frequency display
    int rightBound = 1061; // Right edge of frequency display (38 + 1024 - 1)

    // Clamp position to display boundaries
    if (getX() < leftBound) {
        setBounds(leftBound, getY(), getWidth() - (leftBound - getX()), getHeight());
    }

    if (getRight() > rightBound) {
        setBounds(getX(), getY(), getWidth() - (getRight() - rightBound), getHeight());
    }

    // Synchronize destination bar size with selection bar
    destinationBar.setSize(getWidth(), getHeight());

    // Update processor with new selection bounds
    updateProcessorOfBars();
}

void SelectionBar::mouseDown(const MouseEvent& event)
{
    // Initialize drag operation for bar repositioning
    dragger.startDraggingComponent(this, event);
}

void SelectionBar::mouseDrag(const MouseEvent& event)
{
    // Handle dragging with boundary constraints
    int leftBound = 38;    // Display left boundary
    int rightBound = 1061; // Display right boundary
    int yPos = this->getY(); // Lock vertical position during drag

    dragger.dragComponent(this, event, nullptr);
    
    // Apply horizontal constraints during drag
    if (this->getX() < leftBound) {
        this->setBounds(leftBound, yPos, this->getWidth(), this->getHeight());
    }
    else if (this->getRight() > rightBound) {
        this->setBounds(rightBound - this->getWidth(), yPos, this->getWidth(), this->getHeight());
    }
    else {
        this->setBounds(this->getX(), yPos, this->getWidth(), this->getHeight());
    }

    // Notify processor of position changes during drag
    updateProcessorOfBars();
}

void SelectionBar::mouseUp(const MouseEvent& event)
{
    // Drag operation complete - no additional processing needed
}

void SelectionBar::updateProcessorOfBars()
{
    // Communicate current selection bounds to audio processor via parent editor
    auto* parentEditor = findParentComponentOfClass<Dumumub0000004AudioProcessorEditor>();
    if (parentEditor != nullptr)
    {
        parentEditor->updateProcessorOfBars();
    }
}

