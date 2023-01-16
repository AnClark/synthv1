#include "synthv1_dpfui.h"
#include "synthv1_dpf.h"

#include <QWindow>
#include <QApplication>

//-------------------------------------------------------------------------
// SynthV1PluginUI - DPF Plugin UI interface.
//

START_NAMESPACE_DISTRHO

SynthV1PluginUI::SynthV1PluginUI()
	: UI(), fWidget(nullptr)
{
	const bool standalone = isStandalone();
	d_stdout("isStandalone %d", (int)standalone);

	// synthv1 UI requires directly accessing synth instance.
	// This is discouraged by DPF, but synthv1 do need this due to its designation.
	SynthV1Plugin *fDspInstance = (SynthV1Plugin*) UI::getPluginInstancePointer();

	fWidget = new synthv1widget_dpf(fDspInstance->getSynthesizer(), this);

	const QSize& hint = fWidget->sizeHint();
	fWidget->resize(hint.width(), hint.height());

	fParent = (WId) getParentWindowHandle();
	fWinId = fWidget->winId();

	if (fParent)
	{
		fWidget->windowHandle()->setParent(QWindow::fromWinId(fParent));
	}

	fWidget->show();
	
}

SynthV1PluginUI::~SynthV1PluginUI()
{
	if (fWidget)
	{
		delete fWidget;
		fWidget = nullptr;
	}
}

/* --------------------------------------------------------------------------------------------------------
* DSP/Plugin Callbacks */

/**
	A parameter has changed on the plugin side.
	This is called by the host to inform the UI about parameter changes.
*/
void SynthV1PluginUI::parameterChanged(uint32_t index, float value)
{
	fWidget->setUIParamValue(synthv1::ParamIndex(index), value);
}

/* --------------------------------------------------------------------------------------------------------
* External Window overrides */

void SynthV1PluginUI::focus()
{
	d_stdout("focus");

	fWidget->setFocus();
}

uintptr_t SynthV1PluginUI::getNativeWindowHandle() const noexcept
{
	return (uintptr_t)fWidget->windowHandle()->winId();
}

void SynthV1PluginUI::sizeChanged(uint width, uint height)
{
	d_stdout("sizeChanged %u %u", width, height);
	UI::sizeChanged(width, height);

	if (fWidget != 0)
		fWidget->resize(width, height);
}

void SynthV1PluginUI::titleChanged(const char* const title)
{
	d_stdout("titleChanged %s", title);

	DISTRHO_SAFE_ASSERT_RETURN(fWidget != 0,);
	fWidget->setWindowTitle(QString(title));
}

void SynthV1PluginUI::transientParentWindowChanged(const uintptr_t winId)
{
	d_stdout("transientParentWindowChanged %lu", winId);

	DISTRHO_SAFE_ASSERT_RETURN(fWidget != 0,);
	// NOTICE: Seems not implemented by Qt
}

void SynthV1PluginUI::visibilityChanged(const bool visible)
{
	d_stdout("visibilityChanged %d", visible);

	DISTRHO_SAFE_ASSERT_RETURN(fWidget != 0,);

	if (visible)
	{
		fWidget->show();
		fWidget->raise();
		fWidget->activateWindow();
	}
	else
		fWidget->hide();
}

void SynthV1PluginUI::uiIdle()
{
	// d_stdout("uiIdle");

	if (fWidget)
	{
		QApplication::processEvents();
		return;
	}
}

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
	return new SynthV1PluginUI();
}

END_NAMESPACE_DISTRHO


//-------------------------------------------------------------------------
// synthv1_dpfui - Synth engine accessor: impl.
//

synthv1_dpfui::synthv1_dpfui(synthv1_dpf *pSynth, DISTRHO::SynthV1PluginUI *pluginUiInterface)
	: synthv1_ui(pSynth, true), m_plugin_ui(pluginUiInterface)
{
}

void synthv1_dpfui::write_function(synthv1::ParamIndex index, float fValue) const
{
	m_plugin_ui->setParameterValue(index, fValue);
}
