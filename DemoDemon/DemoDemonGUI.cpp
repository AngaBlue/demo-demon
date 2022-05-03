#include "pch.h"
#include "DemoDemon.h"

std::string DemoDemon::GetPluginName() {
	return menuTitle;
}

void DemoDemon::RenderSettings() {
	// Settings
	CreateToggleableCheckbox("demodemon_enabled", "Enable overlay");
	CreateToggleableCheckbox("demodemon_display_game", "Display game stats");
	CreateToggleableCheckbox("demodemon_display_session", "Display session stats");
	CreateToggleableCheckbox("demodemon_display_total", "Display total demos");

	CVarWrapper cvar = cvarManager->getCvar("demodemon_background_opacity");
	if (!cvar) return;

	float opacity = cvar.getFloatValue();

	ImGui::TextUnformatted("Background opacity:");
	if (ImGui::SliderFloat("", &opacity, 0, 1, "%.2f")) {
		cvar.setValue(opacity);
	}

	CreateToggleableCheckbox("demodemon_force_display", "Force display overlay");

	// Note
	ImGui::Separator();
	ImGui::TextUnformatted("Note:\nReposition the overlay by dragging it whilst the Bakkesmod menu is open.");

	// Credit
	ImGui::Separator();
	ImGui::TextUnformatted("Demo Demon by ");
	ImGui::SameLine(0, 0);
	ImGui::TextColored({ 0, 1, 1, 1 }, "AngaBlue");
	ImGui::TextUnformatted("https://anga.blue");
}

void DemoDemon::Render()
{
	// Check enabled
	if (!GetBoolCvar("demodemon_enabled")) return;

	// Check if in game
	if (!GetBoolCvar("demodemon_force_display") && !(gameWrapper->IsInGame() || gameWrapper->IsInOnlineGame()) || gameWrapper->IsInFreeplay()) return;

	// Settings
	bool displayGame = GetBoolCvar("demodemon_display_game");
	bool displaySession = GetBoolCvar("demodemon_display_session");
	bool displayTotal = GetBoolCvar("demodemon_display_total");

	if (!displayGame && !displaySession && !displayTotal) return;

	// Style
	float opacity = GetFloatCvar("demodemon_background_opacity", 0.5);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, 0);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0, 0, 0, opacity });

	// Font
	if (font) ImGui::PushFont(font);

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar ;
	if (!ImGui::Begin(menuTitle.c_str(), &isWindowOpen_, flags))
	{
		ImGui::End();
		return;
	}

	// Size & Position
	float height = 16 + (FONT_SIZE + 4) * (displayGame + displaySession + displayTotal);
	ImGui::SetWindowSize({ 340, height });
	ImGui::SetWindowPos({ ImGui::GetIO().DisplaySize.x - ImGui::GetWindowSize().x - 10, 10}, ImGuiCond_FirstUseEver);

	// Content
	ImGui::Columns(3, 0, false);

	ImGui::SetColumnWidth(0, 160);
	if (displayGame) ImGui::Text("GAME KD");
	if (displaySession) ImGui::Text("SESSION KD");
	if (displayTotal) ImGui::Text("TOTAL");

	ImGui::NextColumn();
	ImGui::SetColumnWidth(1, 80);
	if (displayGame) {
		std::string text = fmt::format("{:.2f}", game.getKD());
		RightAlignTextInColumn(text);
		ImGui::TextColored(game.getKDColor(), text.c_str());
	}
	if (displaySession) {
		std::string text = fmt::format("{:.2f}", session.getKD());
		RightAlignTextInColumn(text);
		ImGui::TextColored(session.getKDColor(), text.c_str());
	}

	ImGui::NextColumn();
	ImGui::SetColumnWidth(2, 100);

	if (displayGame) {
		std::string text = fmt::format("{:d}:{:d}", game.getKills(), game.getDeaths());
		RightAlignTextInColumn(text);
		ImGui::Text(text.c_str());
	}
	if (displaySession) {
		std::string text = fmt::format("{:d}:{:d}", session.getKills(), session.getDeaths());
		RightAlignTextInColumn(text);
		ImGui::Text(text.c_str());
	}
	if (displayTotal) {
		std::string text = fmt::format("{:d}", total);
		RightAlignTextInColumn(text);
		ImGui::Text(text.c_str());
	}

	// End columns
	ImGui::Columns();

	ImGui::End();

	// Remove font
	if (font) ImGui::PopFont();

	// Remove style vars
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor();
}

std::string DemoDemon::GetMenuTitle()
{
	return menuTitle;
}

std::string DemoDemon::GetMenuName()
{
	return menuName;
}

void DemoDemon::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

bool DemoDemon::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

bool DemoDemon::IsActiveOverlay()
{
	return false;
}

void DemoDemon::OnOpen()
{
	isWindowOpen_ = true;
}

void DemoDemon::RightAlignTextInColumn(const std::string text)
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
}

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

void DemoDemon::CreateToggleableCheckbox(const std::string name, const char* const display)
{
	CVarWrapper cvar = cvarManager->getCvar(name);
	if (!cvar) return;

	bool state = cvar.getBoolValue();

	if (ImGui::Checkbox(display, &state)) {
		cvar.setValue(state);
	}
}