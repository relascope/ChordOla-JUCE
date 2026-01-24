#include "chord.h"
#include "ChordDetector.h"

std::string noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    std::string chordQualityNames[] = {"Minor", "Major", "Suspended", "Dominant", "Dimished5th", "Augmented5th"};

    bool Chord::operator==(const Chord& other) const
    {
        return rootNote==other.rootNote && quality==other.quality && intervals==other.intervals;
    }


juce::String Chord::toJuceString() const 
    {
        std::stringstream ss;
        ss << *this;
        return juce::String (ss.str());
    }

    std::ostream& operator<<(std::ostream& os, const Chord& chord)
    {
        os << noteNames[chord.rootNote];
        
        // //    enum ChordQuality
        // {
        //     Minor,
        //     Major,
        //     Suspended,
        //     Dominant,
        //     Dimished5th,
        //     Augmented5th
        // };

        if (chord.quality==ChordDetector::ChordQuality::Minor)
            os << "m";

        if (chord.quality==ChordDetector::ChordQuality::Suspended)
            os << "sus";
        
        else if (chord.quality==ChordDetector::ChordQuality::Dimished5th)
            os << "°";
        else if (chord.quality==ChordDetector::ChordQuality::Augmented5th)
            os << "+";

        if (chord.intervals!=0)
            os << chord.intervals;
        
        return os;
    }
