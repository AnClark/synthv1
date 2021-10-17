// synthv1_vstui.cpp
//
/****************************************************************************
   Copyright (C) 2012-2021, rncbc aka Rui Nuno Capela. All rights reserved.

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

#include "synthv1_vstui.h"
#include "synthv1_vst.h"

#include <synthv1widget_vst.h>

#include <string.h>

//-------------------------------------------------------------------------
// synthv1_vstui - impl.
//

synthv1_vstui::synthv1_vstui(synthv1_vst *pSynth, AEffect *pEffect)
	: synthv1_ui(pSynth, true)
{
	this->m_pEffect = pEffect;
	this->m_pSynth = pSynth;
}

// Accessor: Update VST parameter when modifying on UI
void synthv1_vstui::write_function(
	synthv1::ParamIndex index, float fValue) const
{
	Plugin *plugin = (Plugin *)m_pEffect->ptr3;
	if (plugin->audioMaster && !strstr(hostProductString, "Qtractor"))
	{
		plugin->audioMaster(m_pEffect, audioMasterAutomate, index, 0, nullptr, m_pSynth->getNormalizedParamValue(index));
	}
}

// end of synthv1_vstui.cpp
