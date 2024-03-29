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
	} while (!file.eof() && (line.empty() || line[0] == '#')); // Si la l�nea que se lee esta vac�a o es un comentario (empieza con '#'), se lee la siguiente.
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
sdl(SDLUtils::instance()), restaurantMusic(&sdl->musics().at("RESTAURANT_MUSIC")), pantryMusic(&sdl->musics().at("PANTRY_MUSIC"))
{
	// file
	file.open("assets/dayConfig.rsdat");
	if (file.fail()) throw std::exception("Data for days not found.\n");
	file >> maxDays;
	restaurantMusic->setMusicVolume(MUSIC_VOL);
}

DayManager::~DayManager() {
	if (file.is_open()) file.close();
}

void DayManager::checkDayFinished() {
	if (ClockComponent::get()->dayHasFinished() && ClientsManager::get()->noClients() && !ThievesManager::get()->areThereThieves()) {
		GameManager::get()->getCurrentScene()->haltSound();
		GameManager::get()->changeScene(GameManager::get()->getScene(sc_ENDOFDAY));
		GameManager::get()->getScene(sc_ENDOFDAY)->reset();
	}
}

void DayManager::newDay() {
	GameManager::get()->getCurrentScene()->haltSound();
	timeUpSound->play();
	nextDay();
}

void DayManager::nextDay(bool loading) {
	if (!loading) {
		// Game over (el bad ending 3 y 4 se comprueban en thievesState
		// Bad ending 1 (si la reputaci�n es negativa)
		if (GameManager::get()->getReputation()->getReputation() < 0) {
			GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_BADENDING1), true);
			GameManager::get()->setGameOver(true);
		}
		// Bad ending 2 (si no se ha alcanzado el objetivo diario)
		else if (day > 0 && GameManager::get()->getMoney()->getEarnedMoney() < dailyObjective) {
			GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_BADENDING2), true);
			GameManager::get()->setGameOver(true);
		}

		// Final d�a 1 (en la propia escena del final del d�a 1 se pasa a la intro del d�a 2)
		else if (day == 1) {
			GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_ENDINGDAY1), true);
		}
		
		// Finales si ha matado
		else if (GameManager::get()->getHasEverKilled().first) {
			// Final normal
			if (day > maxDays) GameManager::get()->changeScene(GameManager::get()->getScene(sc_NORMALENDING), true);

			// Escena final del d�a 2 si ha matado (en la propia escena se pasa a la intro del d�a 3 si no se ha matado)
			else if (day == 2) GameManager::get()->changeScene(GameManager::get()->getScene(sc_ENDINGDAY2KILL), true);
			
			// Escena inicial tras el primer d�a de haber matado (si mata despu�s del d�a 2)
			else if (day - GameManager::get()->getHasEverKilled().second == 0)
				GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_FIRSTDAYAFTERKILL), true);

			// Escena inicial tras el segundo d�a de haber matado
			else if (day - GameManager::get()->getHasEverKilled().second == 1)
				GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_SECONDDAYAFTERKILL), true);

		}
		// Si no hay ninguna cinem�tica ese dia
		else if (day >= 3) GameManager::get()->changeScene(GameManager::get()->getScene(sc_BEFOREDAYSTART), true);
		// Finales si no ha matado
		else {
			// Final bueno
			if (day > maxDays) GameManager::get()->changeScene(GameManager::get()->getScene(sc_HAPPYENDING), true);

			// Escena final del d�a 2 si no ha matado (en la propia escena se pasa a la intro del d�a 3 si no se ha matado)
			else if (day == 2) GameManager::get()->changeScene(GameManager::get()->getScene(sc_ENDINGDAY2NOKILL), true);
		}

		day++;
		// Si no se ha perdido, guarda
		if (day - 1 > 0 && !GameManager::get()->getGameOver())
			GameManager::get()->save();

	}
	// Intros si se est� cargando partida
	else {
		if (day == 1) GameManager::get()->changeScene(GameManager::get()->getScene(sc_INTRO1), true);
		else if (day == 2) GameManager::get()->changeScene(GameManager::get()->getScene(sc_INTRO2), true);
		else if (day == 3 && !GameManager::get()->getHasEverKilled().first)
			GameManager::get()->changeScene(GameManager::get()->getScene(sc_INTRO3NOKILL), true);
		else GameManager::get()->changeScene(GameManager::get()->getScene(sc_BEFOREDAYSTART));
	}

	// Leer dayConfig
	std::string line = "";
	while (!file.eof() && line != "begin") {
		readLine(line);
	}

	while (!file.eof() && line != "end") {
		readLine(line);

		// Recoger los par�metros de este d�a
		if (line.substr(0, 9) == "objective") 
			dailyObjective = to_int(line.substr(10));
		
		else if (line.substr(0, 15) == "clientFrequency") 
			clientFrequency = to_float(line.substr(16));
		
		else if (line.substr(0, 18) == "happinessFrequency") 
			happinessFrequency = to_float(line.substr(19));
		
		else if (line.substr(0, 18) == "reputationDecrease") 
			reputationDecrease = to_float(line.substr(19));
		
		else if (line.substr(0, 21) == "minimumThiefFrequency") 
			minimumThiefFrequency = to_float(line.substr(22));
		
		else if (line.substr(0, 21) == "maximumThiefFrequency") 
			maximumThiefFrequency = to_float(line.substr(22));
		
	}


	GameManager::get()->resetScenes();
}

void DayManager::setDay(int x, bool loading) {
	if (file.is_open()) file.close();

	file.open("assets/dayConfig.rsdat");
	if (file.fail()) throw std::exception("Data for days not found.\n");
	file >> maxDays;

	for (int i = 0; i < x - 1; i++) {
		string line = "";
		while (!file.eof() && line != "end") {
			readLine(line);
		}
	}
	day = x;
	nextDay(loading);
}

int DayManager::getDay() { return day; }
int DayManager::getDailyObjective() { return dailyObjective; }
float DayManager::getClientFrequency() { return clientFrequency; }
float DayManager::getHappinessFrequency() { return happinessFrequency; }
float DayManager::getReputationDecrease() { return reputationDecrease; }
float DayManager::getMinThiefFrequency() { return minimumThiefFrequency; }
float DayManager::getMaxThiefFrequency() { return maximumThiefFrequency; }