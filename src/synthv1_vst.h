#ifndef __synthv1_vst_h
#define __synthv1_vst_h

#include "synthv1.h"
#include "synthv1_param.h"
#include "synthv1_sched.h"
#include "synthv1_config.h"

#include "synthv1_vstui.h"

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>

#include "vst/vestige.h"
#include "vst/midi.h"

#if _WIN32
#include <windows.h>
#endif

#include <QApplication>
#include <QDomDocument>

// Forward decls.
class QApplication;

/**
 * Constants
 */

// Default UI size
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 540;

// Default buffer size
const uint32_t DEFAULT_BUFFER_SIZE = 512;

/**
 * MIDI data struct
 * Ported from AmSynth
 */
struct synthv1_midi_event_t
{
    unsigned int offset_frames;
    unsigned int length;
    unsigned char *buffer;
};

struct synthv1_midi_cc_t
{
    unsigned char channel;
    unsigned char cc;
    unsigned char value;
};

/**
 * SynthV1 VST synthesizer class
 */
class synthv1_vst : public synthv1
{
public:
    synthv1_vst();

    ~synthv1_vst();

    void process(int numSampleFrames, const std::vector<synthv1_midi_event_t> &midi_in, float **inputs, float **outputs);

    static void qapp_instantiate();
    static void qapp_cleanup();

    static QApplication *qapp_instance();

    int loadState(const char *buffer);
    int saveState(char *buffer);

protected:
    void updatePreset(bool bDirty);
    void updateParam(synthv1::ParamIndex index);
    void updateParams();
    void updateTuning();
    void updateSampleRate(double sample_rate);

private:
    double sample_rate;

    float m_params[synthv1::NUM_PARAMS];

    static QApplication *g_qapp_instance;
    static unsigned int g_qapp_refcount;
};

/**
 * Rectangle definition object
 */
struct ERect
{
    short top;
    short left;
    short bottom;
    short right;
};

static char hostProductString[64] = "";

struct Plugin
{
    Plugin(audioMasterCallback master)
    {
        audioMaster = master;

        synthv1_vst::qapp_instantiate();
        synthesizer = new synthv1_vst;

        midiBuffer = (unsigned char *)malloc(4096);
    }

    ~Plugin()
    {
        synthv1_vst::qapp_cleanup();
        delete synthesizer;

        free(midiBuffer);
    }

    audioMasterCallback audioMaster;
    synthv1_vst *synthesizer;
    unsigned char *midiBuffer;
    std::vector<synthv1_midi_event_t> midiEvents;

    int programNumber = 0;
};

#endif // __synthv1_vst_h

// end of synthv1_vst.h