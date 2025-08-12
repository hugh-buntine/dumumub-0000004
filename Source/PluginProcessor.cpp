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
    initialiseBinMap();
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

void Dumumub0000004AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Dumumub0000004AudioProcessor::getProgramName (int index)
{
    return {};
}

void Dumumub0000004AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Dumumub0000004AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fftProcessor.prepare (sampleRate, samplesPerBlock, 2, 2);
}

void Dumumub0000004AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Dumumub0000004AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void Dumumub0000004AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    dsp::AudioBlock<float> ab (buffer);
    dsp::ProcessContextReplacing<float> context (ab);
    fftProcessor.process (context);
}


//==============================================================================
bool Dumumub0000004AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Dumumub0000004AudioProcessor::createEditor()
{
    return new Dumumub0000004AudioProcessorEditor (*this);
}

//==============================================================================
void Dumumub0000004AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Create a ValueTree to store the plugin state
    juce::ValueTree state("PluginState");

    // Save the current binMap
    juce::ValueTree binMapTree("BinMap");
    auto binMap = fftProcessor.getBinMap();
    for (size_t i = 0; i < binMap.size(); ++i)
    {
        binMapTree.setProperty("Bin" + juce::String(i), binMap[i], nullptr);
    }
    state.addChild(binMapTree, -1, nullptr);

    // Save the binMapStack
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

    // Convert the ValueTree to XML and write it to the MemoryBlock
    std::unique_ptr<juce::XmlElement> xml = state.createXml();
    copyXmlToBinary(*xml, destData);
}

void Dumumub0000004AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Convert the binary data back to an XML element
    std::unique_ptr<juce::XmlElement> xmlState = getXmlFromBinary(data, sizeInBytes);

    if (xmlState != nullptr && xmlState->hasTagName("PluginState"))
    {
        // Restore the state from the XML
        juce::ValueTree state = juce::ValueTree::fromXml(*xmlState);

        // Restore the binMap
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

        // Restore the binMapStack
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
    std::vector<int> binMap;
    binMap.reserve(1024);
    for (int i = 0; i < 1024; ++i) {
        binMap.push_back(i);
    }
    fftProcessor.setBinMap(binMap);
}

void Dumumub0000004AudioProcessor::editBinMap()
{
    int ss = selectionBarLeft;
    int se = selectionBarRight;
    int ds = destinationBarLeft;
    int de = destinationBarRight;
    int blockSize = se - ss;
    bool movedRight = ds > ss;

    std::vector<int> binMap = fftProcessor.getBinMap();
    std::vector<int> newBinMap(binMap.size(), -1);

    if (ds == ss) // No change
    {
        fftProcessor.setBinMap(newBinMap);
    }
    else
    {
        // SET THE BLOCK
        for (int i = ss; i < se; ++i)
        {
            int index = find(binMap.begin(), binMap.end(), i) - binMap.begin();
            newBinMap[index] = ds + (i - ss);
        }

        // SET THE REST
        for (int i = 0; i < binMap.size(); ++i)
        {
            int value = binMap[i];
            if (((value < ss || value >= de) && movedRight) || ((value < ds || value >= se) && !movedRight)) // if outside of action, dont move
            {
                newBinMap[i] = value;
            }
            else if (value >= ss && value < se) // in the block
            {
                continue;
            }
            else // outside the block
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
    auto binMap = fftProcessor.getBinMap();
    if (binMap.empty())
    {
        return;
    }
    fftProcessor.getBinMapStack().push_back(binMap);
}

void Dumumub0000004AudioProcessor::undoBinMap()
{
    if (fftProcessor.getBinMapStack().size() > 1)
    {
        fftProcessor.getBinMapStack().pop_back();
        fftProcessor.setBinMap(fftProcessor.getBinMapStack().back());
    }
}

