#include "pch.h"
#include "DemoDemon.h"


BAKKESMOD_PLUGIN(DemoDemon, "Demo Demon", plugin_version, PLUGINTYPE_BOTAI)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void DemoDemon::onLoad()
{
	_globalCvarManager = cvarManager;
	//cvarManager->log("Plugin loaded!");

	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	cvarManager->log("Hello notifier!");
	//}, "", 0);

	//auto cvar = cvarManager->registerCvar("template_cvar", "hello-cvar", "just a example of a cvar");
	//auto cvar2 = cvarManager->registerCvar("template_cvar2", "0", "just a example of a cvar with more settings", true, true, -10, true, 10 );

	//cvar.addOnValueChanged([this](std::string cvarName, CVarWrapper newCvar) {
	//	cvarManager->log("the cvar with name: " + cvarName + " changed");
	//	cvarManager->log("the new value is:" + newCvar.getStringValue());
	//});

	//cvar2.addOnValueChanged(std::bind(&DemoDemon::YourPluginMethod, this, _1, _2));

	// enabled decleared in the header
	//enabled = std::make_shared<bool>(false);
	//cvarManager->registerCvar("TEMPLATE_Enabled", "0", "Enable the TEMPLATE plugin", true, true, 0, true, 1).bindTo(enabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&TEMPLATE::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&DemoDemon::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(bind(&TEMPLATE::Render, this, std::placeholders::_1));


	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", [this](std::string eventName) {
	//	cvarManager->log("Your hook got called and the ball went POOF");
	//});
	// You could also use std::bind here
	gameWrapper->HookEventWithCallerPost<ServerWrapper>("Function TAGame.GFxHUD_TA.HandleStatTickerMessage",
		[this](ServerWrapper caller, void* params, std::string eventname) {
			DemoDemon::onStatTickerMessage(params);
		});
	gameWrapper->HookEvent("Function TAGame.GameEvent_TA.Init", std::bind(&DemoDemon::StartGame, this));
	
	gameWrapper->SetTimeout([this](GameWrapper *gameWrapper) {
		StartRender();
		}, 0.1f);
		
}

void DemoDemon::onUnload()
{
}

void DemoDemon::onStatTickerMessage(void* params) 
{
	StatTickerParams *pStruct = (StatTickerParams*)params;
	PriWrapper receiver = PriWrapper(pStruct->Receiver);
	PriWrapper victim = PriWrapper(pStruct->Victim);
	StatEventWrapper statEvent = StatEventWrapper(pStruct->StatEvent);

	if (statEvent.GetEventName() == "Demolish") {
		if (!receiver) return;
		if (!victim) return;

		// Find the primary player's PRI
		PlayerControllerWrapper playerController = gameWrapper->GetPlayerController();
		if (!playerController) return;

		PriWrapper playerPRI = playerController.GetPRI();
		if (!playerPRI) return;

		if (playerPRI.memory_address == victim.memory_address) 
		{
			this->deaths++;
			this->SetKD();
		}
		
		if (playerPRI.memory_address == receiver.memory_address)
		{
			this->kills++;
			this->SetKD();
		}

		// gameWrapper->LogToChatbox(victim.GetPlayerName().ToString() + " was demoed by " + receiver.GetPlayerName().ToString());
	}
}

void DemoDemon::SetKD()
{
	auto deaths = this->deaths;
	if (!deaths) deaths = 1;
	this->kd = (double)this->kills / deaths;
}

double DemoDemon::GetKD()
{
	return this->kd;
}

void DemoDemon::StartGame()
{
	kills = 0;
	deaths = 0;
	kd = 0;
	StartRender();
}