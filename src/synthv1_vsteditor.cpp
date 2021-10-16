// synthv1_vsteditor.cpp
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

#include "synthv1_vsteditor.h"
#include "synthv1_vst.h"

#include <synthv1widget_vst.h>

#include <QApplication>

//-------------------------------------------------------------------------
// synthv1_vst_editor - impl.
//

synthv1_vst_editor::synthv1_vst_editor(synthv1_vst *pSynth, void *pParent)
{
    this->m_pSynth = pSynth;
    this->m_pParent = (WId)pParent;

    this->instanciate();
}

synthv1_vst_editor::~synthv1_vst_editor()
{
    this->cleanup();
}

//-------------------------------------------------------------------------
// synthv1_vst_editor - VST UI desc.
//

void synthv1_vst_editor::instanciate()
{
    if (this->m_pSynth == nullptr)
        return;
    if (!m_pParent)
        return;

    this->m_pWidget = new synthv1widget_vst(this->m_pSynth);

    this->m_pWinId = this->m_pWidget->winId();
    this->m_pWidget->windowHandle()->setParent(QWindow::fromWinId(this->m_pParent));
    this->m_pWidget->show();
}

void synthv1_vst_editor::cleanup()
{
    if (this->m_pWidget)
    {
        delete m_pWidget;
        m_pWidget = nullptr;
    }
}

void synthv1_vst_editor::updateUIParamValues()
{
    for (int i = 0; i < synthv1::ParamIndex::NUM_PARAMS; i++)
    {
        const synthv1::ParamIndex index = synthv1::ParamIndex(i);
        const float value = this->m_pSynth->paramValue(index);

        if (this->m_pWidget)
            this->m_pWidget->setUIParamValue(index, value);
    }
}

int synthv1_vst_editor::idle()
{
    if (this->m_pWidget && !this->m_pWidget->isIdleClosed())
    {
        QApplication::processEvents();
        return 0;
    }
    else
    {
        return 1;
    }
}

int synthv1_vst_editor::showUI()
{
    if (this->m_pWidget)
    {
        m_pWidget->show();
        m_pWidget->raise();
        m_pWidget->activateWindow();
        return 0;
    }
    else
    {
        return 1;
    }
}

int synthv1_vst_editor::hideUI()
{
    if (this->m_pWidget)
    {
        m_pWidget->hide();
        return 0;
    }
    else
    {
        return 1;
    }
}

int synthv1_vst_editor::resizeUI(int width, int height) const
{
    if (this->m_pWidget)
    {
        m_pWidget->resize(width, height);
        return 0;
    }
    else
    {
        return 1;
    }
}

// end of synthv1_vst_editor.cpp
