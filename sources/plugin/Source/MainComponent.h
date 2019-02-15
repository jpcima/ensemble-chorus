/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "ensemble_chorus.h"
class EnsembleChorusAudioProcessor;
class ModulatorVisu;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainComponent  : public Component,
                       public Slider::Listener,
                       public ComboBox::Listener,
                       public Button::Listener
{
public:
    //==============================================================================
    MainComponent (EnsembleChorusAudioProcessor &p);
    ~MainComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setModulationValues(const float slow[6], const float fast[6]);
    void updateDisplayWithEcp(ec_parameter p, float value);
    void updateDelayRange();
    void updateDelayDisplay();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    EnsembleChorusAudioProcessor *processor_ = nullptr;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Slider> dl_mod_range;
    std::unique_ptr<Slider> dl_slow_rate;
    std::unique_ptr<Slider> dl_fast_rate;
    std::unique_ptr<Slider> dl_slow_rand;
    std::unique_ptr<Slider> dl_fast_rand;
    std::unique_ptr<Slider> dl_lpf_cutoff;
    std::unique_ptr<Slider> dl_lpf_q;
    std::unique_ptr<Slider> sl_delay;
    std::unique_ptr<Slider> sl_wet;
    std::unique_ptr<Slider> sl_dry;
    std::unique_ptr<Slider> sl_gain_in;
    std::unique_ptr<Slider> sl_gain_out;
    std::unique_ptr<ComboBox> cb_nstages;
    std::unique_ptr<Slider> sl_depth1;
    std::unique_ptr<Slider> sl_depth2;
    std::unique_ptr<Slider> sl_depth3;
    std::unique_ptr<Slider> sl_depth4;
    std::unique_ptr<Slider> sl_depth5;
    std::unique_ptr<Slider> sl_depth6;
    std::unique_ptr<Slider> sl_phase1;
    std::unique_ptr<Slider> sl_phase2;
    std::unique_ptr<Slider> sl_phase3;
    std::unique_ptr<Slider> sl_phase4;
    std::unique_ptr<Slider> sl_phase5;
    std::unique_ptr<Slider> sl_phase6;
    std::unique_ptr<TextButton> btn_enable1;
    std::unique_ptr<TextButton> btn_enable2;
    std::unique_ptr<TextButton> btn_enable3;
    std::unique_ptr<TextButton> btn_enable4;
    std::unique_ptr<TextButton> btn_enable5;
    std::unique_ptr<TextButton> btn_enable6;
    std::unique_ptr<TextButton> btn_routeL1;
    std::unique_ptr<TextButton> btn_routeL2;
    std::unique_ptr<TextButton> btn_routeL3;
    std::unique_ptr<TextButton> btn_routeL4;
    std::unique_ptr<TextButton> btn_routeL5;
    std::unique_ptr<TextButton> btn_routeL6;
    std::unique_ptr<TextButton> btn_routeR1;
    std::unique_ptr<TextButton> btn_routeR2;
    std::unique_ptr<TextButton> btn_routeR3;
    std::unique_ptr<TextButton> btn_routeR4;
    std::unique_ptr<TextButton> btn_routeR5;
    std::unique_ptr<TextButton> btn_routeR6;
    std::unique_ptr<ComboBox> cb_slow_wave;
    std::unique_ptr<ComboBox> cb_fast_wave;
    std::unique_ptr<TextButton> btn_stereo;
    std::unique_ptr<TextButton> btn_mono;
    std::unique_ptr<ToggleButton> tick_bypass;
    std::unique_ptr<Label> label;
    std::unique_ptr<Label> label2;
    std::unique_ptr<Label> label3;
    std::unique_ptr<Label> label4;
    std::unique_ptr<Label> label5;
    std::unique_ptr<Label> label6;
    std::unique_ptr<Label> label7;
    std::unique_ptr<Label> label8;
    std::unique_ptr<Label> label9;
    std::unique_ptr<ModulatorVisu> visu_mod1;
    std::unique_ptr<ModulatorVisu> visu_mod2;
    std::unique_ptr<ModulatorVisu> visu_mod3;
    std::unique_ptr<ModulatorVisu> visu_mod4;
    std::unique_ptr<ModulatorVisu> visu_mod5;
    std::unique_ptr<ModulatorVisu> visu_mod6;
    std::unique_ptr<Slider> dl_aa_cutoff;
    std::unique_ptr<Label> label10;
    std::unique_ptr<Label> lbl_delay;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
