#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND "rncbc"
#define DISTRHO_PLUGIN_NAME "synthv1"
#define DISTRHO_PLUGIN_URI "https://github.com/rncbc/synthv1"
#define DISTRHO_PLUGIN_CLAP_ID "rncbc.synthv1"

#define DISTRHO_PLUGIN_HAS_UI 1
#define DISTRHO_PLUGIN_HAS_EMBED_UI 1
#define DISTRHO_PLUGIN_HAS_EXTERNAL_UI 1
#define DISTRHO_PLUGIN_IS_RT_SAFE 1
#define DISTRHO_PLUGIN_IS_SYNTH 1
#define DISTRHO_PLUGIN_NUM_INPUTS 2
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2
#define DISTRHO_UI_USER_RESIZABLE 1

// Allow UI to access DSP side directly.
// This is discouraged by DPF, but synthv1 indeed need this,
// because synthv1 UI accesses synth engine, see synthv1widget.cpp.
#define DISTRHO_PLUGIN_WANT_DIRECT_ACCESS 1

//#define DISTRHO_UI_DEFAULT_WIDTH DistrhoArtworkNekobi::backgroundWidth
//#define DISTRHO_UI_DEFAULT_HEIGHT DistrhoArtworkNekobi::backgroundHeight

#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED