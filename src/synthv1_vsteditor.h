// synthv1_vsteditor.h
//
/****************************************************************************
   Copyright (C) 2021, AnClark Liu. All rights reserved.

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

#ifndef __synthv1_vsteditor_h
#define __synthv1_vsteditor_h

#include <QWindow>

#include "vst/vestige.h"

// Polyfill for windows size (minimal suitable size)
// Qt cannot determine the right window size on Windows.
#define UI_WINDOWS_RECOMMENDED_WIDTH 1380
#define UI_WINDOWS_RECOMMENDED_HEIGHT 650

// Forward decls.
class synthv1_vst;
class synthv1widget_vst;

//-------------------------------------------------------------------------
// synthv1_vst_editor - decl.
//

class synthv1_vst_editor
{
public:
    // Constructor.
    synthv1_vst_editor(synthv1_vst *pSynth, void *pParent, AEffect *pEffect);

    // Destructor.
    ~synthv1_vst_editor();

    // Accessors.
    //const LV2UI_Controller& controller() const;
    //void write_function(synthv1::ParamIndex index, float fValue) const;

    // Operate UI instances.
    void instanciate();
    void cleanup();
    synthv1widget_vst *getWidget() { return this->m_pWidget; }

    // Operate window handle.
    // WId is Qt's internal type. See: /usr/include/qt6/QtGui/qwindowdefs.h
    WId getWinId() { return this->m_pWinId; }
    WId getParent() { return this->m_pParent; }
    void setParent(WId pParent) { this->m_pParent = pParent; }

    // UI behavior controls
    int showUI();
    int hideUI();
    int resizeUI(int width, int height) const;
    int idle();

    // Param handle.
    void updateUIParamValues();

private:
    synthv1_vst *m_pSynth; // NOTICE: LV2 stores synthv1_lv2 instance pointer into SDK. But VST not.
    synthv1widget_vst *m_pWidget = nullptr;
    WId m_pWinId;
    WId m_pParent;

    AEffect *m_pEffect;

    // Instance variables.
    //LV2UI_Controller     m_controller;
    //LV2UI_Write_Function m_write_function;
};

#endif // __synthv1_vsteditor_h

// end of synthv1_vsteditor.h
