/*
  ==============================================================================

    FreqDisplay.cpp
    Created: 15 Mar 2025 2:31:11pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FreqDisplay.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FreqDisplay::FreqDisplay(Dumumub0000004AudioProcessor& p) : audioProcessor(p)
{
    freqBars.reserve(1024); // Reserve space for 1024 elements in the vector
    for (int i = 0; i < 1024; ++i) {
        auto freqBar = std::make_unique<FreqBar>(i); // Create a unique pointer to a FreqBar object
        freqBar->setBounds(0, 0, 1024, 500); // Set the bounds of the FreqBar object
        freqBar->setCurrentIndex(audioProcessor.getBinMap()[i]); // Set the current index of the FreqBar object
        addAndMakeVisible(*freqBar); // Add the FreqBar object to the display
        freqBars.push_back(std::move(freqBar)); // Move the unique pointer into the vector
    }
    startTimerHz(30); 
}

FreqDisplay::~FreqDisplay()
{
    stopTimer();
}

void FreqDisplay::paint (juce::Graphics& g)
{
    // Set the color to black and draw an outline around the component
    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void FreqDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}


void FreqDisplay::timerCallback() 
{
    auto binMags = audioProcessor.getBinMags(); // Get the magnitudes of the frequency bins
    for (int i = 0; i < 1024; ++i) {
        float height = binMags[i]; // Get the magnitude of the ith frequency bin
        if (height > 100){ // visual compression
            height = 100 + ((height - 100) / 1.5); 
            if (height > 200) {
                height = 200 + ((height - 200) / 2);
                if (height > 300) {
                    height = 300 + ((height - 300) / 3);
                    if (height > 400) {
                        height = 400 + ((height - 400) / 5);
                        if (height > 500) {
                            height = 500;
                        }
                    }
                }
            };
        }

        // set the highlighted bars
        auto* parentEditor = findParentComponentOfClass<Dumumub0000004AudioProcessorEditor>();
        if (parentEditor != nullptr)
        {
            if (freqBars[i]->getCurrentIndex() >= parentEditor->getSelectionBarLeftBound() && freqBars[i]->getCurrentIndex() <= parentEditor->getSelectionBarRightBound())
            {
                freqBars[i]->setIsHighlighted(true);
            }
            else
            {
                freqBars[i]->setIsHighlighted(false);
            }
        }

        freqBars[i]->setHeight(height); // Set the height of the FreqBar object
    }
    repaint(); // Repaint the display
}

void FreqDisplay::updateBarIndexs()
{
  std::vector<int> binMap = audioProcessor.getBinMap();
    for (int i = 0; i < 1024; ++i) {
        freqBars[i]->setCurrentIndex(binMap[i]);
    }
    repaint();
}


