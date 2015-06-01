#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "lv2.h"

#define PLUGIN_URI "http://github.com/beraldoleal/lv2-noise"

#define NOISE_OUTPUT 0

typedef struct {
        float *output;
} noise_t;

static LV2_Handle instantiate(const LV2_Descriptor *descriptor,
                              double rate, const char *bundle_path,
                              const LV2_Feature * const *features)
{
        noise_t *plugin_data = (noise_t *) malloc(sizeof(noise_t));
        return (LV2_Handle) plugin_data;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    noise_t *plugin = (noise_t *) instance;

    switch (port) {
    case NOISE_OUTPUT:
        plugin->output = (float *) data;
        break;
    }
}

static void run(LV2_Handle instance, uint32_t n_samples)
{
        const noise_t *plugin_data = (const noise_t *) instance;

        float * const output = plugin_data->output;

        uint32_t pos = 0;

        srand(time(NULL));
        for (pos = 0; pos < n_samples; pos++) {
            output[pos] = (float)((rand() & 0x7FFF) - 16384);
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
