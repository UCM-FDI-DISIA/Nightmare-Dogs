#include "DayManager.h"
#include "../structure/GameManager.h"
#include "ClientsManager.h"
#include "ThievesManager.h"
#include "Reputation.h"
#include "Money.h"

#include "../Scenes/GameScenes/EndOfDayScene.h"
#include "../Scenes/Cutscenes/FirstDayAfterKillScene.h"
#include "../Scenes/Cutscenes/SecondDayAfterKillScene.h"
#include "../Scenes/Cutscenes/BadEnding1Scene.h"

#include "../Utilities/checkML.h"

void DayManager::readLine(std::string & line) {
	do {
		std::getline(file, line);
		while (!line.empty() && line[0] == '\t') line = line.substr(1);
	} while (!file.eof() && (line.empty() || line[0] == '#')); // Si la línea que se lee est?vacía o es un comentario (empieza con '#'), se lee la siguiente.
	if (file.eof()) { line = ""; }
}

int DayManager::to_int(std::string str) {
	int i = 0;

	for (auto& ch : str) {
		if (ch < '0' || ch > '9') return -1;
		i = i * 10 + ch - '0';
	}

	return i;
}

float DayManager::to_float(std::string str) {
	int i = 0; float j = 0;
	bool decimal = false;

	for (auto& ch : str) {
		if (ch < '0' || ch > '9') {
			if (!decimal && ch != '.') return -1;
			decimal = true;
		}
		else if (decimal) {
			j = j * 10 + ch - '0';
		}
		else {
			i = i * 10 + ch - '0';
		}
	}
	while (j >= 1) j /= 10;

	return i + j;
}

DayManager::DayManager() : day(0), timeUpSound(&sdlutils().soundEffects().at("TIME_UP")),
sdl(SDLUtils::instance()), restaurantMusic(&sdl->musics().at("RESTAURANT_MUSIC"))
{
	// file
	file.open("assets/dayConfig.rsdat");
	if (file.fail()) throw std::exception("Data for days not found.\n");
	file >> maxDays;
	restaurantMusic->setMusicVolume(MUSIC_VOL);

#ifdef _DEBUG
	std::cout << "DayManager initiated.\n";
#endif // _DEBUG
}

DayManager::~DayManager() {
	if (file.is_open()) file.close();
}

void DayManager::checkDayFinished() {

	if (ClockComponent::get()->dayHasFinished() && ClientsManager::get()->noClients() && !ThievesManager::get()->areThereThieves()) {
		GameManager::get()->save();
		GameManager::get()->changeScene(GameManager::get()->getScene(sc_ENDOFDAY));
	}
}

void DayManager::newDay() {
	restaurantMusic->pauseMusic();
	timeUpSound->play();
	nextDay();
}

void DayManager::nextDay(bool loading) {
	if (!loading) {
		// Bad ending 1
		if (GameManager::get()->getReputation()->getReputation() < 0) {
			GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_BADENDING1), true);
		}
		// Bad ending 2
		// else if (){

		// }
		// Bad ending 3
		// else if (){

		// }
		// Bad ending 4
		// else if (){

		// }
		// Final día 1 (en la propia escena del final del día 1 se pasa a la intro del día 2)
		else if (day == 1) {
			GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_ENDINGDAY1), true);
		}

		
		// Finales si ha matado
		else if (GameManager::get()->getHasEverKilled().first) {
			// Final normal
			if (day > maxDays) GameManager::get()->changeScene(GameManager::get()->getScene(sc_NORMALENDING));

			// Escena final del día 2 si ha matado (en la propia escena se pasa a la intro del día 3 si no se ha matado)
			else if (day == 2) GameManager::get()->changeScene(GameManager::get()->getScene(sc_ENDINGDAY2KILL));
			
			// Escena inicial tras el primer día de haber matado (si mata después del día 2)
			else if (day - GameManager::get()->getHasEverKilled().second == 0) {
				GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_FIRSTDAYAFTERKILL), true);
			}
			// Escena inicial tras el segundo día de haber matado
			else if (day - GameManager::get()->getHasEverKilled().second == 1) {
				GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_SECONDDAYAFTERKILL), true);
			}
		}
		// Si no hay ninguna cinemática ese dia
		else if (day >= 3) GameManager::get()->changeScene(GameManager::get()->getScene(sc_BEFOREDAYSTART));
		// Finales si no ha matado
		else {
			// Final bueno
			if (day > maxDays) GameManager::get()->changeScene(GameManager::get()->getScene(sc_HAPPYENDING));

			// Escena final del día 2 si no ha matado (en la propia escena se pasa a la intro del día 3 si no se ha matado)
			else if (day == 2) GameManager::get()->changeScene(GameManager::get()->getScene(sc_ENDINGDAY2NOKILL));
;
		}
	}
	// Intros si se está cargando partida
	else {
		if (day <= 1) GameManager::get()->changeScene(GameManager::get()->getScene(sc_INTRO1));
		else if (day == 2) GameManager::get()->changeScene(GameManager::get()->getScene(sc_INTRO2));
		else if (day == 3) GameManager::get()->changeScene(GameManager::get()->getScene(sc_INTRO3NOKILL));
	}
	

	day++;
	if (day > 0)
		GameManager::get()->resetScenes();

	// Leer dayConfig
	std::string line = "";
	while (!file.eof() && line != "begin") {
		readLine(line);
	}

	while (!file.eof() && line != "end") {
		readLine(line);

		// Recoger los parámetros de este día
		if (line.substr(0, 9) == "objective") {
			dailyObjective = to_int(line.substr(10));
		}
		else if (line.substr(0, 15) == "clientFrequency") {
			clientFrequency = to_float(line.substr(16));
		}
		else if (line.substr(0, 18) == "happinessFrequency") {
			happinessFrequency = to_float(line.substr(19));
		}
		else if (line.substr(0, 18) == "reputationDecrease") {
			reputationDecrease = to_float(line.substr(19));
		}
		else if (line.substr(0, 21) == "minimumThiefFrequency") {
			minimumThiefFrequency = to_float(line.substr(22));
		}
		else if (line.substr(0, 21) == "maximumThiefFrequency") {
			maximumThiefFrequency = to_float(line.substr(22));
		}
	}

}

void DayManager::setDay(int x, bool loading) {
	if (file.is_open()) file.close();

	file.open("assets/dayConfig.rsdat");
	if (file.fail()) throw std::exception("Data for days not found.\n");
	file >> maxDays;

	string line = "";
	for (int i = 0; i < x - 1; i++) {
		while (!file.eof() && line != "end") {
			readLine(line);
		}
	}
	day = x - 1;
	nextDay(loading);
}

int DayManager::getDay() { return day; }
int DayManager::getDailyObjective() { return dailyObjective; }
float DayManager::getClientFrequency() { return clientFrequency; }
float DayManager::getHappinessFrequency() { return happinessFrequency; }
float DayManager::getReputationDecrease() { return reputationDecrease; }
float DayManager::getMinThiefFrequency() { return minimumThiefFrequency; }
float DayManager::getMaxThiefFrequency() { return maximumThiefFrequency; }