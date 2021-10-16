// synthv1widget_vst.cpp
//
/****************************************************************************
   Copyright (C) 2012-2020, rncbc aka Rui Nuno Capela. All rights reserved.
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

#include "synthv1widget_vst.h"

#include "synthv1_vst.h"

#include <QApplication>
#include <QFileInfo>
#include <QDir>

#include "synthv1widget_palette.h"

#include <QCloseEvent>

#include <QStyleFactory>

#ifndef CONFIG_LIBDIR
#if defined(__x86_64__)
#define CONFIG_LIBDIR CONFIG_PREFIX "/lib64"
#else
#define CONFIG_LIBDIR CONFIG_PREFIX "/lib"
#endif
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define CONFIG_PLUGINSDIR CONFIG_LIBDIR "/qt4/plugins"
#else
#define CONFIG_PLUGINSDIR CONFIG_LIBDIR "/qt5/plugins"
#endif

//-------------------------------------------------------------------------
// synthv1widget_vst - impl.
//

synthv1widget_vst::synthv1widget_vst(synthv1_vst *pSynth)
	//LV2UI_Controller controller, LV2UI_Write_Function write_function )
	: synthv1widget()
{
	// Check whether under a dedicated application instance...
	QApplication *pApp = synthv1_vst::qapp_instance();
	if (pApp)
	{
		// Special style paths...
		if (QDir(CONFIG_PLUGINSDIR).exists())
			pApp->addLibraryPath(CONFIG_PLUGINSDIR);
		// Custom color/style themes...
		synthv1_config *pConfig = synthv1_config::getInstance();
		if (pConfig)
		{
			if (!pConfig->sCustomColorTheme.isEmpty())
			{
				QPalette pal;
				if (synthv1widget_palette::namedPalette(
						pConfig, pConfig->sCustomColorTheme, pal))
					pApp->setPalette(pal);
			}
			if (!pConfig->sCustomStyleTheme.isEmpty())
			{
				pApp->setStyle(
					QStyleFactory::create(pConfig->sCustomStyleTheme));
			}
		}
	}

	m_bIdleClosed = false;

	// Initialize (user) interface stuff...
	m_pSynthUi = new synthv1_ui(pSynth, false);

	// Initialise preset stuff...
	clearPreset();

	// Initial update, always...
	//resetParamValues();
	resetParamKnobs();

	// May initialize the scheduler/work notifier.
	openSchedNotifier();
}

// Destructor.
synthv1widget_vst::~synthv1widget_vst(void)
{
	delete m_pSynthUi;
}

bool synthv1widget_vst::isIdleClosed(void) const
{
	return m_bIdleClosed;
}

// Close event handler.
void synthv1widget_vst::closeEvent(QCloseEvent *pCloseEvent)
{
	synthv1widget::closeEvent(pCloseEvent);

	if (pCloseEvent->isAccepted())
		m_bIdleClosed = true;
}

// Synth engine accessor.
synthv1_ui *synthv1widget_vst::ui_instance(void) const
{
	return m_pSynthUi;
}

// Render parameter values on UI.
void synthv1widget_vst::setUIParamValue(synthv1::ParamIndex paramIndex, float value)
{
	this->setParamValue(paramIndex, value);
}

// Param method.
// This method sets host's param values
void synthv1widget_vst::updateParam(synthv1::ParamIndex index, float fValue) const
{
	m_pSynthUi->setParamValue(index, fValue);
}

// end of synthv1widget_vst.cpp
