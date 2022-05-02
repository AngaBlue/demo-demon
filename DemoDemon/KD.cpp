#include "pch.h"
#include "KD.h"

void KD::updateKD()
{
	// Set deaths = 1 if 0 to avoide DIV/0 error
	unsigned int deaths = this->deaths;
	if (!deaths) deaths = 1;

	this->kd = (float)this->kills / deaths;
}

void KD::addKill()
{
	++this->kills;
	updateKD();
}

unsigned int KD::getKills()
{
	return kills;
}

void KD::addDeath()
{
	++this->deaths;
	updateKD();
}

unsigned int KD::getDeaths()
{
	return deaths;
}


float KD::getKD()
{
	return kd;
}

ImVec4 KD::getKDColor()
{
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

	return ImVec4{ r, g, b, 1 };
}