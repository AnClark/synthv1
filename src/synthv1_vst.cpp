// synthv1_vst.cpp
//
/****************************************************************************
   Copyright (C) 2012-2019, AnClark Liu. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#include "synthv1_vst.h"

#include <stdio.h>
#include <string.h>

/**
 * SynthV1 VST synthesizer class implementations
 */

//-------------------------------------------------------------------------
// synthv1_vst - constants.
//

// Parameter names.
// Move it from header file to here to avoid multiple definition linker error
const char *ParamNames[] = {
    "DCO1 Wave Shape 1",
    "DCO1 Wave Width 1",
    "DCO1 Wave Bandlimit 1",
    "DCO1 Wave Sync 1",
    "DCO1 Wave Shape 2",
    "DCO1 Width 2",
    "DCO1 Wave Bandlimit 2",
    "DCO1 Wave Sync 2",
    "DCO1 Balance",
    "DCO1 Detune",
    "DCO1 Phase",
    "DCO1 Ring Mod",
    "DCO1 Octave",
    "DCO1 Tuning",
    "DCO1 Glide",
    "DCO1 Env.Time",
    "DCF1 Enabled",
    "DCF1 Cutoff",
    "DCF1 Resonance",
    "DCF1 Type",
    "DCF1 Slope",
    "DCF1 Envelope",
    "DCF1 Attack",
    "DCF1 Decay",
    "DCF1 Sustain",
    "DCF1 Release",
    "LFO1 Enabled",
    "LFO1 Wave Shape",
    "LFO1 Wave Width",
    "LFO1 BPM",
    "LFO1 Rate",
    "LFO1 Sync",
    "LFO1 Sweep",
    "LFO1 Pitch",
    "LFO1 Balance",
    "LFO1 Ring Mod",
    "LFO1 Cutoff",
    "LFO1 Resonance",
    "LFO1 Panning",
    "LFO1 Volume",
    "LFO1 Attack",
    "LFO1 Decay",
    "LFO1 Sustain",
    "LFO1 Release",
    "DCA1 Volume",
    "DCA1 Attack",
    "DCA1 Decay",
    "DCA1 Sustain",
    "DCA1 Release",
    "OUT1 Stereo Width",
    "OUT1 Panning",
    "OUT1 FX Send",
    "OUT1 Volume",
    "DEF1 Pitchbend",
    "DEF1 Modwheel",
    "DEF1 Pressure",
    "DEF1 Velocity",
    "DEF1 Channel",
    "DEF1 Mono",
    "DCO2 Wave Shape 1",
    "DCO2 Wave Width 1",
    "DCO2 Wave Bandlimit 1",
    "DCO2 Wave Sync 1",
    "DCO2 Wave Shape 2",
    "DCO2 Wave Width 2",
    "DCO2 Wave Bandlimit 2",
    "DCO2 Wave Sync 2",
    "DCO2 Balance",
    "DCO2 Detune",
    "DCO2 Phase",
    "DCO2 Ring Mod",
    "DCO2 Octave",
    "DCO2 Tuning",
    "DCO2 Glide",
    "DCO2 Env.Time",
    "DCF2 Enabled",
    "DCF2 Cutoff",
    "DCF2 Resonance",
    "DCF2 Type",
    "DCF2 Slope",
    "DCF2 Envelope",
    "DCF2 Attack",
    "DCF2 Decay",
    "DCF2 Sustain",
    "DCF2 Release",
    "LFO2 Enabled",
    "LFO2 Wave Shape",
    "LFO2 Wave Width",
    "LFO2 BPM",
    "LFO2 Rate",
    "LFO2 Sync",
    "LFO2 Sweep",
    "LFO2 Pitch",
    "LFO2 Balance",
    "LFO2 Ring Mod",
    "LFO2 Cutoff",
    "LFO2 Resonance",
    "LFO2 Panning",
    "LFO2 Volume",
    "LFO2 Attack",
    "LFO2 Decay",
    "LFO2 Sustain",
    "LFO2 Release",
    "DCA2 Volume",
    "DCA2 Attack",
    "DCA2 Decay",
    "DCA2 Sustain",
    "DCA2 Release",
    "OUT2 Stereo Width",
    "OUT2 Panning",
    "OUT2 FX Send",
    "OUT2 Volume",
    "DEF2 Pitchbend",
    "DEF2 Modwheel",
    "DEF2 Pressure",
    "DEF2 Velocity",
    "DEF2 Channel",
    "DEF2 Mono",
    "Chorus Wet",
    "Chorus Delay",
    "Chorus Feedback",
    "Chorus Rate",
    "Chorus Modulation",
    "Flanger Wet",
    "Flanger Delay",
    "Flanger Feedback",
    "Flanger Daft",
    "Phaser Wet",
    "Phaser Rate",
    "Phaser Feedback",
    "Phaser Depth",
    "Phaser Daft",
    "Delay Wet",
    "Delay Delay",
    "Delay Feedback",
    "Delay BPM",
    "Reverb Wet",
    "Reverb Room",
    "Reverb Damp",
    "Reverb Feedback",
    "Reverb Width",
    "Dynamic Compressor",
    "Dynamic Limiter",
    "Keyboard Low",
    "Keyboard High"};

//-------------------------------------------------------------------------
// synthv1_vst - impl.
//

void synthv1_vst::process(int nframes, const std::vector<synthv1_midi_event_t> &midi_in, float **inputs, float **outputs)
{
    const uint16_t nchannels = synthv1::channels();

    std::vector<synthv1_midi_event_t>::const_iterator event;

    uint32_t ndelta = 0;

    for (event = midi_in.begin(); event != midi_in.end(); ++event)
    {
        if (event->offset_frames > ndelta)
        {
            const uint32_t nread = event->offset_frames - ndelta;
            if (nread > 0)
            {
                synthv1::process(inputs, outputs, nread);
                for (uint16_t k = 0; k < nchannels; ++k)
                {
                    inputs[k] += nread;
                    outputs[k] += nread;
                }
            }
            ndelta = event->offset_frames;
        }
        synthv1::process_midi(event->buffer, event->length);
    }

    if (nframes > ndelta)
        synthv1::process(inputs, outputs, nframes - ndelta);
}

int synthv1_vst::loadState(const char *buffer)
{
    const bool running = this->running(false);

    this->setTuningEnabled(false);
    this->reset();

    static QHash<QString, synthv1::ParamIndex> s_hash;
    if (s_hash.isEmpty())
    {
        for (uint32_t i = 0; i < synthv1::NUM_PARAMS; ++i)
        {
            const synthv1::ParamIndex index = synthv1::ParamIndex(i);
            s_hash.insert(synthv1_param::paramName(index), index);
        }
    }

    QDomDocument doc(SYNTHV1_TITLE);
    if (doc.setContent(QByteArray(buffer)))
    {
        QDomElement ePreset = doc.documentElement();
        if (ePreset.tagName() == "preset")
        {
            for (QDomNode nChild = ePreset.firstChild();
                 !nChild.isNull();
                 nChild = nChild.nextSibling())
            {
                QDomElement eChild = nChild.toElement();
                if (eChild.isNull())
                    continue;
                if (eChild.tagName() == "params")
                {
                    for (QDomNode nParam = eChild.firstChild();
                         !nParam.isNull();
                         nParam = nParam.nextSibling())
                    {
                        QDomElement eParam = nParam.toElement();
                        if (eParam.isNull())
                            continue;
                        if (eParam.tagName() == "param")
                        {
                            synthv1::ParamIndex index = synthv1::ParamIndex(
                                eParam.attribute("index").toULong());
                            const QString &sName = eParam.attribute("name");
                            if (!sName.isEmpty())
                            {
                                if (!s_hash.contains(sName))
                                    continue;
                                index = s_hash.value(sName);
                            }
                            const float fValue = eParam.text().toFloat();
                            this->setParamValue(index,
                                                synthv1_param::paramSafeValue(index, fValue));
                        }
                    }
                }
                else if (eChild.tagName() == "tuning")
                {
                    synthv1_param::loadTuning(this, eChild);
                }
            }
        }
    }

    this->stabilize();
    this->reset();
    this->running(running);

    return 0;
}

int synthv1_vst::saveState(char **buffer)
{
    this->stabilize();

    QDomDocument doc(SYNTHV1_TITLE);
    QDomElement ePreset = doc.createElement("preset");
    ePreset.setAttribute("name", "VST_STATE");
    ePreset.setAttribute("version", CONFIG_BUILD_VERSION);

    QDomElement eParams = doc.createElement("params");
    for (uint32_t i = 0; i < synthv1::NUM_PARAMS; ++i)
    {
        QDomElement eParam = doc.createElement("param");
        const synthv1::ParamIndex index = synthv1::ParamIndex(i);
        eParam.setAttribute("index", QString::number(i));
        eParam.setAttribute("name", synthv1_param::paramName(index));
        const float fValue = this->paramValue(index);
        eParam.appendChild(doc.createTextNode(QString::number(fValue)));
        eParams.appendChild(eParam);
    }
    ePreset.appendChild(eParams);

    if (this->isTuningEnabled())
    {
        QDomElement eTuning = doc.createElement("tuning");
        synthv1_param::saveTuning(this, doc, eTuning);
        ePreset.appendChild(eTuning);
    }

    doc.appendChild(ePreset);

    const QByteArray data(doc.toByteArray());
    const char *value = data.constData();
    size_t size = data.size();

    *buffer = (char *)malloc(size);
    return sprintf(*buffer, "%s", value);
}

/**
 * NOTICE: About parameter helpers
 * 
 * VST plugins only support parameters ranging from 0.0f to 1.0f.
 * So actual parameter values beyond this range must be rescaled.
 * 
 * Related SynthV1 parameter convertion methods:
 * - synthv1_param::paramValue():   Convert VST scaled value to real value
 * - synthv1_param::paramScale()
 * - synthv1_param::paramScaleVST():
 *                                  Rescale real value to VST value
 * - synthv1_param::paramDisplay(): Get user-friendly display value
 *                                  (to display on host's generic UI)
 */

void synthv1_vst::setNormalizedParamValue(ParamIndex index, float fScale)
{
    float fValue = synthv1_param::paramValue(index, fScale);
    this->setParamValue(index, fValue);
}

float synthv1_vst::getNormalizedParamValue(ParamIndex index)
{
    float fValue = this->paramValue(index);
    return synthv1_param::paramScaleVST(index, fValue);
}

void synthv1_vst::getParamDisplay(ParamIndex index, char *buffer, size_t maxLen)
{
    float fValue = this->paramValue(index);
    strncpy(buffer, synthv1_param::paramDisplay(index, fValue).c_str(), maxLen);
}

void synthv1_vst::updatePreset(bool)
{
}

void synthv1_vst::updateParam(synthv1::ParamIndex index)
{
}

void synthv1_vst::updateParams()
{
}

void synthv1_vst::updateTuning()
{
}

void synthv1_vst::updateSampleRate(double)
{
}

//-------------------------------------------------------------------------
// synthv1_vst - Instantiation and cleanup.
//

synthv1_vst::synthv1_vst()
{
    // Init buffer size
    synthv1::setBufferSize(DEFAULT_BUFFER_SIZE);

    // Init parameter ports
    for (uint32_t i = 0; i < synthv1::NUM_PARAMS; ++i)
    {
        const synthv1::ParamIndex index = synthv1::ParamIndex(i);
        synthv1::setParamPort(index, &m_params[i]);
        this->setParamValue(index, synthv1_param::paramDefaultValue(index));
    }
}

synthv1_vst::~synthv1_vst()
{
}

QApplication *synthv1_vst::g_qapp_instance = nullptr;
unsigned int synthv1_vst::g_qapp_refcount = 0;

void synthv1_vst::qapp_instantiate(void)
{
    if (qApp == nullptr && g_qapp_instance == nullptr)
    {
        static int s_argc = 1;
        static const char *s_argv[] = {SYNTHV1_TITLE, nullptr};
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
        ::_putenv_s("QT_NO_GLIB", "1"); // Avoid glib event-loop...
#else
        ::setenv("QT_NO_GLIB", "1", 1); // Avoid glib event-loop...
#endif
        g_qapp_instance = new QApplication(s_argc, (char **)s_argv);
    }

    if (g_qapp_instance)
        ++g_qapp_refcount;
}

void synthv1_vst::qapp_cleanup(void)
{
    if (g_qapp_instance && --g_qapp_refcount == 0)
    {
        delete g_qapp_instance;
        g_qapp_instance = nullptr;
    }
}

QApplication *synthv1_vst::qapp_instance(void)
{
    return g_qapp_instance;
}

/**
 * VST plugin interface implementations
 */
#if 0
static void on_adjustment_value_changed(float values[], AEffect *effect)
{
    Plugin *plugin = (Plugin *)effect->ptr3;

    static Preset dummyPreset;

    for (int i = 0; i < kAmsynthParameterCount; i++)
    {
        Parameter &param = dummyPreset.getParameter(i);
        param.setValue(values[i]);
        plugin->synthesizer->setParameterValue((Param)i, values[i]);
        if (plugin->audioMaster && !strstr(hostProductString, "Qtractor"))
        {
            plugin->audioMaster(effect, audioMasterAutomate, i, 0, nullptr, param.getNormalisedValue());
        }
    }
}

void modal_midi_learn(Param param_index) {}

#endif // WITH_GUI

static intptr_t dispatcher(AEffect *effect, int opcode, int index, intptr_t val, void *ptr, float f)
{
    Plugin *plugin = (Plugin *)effect->ptr3;

    switch (opcode)
    {
    case effOpen:
        return 0;

    case effClose:
    {
        delete plugin;
        memset(effect, 0, sizeof(AEffect));
        free(effect);
        return 0;
    }

    case effSetProgram:
    {
        // @param val: Target program number
        return 1;
    }

    case effGetProgram:
        return plugin->programNumber;

    case effGetProgramName:
    {
        // @param ptr: Feed this pointer with program name
        //strncpy((char *)ptr, plugin->presetName.c_str(), 24);
        return 1;
    }

    case effGetParamLabel:
    {
        // TODO: "Label" examples: seconds, percentage, etc.
        //strncpy((char *)ptr, ParamLabels[index], 32);
        return 0;
    }

    case effGetParamDisplay:
    {
        plugin->synthesizer->getParamDisplay((synthv1::ParamIndex)index, (char *)ptr, 32);
        return 0;
    }

    case effGetParamName:
    {
        strncpy((char *)ptr, ParamNames[index], 32);
        return 0;
    }

    case effSetSampleRate:
    {
        plugin->synthesizer->setSampleRate(f);
        return 0;
    }

    case effSetBlockSize:
    case effMainsChanged:
        return 0;

#if 1
    case effEditGetRect:
    {
        static ERect rect = {0, 0, WINDOW_HEIGHT, WINDOW_WIDTH};
        ERect **er = (ERect **)ptr;
        *er = &rect;
        return 1;
    }
    case effEditOpen:
    {
        if (!plugin->editor)
        {
            plugin->editor = new synthv1_vst_editor(plugin->synthesizer, ptr);
            plugin->editor->updateUIParamValues();
        }

        return 1;
    }
    case effEditClose:
    {
        if (plugin->editor)
        {
            delete plugin->editor;
            plugin->editor = nullptr; // "delete" won't reset the pointer.
                                      // So must explicitly set to empty to avoid crash
        }

        return 0;
    }
    case effEditIdle:
    {
        if (plugin->editor)
            plugin->editor->idle();

        return 1;
    }
#endif

    case effGetChunk:
        return plugin->synthesizer->saveState((char **)ptr);

    case effSetChunk:
    {
        plugin->synthesizer->loadState((const char *)ptr);

        if (plugin->editor)
            plugin->editor->updateUIParamValues();

        return 0;
    }

    case effProcessEvents:
    {
        VstEvents *events = (VstEvents *)ptr;

        memset(plugin->midiBuffer, 0, 4096);
        unsigned char *buffer = plugin->midiBuffer;

        for (int32_t i = 0; i < events->numEvents; i++)
        {
            VstMidiEvent *event = (VstMidiEvent *)events->events[i];
            if (event->type != kVstMidiType)
            {
                continue;
            }

            int msgLength = 0;
            unsigned char statusByte = event->midiData[0];
            if (statusByte < MIDI_STATUS_NOTE_OFF)
            {
                continue; // Not a status byte
            }
            if (statusByte >= 0xF0)
            {
                continue; // Ignore system messages
            }
            switch (statusByte & 0xF0)
            {
            case MIDI_STATUS_PROGRAM_CHANGE:
            case MIDI_STATUS_CHANNEL_PRESSURE:
                msgLength = 2;
                break;
            default:
                msgLength = 3;
            }

            memcpy(buffer, event->midiData, msgLength);

            synthv1_midi_event_t midi_event;
            memset(&midi_event, 0, sizeof(midi_event));
            midi_event.offset_frames = event->deltaFrames;
            midi_event.buffer = buffer;
            midi_event.length = msgLength;
            plugin->midiEvents.push_back(midi_event);

            buffer += msgLength;

            assert(buffer < plugin->midiBuffer + 4096);
        }

        return 1;
    }

    case effCanBeAutomated:
    case effGetOutputProperties:
        return 0;

    case effGetPlugCategory:
        return kPlugCategSynth;
    case effGetEffectName:
        strcpy((char *)ptr, "SynthV1 VSTi");
        return 1;
    case effGetVendorString:
        strcpy((char *)ptr, "Rui no Capella, AnClark Liu");
        return 1;
    case effGetProductString:
        strcpy((char *)ptr, "synthv1");
        return 1;
    case effGetVendorVersion:
        return 0;

    case effCanDo:
        if (strcmp("receiveVstMidiEvent", (char *)ptr) == 0 ||
            false)
            return 1;
        if (strcmp("midiKeyBasedInstrumentControl", (char *)ptr) == 0 ||
            strcmp("midiSingleNoteTuningChange", (char *)ptr) == 0 ||
            strcmp("receiveVstSysexEvent", (char *)ptr) == 0 ||
            strcmp("sendVstMidiEvent", (char *)ptr) == 0 ||
            false)
            return 0;
#if DEBUG
        fprintf(logFile, "[amsynth_vst] unhandled canDo: %s\n", (char *)ptr);
        fflush(logFile);
#endif
        return 0;

    case effGetTailSize:
    case effIdle:
    case effGetParameterProperties:
        return 0;

    case effGetVstVersion:
        return 2400;

    case effGetMidiKeyName:
    case effStartProcess:
    case effStopProcess:
    case effBeginSetProgram:
    case effEndSetProgram:
    case effBeginLoadBank:
        return 0;

    default:
        return 0;
    }
}

static void process(AEffect *effect, float **inputs, float **outputs, int numSampleFrames)
{
    Plugin *plugin = (Plugin *)effect->ptr3;
    plugin->synthesizer->process(numSampleFrames, plugin->midiEvents, inputs, outputs);
    plugin->midiEvents.clear();
}

static void processReplacing(AEffect *effect, float **inputs, float **outputs, int numSampleFrames)
{
    Plugin *plugin = (Plugin *)effect->ptr3;
    plugin->synthesizer->process(numSampleFrames, plugin->midiEvents, inputs, outputs);
    plugin->midiEvents.clear();
}

static void setParameter(AEffect *effect, int i, float f)
{
    Plugin *plugin = (Plugin *)effect->ptr3;
    plugin->synthesizer->setNormalizedParamValue((synthv1::ParamIndex)i, f);

    if (plugin->editor)
        plugin->editor->updateUIParamValues();
}

static float getParameter(AEffect *effect, int i)
{
    Plugin *plugin = (Plugin *)effect->ptr3;
    return plugin->synthesizer->getNormalizedParamValue((synthv1::ParamIndex)i);
}

// TODO: Currently unimplemented
static int getNumPrograms()
{
    //return PresetController::getPresetBanks().size() * kPresetsPerBank;
    return 0;
}

extern "C"
#if _WIN32
    __declspec(dllexport)
#else
    __attribute__((visibility("default")))
#endif
        AEffect *VSTPluginMain(audioMasterCallback audioMaster)
{
    if (audioMaster)
    {
        audioMaster(nullptr, audioMasterGetProductString, 0, 0, hostProductString, 0.0f);
    }
    AEffect *effect = (AEffect *)calloc(1, sizeof(AEffect));
    effect->magic = kEffectMagic;
    effect->dispatcher = dispatcher;
    effect->process = process;
    effect->setParameter = setParameter;
    effect->getParameter = getParameter;
    effect->numPrograms = getNumPrograms();
    effect->numParams = synthv1::ParamIndex::NUM_PARAMS;
    effect->numInputs = 2;
    effect->numOutputs = 2;
    effect->flags = effFlagsCanReplacing | effFlagsIsSynth | effFlagsProgramChunks;
#if 1
    effect->flags |= effFlagsHasEditor; // On Windows, amsynth's GTK GUI works in REAPER! (^.^)
#endif                                  // WITH_GUI
    // Do no use the ->user pointer because ardour clobbers it
    effect->ptr3 = new Plugin(audioMaster);
    effect->uniqueID = CCONST('s', 'y', 'v', '1');
    effect->processReplacing = processReplacing;

    return effect;
}

#if _WIN32

/** Entrance for MSVC */
extern "C" AEffect *MAIN(audioMasterCallback audioMaster)
{
    return VSTPluginMain(audioMaster);
}

/** Entrance for CMake */
extern "C"
{
    __attribute__((visibility("default")))
    AEffect *
    wrap(audioMasterCallback audioMaster) asm("main");

    AEffect *wrap(audioMasterCallback audioMaster)
    {
        return VSTPluginMain(audioMaster);
    }
}

#else

// this is required because GCC throws an error if we declare a non-standard function named 'main'
extern "C" __attribute__((visibility("default"))) AEffect *main_plugin(audioMasterCallback audioMaster) asm("main");

extern "C" __attribute__((visibility("default"))) AEffect *main_plugin(audioMasterCallback audioMaster)
{
    return VSTPluginMain(audioMaster);
}

#endif
