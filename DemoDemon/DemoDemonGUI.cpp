#include "pch.h"
#include "DemoDemon.h"
#include "bakkesmod/wrappers/GuiManagerWrapper.h"

#define FONT_SIZE 32
#define FONT_NAME "Bourgeois"
#define FONT_FILE "Bourgeois-BoldItalic.ttf"

// Plugin Settings Window code here
std::string DemoDemon::GetPluginName() {
	return menuTitle;
}

void DemoDemon::RenderSettings() {
	ImGui::TextUnformatted("Demo Demon Plugin Settings");
	ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, "KD: %.2f", this->GetKD());
	CVarWrapper overlayCvar = cvarManager->getCvar("demo_overlay");

	if (!overlayCvar) {
		return;
	}

	bool overlay = overlayCvar.getBoolValue();

	if (ImGui::Checkbox("Enable overlay", &overlay)) {
		overlayCvar.setValue(overlay);
	}
}


// Do ImGui rendering here
void DemoDemon::Render()
{
	// Check if in game
	if (!(gameWrapper->IsInGame() || gameWrapper->IsInOnlineGame()) || gameWrapper->IsInFreeplay()) return;

	// Load font
	if (!largeFont) {
		auto gui = gameWrapper->GetGUIManager();
		largeFont = gui.GetFont(FONT_NAME);
	}

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;
	if (!ImGui::Begin(menuTitle.c_str(), &isWindowOpen_, flags))
	{
		ImGui::End();
		return;
	}

	// Style
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowMinSize = { 200, 140 };

	// Content
	auto kd = this->GetKD();
	// Calculate KD RGB
	float r, g, b = 0;
	if (kd <= 1) {
		r = 1;
		g = kd;
	}
	else if (kd <= 2)
	{
		r = (2 - kd);
		g = 1;
	}
	else {
		r = 0;
		g = 1;
	}

	if (largeFont) {
		ImGui::PushFont(largeFont);
	}

	ImGui::Columns(2, 0, false);

	ImGui::SetColumnWidth(0, 120);
	ImGui::Text("KD");
	ImGui::Text("KILLS");
	ImGui::Text("DEATHS");

	ImGui::NextColumn();
	ImGui::SetColumnWidth(1, 80);
	std::string text = fmt::format("{:.2f}", kd);
	RightAlignTextInColumn(text);
	ImGui::TextColored({ r, g, b, 1.0f }, text.c_str());
	text = fmt::format("{:d}", kills);
	RightAlignTextInColumn(text);
	ImGui::Text(text.c_str());
	text = fmt::format("{:d}", deaths);
	RightAlignTextInColumn(text);
	ImGui::Text(text.c_str());

	ImGui::Columns();

	if (largeFont) {
		ImGui::PopFont();
	}
	// ImGui renreding ends here
	ImGui::End();
}

// Title to give the menu
std::string DemoDemon::GetMenuTitle()
{
	return menuTitle;
}

// Title to give the menu
std::string DemoDemon::GetMenuName()
{
	return menuName;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void DemoDemon::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));

	auto gui = gameWrapper->GetGUIManager();
	auto [res, font] = gui.LoadFont(FONT_FILE, FONT_FILE, FONT_SIZE);

	if (res == 0) {
		cvarManager->log("Failed to load the font!");
	}
	else if (res == 1) {
		cvarManager->log("The font will be loaded");
	}
	else if (res == 2 && font) {
		largeFont = font;
	}
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool DemoDemon::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool DemoDemon::IsActiveOverlay()
{
	return false;
}

// Called when window is opened
void DemoDemon::OnOpen()
{
	isWindowOpen_ = true;
}

void DemoDemon::RightAlignTextInColumn(std::string text)
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
}

// Called when window is closed
void DemoDemon::OnClose()
{
	isWindowOpen_ = false;
}

void DemoDemon::StartRender()
{
	cvarManager->executeCommand("openmenu " + GetMenuName());
}

void DemoDemon::StopRender()
{
	cvarManager->executeCommand("closemenu " + GetMenuName());
}