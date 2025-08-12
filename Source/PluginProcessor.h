/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../JuceLibraryCode/JuceHeader.h"
#include "OverlappingFFTProcessor.h"


/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "OverlappingFFTProcessor.h"

/**
 * Custom FFT processor for real-time frequency bin manipulation.
 * 
 * Extends OverlappingFFTProcessor to perform spectral analysis and allow
 * real-time remapping of frequency bins. Provides 2048-point FFT with 50%
 * overlap, displaying magnitude data for the first 1024 bins (0-10kHz).
 */
class MyProcessor : public OverlappingFFTProcessor
{
    // OverlappingFFTProcessor constructor parameters:
    // x =  9 means 256 frequency bins    (2^9)
    // x = 10 means 512 frequency bins    (2^10)
    // x = 11 means 1024 frequency bins   (2^11)
    // x = 12 means 2048 frequency bins   (2^12)
    // x = 13 means 4096 frequency bins   (2^13)
    // 
    // y = 1 means 50% overlap
    // y = 2 means 75% overlap
    // y = 3 means 87.5% overlap
    // y = 4 means 93.75% overlap
    // y = 5 means 96.875% overlap
    // 
    // Currently: 2048 bins but only displaying first 1024 (0Hz - 10kHz)
    
public:
    /** Constructor - initializes with 2048 bins and 50% overlap */
    MyProcessor() : OverlappingFFTProcessor(12, 1) {}
    
    /** Destructor */
    ~MyProcessor() {}
    
    /** @returns FFT size (number of samples per frame) */
    int getFFTSize() { return fftSize; }
    
    /** @returns Current frequency bin magnitudes for display */
    std::array<float, 1024> getBinMags() { return binMags; }
    
    /** @returns Current frequency bin mapping */
    std::vector<int> getBinMap() { return binMap; }
    
    /** Sets new frequency bin mapping */
    void setBinMap(std::vector<int> newBinMap) { binMap = newBinMap; }
    
    /** @returns Reference to bin mapping history stack */
    std::vector<std::vector<int>>& getBinMapStack() { return binMapStack; }
    
    /** Sets bin mapping history stack (for undo functionality) */
    void setBinMapStack(std::vector<std::vector<int>> newBinMapStack) { binMapStack = newBinMapStack; }

private:
    /**
     * Core FFT processing with frequency bin remapping.
     * 
     * Performs forward FFT, applies frequency-dependent scaling for visualization,
     * remaps frequency bins according to current binMap, then performs inverse FFT.
     * Updates magnitude data for real-time display.
     */
    void processFrameInBuffer(const int maxNumChannels) override
    {
        // Perform forward FFT on all channels
        for (int ch = 0; ch < maxNumChannels; ++ch)
        {
            fft.performRealOnlyForwardTransform(fftInOutBuffer.getWritePointer(ch), true);
        }
            
        // Apply frequency-dependent scaling for better visualization
        int numBins = fftSize / 2 + 1;
        for (int i = 0; i < numBins; ++i)
        {
            // Extract real and imaginary components for both channels
            float realLeft = fftInOutBuffer.getReadPointer(0)[i * 2];
            float imagLeft = fftInOutBuffer.getReadPointer(0)[i * 2 + 1];
            float realRight = fftInOutBuffer.getReadPointer(1)[i * 2];
            float imagRight = fftInOutBuffer.getReadPointer(1)[i * 2 + 1];

            // Apply power-law scaling (emphasizes higher frequencies)
            float scaleFactor = std::pow(i + 1, 1.0f);
    
            // Apply scaling to all channels
            fftInOutBuffer.getWritePointer(0)[i * 2] = realLeft * scaleFactor;
            fftInOutBuffer.getWritePointer(0)[i * 2 + 1] = imagLeft * scaleFactor;
            fftInOutBuffer.getWritePointer(1)[i * 2] = realRight * scaleFactor;
            fftInOutBuffer.getWritePointer(1)[i * 2 + 1] = imagRight * scaleFactor;
        }
    
        // Calculate magnitudes for display (first 1024 bins only)
        for (int i = 0; i < 1024; ++i)
        {
            float real = fftInOutBuffer.getWritePointer(0)[i * 2];
            float imag = fftInOutBuffer.getWritePointer(0)[i * 2 + 1];
            binMags[i] = std::sqrt(real * real + imag * imag);
        }

        // Apply frequency bin remapping
        juce::AudioBuffer<float> fftInOutBufferCopy;
        fftInOutBufferCopy.setSize(fftInOutBuffer.getNumChannels(), fftInOutBuffer.getNumSamples());
        
        // Create copy of current FFT data
        for (int ch = 0; ch < fftInOutBuffer.getNumChannels(); ++ch) {
            fftInOutBufferCopy.copyFrom(ch, 0, fftInOutBuffer, ch, 0, fftInOutBuffer.getNumSamples());
        }

        // Remap frequency bins according to binMap
        for (int i = 0; i < 1024; ++i)
        {
            int newBinIndex = binMap[i];
            // Copy complex data (real and imaginary) for both channels
            fftInOutBuffer.getWritePointer(0)[newBinIndex * 2] = fftInOutBufferCopy.getReadPointer(0)[i * 2];
            fftInOutBuffer.getWritePointer(0)[newBinIndex * 2 + 1] = fftInOutBufferCopy.getReadPointer(0)[i * 2 + 1];
            fftInOutBuffer.getWritePointer(1)[newBinIndex * 2] = fftInOutBufferCopy.getReadPointer(1)[i * 2];
            fftInOutBuffer.getWritePointer(1)[newBinIndex * 2 + 1] = fftInOutBufferCopy.getReadPointer(1)[i * 2 + 1];
        }
        
        // Remove scaling before inverse FFT
        for (int i = 0; i < numBins; ++i)
        {
            float scaleFactor = std::pow(i + 1, 1.0f);
            fftInOutBuffer.getWritePointer(0)[i * 2] /= scaleFactor;
            fftInOutBuffer.getWritePointer(0)[i * 2 + 1] /= scaleFactor;
            fftInOutBuffer.getWritePointer(1)[i * 2] /= scaleFactor;
            fftInOutBuffer.getWritePointer(1)[i * 2 + 1] /= scaleFactor;
        }
    
        // Perform inverse FFT to convert back to time domain
        for (int ch = 0; ch < maxNumChannels; ++ch) 
        {
            fft.performRealOnlyInverseTransform(fftInOutBuffer.getWritePointer(ch));
        }
    }

    // Member variables
    std::array<float, 1024> binMags;                    ///< Frequency bin magnitudes for display
    std::vector<int> binMap;                            ///< Current frequency bin mapping
    std::vector<std::vector<int>> binMapStack;          ///< History stack for undo functionality
};

//==============================================================================
/**
 * Main audio processor for the dumumub-0000004 frequency manipulation plugin.
 * 
 * Handles real-time audio processing using spectral analysis and frequency bin
 * remapping. Manages UI state for selection and destination bars, provides
 * undo functionality, and maintains bin mapping configuration.
 */
class Dumumub0000004AudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    /** Constructor - initializes FFT processor and bin mapping */
    Dumumub0000004AudioProcessor();
    
    /** Destructor */
    ~Dumumub0000004AudioProcessor() override;

    //==============================================================================
    // AudioProcessor interface implementation
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
   #endif

    /** Main audio processing callback - applies FFT and bin remapping */
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    // Editor management
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    // Plugin information
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    // Program management (unused)
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    // State management
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // Public interface for UI components
    
    /** @returns Current frequency bin magnitudes for spectrum display */
    std::array<float, 1024> getBinMags() { return fftProcessor.getBinMags(); }

    /** Initializes frequency bin mapping to identity (no remapping) */
    void initialiseBinMap();
    
    /** @returns Current frequency bin mapping */
    std::vector<int> getBinMap() { return fftProcessor.getBinMap(); }
    
    /** Applies frequency bin remapping based on current UI selection */
    void editBinMap();

    // UI state management for selection and destination bars
    void setSelectionBarLeft(int left) { selectionBarLeft = left; }
    void setSelectionBarRight(int right) { selectionBarRight = right; }
    void setDestinationBarLeft(int left) { destinationBarLeft = left; }
    void setDestinationBarRight(int right) { destinationBarRight = right; }

    /** Saves current bin map to history stack for undo functionality */
    void addBinMapToStack();
    
    /** Restores previous bin map from history stack */
    void undoBinMap();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dumumub0000004AudioProcessor)
    
    MyProcessor fftProcessor;  ///< Custom FFT processor for spectral manipulation

    // UI state variables for frequency range selection
    int selectionBarLeft;      ///< Left boundary of frequency selection
    int selectionBarRight;     ///< Right boundary of frequency selection  
    int destinationBarLeft;    ///< Left boundary of destination range
    int destinationBarRight;   ///< Right boundary of destination range
};
