/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Dumumub0000004AudioProcessor::Dumumub0000004AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
                     #endif
                       
#endif
{
    // Initialize frequency bin mapping to default linear mapping (0→0, 1→1, etc.)
    initialiseBinMap();
    // Save initial state to undo stack
    addBinMapToStack();
}

Dumumub0000004AudioProcessor::~Dumumub0000004AudioProcessor()
{
}

//==============================================================================
const juce::String Dumumub0000004AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Dumumub0000004AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Dumumub0000004AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Dumumub0000004AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Dumumub0000004AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Dumumub0000004AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Dumumub0000004AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Dumumub0000004AudioProcessor::setCurrentProgram(int index)
{
}

const juce::String Dumumub0000004AudioProcessor::getProgramName(int index)
{
    return {};
}

void Dumumub0000004AudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void Dumumub0000004AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Initialize FFT processor with 2048-point FFT, stereo input/output
    fftProcessor.prepare(sampleRate, samplesPerBlock, 2, 2);
}

void Dumumub0000004AudioProcessor::releaseResources()
{
    // Release any allocated resources when playback stops
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Dumumub0000004AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
  #else
    // Support mono or stereo configurations only
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // Ensure input and output channel counts match
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Dumumub0000004AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Prevent denormal floating point numbers for performance
    ScopedNoDenormals noDenormals;

    // Process audio through overlapping FFT with frequency bin remapping
    dsp::AudioBlock<float> ab(buffer);
    dsp::ProcessContextReplacing<float> context(ab);
    fftProcessor.process(context);
}

//==============================================================================
bool Dumumub0000004AudioProcessor::hasEditor() const
{
    return true; // Plugin provides a graphical user interface
}

juce::AudioProcessorEditor* Dumumub0000004AudioProcessor::createEditor()
{
    return new Dumumub0000004AudioProcessorEditor(*this);
}

//==============================================================================
void Dumumub0000004AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Serialize plugin state including bin mappings and undo history
    juce::ValueTree state("PluginState");

    // Save current frequency bin mapping
    juce::ValueTree binMapTree("BinMap");
    auto binMap = fftProcessor.getBinMap();
    for (size_t i = 0; i < binMap.size(); ++i)
    {
        binMapTree.setProperty("Bin" + juce::String(i), binMap[i], nullptr);
    }
    state.addChild(binMapTree, -1, nullptr);

    // Save undo history stack
    juce::ValueTree binMapStackTree("BinMapStack");
    auto binMapStack = fftProcessor.getBinMapStack();
    for (size_t i = 0; i < binMapStack.size(); ++i)
    {
        juce::ValueTree stackEntry("StackEntry");
        for (size_t j = 0; j < binMapStack[i].size(); ++j)
        {
            stackEntry.setProperty("Bin" + juce::String(j), binMapStack[i][j], nullptr);
        }
        binMapStackTree.addChild(stackEntry, -1, nullptr);
    }
    state.addChild(binMapStackTree, -1, nullptr);

    // Convert to binary format for host storage
    std::unique_ptr<juce::XmlElement> xml = state.createXml();
    copyXmlToBinary(*xml, destData);
}

void Dumumub0000004AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Deserialize plugin state from host-provided binary data
    std::unique_ptr<juce::XmlElement> xmlState = getXmlFromBinary(data, sizeInBytes);

    if (xmlState != nullptr && xmlState->hasTagName("PluginState"))
    {
        juce::ValueTree state = juce::ValueTree::fromXml(*xmlState);

        // Restore frequency bin mapping
        juce::ValueTree binMapTree = state.getChildWithName("BinMap");
        if (binMapTree.isValid())
        {
            std::vector<int> binMap;
            for (int i = 0; i < binMapTree.getNumProperties(); ++i)
            {
                binMap.push_back(binMapTree.getProperty("Bin" + juce::String(i)));
            }
            fftProcessor.setBinMap(binMap);
        }

        // Restore undo history
        juce::ValueTree binMapStackTree = state.getChildWithName("BinMapStack");
        if (binMapStackTree.isValid())
        {
            std::vector<std::vector<int>> binMapStack;
            for (int i = 0; i < binMapStackTree.getNumChildren(); ++i)
            {
                juce::ValueTree stackEntry = binMapStackTree.getChild(i);
                std::vector<int> stackEntryMap;
                for (int j = 0; j < stackEntry.getNumProperties(); ++j)
                {
                    stackEntryMap.push_back(stackEntry.getProperty("Bin" + juce::String(j)));
                }
                binMapStack.push_back(stackEntryMap);
            }
            fftProcessor.setBinMapStack(binMapStack);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Dumumub0000004AudioProcessor();
}

void Dumumub0000004AudioProcessor::initialiseBinMap()
{
    // Create default linear frequency bin mapping (identity mapping)
    std::vector<int> binMap;
    binMap.reserve(1024);
    for (int i = 0; i < 1024; ++i) {
        binMap.push_back(i);
    }
    fftProcessor.setBinMap(binMap);
}

void Dumumub0000004AudioProcessor::editBinMap()
{
    // Apply frequency bin remapping based on selection and destination ranges
    int ss = selectionBarLeft;   // Source start index
    int se = selectionBarRight;  // Source end index
    int ds = destinationBarLeft; // Destination start index
    int de = destinationBarRight; // Destination end index
    int blockSize = se - ss;
    bool movedRight = ds > ss;

    std::vector<int> binMap = fftProcessor.getBinMap();
    std::vector<int> newBinMap(binMap.size(), -1);

    if (ds == ss) // No change needed
    {
        fftProcessor.setBinMap(newBinMap);
    }
    else
    {
        // Map selected frequency range to destination range
        for (int i = ss; i < se; ++i)
        {
            int index = find(binMap.begin(), binMap.end(), i) - binMap.begin();
            newBinMap[index] = ds + (i - ss);
        }

        // Remap remaining frequencies to accommodate the moved block
        for (int i = 0; i < binMap.size(); ++i)
        {
            int value = binMap[i];
            
            // Check if frequency bin is outside the affected range
            if (((value < ss || value >= de) && movedRight) || 
                ((value < ds || value >= se) && !movedRight))
            {
                newBinMap[i] = value; // No change needed
            }
            else if (value >= ss && value < se) // Part of moved block
            {
                continue; // Already handled above
            }
            else // Needs to shift to make room for moved block
            {
                if (movedRight)
                {
                    newBinMap[i] = value - blockSize;
                }
                else
                {
                    newBinMap[i] = value + blockSize;
                }
            }
        }

        fftProcessor.setBinMap(newBinMap);
        addBinMapToStack();
    }
} 

void Dumumub0000004AudioProcessor::addBinMapToStack()
{
    // Add current bin mapping to undo history
    auto binMap = fftProcessor.getBinMap();
    if (binMap.empty())
    {
        return;
    }
    fftProcessor.getBinMapStack().push_back(binMap);
}

void Dumumub0000004AudioProcessor::undoBinMap()
{
    // Revert to previous bin mapping state if available
    if (fftProcessor.getBinMapStack().size() > 1)
    {
        fftProcessor.getBinMapStack().pop_back();
        fftProcessor.setBinMap(fftProcessor.getBinMapStack().back());
    }
}

