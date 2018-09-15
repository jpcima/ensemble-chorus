//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "main_view.h"
#include "modulator_visu.h"
#include "widgets/Fl_Valuator_Ex.h"
#include <ensemble_chorus.h>
#include <Fl_Knob/Fl_Knob.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Toggle_Button.H>
#include <string>
#include <cmath>
#include <cassert>

double Main_View::from_logarithmic(double value)
{
    return std::asin(value) * M_2_PI;
}

double Main_View::to_logarithmic(double value)
{
    return std::sin(value * M_PI_2);
}

void Main_View::parameter(unsigned id, float value, void *userdata)
{
    auto &self = *reinterpret_cast<Main_View *>(userdata);
    switch (id) {
    case ECP_BYPASS: self.tick_bypass_->value(value); break;
    case ECP_CHANNEL_LAYOUT: {
        switch ((int)value) {
        case ECC_STEREO:
            self.btn_stereo_->value(1);
            self.btn_mono_->value(0);
            break;
        case ECC_MONO:
            self.btn_stereo_->value(0);
            self.btn_mono_->value(1);
            break;
        default:
            assert(false);
        }
        break;
    };
    case ECP_DELAY: self.sl_delay_->value(value); break;
    case ECP_NSTAGES: self.cb_nstages_->value(value); break;
    case ECP_AA_CUTOFF: {
        float min = ensemble_chorus_parameter_min((ec_parameter)id);
        float max = ensemble_chorus_parameter_max((ec_parameter)id);
        self.dl_aa_cutoff_->value((value - min) / (max - min));
        break;
    }
    case ECP_MOD_RANGE: self.dl_mod_range_->value(value); break;
    case ECP_SLOW_RATE: self.dl_slow_rate_->value(value); break;
    case ECP_SLOW_WAVE: self.cb_slow_wave_->value(value); break;
    case ECP_SLOW_RAND: self.dl_slow_rand_->value(value); break;
    case ECP_FAST_RATE: self.dl_fast_rate_->value(value); break;
    case ECP_FAST_WAVE: self.cb_fast_wave_->value(value); break;
    case ECP_FAST_RAND: self.dl_fast_rand_->value(value); break;
    case ECP_LPF_CUTOFF: {
        float min = ensemble_chorus_parameter_min((ec_parameter)id);
        float max = ensemble_chorus_parameter_max((ec_parameter)id);
        self.dl_lpf_cutoff_->value((value - min) / (max - min));
        break;
    }
    case ECP_LPF_Q: self.dl_lpf_q_->value(value); break;
    case ECP_GAIN_IN: self.sl_gain_in_->value(to_logarithmic(value * (1.0 / 3.0))); break;
    case ECP_GAIN_OUT: self.sl_gain_out_->value(to_logarithmic(value * (1.0 / 3.0))); break;
    case ECP_MIX_DRY: self.sl_dry_->value(to_logarithmic(value)); break;
    case ECP_MIX_WET: self.sl_wet_->value(to_logarithmic(value)); break;

    case ECP_ENABLE1: self.btn_enable1_->value(value); break;
    case ECP_PHASE1: self.sl_phase1_->value(value * 360); break;
    case ECP_DEPTH1: self.sl_depth1_->value(value); break;
    case ECP_ROUTE_L1: self.btn_routeL1_->value(value); break;
    case ECP_ROUTE_R1: self.btn_routeR1_->value(value); break;

    case ECP_ENABLE2: self.btn_enable2_->value(value); break;
    case ECP_PHASE2: self.sl_phase2_->value(value * 360); break;
    case ECP_DEPTH2: self.sl_depth2_->value(value); break;
    case ECP_ROUTE_L2: self.btn_routeL2_->value(value); break;
    case ECP_ROUTE_R2: self.btn_routeR2_->value(value); break;

    case ECP_ENABLE3: self.btn_enable3_->value(value); break;
    case ECP_PHASE3: self.sl_phase3_->value(value * 360); break;
    case ECP_DEPTH3: self.sl_depth3_->value(value); break;
    case ECP_ROUTE_L3: self.btn_routeL3_->value(value); break;
    case ECP_ROUTE_R3: self.btn_routeR3_->value(value); break;

    case ECP_ENABLE4: self.btn_enable4_->value(value); break;
    case ECP_PHASE4: self.sl_phase4_->value(value * 360); break;
    case ECP_DEPTH4: self.sl_depth4_->value(value); break;
    case ECP_ROUTE_L4: self.btn_routeL4_->value(value); break;
    case ECP_ROUTE_R4: self.btn_routeR4_->value(value); break;

    case ECP_ENABLE5: self.btn_enable5_->value(value); break;
    case ECP_PHASE5: self.sl_phase5_->value(value * 360); break;
    case ECP_DEPTH5: self.sl_depth5_->value(value); break;
    case ECP_ROUTE_L5: self.btn_routeL5_->value(value); break;
    case ECP_ROUTE_R5: self.btn_routeR5_->value(value); break;

    case ECP_ENABLE6: self.btn_enable6_->value(value); break;
    case ECP_PHASE6: self.sl_phase6_->value(value * 360); break;
    case ECP_DEPTH6: self.sl_depth6_->value(value); break;
    case ECP_ROUTE_L6: self.btn_routeL6_->value(value); break;
    case ECP_ROUTE_R6: self.btn_routeR6_->value(value); break;
    }
}

void Main_View::modulation(const float slow[6], const float fast[6], void *userdata)
{
    auto &self = *reinterpret_cast<Main_View *>(userdata);
    self.visu_mod1_->value(slow[0], fast[0]);
    self.visu_mod2_->value(slow[1], fast[1]);
    self.visu_mod3_->value(slow[2], fast[2]);
    self.visu_mod4_->value(slow[3], fast[3]);
    self.visu_mod5_->value(slow[4], fast[4]);
    self.visu_mod6_->value(slow[5], fast[5]);
}
