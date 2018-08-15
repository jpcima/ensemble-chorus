//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "processor.h"
#include "message_queue.h"

Chorus_Plugin::Chorus_Plugin()
    : Plugin(ensemble_chorus_parameter_count(), 0, 0)
{
    chorus *ec = ensemble_chorus_alloc();
    ec_.reset(ec);
    ensemble_chorus_init(ec, 44100, 1024);
    mq_to_ui_.reset(new Message_Queue(1024));
}

Chorus_Plugin::~Chorus_Plugin()
{
}

const char *Chorus_Plugin::getLabel() const
{
    return "jpc_ensemble_chorus";
}

const char *Chorus_Plugin::getMaker() const
{
    return "Jean Pierre Cimalando";
}

const char *Chorus_Plugin::getLicense() const
{
    return "Boost";
}

uint32_t Chorus_Plugin::getVersion() const
{
    return d_version(
        PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
}

int64_t Chorus_Plugin::getUniqueId() const
{
    return d_cconst('J', 'C', 'E', 'c');
}

void Chorus_Plugin::initParameter(uint32_t index, Parameter &parameter)
{
    ec_parameter p = (ec_parameter)index;

    parameter.ranges.min = ensemble_chorus_parameter_min(p);
    parameter.ranges.max = ensemble_chorus_parameter_max(p);
    parameter.ranges.def = ensemble_chorus_parameter_default(p);
    parameter.symbol = ensemble_chorus_parameter_name(p);
    parameter.name = parameter.symbol;

    static_assert(ECP_FLOAT == 0, "");
    static_assert(ECP_BOOLEAN == kParameterIsBoolean, "");
    static_assert(ECP_INTEGER == kParameterIsInteger, "");
    static_assert(ECP_LOGARITHMIC == kParameterIsLogarithmic, "");

    parameter.hints = kParameterIsAutomable|ensemble_chorus_parameter_flags(p);
}

float Chorus_Plugin::getParameterValue(uint32_t index) const
{
    chorus *ec = ec_.get();
    return ensemble_chorus_get_parameter(ec, (ec_parameter)index);
}

void Chorus_Plugin::setParameterValue(uint32_t index, float value)
{
    chorus *ec = ec_.get();
    ensemble_chorus_set_parameter(ec, (ec_parameter)index, value);
}

void Chorus_Plugin::activate()
{
    chorus *ec = ec_.get();
    ensemble_chorus_init(ec, getSampleRate(), getBufferSize());
}

void Chorus_Plugin::run(const float **inputs, float **outputs, uint32_t frames)
{
    chorus *ec = ec_.get();
    for (unsigned c = 0; c < 2; ++c)
        memcpy(outputs[c], inputs[c], frames * sizeof(float));
    ensemble_chorus_process(ec, outputs, frames);

    Messages::NotifyModulation msg_mod;
    ensemble_chorus_get_current_modulation(ec, msg_mod.slow, msg_mod.fast);
    mq_to_ui_->write_message(msg_mod);
}

Plugin *DISTRHO::createPlugin()
{
    return new Chorus_Plugin;
}
