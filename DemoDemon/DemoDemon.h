#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
#include "KD.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

struct StatTickerParams {
	uintptr_t Receiver;
	uintptr_t Victim;
	uintptr_t StatEvent;
};

class DemoDemon: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{
	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;

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
	virtual void OnClose() override;

	/*
	 * Demo Demon
	 */

	// Logic
	KD game;
	KD session;
	unsigned int total = 0;

	void StartGame();
	void onStatTickerMessage(void* params);
	bool GetBoolCvar(const std::string name, const bool fallback = false);
	float GetFloatCvar(const std::string name, const float fallback = 1);

	// Render
	ImFont* font;

	void StartRender();
	void StopRender();

	void RightAlignTextInColumn(const std::string text);
	void CreateToggleableCheckbox(const std::string name, const char* const display);
};

