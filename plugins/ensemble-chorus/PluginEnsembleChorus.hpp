#pragma once
#include "DistrhoPlugin.hpp"
#include "EnsembleChorusShared.hpp"

// -----------------------------------------------------------------------

class PluginEnsembleChorus : public Plugin {
public:
    PluginEnsembleChorus();

protected:
    // -------------------------------------------------------------------
    // Information

    const char *getLabel() const noexcept override
    {
        return "EnsembleChorus";
    }

    const char *getDescription() const override
    {
        return "Digital model of electronic string ensemble chorus";
    }

    const char *getMaker() const noexcept override
    {
        return "Jean Pierre Cimalando";
    }

    const char *getHomePage() const override
    {
        return "http://jpcima.sdf1.org/lv2/ensemble-chorus";
    }

    const char *getLicense() const noexcept override
    {
        return "https://spdx.org/licenses/BSL-1.0";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(0, 1, 0);
    }

    // Go to:
    //
    // http://service.steinberg.de/databases/plugin.nsf/plugIn
    //
    // Get a proper plugin UID and fill it in here!
    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('E', 'n', 'C', 'h');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter &parameter) override;
    void initProgramName(uint32_t index, String &programName) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;
    void loadProgram(uint32_t index) override;

    // -------------------------------------------------------------------
    // Optional

    // Optional callback to inform the plugin about a sample rate change.
    void sampleRateChanged(double newSampleRate) override;

    // Optional callback to inform the plugin about a buffer size change.
    void bufferSizeChanged(uint32_t newBufferSize) override;

    // -------------------------------------------------------------------

    void updateSampleRateOrBufferSize();

    // -------------------------------------------------------------------
    // Process

    void run(const float **, float **outputs, uint32_t frames) override;


    // -------------------------------------------------------------------

private:
    chorus_u fChorus;
    double fSampleRate;
    uint32_t fBufferSize;

    float fCurrentSlowModulator[6] = {};
    float fCurrentFastModulator[6] = {};

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEnsembleChorus)
};

// -----------------------------------------------------------------------
