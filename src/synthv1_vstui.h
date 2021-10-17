// synthv1_vstui.h
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

#ifndef __synthv1_lv2ui_h
#define __synthv1_lv2ui_h

#include "synthv1_ui.h"

#include "vst/vestige.h"

// Forward decls.
class synthv1_vst;

//-------------------------------------------------------------------------
// synthv1_vstui - decl.
//

class synthv1_vstui : public synthv1_ui
{
public:
    // Constructor.
    synthv1_vstui(synthv1_vst *pSynth, AEffect *pEffect);

    // Accessor: Update VST parameter when modifying on UI
    void write_function(synthv1::ParamIndex index, float fValue) const;

private:
    // Instance variables.
    synthv1_vst *m_pSynth;
    AEffect *m_pEffect;
};

#endif // __synthv1_vstui_h

// end of synthv1_vstui.h
