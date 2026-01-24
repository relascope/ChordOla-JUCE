#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    // addAndMakeVisible (inspectButton);
    //
    // // // this chunk of code instantiates and opens the melatonin inspector
    // inspectButton.onClick = [&] {
    //     if (!inspector)
    //     {
    //         inspector = std::make_unique<melatonin::Inspector> (*this);
    //         inspector->onClose = [this]() { inspector.reset(); };
    //     }
    //
    //     inspector->setVisible (true);
    // };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    startTimerHz(250);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto area = getLocalBounds().reduced (20);
    g.setColour (juce::Colours::white);

    if (chordHistory.isEmpty())
    {
        g.setFont (24.0f);
        g.drawText ("Play something to detect chords...", area, juce::Justification::centred, false);
        return;
    }


    
    float fontSize = 64.0f;
    g.setFont (fontSize);
    auto chordText = chordHistory[0].toJuceString();
    g.drawText (chordText, area.removeFromTop (20), juce::Justification::centred, false);

    float baseFontSize = 40.0f;
    float minFontSize = 14.0f;
    int lineheight = 40;  
    
    for (int i = 1; i < chordHistory.size(); ++i)
    {
        // Calculate font size: top chord is largest, others grow smaller
        float fontSize = std::max (minFontSize, baseFontSize - (i * 4.0f));
        g.setFont (fontSize);

        // Fade out older chords
        float alpha = juce::jmap ((float) i, 0.0f, (float) maxHistorySize, 1.0f, 0.2f);
        g.setOpacity (alpha);

        chordText = chordHistory[i].toJuceString();
        g.drawText (chordText, area.removeFromTop (lineheight), juce::Justification::centred, false);

        // Adjust line height for smaller fonts
        lineheight = std::max (20, lineheight - 2);
    }
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    area.removeFromBottom(50);
    inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));
}

void PluginEditor::timerCallback()
{
    auto currentChord = processorRef.getDetectedChordName();

    // Only update history if the chord is valid and changed
    if (currentChord.rootNote != -1 && currentChord != lastReportedChord)
    {
        chordHistory.insert (0, currentChord);

        if (chordHistory.size() > maxHistorySize)
            chordHistory.removeLast();

        lastReportedChord = currentChord;
    }

    repaint();
}
