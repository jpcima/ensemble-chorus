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

//[Headers] You can add your own extra header files here...
#include "PluginProcessor.h"
#include "ModulatorVisu.h"
#include "BinaryData.h"
#include <cassert>
//[/Headers]

#include "MainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent (EnsembleChorusAudioProcessor &p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    processor_ = &p;
    //[/Constructor_pre]

    dl_mod_range.reset (new Slider ("new slider"));
    addAndMakeVisible (dl_mod_range.get());
    dl_mod_range->setRange (0, 1, 0);
    dl_mod_range->setSliderStyle (Slider::Rotary);
    dl_mod_range->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dl_mod_range->addListener (this);

    dl_mod_range->setBounds (380, 125, 40, 40);

    dl_slow_rate.reset (new Slider ("new slider"));
    addAndMakeVisible (dl_slow_rate.get());
    dl_slow_rate->setRange (0, 1, 0);
    dl_slow_rate->setSliderStyle (Slider::Rotary);
    dl_slow_rate->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dl_slow_rate->addListener (this);

    dl_slow_rate->setBounds (380, 210, 45, 45);

    dl_fast_rate.reset (new Slider ("new slider"));
    addAndMakeVisible (dl_fast_rate.get());
    dl_fast_rate->setRange (0, 1, 0);
    dl_fast_rate->setSliderStyle (Slider::Rotary);
    dl_fast_rate->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dl_fast_rate->addListener (this);

    dl_fast_rate->setBounds (500, 210, 45, 45);

    dl_slow_rand.reset (new Slider ("new slider"));
    addAndMakeVisible (dl_slow_rand.get());
    dl_slow_rand->setRange (0, 1, 0);
    dl_slow_rand->setSliderStyle (Slider::Rotary);
    dl_slow_rand->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dl_slow_rand->addListener (this);

    dl_slow_rand->setBounds (425, 240, 35, 35);

    dl_fast_rand.reset (new Slider ("new slider"));
    addAndMakeVisible (dl_fast_rand.get());
    dl_fast_rand->setRange (0, 1, 0);
    dl_fast_rand->setSliderStyle (Slider::Rotary);
    dl_fast_rand->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dl_fast_rand->addListener (this);

    dl_fast_rand->setBounds (545, 240, 35, 35);

    dl_lpf_cutoff.reset (new Slider ("new slider"));
    addAndMakeVisible (dl_lpf_cutoff.get());
    dl_lpf_cutoff->setRange (0, 1, 0);
    dl_lpf_cutoff->setSliderStyle (Slider::Rotary);
    dl_lpf_cutoff->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dl_lpf_cutoff->addListener (this);

    dl_lpf_cutoff->setBounds (160, 235, 40, 40);

    dl_lpf_q.reset (new Slider ("new slider"));
    addAndMakeVisible (dl_lpf_q.get());
    dl_lpf_q->setRange (0, 1, 0);
    dl_lpf_q->setSliderStyle (Slider::Rotary);
    dl_lpf_q->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dl_lpf_q->addListener (this);

    dl_lpf_q->setBounds (255, 235, 40, 40);

    sl_delay.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_delay.get());
    sl_delay->setRange (0, 1, 0);
    sl_delay->setSliderStyle (Slider::LinearVertical);
    sl_delay->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_delay->addListener (this);

    sl_delay->setBounds (455, 30, 25, 125);

    sl_wet.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_wet.get());
    sl_wet->setRange (0, 1, 0);
    sl_wet->setSliderStyle (Slider::LinearVertical);
    sl_wet->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_wet->addListener (this);

    sl_wet->setBounds (500, 30, 25, 95);

    sl_dry.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_dry.get());
    sl_dry->setRange (0, 1, 0);
    sl_dry->setSliderStyle (Slider::LinearVertical);
    sl_dry->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_dry->addListener (this);

    sl_dry->setBounds (525, 30, 25, 95);

    sl_gain_in.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_gain_in.get());
    sl_gain_in->setRange (0, 3, 0);
    sl_gain_in->setSliderStyle (Slider::LinearVertical);
    sl_gain_in->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_gain_in->addListener (this);

    sl_gain_in->setBounds (560, 30, 25, 95);

    sl_gain_out.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_gain_out.get());
    sl_gain_out->setRange (0, 3, 0);
    sl_gain_out->setSliderStyle (Slider::LinearVertical);
    sl_gain_out->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_gain_out->addListener (this);

    sl_gain_out->setBounds (585, 30, 25, 95);

    cb_nstages.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_nstages.get());
    cb_nstages->setEditableText (false);
    cb_nstages->setJustificationType (Justification::centredLeft);
    cb_nstages->setTextWhenNothingSelected (String());
    cb_nstages->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_nstages->addListener (this);

    cb_nstages->setBounds (370, 30, 65, 20);

    sl_depth1.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_depth1.get());
    sl_depth1->setRange (0, 1, 0);
    sl_depth1->setSliderStyle (Slider::LinearBar);
    sl_depth1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_depth1->addListener (this);

    sl_depth1->setBounds (300, 80, 60, 25);

    sl_depth2.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_depth2.get());
    sl_depth2->setRange (0, 1, 0);
    sl_depth2->setSliderStyle (Slider::LinearBar);
    sl_depth2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_depth2->addListener (this);

    sl_depth2->setBounds (300, 105, 60, 25);

    sl_depth3.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_depth3.get());
    sl_depth3->setRange (0, 1, 0);
    sl_depth3->setSliderStyle (Slider::LinearBar);
    sl_depth3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_depth3->addListener (this);

    sl_depth3->setBounds (300, 130, 60, 25);

    sl_depth4.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_depth4.get());
    sl_depth4->setRange (0, 1, 0);
    sl_depth4->setSliderStyle (Slider::LinearBar);
    sl_depth4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_depth4->addListener (this);

    sl_depth4->setBounds (300, 155, 60, 25);

    sl_depth5.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_depth5.get());
    sl_depth5->setRange (0, 1, 0);
    sl_depth5->setSliderStyle (Slider::LinearBar);
    sl_depth5->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_depth5->addListener (this);

    sl_depth5->setBounds (300, 180, 60, 25);

    sl_depth6.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_depth6.get());
    sl_depth6->setRange (0, 1, 0);
    sl_depth6->setSliderStyle (Slider::LinearBar);
    sl_depth6->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_depth6->addListener (this);

    sl_depth6->setBounds (300, 205, 60, 25);

    sl_phase1.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_phase1.get());
    sl_phase1->setRange (0, 359, 1);
    sl_phase1->setSliderStyle (Slider::LinearBar);
    sl_phase1->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sl_phase1->addListener (this);

    sl_phase1->setBounds (65, 80, 105, 25);

    sl_phase2.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_phase2.get());
    sl_phase2->setRange (0, 359, 1);
    sl_phase2->setSliderStyle (Slider::LinearBar);
    sl_phase2->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sl_phase2->addListener (this);

    sl_phase2->setBounds (65, 105, 105, 25);

    sl_phase3.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_phase3.get());
    sl_phase3->setRange (0, 359, 1);
    sl_phase3->setSliderStyle (Slider::LinearBar);
    sl_phase3->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sl_phase3->addListener (this);

    sl_phase3->setBounds (65, 130, 105, 25);

    sl_phase4.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_phase4.get());
    sl_phase4->setRange (0, 359, 1);
    sl_phase4->setSliderStyle (Slider::LinearBar);
    sl_phase4->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sl_phase4->addListener (this);

    sl_phase4->setBounds (65, 155, 105, 25);

    sl_phase5.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_phase5.get());
    sl_phase5->setRange (0, 359, 1);
    sl_phase5->setSliderStyle (Slider::LinearBar);
    sl_phase5->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sl_phase5->addListener (this);

    sl_phase5->setBounds (65, 180, 105, 25);

    sl_phase6.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_phase6.get());
    sl_phase6->setRange (0, 359, 1);
    sl_phase6->setSliderStyle (Slider::LinearBar);
    sl_phase6->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sl_phase6->addListener (this);

    sl_phase6->setBounds (65, 205, 105, 25);

    btn_enable1.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_enable1.get());
    btn_enable1->setButtonText (TRANS("1"));
    btn_enable1->addListener (this);

    btn_enable1->setBounds (20, 80, 25, 25);

    btn_enable2.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_enable2.get());
    btn_enable2->setButtonText (TRANS("2"));
    btn_enable2->addListener (this);

    btn_enable2->setBounds (20, 105, 25, 25);

    btn_enable3.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_enable3.get());
    btn_enable3->setButtonText (TRANS("3"));
    btn_enable3->addListener (this);

    btn_enable3->setBounds (20, 130, 25, 25);

    btn_enable4.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_enable4.get());
    btn_enable4->setButtonText (TRANS("4"));
    btn_enable4->addListener (this);

    btn_enable4->setBounds (20, 155, 25, 25);

    btn_enable5.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_enable5.get());
    btn_enable5->setButtonText (TRANS("5"));
    btn_enable5->addListener (this);

    btn_enable5->setBounds (20, 180, 25, 25);

    btn_enable6.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_enable6.get());
    btn_enable6->setButtonText (TRANS("6"));
    btn_enable6->addListener (this);

    btn_enable6->setBounds (20, 205, 25, 25);

    btn_routeL1.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeL1.get());
    btn_routeL1->setButtonText (TRANS("L"));
    btn_routeL1->addListener (this);

    btn_routeL1->setBounds (180, 80, 25, 25);

    btn_routeL2.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeL2.get());
    btn_routeL2->setButtonText (TRANS("L"));
    btn_routeL2->addListener (this);

    btn_routeL2->setBounds (180, 105, 25, 25);

    btn_routeL3.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeL3.get());
    btn_routeL3->setButtonText (TRANS("L"));
    btn_routeL3->addListener (this);

    btn_routeL3->setBounds (180, 130, 25, 25);

    btn_routeL4.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeL4.get());
    btn_routeL4->setButtonText (TRANS("L"));
    btn_routeL4->addListener (this);

    btn_routeL4->setBounds (180, 155, 25, 25);

    btn_routeL5.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeL5.get());
    btn_routeL5->setButtonText (TRANS("L"));
    btn_routeL5->addListener (this);

    btn_routeL5->setBounds (180, 180, 25, 25);

    btn_routeL6.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeL6.get());
    btn_routeL6->setButtonText (TRANS("L"));
    btn_routeL6->addListener (this);

    btn_routeL6->setBounds (180, 205, 25, 25);

    btn_routeR1.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeR1.get());
    btn_routeR1->setButtonText (TRANS("R"));
    btn_routeR1->addListener (this);

    btn_routeR1->setBounds (210, 80, 25, 25);

    btn_routeR2.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeR2.get());
    btn_routeR2->setButtonText (TRANS("R"));
    btn_routeR2->addListener (this);

    btn_routeR2->setBounds (210, 105, 25, 25);

    btn_routeR3.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeR3.get());
    btn_routeR3->setButtonText (TRANS("R"));
    btn_routeR3->addListener (this);

    btn_routeR3->setBounds (210, 130, 25, 25);

    btn_routeR4.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeR4.get());
    btn_routeR4->setButtonText (TRANS("R"));
    btn_routeR4->addListener (this);

    btn_routeR4->setBounds (210, 155, 25, 25);

    btn_routeR5.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeR5.get());
    btn_routeR5->setButtonText (TRANS("R"));
    btn_routeR5->addListener (this);

    btn_routeR5->setBounds (210, 180, 25, 25);

    btn_routeR6.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_routeR6.get());
    btn_routeR6->setButtonText (TRANS("R"));
    btn_routeR6->addListener (this);

    btn_routeR6->setBounds (210, 205, 25, 25);

    cb_slow_wave.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_slow_wave.get());
    cb_slow_wave->setEditableText (false);
    cb_slow_wave->setJustificationType (Justification::centredLeft);
    cb_slow_wave->setTextWhenNothingSelected (String());
    cb_slow_wave->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_slow_wave->addListener (this);

    cb_slow_wave->setBounds (435, 210, 55, 25);

    cb_fast_wave.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_fast_wave.get());
    cb_fast_wave->setEditableText (false);
    cb_fast_wave->setJustificationType (Justification::centredLeft);
    cb_fast_wave->setTextWhenNothingSelected (String());
    cb_fast_wave->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_fast_wave->addListener (this);

    cb_fast_wave->setBounds (555, 210, 55, 25);

    btn_stereo.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_stereo.get());
    btn_stereo->setButtonText (TRANS("Stereo"));
    btn_stereo->setConnectedEdges (Button::ConnectedOnRight);
    btn_stereo->addListener (this);

    btn_stereo->setBounds (500, 155, 55, 25);

    btn_mono.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_mono.get());
    btn_mono->setButtonText (TRANS("Mono"));
    btn_mono->setConnectedEdges (Button::ConnectedOnLeft);
    btn_mono->addListener (this);

    btn_mono->setBounds (555, 155, 55, 25);

    tick_bypass.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (tick_bypass.get());
    tick_bypass->setButtonText (TRANS("Bypass"));
    tick_bypass->addListener (this);

    tick_bypass->setBounds (235, 10, 75, 25);

    label.reset (new Label ("new label",
                            TRANS("Cutoff")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (200, 255, 50, 25);

    label2.reset (new Label ("new label",
                             TRANS("Q")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (295, 255, 50, 25);

    label3.reset (new Label ("new label",
                             TRANS("Rand")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (455, 240, 45, 35);

    label4.reset (new Label ("new label",
                             TRANS("Rand")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (575, 240, 45, 35);

    label5.reset (new Label ("new label",
                             TRANS("Mod range")));
    addAndMakeVisible (label5.get());
    label5->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (365, 155, 75, 25);

    label6.reset (new Label ("new label",
                             TRANS("Wet")));
    addAndMakeVisible (label6.get());
    label6->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (Justification::centred);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label6->setBounds (495, 120, 35, 25);

    label7.reset (new Label ("new label",
                             TRANS("Dry")));
    addAndMakeVisible (label7.get());
    label7->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (Justification::centred);
    label7->setEditable (false, false, false);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label7->setBounds (520, 120, 35, 25);

    label8.reset (new Label ("new label",
                             TRANS("In")));
    addAndMakeVisible (label8.get());
    label8->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (Justification::centred);
    label8->setEditable (false, false, false);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label8->setBounds (555, 120, 35, 25);

    label9.reset (new Label ("new label",
                             TRANS("Out")));
    addAndMakeVisible (label9.get());
    label9->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (Justification::centred);
    label9->setEditable (false, false, false);
    label9->setColour (TextEditor::textColourId, Colours::black);
    label9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label9->setBounds (580, 120, 35, 25);

    visu_mod1.reset (new ModulatorVisu());
    addAndMakeVisible (visu_mod1.get());
    visu_mod1->setName ("new component");

    visu_mod1->setBounds (245, 80, 55, 25);

    visu_mod2.reset (new ModulatorVisu());
    addAndMakeVisible (visu_mod2.get());
    visu_mod2->setName ("new component");

    visu_mod2->setBounds (245, 105, 55, 25);

    visu_mod3.reset (new ModulatorVisu());
    addAndMakeVisible (visu_mod3.get());
    visu_mod3->setName ("new component");

    visu_mod3->setBounds (245, 130, 55, 25);

    visu_mod4.reset (new ModulatorVisu());
    addAndMakeVisible (visu_mod4.get());
    visu_mod4->setName ("new component");

    visu_mod4->setBounds (245, 155, 55, 25);

    visu_mod5.reset (new ModulatorVisu());
    addAndMakeVisible (visu_mod5.get());
    visu_mod5->setName ("new component");

    visu_mod5->setBounds (245, 180, 55, 25);

    visu_mod6.reset (new ModulatorVisu());
    addAndMakeVisible (visu_mod6.get());
    visu_mod6->setName ("new component");

    visu_mod6->setBounds (245, 205, 55, 25);

    dl_aa_cutoff.reset (new Slider ("new slider"));
    addAndMakeVisible (dl_aa_cutoff.get());
    dl_aa_cutoff->setRange (0, 1, 0);
    dl_aa_cutoff->setSliderStyle (Slider::Rotary);
    dl_aa_cutoff->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dl_aa_cutoff->addListener (this);

    dl_aa_cutoff->setBounds (365, 80, 35, 35);

    label10.reset (new Label ("new label",
                              TRANS("Cutoff")));
    addAndMakeVisible (label10.get());
    label10->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (Justification::centred);
    label10->setEditable (false, false, false);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label10->setBounds (395, 85, 50, 25);

    lbl_delay.reset (new Label ("new label",
                                TRANS("888 ms")));
    addAndMakeVisible (lbl_delay.get());
    lbl_delay->setFont (Font (Font::getDefaultMonospacedFontName(), 15.0f, Font::plain).withTypefaceStyle ("Bold"));
    lbl_delay->setJustificationType (Justification::centred);
    lbl_delay->setEditable (false, false, false);
    lbl_delay->setColour (Label::backgroundColourId, Colour (0x0000247f));
    lbl_delay->setColour (Label::textColourId, Colour (0xff3fda00));
    lbl_delay->setColour (TextEditor::textColourId, Colours::black);
    lbl_delay->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_delay->setBounds (443, 155, 49, 25);


    //[UserPreSize]
    for (unsigned i = 0; i < EC_SUPPORTED_NSTAGES_COUNT; ++i) {
        unsigned nstages = EC_NSTAGES_MIN << i;
        cb_nstages->addItem(String(nstages), i + 1);
    }

    PopupMenu *menu_slow_wave = cb_slow_wave->getRootMenu();
    PopupMenu *menu_fast_wave = cb_fast_wave->getRootMenu();
    for (unsigned i = 0; i < EC_LFO_WAVE_COUNT; ++i) {
        Image img;
        switch (i) {
        case 0:
            img = ImageCache::getFromMemory(BinaryData::sine_png, BinaryData::sine_pngSize);
            break;
        case 1:
            img = ImageCache::getFromMemory(BinaryData::square_png, BinaryData::square_pngSize);
            break;
        case 2:
            img = ImageCache::getFromMemory(BinaryData::saw_png, BinaryData::saw_pngSize);
            break;
        case 3:
            img = ImageCache::getFromMemory(BinaryData::ramp_png, BinaryData::ramp_pngSize);
            break;
        case 4:
            img = ImageCache::getFromMemory(BinaryData::triangle_png, BinaryData::triangle_pngSize);
            break;
        default:
            assert(false);
        }
        for (PopupMenu *menu : {menu_slow_wave, menu_fast_wave})
            menu->addItem(i + 1, String(i + 1), true, false, img);
    }

    int radioGroupId = 0;

    ++radioGroupId;
    for (TextButton *btn : {btn_stereo.get(), btn_mono.get()}) {
        btn->setClickingTogglesState(true);
        btn->setRadioGroupId(radioGroupId, dontSendNotification);
    }

    for (TextButton *btn : {
            btn_enable1.get(), btn_enable2.get(), btn_enable3.get(),
            btn_enable4.get(), btn_enable5.get(), btn_enable6.get(),
            btn_routeL1.get(), btn_routeL2.get(), btn_routeL3.get(),
            btn_routeL4.get(), btn_routeL5.get(), btn_routeL6.get(),
            btn_routeR1.get(), btn_routeR2.get(), btn_routeR3.get(),
            btn_routeR4.get(), btn_routeR5.get(), btn_routeR6.get()})
        btn->setClickingTogglesState(true);

    for (Slider *sl : {
            sl_wet.get(), sl_dry.get(), sl_gain_in.get(), sl_gain_out.get()})
        sl->setSkewFactor(0.6);

    for (unsigned i = 0, n = ensemble_chorus_parameter_count(); i < n; ++i)
        updateDisplayWithEcp((ec_parameter)i, p.getEcp((ec_parameter)i));
    //[/UserPreSize]

    setSize (625, 280);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MainComponent::~MainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    dl_mod_range = nullptr;
    dl_slow_rate = nullptr;
    dl_fast_rate = nullptr;
    dl_slow_rand = nullptr;
    dl_fast_rand = nullptr;
    dl_lpf_cutoff = nullptr;
    dl_lpf_q = nullptr;
    sl_delay = nullptr;
    sl_wet = nullptr;
    sl_dry = nullptr;
    sl_gain_in = nullptr;
    sl_gain_out = nullptr;
    cb_nstages = nullptr;
    sl_depth1 = nullptr;
    sl_depth2 = nullptr;
    sl_depth3 = nullptr;
    sl_depth4 = nullptr;
    sl_depth5 = nullptr;
    sl_depth6 = nullptr;
    sl_phase1 = nullptr;
    sl_phase2 = nullptr;
    sl_phase3 = nullptr;
    sl_phase4 = nullptr;
    sl_phase5 = nullptr;
    sl_phase6 = nullptr;
    btn_enable1 = nullptr;
    btn_enable2 = nullptr;
    btn_enable3 = nullptr;
    btn_enable4 = nullptr;
    btn_enable5 = nullptr;
    btn_enable6 = nullptr;
    btn_routeL1 = nullptr;
    btn_routeL2 = nullptr;
    btn_routeL3 = nullptr;
    btn_routeL4 = nullptr;
    btn_routeL5 = nullptr;
    btn_routeL6 = nullptr;
    btn_routeR1 = nullptr;
    btn_routeR2 = nullptr;
    btn_routeR3 = nullptr;
    btn_routeR4 = nullptr;
    btn_routeR5 = nullptr;
    btn_routeR6 = nullptr;
    cb_slow_wave = nullptr;
    cb_fast_wave = nullptr;
    btn_stereo = nullptr;
    btn_mono = nullptr;
    tick_bypass = nullptr;
    label = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    label6 = nullptr;
    label7 = nullptr;
    label8 = nullptr;
    label9 = nullptr;
    visu_mod1 = nullptr;
    visu_mod2 = nullptr;
    visu_mod3 = nullptr;
    visu_mod4 = nullptr;
    visu_mod5 = nullptr;
    visu_mod6 = nullptr;
    dl_aa_cutoff = nullptr;
    label10 = nullptr;
    lbl_delay = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffefefef));

    {
        int x = 0, y = 0, width = 625, height = 480;
        Colour fillColour = Colours::silver;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 4, y = 42, width = 230, height = 4;
        Colour fillColour = Colour (0xff404040);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 370, y = 5, width = 65, height = 25;
        String text (TRANS("Stages"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 440, y = 5, width = 55, height = 25;
        String text (TRANS("Delay"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 500, y = 5, width = 50, height = 25;
        String text (TRANS("Mix"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 560, y = 5, width = 50, height = 25;
        String text (TRANS("Gain"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 370, y = 185, width = 65, height = 25;
        String text (TRANS("Chorus"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 490, y = 185, width = 65, height = 25;
        String text (TRANS("Vibrato"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 30, y = 245, width = 125, height = 25;
        String text (TRANS("Low-pass filter"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 65, y = 55, width = 105, height = 25;
        String text (TRANS("Phase"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 170, y = 55, width = 75, height = 25;
        String text (TRANS("Routing"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 245, y = 55, width = 95, height = 25;
        String text (TRANS("Modulator"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 0, y = 0, width = 230, height = 42;
        String text (TRANS("JPC Ensemble Chorus"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 21.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 0, y = 0, width = 230, height = 42;
        Colour strokeColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 230, y = 4, width = 4, height = 42;
        Colour fillColour = Colour (0xff404040);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 360, y = 55, width = 90, height = 25;
        String text (TRANS("Anti-alias"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (Font::getDefaultSerifFontName(), 18.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        float x = 443.0f, y = 155.0f, width = 49.0f, height = 25.0f;
        Colour fillColour = Colour (0xff00247f);
        Colour strokeColour = Colours::grey;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 3.0f);
        g.setColour (strokeColour);
        g.drawRoundedRectangle (x, y, width, height, 3.0f, 1.0f);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    EnsembleChorusAudioProcessor &processor = *processor_;
    ec_parameter ecp;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == dl_mod_range.get())
    {
        //[UserSliderCode_dl_mod_range] -- add your slider handling code here..
        ecp = ECP_MOD_RANGE;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_dl_mod_range]
    }
    else if (sliderThatWasMoved == dl_slow_rate.get())
    {
        //[UserSliderCode_dl_slow_rate] -- add your slider handling code here..
        ecp = ECP_SLOW_RATE;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_dl_slow_rate]
    }
    else if (sliderThatWasMoved == dl_fast_rate.get())
    {
        //[UserSliderCode_dl_fast_rate] -- add your slider handling code here..
        ecp = ECP_FAST_RATE;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_dl_fast_rate]
    }
    else if (sliderThatWasMoved == dl_slow_rand.get())
    {
        //[UserSliderCode_dl_slow_rand] -- add your slider handling code here..
        ecp = ECP_SLOW_RAND;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_dl_slow_rand]
    }
    else if (sliderThatWasMoved == dl_fast_rand.get())
    {
        //[UserSliderCode_dl_fast_rand] -- add your slider handling code here..
        ecp = ECP_FAST_RAND;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_dl_fast_rand]
    }
    else if (sliderThatWasMoved == dl_lpf_cutoff.get())
    {
        //[UserSliderCode_dl_lpf_cutoff] -- add your slider handling code here..
        ecp = ECP_LPF_CUTOFF;
        float min = ensemble_chorus_parameter_min(ecp);
        float max = ensemble_chorus_parameter_max(ecp);
        processor.setEcp(ecp, min + (max - min) * sliderThatWasMoved->getValue());
        //[/UserSliderCode_dl_lpf_cutoff]
    }
    else if (sliderThatWasMoved == dl_lpf_q.get())
    {
        //[UserSliderCode_dl_lpf_q] -- add your slider handling code here..
        ecp = ECP_LPF_Q;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_dl_lpf_q]
    }
    else if (sliderThatWasMoved == sl_delay.get())
    {
        //[UserSliderCode_sl_delay] -- add your slider handling code here..
        float value = sliderThatWasMoved->getValue();
        ecp = ECP_DELAY;
        processor.setEcp(ecp, value);
        updateDelayDisplay();
        //[/UserSliderCode_sl_delay]
    }
    else if (sliderThatWasMoved == sl_wet.get())
    {
        //[UserSliderCode_sl_wet] -- add your slider handling code here..
        ecp = ECP_MIX_WET;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_sl_wet]
    }
    else if (sliderThatWasMoved == sl_dry.get())
    {
        //[UserSliderCode_sl_dry] -- add your slider handling code here..
        ecp = ECP_MIX_DRY;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_sl_dry]
    }
    else if (sliderThatWasMoved == sl_gain_in.get())
    {
        //[UserSliderCode_sl_gain_in] -- add your slider handling code here..
        ecp = ECP_GAIN_IN;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_sl_gain_in]
    }
    else if (sliderThatWasMoved == sl_gain_out.get())
    {
        //[UserSliderCode_sl_gain_out] -- add your slider handling code here..
        ecp = ECP_GAIN_OUT;
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_sl_gain_out]
    }
    else if (sliderThatWasMoved == sl_depth1.get())
    {
        //[UserSliderCode_sl_depth1] -- add your slider handling code here..
        ecp = ECP_DEPTH1;
    handle_depthN:
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_sl_depth1]
    }
    else if (sliderThatWasMoved == sl_depth2.get())
    {
        //[UserSliderCode_sl_depth2] -- add your slider handling code here..
        ecp = ECP_DEPTH2;
        goto handle_depthN;
        //[/UserSliderCode_sl_depth2]
    }
    else if (sliderThatWasMoved == sl_depth3.get())
    {
        //[UserSliderCode_sl_depth3] -- add your slider handling code here..
        ecp = ECP_DEPTH3;
        goto handle_depthN;
        //[/UserSliderCode_sl_depth3]
    }
    else if (sliderThatWasMoved == sl_depth4.get())
    {
        //[UserSliderCode_sl_depth4] -- add your slider handling code here..
        ecp = ECP_DEPTH4;
        goto handle_depthN;
        //[/UserSliderCode_sl_depth4]
    }
    else if (sliderThatWasMoved == sl_depth5.get())
    {
        //[UserSliderCode_sl_depth5] -- add your slider handling code here..
        ecp = ECP_DEPTH5;
        goto handle_depthN;
        //[/UserSliderCode_sl_depth5]
    }
    else if (sliderThatWasMoved == sl_depth6.get())
    {
        //[UserSliderCode_sl_depth6] -- add your slider handling code here..
        ecp = ECP_DEPTH6;
        goto handle_depthN;
        //[/UserSliderCode_sl_depth6]
    }
    else if (sliderThatWasMoved == sl_phase1.get())
    {
        //[UserSliderCode_sl_phase1] -- add your slider handling code here..
        ecp = ECP_PHASE1;
    handle_phaseN:
        processor.setEcp(ecp, sliderThatWasMoved->getValue());
        //[/UserSliderCode_sl_phase1]
    }
    else if (sliderThatWasMoved == sl_phase2.get())
    {
        //[UserSliderCode_sl_phase2] -- add your slider handling code here..
        ecp = ECP_PHASE2;
        goto handle_phaseN;
        //[/UserSliderCode_sl_phase2]
    }
    else if (sliderThatWasMoved == sl_phase3.get())
    {
        //[UserSliderCode_sl_phase3] -- add your slider handling code here..
        ecp = ECP_PHASE3;
        goto handle_phaseN;
        //[/UserSliderCode_sl_phase3]
    }
    else if (sliderThatWasMoved == sl_phase4.get())
    {
        //[UserSliderCode_sl_phase4] -- add your slider handling code here..
        ecp = ECP_PHASE4;
        goto handle_phaseN;
        //[/UserSliderCode_sl_phase4]
    }
    else if (sliderThatWasMoved == sl_phase5.get())
    {
        //[UserSliderCode_sl_phase5] -- add your slider handling code here..
        ecp = ECP_PHASE5;
        goto handle_phaseN;
        //[/UserSliderCode_sl_phase5]
    }
    else if (sliderThatWasMoved == sl_phase6.get())
    {
        //[UserSliderCode_sl_phase6] -- add your slider handling code here..
        ecp = ECP_PHASE6;
        goto handle_phaseN;
        //[/UserSliderCode_sl_phase6]
    }
    else if (sliderThatWasMoved == dl_aa_cutoff.get())
    {
        //[UserSliderCode_dl_aa_cutoff] -- add your slider handling code here..
        ecp = ECP_AA_CUTOFF;
        float min = ensemble_chorus_parameter_min(ecp);
        float max = ensemble_chorus_parameter_max(ecp);
        processor.setEcp(ecp, min + (max - min) * sliderThatWasMoved->getValue());
        //[/UserSliderCode_dl_aa_cutoff]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MainComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    EnsembleChorusAudioProcessor &processor = *processor_;
    ec_parameter ecp;
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cb_nstages.get())
    {
        //[UserComboBoxCode_cb_nstages] -- add your combo box handling code here..
        ecp = ECP_NSTAGES;
        processor.setEcp(ecp, comboBoxThatHasChanged->getSelectedId() - 1);
        updateDelayRange();
        updateDelayDisplay();
        //[/UserComboBoxCode_cb_nstages]
    }
    else if (comboBoxThatHasChanged == cb_slow_wave.get())
    {
        //[UserComboBoxCode_cb_slow_wave] -- add your combo box handling code here..
        ecp = ECP_SLOW_WAVE;
        processor.setEcp(ecp, comboBoxThatHasChanged->getSelectedId() - 1);
        //[/UserComboBoxCode_cb_slow_wave]
    }
    else if (comboBoxThatHasChanged == cb_fast_wave.get())
    {
        //[UserComboBoxCode_cb_fast_wave] -- add your combo box handling code here..
        ecp = ECP_FAST_WAVE;
        processor.setEcp(ecp, comboBoxThatHasChanged->getSelectedId() - 1);
        //[/UserComboBoxCode_cb_fast_wave]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    EnsembleChorusAudioProcessor &processor = *processor_;
    ec_parameter ecp;
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btn_enable1.get())
    {
        //[UserButtonCode_btn_enable1] -- add your button handler code here..
        ecp = ECP_ENABLE1;
    handle_enableN:
        processor.setEcp(ecp, buttonThatWasClicked->getToggleState());
        //[/UserButtonCode_btn_enable1]
    }
    else if (buttonThatWasClicked == btn_enable2.get())
    {
        //[UserButtonCode_btn_enable2] -- add your button handler code here..
        ecp = ECP_ENABLE2;
        goto handle_enableN;
        //[/UserButtonCode_btn_enable2]
    }
    else if (buttonThatWasClicked == btn_enable3.get())
    {
        //[UserButtonCode_btn_enable3] -- add your button handler code here..
        ecp = ECP_ENABLE3;
        goto handle_enableN;
        //[/UserButtonCode_btn_enable3]
    }
    else if (buttonThatWasClicked == btn_enable4.get())
    {
        //[UserButtonCode_btn_enable4] -- add your button handler code here..
        ecp = ECP_ENABLE4;
        goto handle_enableN;
        //[/UserButtonCode_btn_enable4]
    }
    else if (buttonThatWasClicked == btn_enable5.get())
    {
        //[UserButtonCode_btn_enable5] -- add your button handler code here..
        ecp = ECP_ENABLE5;
        goto handle_enableN;
        //[/UserButtonCode_btn_enable5]
    }
    else if (buttonThatWasClicked == btn_enable6.get())
    {
        //[UserButtonCode_btn_enable6] -- add your button handler code here..
        ecp = ECP_ENABLE6;
        goto handle_enableN;
        //[/UserButtonCode_btn_enable6]
    }
    else if (buttonThatWasClicked == btn_routeL1.get())
    {
        //[UserButtonCode_btn_routeL1] -- add your button handler code here..
        ecp = ECP_ROUTE_L1;
    handle_routeLN:
        processor.setEcp(ecp, buttonThatWasClicked->getToggleState());
        //[/UserButtonCode_btn_routeL1]
    }
    else if (buttonThatWasClicked == btn_routeL2.get())
    {
        //[UserButtonCode_btn_routeL2] -- add your button handler code here..
        ecp = ECP_ROUTE_L2;
        goto handle_routeLN;
        //[/UserButtonCode_btn_routeL2]
    }
    else if (buttonThatWasClicked == btn_routeL3.get())
    {
        //[UserButtonCode_btn_routeL3] -- add your button handler code here..
        ecp = ECP_ROUTE_L3;
        goto handle_routeLN;
        //[/UserButtonCode_btn_routeL3]
    }
    else if (buttonThatWasClicked == btn_routeL4.get())
    {
        //[UserButtonCode_btn_routeL4] -- add your button handler code here..
        ecp = ECP_ROUTE_L4;
        goto handle_routeLN;
        //[/UserButtonCode_btn_routeL4]
    }
    else if (buttonThatWasClicked == btn_routeL5.get())
    {
        //[UserButtonCode_btn_routeL5] -- add your button handler code here..
        ecp = ECP_ROUTE_L5;
        goto handle_routeLN;
        //[/UserButtonCode_btn_routeL5]
    }
    else if (buttonThatWasClicked == btn_routeL6.get())
    {
        //[UserButtonCode_btn_routeL6] -- add your button handler code here..
        ecp = ECP_ROUTE_L6;
        goto handle_routeLN;
        //[/UserButtonCode_btn_routeL6]
    }
    else if (buttonThatWasClicked == btn_routeR1.get())
    {
        //[UserButtonCode_btn_routeR1] -- add your button handler code here..
        ecp = ECP_ROUTE_R1;
    handle_routeRN:
        processor.setEcp(ecp, buttonThatWasClicked->getToggleState());
        //[/UserButtonCode_btn_routeR1]
    }
    else if (buttonThatWasClicked == btn_routeR2.get())
    {
        //[UserButtonCode_btn_routeR2] -- add your button handler code here..
        ecp = ECP_ROUTE_R2;
        goto handle_routeRN;
        //[/UserButtonCode_btn_routeR2]
    }
    else if (buttonThatWasClicked == btn_routeR3.get())
    {
        //[UserButtonCode_btn_routeR3] -- add your button handler code here..
        ecp = ECP_ROUTE_R3;
        goto handle_routeRN;
        //[/UserButtonCode_btn_routeR3]
    }
    else if (buttonThatWasClicked == btn_routeR4.get())
    {
        //[UserButtonCode_btn_routeR4] -- add your button handler code here..
        ecp = ECP_ROUTE_R4;
        goto handle_routeRN;
        //[/UserButtonCode_btn_routeR4]
    }
    else if (buttonThatWasClicked == btn_routeR5.get())
    {
        //[UserButtonCode_btn_routeR5] -- add your button handler code here..
        ecp = ECP_ROUTE_R5;
        goto handle_routeRN;
        //[/UserButtonCode_btn_routeR5]
    }
    else if (buttonThatWasClicked == btn_routeR6.get())
    {
        //[UserButtonCode_btn_routeR6] -- add your button handler code here..
        ecp = ECP_ROUTE_R6;
        goto handle_routeRN;
        //[/UserButtonCode_btn_routeR6]
    }
    else if (buttonThatWasClicked == btn_stereo.get())
    {
        //[UserButtonCode_btn_stereo] -- add your button handler code here..
        ecp = ECP_CHANNEL_LAYOUT;
        processor.setEcp(ecp, ECC_STEREO);
        //[/UserButtonCode_btn_stereo]
    }
    else if (buttonThatWasClicked == btn_mono.get())
    {
        //[UserButtonCode_btn_mono] -- add your button handler code here..
        ecp = ECP_CHANNEL_LAYOUT;
        processor.setEcp(ecp, ECC_MONO);
        //[/UserButtonCode_btn_mono]
    }
    else if (buttonThatWasClicked == tick_bypass.get())
    {
        //[UserButtonCode_tick_bypass] -- add your button handler code here..
        ecp = ECP_BYPASS;
        processor.setEcp(ecp, buttonThatWasClicked->getToggleState());
        //[/UserButtonCode_tick_bypass]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MainComponent::setModulationValues(const float slow[6], const float fast[6])
{
    visu_mod1->setValues(slow[0], fast[0]);
    visu_mod2->setValues(slow[1], fast[1]);
    visu_mod3->setValues(slow[2], fast[2]);
    visu_mod4->setValues(slow[3], fast[3]);
    visu_mod5->setValues(slow[4], fast[4]);
    visu_mod6->setValues(slow[5], fast[5]);
}

void MainComponent::updateDisplayWithEcp(ec_parameter p, float value)
{
    Slider *sl;
    Button *btn;
    ToggleButton *tick;

    switch (p) {
    case ECP_BYPASS:
        tick = tick_bypass.get();
        tick->setToggleState(value, dontSendNotification);
        break;
    case ECP_CHANNEL_LAYOUT: {
        ec_channel_layout ecc = (ec_channel_layout)value;
        if (ecc == ECC_STEREO)
            btn_stereo->setToggleState(true, dontSendNotification);
        else if (ecc == ECC_MONO)
            btn_mono->setToggleState(true, dontSendNotification);
        else
            assert(false);
        break;
    }
    case ECP_DELAY:
        sl = sl_delay.get();
        sl->setValue(value, dontSendNotification);
        updateDelayDisplay();
        break;
    case ECP_NSTAGES:
        cb_nstages->setSelectedId((int)value + 1, dontSendNotification);
        updateDelayRange();
        updateDelayDisplay();
        break;
    case ECP_AA_CUTOFF: {
        float min = ensemble_chorus_parameter_min(p);
        float max = ensemble_chorus_parameter_max(p);
        sl = dl_aa_cutoff.get();
        sl->setValue((value - min) / (max - min), dontSendNotification);
        break;
    }
    case ECP_MOD_RANGE:
        sl = dl_mod_range.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_SLOW_RATE:
        sl = dl_slow_rate.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_SLOW_WAVE:
        cb_slow_wave->setSelectedId((int)value + 1, dontSendNotification);
        break;
    case ECP_SLOW_RAND:
        sl = dl_slow_rand.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_FAST_RATE:
        sl = dl_fast_rate.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_FAST_WAVE:
        cb_fast_wave->setSelectedId((int)value + 1, dontSendNotification);
        break;
    case ECP_FAST_RAND:
        sl = dl_fast_rand.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_LPF_CUTOFF: {
        float min = ensemble_chorus_parameter_min(p);
        float max = ensemble_chorus_parameter_max(p);
        sl = dl_lpf_cutoff.get();
        sl->setValue((value - min) / (max - min), dontSendNotification);
        break;
    }
    case ECP_LPF_Q:
        sl = dl_lpf_q.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_GAIN_IN:
        sl = sl_gain_in.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_GAIN_OUT:
        sl = sl_gain_out.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_MIX_DRY:
        sl = sl_dry.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_MIX_WET:
        sl = sl_wet.get();
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_ENABLE1:
        btn = btn_enable1.get();
    handleEnableN:
        btn->setToggleState(value, dontSendNotification);
        break;
    case ECP_PHASE1:
        sl = sl_phase1.get();
    handlePhaseN:
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_DEPTH1:
        sl = sl_depth1.get();
    handleDepthN:
        sl->setValue(value, dontSendNotification);
        break;
    case ECP_ROUTE_L1:
        btn = btn_routeL1.get();
    handleRouteLN:
        btn->setToggleState(value, dontSendNotification);
        break;
    case ECP_ROUTE_R1:
        btn = btn_routeR1.get();
    handleRouteRN:
        btn->setToggleState(value, dontSendNotification);
        break;
#define HandleN(N)                                \
        case ECP_ENABLE##N:                       \
            btn = btn_enable##N.get();            \
            goto handleEnableN;                   \
        case ECP_PHASE##N:                        \
            sl = sl_phase##N.get();               \
            goto handlePhaseN;                    \
        case ECP_DEPTH##N:                        \
            sl = sl_depth##N.get();               \
            goto handleDepthN;                    \
        case ECP_ROUTE_L##N:                      \
            btn = btn_routeL##N.get();            \
            goto handleRouteLN;                   \
        case ECP_ROUTE_R##N:                      \
            btn = btn_routeR##N.get();            \
            goto handleRouteRN;
        HandleN(2) HandleN(3) HandleN(4) HandleN(5) HandleN(6)
#undef HandleN
    default:
        assert(false);
    }
}

void MainComponent::updateDelayRange()
{
    Slider &sl = *sl_delay;
    double delay = processor_->getEcp(ECP_DELAY);
    const unsigned ns = EC_NSTAGES_MIN << (unsigned)(cb_nstages->getSelectedId() - 1);
    sl.setRange(0, ns * (1.0 / (2 * EC_CLOCK_RATE_MIN)));
    sl.setValue(delay, dontSendNotification);
    sl.repaint();
}

void MainComponent::updateDelayDisplay()
{
    double delay = processor_->getEcp(ECP_DELAY);
    double maxdelay = sl_delay->getMaximum();
    delay = (delay < maxdelay) ? delay : maxdelay;

    char buf[32];
    if (delay < 1e-3)
        std::strcpy(buf, "0 ms");
    else if (delay < 1)
        std::sprintf(buf, "%.3g ms", delay * 1e3);
    else
        std::sprintf(buf, "%.3g s", delay);
    lbl_delay->setText(buf, dontSendNotification);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public Component" constructorParams="EnsembleChorusAudioProcessor &amp;p"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.33" fixedSize="0" initialWidth="625" initialHeight="280">
  <BACKGROUND backgroundColour="ffefefef">
    <RECT pos="0 0 625 480" fill="solid: ffc0c0c0" hasStroke="0"/>
    <RECT pos="4 42 230 4" fill="solid: ff404040" hasStroke="0"/>
    <TEXT pos="370 5 65 25" fill="solid: ff000000" hasStroke="0" text="Stages"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="440 5 55 25" fill="solid: ff000000" hasStroke="0" text="Delay"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="500 5 50 25" fill="solid: ff000000" hasStroke="0" text="Mix"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="560 5 50 25" fill="solid: ff000000" hasStroke="0" text="Gain"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="370 185 65 25" fill="solid: ff000000" hasStroke="0" text="Chorus"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="490 185 65 25" fill="solid: ff000000" hasStroke="0" text="Vibrato"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="30 245 125 25" fill="solid: ff000000" hasStroke="0" text="Low-pass filter"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="33" typefaceStyle="Bold Italic"/>
    <TEXT pos="65 55 105 25" fill="solid: ff000000" hasStroke="0" text="Phase"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="33" typefaceStyle="Bold Italic"/>
    <TEXT pos="170 55 75 25" fill="solid: ff000000" hasStroke="0" text="Routing"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="33" typefaceStyle="Bold Italic"/>
    <TEXT pos="245 55 95 25" fill="solid: ff000000" hasStroke="0" text="Modulator"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="33" typefaceStyle="Bold Italic"/>
    <TEXT pos="0 0 230 42" fill="solid: ff000000" hasStroke="0" text="JPC Ensemble Chorus"
          fontname="Default serif font" fontsize="21.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <RECT pos="0 0 230 42" fill="solid: 0" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff000000"/>
    <RECT pos="230 4 4 42" fill="solid: ff404040" hasStroke="0"/>
    <TEXT pos="360 55 90 25" fill="solid: ff000000" hasStroke="0" text="Anti-alias"
          fontname="Default serif font" fontsize="18.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <ROUNDRECT pos="443 155 49 25" cornerSize="3.0" fill="solid: ff00247f" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff808080"/>
  </BACKGROUND>
  <SLIDER name="new slider" id="4106898fdb391e2c" memberName="dl_mod_range"
          virtualName="" explicitFocusOrder="0" pos="380 125 40 40" min="0.0"
          max="1.0" int="0.0" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="ce8811064110cb8c" memberName="dl_slow_rate"
          virtualName="" explicitFocusOrder="0" pos="380 210 45 45" min="0.0"
          max="1.0" int="0.0" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="3f8e1173cd43c79" memberName="dl_fast_rate"
          virtualName="" explicitFocusOrder="0" pos="500 210 45 45" min="0.0"
          max="1.0" int="0.0" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="778f02f9c882a15e" memberName="dl_slow_rand"
          virtualName="" explicitFocusOrder="0" pos="425 240 35 35" min="0.0"
          max="1.0" int="0.0" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="98940ec5e1e4ef36" memberName="dl_fast_rand"
          virtualName="" explicitFocusOrder="0" pos="545 240 35 35" min="0.0"
          max="1.0" int="0.0" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="f20286cbbf6ab90c" memberName="dl_lpf_cutoff"
          virtualName="" explicitFocusOrder="0" pos="160 235 40 40" min="0.0"
          max="1.0" int="0.0" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="35c6651e895d9c6" memberName="dl_lpf_q"
          virtualName="" explicitFocusOrder="0" pos="255 235 40 40" min="0.0"
          max="1.0" int="0.0" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="6a5705d4b322c213" memberName="sl_delay"
          virtualName="" explicitFocusOrder="0" pos="455 30 25 125" min="0.0"
          max="1.0" int="0.0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="66524e132f25157" memberName="sl_wet" virtualName=""
          explicitFocusOrder="0" pos="500 30 25 95" min="0.0" max="1.0"
          int="0.0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="d819af9af738c8e6" memberName="sl_dry" virtualName=""
          explicitFocusOrder="0" pos="525 30 25 95" min="0.0" max="1.0"
          int="0.0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="5be589c1e13ae770" memberName="sl_gain_in"
          virtualName="" explicitFocusOrder="0" pos="560 30 25 95" min="0.0"
          max="3.0" int="0.0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="bb55c884b8a3911d" memberName="sl_gain_out"
          virtualName="" explicitFocusOrder="0" pos="585 30 25 95" min="0.0"
          max="3.0" int="0.0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <COMBOBOX name="new combo box" id="dcf7c687e5aa46ae" memberName="cb_nstages"
            virtualName="" explicitFocusOrder="0" pos="370 30 65 20" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="e9141044978f3496" memberName="sl_depth1"
          virtualName="" explicitFocusOrder="0" pos="300 80 60 25" min="0.0"
          max="1.0" int="0.0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="418f5b8a758d2c8b" memberName="sl_depth2"
          virtualName="" explicitFocusOrder="0" pos="300 105 60 25" min="0.0"
          max="1.0" int="0.0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="e49d08a1b895842b" memberName="sl_depth3"
          virtualName="" explicitFocusOrder="0" pos="300 130 60 25" min="0.0"
          max="1.0" int="0.0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="fb08481c13d078b" memberName="sl_depth4"
          virtualName="" explicitFocusOrder="0" pos="300 155 60 25" min="0.0"
          max="1.0" int="0.0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="f53ea56c0063a721" memberName="sl_depth5"
          virtualName="" explicitFocusOrder="0" pos="300 180 60 25" min="0.0"
          max="1.0" int="0.0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="338d170c83872b86" memberName="sl_depth6"
          virtualName="" explicitFocusOrder="0" pos="300 205 60 25" min="0.0"
          max="1.0" int="0.0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="d8eecd16e807bbde" memberName="sl_phase1"
          virtualName="" explicitFocusOrder="0" pos="65 80 105 25" min="0.0"
          max="359.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="b125a5c44479a95b" memberName="sl_phase2"
          virtualName="" explicitFocusOrder="0" pos="65 105 105 25" min="0.0"
          max="359.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="fafc2c5327b4b9b7" memberName="sl_phase3"
          virtualName="" explicitFocusOrder="0" pos="65 130 105 25" min="0.0"
          max="359.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="85ca0351fcde0c45" memberName="sl_phase4"
          virtualName="" explicitFocusOrder="0" pos="65 155 105 25" min="0.0"
          max="359.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="73cad4e111a23f87" memberName="sl_phase5"
          virtualName="" explicitFocusOrder="0" pos="65 180 105 25" min="0.0"
          max="359.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="f536798990e010e8" memberName="sl_phase6"
          virtualName="" explicitFocusOrder="0" pos="65 205 105 25" min="0.0"
          max="359.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTBUTTON name="new button" id="4aa400e41380f7c4" memberName="btn_enable1"
              virtualName="" explicitFocusOrder="0" pos="20 80 25 25" buttonText="1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="e5a070ce96a71795" memberName="btn_enable2"
              virtualName="" explicitFocusOrder="0" pos="20 105 25 25" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="88c542e1d4b0598d" memberName="btn_enable3"
              virtualName="" explicitFocusOrder="0" pos="20 130 25 25" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="9d92ad5b9e3975bb" memberName="btn_enable4"
              virtualName="" explicitFocusOrder="0" pos="20 155 25 25" buttonText="4"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="a0e30dce3722146c" memberName="btn_enable5"
              virtualName="" explicitFocusOrder="0" pos="20 180 25 25" buttonText="5"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="2582c6ca5c55ebcd" memberName="btn_enable6"
              virtualName="" explicitFocusOrder="0" pos="20 205 25 25" buttonText="6"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="725fdeb5244a3461" memberName="btn_routeL1"
              virtualName="" explicitFocusOrder="0" pos="180 80 25 25" buttonText="L"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="8195736b2f913ab0" memberName="btn_routeL2"
              virtualName="" explicitFocusOrder="0" pos="180 105 25 25" buttonText="L"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d4eb5dde819d5e89" memberName="btn_routeL3"
              virtualName="" explicitFocusOrder="0" pos="180 130 25 25" buttonText="L"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d82ec26f58630b1a" memberName="btn_routeL4"
              virtualName="" explicitFocusOrder="0" pos="180 155 25 25" buttonText="L"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b5f9f09bb4c0717f" memberName="btn_routeL5"
              virtualName="" explicitFocusOrder="0" pos="180 180 25 25" buttonText="L"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5336db69888685fc" memberName="btn_routeL6"
              virtualName="" explicitFocusOrder="0" pos="180 205 25 25" buttonText="L"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c9cb406e92c97466" memberName="btn_routeR1"
              virtualName="" explicitFocusOrder="0" pos="210 80 25 25" buttonText="R"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3e1d6b070bd48ba" memberName="btn_routeR2"
              virtualName="" explicitFocusOrder="0" pos="210 105 25 25" buttonText="R"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="a817f84e2d9aa37c" memberName="btn_routeR3"
              virtualName="" explicitFocusOrder="0" pos="210 130 25 25" buttonText="R"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="7a92bd2442217937" memberName="btn_routeR4"
              virtualName="" explicitFocusOrder="0" pos="210 155 25 25" buttonText="R"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5ec2070461a87184" memberName="btn_routeR5"
              virtualName="" explicitFocusOrder="0" pos="210 180 25 25" buttonText="R"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="2de6f3005a800e43" memberName="btn_routeR6"
              virtualName="" explicitFocusOrder="0" pos="210 205 25 25" buttonText="R"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="2567274c18916b8d" memberName="cb_slow_wave"
            virtualName="" explicitFocusOrder="0" pos="435 210 55 25" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="69b9d68b913ede38" memberName="cb_fast_wave"
            virtualName="" explicitFocusOrder="0" pos="555 210 55 25" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="new button" id="48be6b9116492ac" memberName="btn_stereo"
              virtualName="" explicitFocusOrder="0" pos="500 155 55 25" buttonText="Stereo"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="56b86482f7bd1062" memberName="btn_mono"
              virtualName="" explicitFocusOrder="0" pos="555 155 55 25" buttonText="Mono"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="new toggle button" id="f3d2d80e76c496eb" memberName="tick_bypass"
                virtualName="" explicitFocusOrder="0" pos="235 10 75 25" buttonText="Bypass"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="5e07cc3b5de93fd1" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="200 255 50 25" edTextCol="ff000000"
         edBkgCol="0" labelText="Cutoff" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e0b5eafeaaa1b4fc" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="295 255 50 25" edTextCol="ff000000"
         edBkgCol="0" labelText="Q" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="2ec72c7b193b2465" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="455 240 45 35" edTextCol="ff000000"
         edBkgCol="0" labelText="Rand" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="4aaaef648836af90" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="575 240 45 35" edTextCol="ff000000"
         edBkgCol="0" labelText="Rand" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="1a504956f75fa6f0" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="365 155 75 25" edTextCol="ff000000"
         edBkgCol="0" labelText="Mod range" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="c1eb5c37ea705fb9" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="495 120 35 25" edTextCol="ff000000"
         edBkgCol="0" labelText="Wet" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="d8b68d80b3ac848d" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="520 120 35 25" edTextCol="ff000000"
         edBkgCol="0" labelText="Dry" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="9a9435e75d7d840a" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="555 120 35 25" edTextCol="ff000000"
         edBkgCol="0" labelText="In" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="37a31ff15b45ee79" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="580 120 35 25" edTextCol="ff000000"
         edBkgCol="0" labelText="Out" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="e842fd5785e4c53a" memberName="visu_mod1"
                    virtualName="ModulatorVisu" explicitFocusOrder="0" pos="245 80 55 25"
                    class="Component" params=""/>
  <GENERICCOMPONENT name="new component" id="1c06edc40b77c6e8" memberName="visu_mod2"
                    virtualName="ModulatorVisu" explicitFocusOrder="0" pos="245 105 55 25"
                    class="Component" params=""/>
  <GENERICCOMPONENT name="new component" id="757138db2a7a892d" memberName="visu_mod3"
                    virtualName="ModulatorVisu" explicitFocusOrder="0" pos="245 130 55 25"
                    class="Component" params=""/>
  <GENERICCOMPONENT name="new component" id="37ef33265bbcabc" memberName="visu_mod4"
                    virtualName="ModulatorVisu" explicitFocusOrder="0" pos="245 155 55 25"
                    class="Component" params=""/>
  <GENERICCOMPONENT name="new component" id="8758c12eed6cd298" memberName="visu_mod5"
                    virtualName="ModulatorVisu" explicitFocusOrder="0" pos="245 180 55 25"
                    class="Component" params=""/>
  <GENERICCOMPONENT name="new component" id="189c5da9f8971bea" memberName="visu_mod6"
                    virtualName="ModulatorVisu" explicitFocusOrder="0" pos="245 205 55 25"
                    class="Component" params=""/>
  <SLIDER name="new slider" id="aec553e98f465f1" memberName="dl_aa_cutoff"
          virtualName="" explicitFocusOrder="0" pos="365 80 35 35" min="0.0"
          max="1.0" int="0.0" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="aefffe557433fe09" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="395 85 50 25" edTextCol="ff000000"
         edBkgCol="0" labelText="Cutoff" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="a43b8b27d21e0e29" memberName="lbl_delay"
         virtualName="" explicitFocusOrder="0" pos="443 155 49 25" bkgCol="247f"
         textCol="ff3fda00" edTextCol="ff000000" edBkgCol="0" labelText="888 ms"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default monospaced font" fontsize="15.0" kerning="0.0"
         bold="1" italic="0" justification="36" typefaceStyle="Bold"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
