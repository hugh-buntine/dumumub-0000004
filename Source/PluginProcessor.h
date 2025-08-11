/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../JuceLibraryCode/JuceHeader.h"
#include "OverlappingFFTProcessor.h"


class MyProcessor : public OverlappingFFTProcessor
{
  // OverlappingFFTProcessor (x, y)
  // x =  9 means 256 frequency bins
  // x = 10 means 512 frequency bins
  // x = 11 means 1024 frequency bins
  // x = 12 means 2048 frequency bins
  // x = 13 means 4096 frequency bins
  // y = 1 means 50% overlap
  // y = 2 means 75% overlap
  // y = 3 means 87.5% overlap
  // y = 4 means 93.75% overlap
  // y = 5 means 96.875% overlap
  // CURRENTLY GETTING 2048 BINS BUT ONLY LOOKING AT THE FIRST 1024 (0HZ - 10KHZ)
public:
    MyProcessor () : OverlappingFFTProcessor (12, 1)
    {}
    ~MyProcessor() {}
    int getFFTSize() { return fftSize; }
    
    std::array<float, 1024> getBinMags () { return binMags; }
    std::vector<int> getBinMap() { return binMap; }
    void setBinMap(std::vector<int> newBinMap) {  binMap = newBinMap;  }
    std::vector<std::vector<int>>& getBinMapStack() { return binMapStack; }
    void setBinMapStack(std::vector<std::vector<int>> newBinMapStack) { binMapStack = newBinMapStack; }

private:
    void processFrameInBuffer (const int maxNumChannels) override
    {
        // DO FFT
        for (int ch = 0; ch < maxNumChannels; ++ch)
        {
            fft.performRealOnlyForwardTransform (fftInOutBuffer.getWritePointer (ch), true);
        }
            
        
        // SCALE BINS
        int numBins = fftSize / 2 + 1; // Number of frequency bins
        for (int i = 0; i < numBins; ++i)
        {
            float realLeft = fftInOutBuffer.getReadPointer(0)[i * 2];
            float imagLeft = fftInOutBuffer.getReadPointer(0)[i * 2 + 1];
    
            float realRight = fftInOutBuffer.getReadPointer(1)[i * 2];
            float imagRight = fftInOutBuffer.getReadPointer(1)[i * 2 + 1];

            float scaleFactor = std::pow(i + 1, 1.0f);
    
            // update all bins with scale
            fftInOutBuffer.getWritePointer(0)[i * 2] = realLeft * scaleFactor;
            fftInOutBuffer.getWritePointer(0)[i * 2 + 1] = imagLeft * scaleFactor;
            fftInOutBuffer.getWritePointer(1)[i * 2] = realRight * scaleFactor;
            fftInOutBuffer.getWritePointer(1)[i * 2 + 1] = imagRight * scaleFactor;
        }
    
        // SET THE GOOD BINS FOR THE DISPLAY
        for (int i = 0; i < 1024; ++i)
        {
            binMags[i] = std::sqrt(fftInOutBuffer.getWritePointer(0)[i * 2] * fftInOutBuffer.getWritePointer(0)[i * 2] + fftInOutBuffer.getWritePointer(0)[i * 2 + 1] * fftInOutBuffer.getWritePointer(0)[i * 2 + 1]);
        }

        // MOVE BINS AROUND HERE
        // make a copy
        juce::AudioBuffer<float> fftInOutBufferCopy;
        fftInOutBufferCopy.setSize(fftInOutBuffer.getNumChannels(), fftInOutBuffer.getNumSamples());
        for (int ch = 0; ch < fftInOutBuffer.getNumChannels(); ++ch) {
            fftInOutBufferCopy.copyFrom(ch, 0, fftInOutBuffer, ch, 0, fftInOutBuffer.getNumSamples());
        }

        // move bins around depending on the binMap
        for (int i = 0; i < 1024; ++i)
        {
            int newBinIndex = binMap[i];
            fftInOutBuffer.getWritePointer(0)[newBinIndex * 2] = fftInOutBufferCopy.getReadPointer(0)[i * 2];
            fftInOutBuffer.getWritePointer(0)[newBinIndex * 2 + 1] = fftInOutBufferCopy.getReadPointer(0)[i * 2 + 1];
            fftInOutBuffer.getWritePointer(1)[newBinIndex * 2] = fftInOutBufferCopy.getReadPointer(1)[i * 2];
            fftInOutBuffer.getWritePointer(1)[newBinIndex * 2 + 1] = fftInOutBufferCopy.getReadPointer(1)[i * 2 + 1];
        }
        
    
        // UNSCALE BINS
        for (int i = 0; i < numBins; ++i)
        {
            float scaleFactor = std::pow(i + 1, 1.0f);
    
            fftInOutBuffer.getWritePointer(0)[i * 2] /= scaleFactor;
            fftInOutBuffer.getWritePointer(0)[i * 2 + 1] /= scaleFactor;
            fftInOutBuffer.getWritePointer(1)[i * 2] /= scaleFactor;
            fftInOutBuffer.getWritePointer(1)[i * 2 + 1] /= scaleFactor;
        }
    
        // DO IFFT
        for (int ch = 0; ch < maxNumChannels; ++ch) 
        {
            fft.performRealOnlyInverseTransform (fftInOutBuffer.getWritePointer (ch));
        }
            
    }

    // ATTRIBUTES
    std::array<float, 1024> binMags;
    std::vector<int> binMap;
    std::vector<std::vector<int>> binMapStack;
};

//==============================================================================


//==============================================================================
/**
*/
class Dumumub0000004AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Dumumub0000004AudioProcessor();
    ~Dumumub0000004AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

    std::array<float, 1024> getBinMags () { return fftProcessor.getBinMags(); }

    void initialiseBinMap();
    std::vector<int> getBinMap() { return fftProcessor.getBinMap(); }
    void editBinMap();

    void setSelectionBarLeft(int left) { selectionBarLeft = left; }
    void setSelectionBarRight(int right) { selectionBarRight = right; }
    void setDestinationBarLeft(int left) { destinationBarLeft = left; }
    void setDestinationBarRight(int right) { destinationBarRight = right; }

    void addBinMapToStack();
    void undoBinMap();

    


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dumumub0000004AudioProcessor)
    MyProcessor fftProcessor;

    int selectionBarLeft;
    int selectionBarRight;
    int destinationBarLeft;
    int destinationBarRight;

    
};
