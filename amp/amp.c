#include <math.h>
#include <stdlib.h>
#include "lv2.h"

#define AMP_URI "http://github.com/beraldoleal/lv2-amp"

#define AMP_GAIN   0
#define AMP_INPUT  1
#define AMP_OUTPUT 2

#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

typedef struct {
        const float *gain;
        const float *input;
        float *output;
} amp_t;

static LV2_Handle instantiate(const LV2_Descriptor *descriptor,
                              double rate, const char *bundle_path,
                              const LV2_Feature * const *features)
{
        amp_t *plugin_data = (amp_t *) malloc(sizeof(amp_t));
        return (LV2_Handle) plugin_data;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    amp_t *plugin = (amp_t *) instance;

    switch (port) {
    case AMP_GAIN:
        plugin->gain = (const float *) data;
        break;
    case AMP_INPUT:
        plugin->input = (const float *) data;
        break;
    case AMP_OUTPUT:
        plugin->output = (float *) data;
        break;
    }
}

static void run(LV2_Handle instance, uint32_t n_samples)
{
        const amp_t *plugin_data = (const amp_t *) instance;

        const float gain = *(plugin_data->gain);
        const float * const input = plugin_data->input;
        float * const output = plugin_data->output;

        uint32_t pos = 0;
        const float coef = DB_CO(gain);

        for (pos = 0; pos < n_samples; pos++) {
            output[pos] = input[pos] * coef;
        }
}

static void cleanup(LV2_Handle instance)
{
    free(instance);
}

static const LV2_Descriptor descriptor = {
        AMP_URI,
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
