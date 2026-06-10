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
    // Initialize frequency bars for 1024-bin FFT display
    freqBars.reserve(1024); 
    for (int i = 0; i < 1024; ++i) {
        auto freqBar = std::make_unique<FreqBar>(i);
        freqBar->setBounds(0, 0, 1024, 500);
        freqBar->setCurrentIndex(audioProcessor.getBinMap()[i]);
        addAndMakeVisible(*freqBar);
        freqBars.push_back(std::move(freqBar));
    }
    
    // Start 30 Hz refresh timer for real-time spectrum visualization
    startTimerHz(30); 
}

FreqDisplay::~FreqDisplay()
{
    stopTimer();
}

void FreqDisplay::paint(juce::Graphics& g)
{
    // Draw component border for visual feedback
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 1);
}

void FreqDisplay::resized()
{
    // No child component resizing needed - FreqBars manage their own positioning
}

void FreqDisplay::timerCallback() 
{
    // Update spectrum display with current magnitude data
    auto binMags = audioProcessor.getBinMags();
    
    for (int i = 0; i < 1024; ++i) {
        float height = binMags[i];
        
        // Apply progressive visual compression for better display scaling
        // Reduces extreme peaks while maintaining detail in lower amplitudes
        if (height > 100) {
            height = 100 + ((height - 100) / 1.5); 
            if (height > 200) {
                height = 200 + ((height - 200) / 2);
                if (height > 300) {
                    height = 300 + ((height - 300) / 3);
                    if (height > 400) {
                        height = 400 + ((height - 400) / 5);
                        if (height > 500) {
                            height = 500; // Maximum display height
                        }
                    }
                }
            }
        }

        // Update highlight state based on current selection range
        auto* parentEditor = findParentComponentOfClass<Dumumub0000004AudioProcessorEditor>();
        if (parentEditor != nullptr)
        {
            bool isInSelection = (freqBars[i]->getCurrentIndex() >= parentEditor->getSelectionBarLeftBound() && 
                                 freqBars[i]->getCurrentIndex() <= parentEditor->getSelectionBarRightBound());
            freqBars[i]->setIsHighlighted(isInSelection);
        }

        freqBars[i]->setHeight(height);
    }
    
    // Trigger visual update
    repaint();
}

void FreqDisplay::updateBarIndexs()
{
    // Synchronize frequency bar positions with current bin mapping
    // Called when processor bin map changes due to user interactions
    std::vector<int> binMap = audioProcessor.getBinMap();
    for (int i = 0; i < 1024; ++i) {
        freqBars[i]->setCurrentIndex(binMap[i]);
    }
    repaint();
}


