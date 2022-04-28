#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

struct StatTickerParams {
	uintptr_t Receiver;
	uintptr_t Victim;
	uintptr_t StatEvent;
};

class DemoDemon: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	// Inherited via PluginSettingsWindow
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;

	// Inherited via PluginWindow
	bool isWindowOpen_ = true;
	bool isMinimized_ = false;
	std::string menuTitle = "Demo Demon";
	std::string menuName = "demodemon";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	void RightAlignTextInColumn(std::string text);
	virtual void OnClose() override;


	// Demo Demon
private:
	ImFont* largeFont;

	double kd = 0;
	unsigned int kills, deaths;

	void onStatTickerMessage(void* params);
	void SetKD();
	double GetKD();
	void StartRender();
	void StopRender();
	void StartGame();
};

