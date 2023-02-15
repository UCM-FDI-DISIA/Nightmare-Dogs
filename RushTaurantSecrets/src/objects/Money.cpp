#include "Money.h"

Money::Money() {
	initMoney(100);
}

// inicializa la cantidad de dinero inicial
void Money::initMoney(int num) {
	amount = num;
}

int Money::getMoney() {
	return amount;
}

void Money::addMoney(int num) {
	amount += num;
}

void Money::subtractMoney(int num) {
	amount -= num;
}