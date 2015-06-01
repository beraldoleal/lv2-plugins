#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "lv2.h"

#define PLUGIN_URI "http://github.com/beraldoleal/lv2-synth"

#define SYNTH_FREQUENCY 0
#define SYNTH_OUTPUT 1

typedef struct {
        double sample_rate;
        float phase;

        float *frequency;
        float *output;
} synth_t;

static LV2_Handle instantiate(const LV2_Descriptor *descriptor,
                              double rate, const char *bundle_path,
                              const LV2_Feature * const *features)
{
        synth_t *plugin_data = (synth_t *) malloc(sizeof(synth_t));

        plugin_data->sample_rate = rate;
        plugin_data->phase = 0.0;

        return (LV2_Handle) plugin_data;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    synth_t *plugin = (synth_t *) instance;

    switch (port) {
    case SYNTH_FREQUENCY:
        plugin->frequency = (float *) data;
        break;
    case SYNTH_OUTPUT:
        plugin->output = (float *) data;
        break;
    }
}

static void run(LV2_Handle instance, uint32_t n_samples)
{
        synth_t *plugin_data = (synth_t *) instance;

        const float frequency = *(plugin_data->frequency);
        float * const output = plugin_data->output;

        float samples_per_cycle = plugin_data->sample_rate / frequency;

        float phase_increment = 1.0 / samples_per_cycle;

        uint32_t pos = 0;
        for (pos = 0; pos < n_samples; pos++) {
            output[pos] = sin(plugin_data->phase * 2 * 3.1415) * 0.3;

            plugin_data->phase += phase_increment;

            if (plugin_data->phase > 1.0)
                plugin_data->phase -= 1.0;
        }
}

static void cleanup(LV2_Handle instance)
{
    free(instance);
}

static const LV2_Descriptor descriptor = {
        PLUGIN_URI,
        instantiate,
        connect_port,
        NULL,
        run,
        NULL,
        cleanup,
        NULL
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor * lv2_descriptor(uint32_t index)
{
        switch (index) {
        case 0:  return &descriptor;
        default: return NULL;
        }
}
