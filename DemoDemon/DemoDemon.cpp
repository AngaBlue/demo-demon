#include "pch.h"
#include "DemoDemon.h"


BAKKESMOD_PLUGIN(DemoDemon, "Demo Demon", plugin_version, PLUGINTYPE_BOTAI)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void DemoDemon::onLoad()
{
	_globalCvarManager = cvarManager;

	// Register cVars
	cvarManager->registerCvar("demodemon_enabled", "1");
	cvarManager->registerCvar("demodemon_display_game", "1");
	cvarManager->registerCvar("demodemon_display_session", "1");
	cvarManager->registerCvar("demodemon_display_total", "1");

	// Stat ticket event fired on demo
	gameWrapper->HookEventWithCaller<ServerWrapper>("Function TAGame.GFxHUD_TA.HandleStatTickerMessage",
		[this](ServerWrapper caller, void* params, std::string eventname) {
			DemoDemon::onStatTickerMessage(params);
		});

	// Start Game
	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.WaitingForPlayers.BeginState", std::bind(&DemoDemon::StartGame, this));
	gameWrapper->HookEvent("Function Engine.PlayerInput.InitInputSystem", std::bind(&DemoDemon::StartGame, this));

	// Initialize total demos
	auto stats = CareerStatsWrapper().GetStatValues();
	for (size_t i = 0; i < stats.size(); i++)
	{
		auto stat = stats[i];
		if (stat.stat_name == "Demolish") {
			total = stat.ranked + stat.unranked + stat.private_;
		}
	}

	// Start rendering overlay
	gameWrapper->SetTimeout(std::bind(&DemoDemon::StartGame, this), 0.1f);
}

void DemoDemon::onUnload()
{
	gameWrapper->UnhookEvent("Function TAGame.GFxHUD_TA.HandleStatTickerMessage");
	gameWrapper->UnhookEvent("Function GameEvent_Soccar_TA.WaitingForPlayers.BeginState");
	gameWrapper->UnhookEvent("Function Engine.PlayerInput.InitInputSystem");
}

void DemoDemon::onStatTickerMessage(void* params)
{
	StatTickerParams* pStruct = (StatTickerParams*)params;
	PriWrapper receiver = PriWrapper(pStruct->Receiver);
	PriWrapper victim = PriWrapper(pStruct->Victim);
	StatEventWrapper statEvent = StatEventWrapper(pStruct->StatEvent);

	if (statEvent.GetEventName() != "Demolish") return;
	if (!receiver) return;
	if (!victim) return;

	PlayerControllerWrapper playerController = gameWrapper->GetPlayerController();
	if (!playerController) return;

	PriWrapper playerPRI = playerController.GetPRI();
	if (!playerPRI) return;

	// Death
	if (playerPRI.memory_address == victim.memory_address)
	{
		game.addDeath();
		session.addDeath();
		return;
	}

	// Kill
	if (playerPRI.memory_address == receiver.memory_address)
	{
		game.addKill();
		session.addKill();
		total++;
		return;
	}
}


void DemoDemon::StartGame()
{
	game = KD();
	StartRender();
}

bool DemoDemon::GetBoolCvar(const std::string name, const bool fallback)
{
	CVarWrapper cvar = cvarManager->getCvar(name);
	if (cvar) return cvar.getBoolValue();
	else return fallback;
}