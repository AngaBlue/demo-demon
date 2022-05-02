#pragma once
class KD
{
private:
	unsigned int kills = 0;
	unsigned int deaths = 0;
	float kd = 0;
	void updateKD();

public:
	void addKill();
	unsigned int getKills();

	void addDeath();
	unsigned int getDeaths();

	float getKD();
	ImVec4 getKDColor();
};

