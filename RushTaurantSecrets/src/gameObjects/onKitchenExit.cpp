#include "onKitchenExit.h"

void onKitchenExit::onTriggerExit() {
	ing->removeAllIngredients(); //hay que modificar este m�todo para que devuelva los ingredientes a su sitio y no solo los elimine del player
}
