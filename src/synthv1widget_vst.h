// synthv1widget_vst.h
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

#ifndef __synthv1widget_vst_h
#define __synthv1widget_vst_h

#include "synthv1widget.h"
#include "synthv1_vstui.h"

// Forward decls.
class synthv1_vst;

//-------------------------------------------------------------------------
// synthv1widget_vst - decl.
//

class synthv1widget_vst : public synthv1widget
{
public:
	// Constructor.
	synthv1widget_vst(synthv1_vst *pSynth, AEffect *pEffect);

	// Destructor.
	~synthv1widget_vst();

	// LV2 port event dispatcher.
	//void port_event(uint32_t port_index,
	//	uint32_t buffer_size, uint32_t format, const void *buffer);

	// Render parameter values on UI
	void setUIParamValue(synthv1::ParamIndex paramIndex, float value);

	bool isIdleClosed() const;

protected:
	// Synth engine accessor.
	synthv1_ui *ui_instance() const;

	// Param methods.
	void updateParam(synthv1::ParamIndex index, float fValue) const;

	// Close event handler.
	void closeEvent(QCloseEvent *pCloseEvent);

private:
	// Instance variables.
	synthv1_vstui *m_pSynthUi;
	AEffect *m_pEffect;

	bool m_bIdleClosed;
};

#endif // __synthv1widget_vst_h

// end of synthv1widget_vst.h
