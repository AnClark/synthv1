// DistrhoPluginInfo.h
//
/****************************************************************************
   Copyright (C) 2023, AnClark Liu. All rights reserved.

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

// Specify a default UI size.
// This can avoid temporary UI creations by DPF (in order to know the actual UI size)
#define DISTRHO_UI_DEFAULT_WIDTH 1100
#define DISTRHO_UI_DEFAULT_HEIGHT 510

#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED

// end of DistrhoPluginInfo.h
