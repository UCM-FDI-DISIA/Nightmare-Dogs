#pragma once
#include <cstdint>

namespace _ecs {
	using id_type = uint8_t;

	// Components
	// EL ORDEN DE EJECUCIÓN DE LOS COMPONENTES EST?DETERMINADO POR COMO SE AÑADEN
	enum _cmp_id : id_type {
		cmp_MAPCREATOR,
		cmp_MAPRENDER,
		cmp_CLIENTS_MANAGER,
		cmp_THIEFS_MANAGER,
		cmp_MONEY,
		cmp_TRANSFORM,
		cmp_STRAIGHT_MOVEMENT,
		cmp_MOVEMENT,
		cmp_COLLISIONS,
		cmp_TRIGGER,
		cmp_DESK,
		cmp_BIN,
		cmp_THIEF_TRIGGER,
		cmp_COOKMACHINE,
		cmp_CLIENTSTATE,
		cmp_KITCHENISLAND,
		cmp_INGREDIENTS,
		cmp_DAILYMENU,
		cmp_BUTTON,
		cmp_INVENTORY,
		cmp_SM_INVENTORY,
		cmp_CLIENTSTATE_RENDER,
		cmp_HEART_RENDER,
		cmp_MURDER_RENDER,
		cmp_SHOO_RENDER,
		cmp_IMAGE,
		cmp_BUBBLE,
		cmp_BUTTON_COMP,
		cmp_CHARACANIM,
		cmp_UIANIM,
		cmp_WARNING,
		cmp_SHOP_MENU,
		cmp_FREEZER,
		cmp_RENDER_LIST,
		// Do not erase pls
		cmp_INVALID
	};
	constexpr id_type cmpNum = cmp_INVALID;
	
	// Groups
	enum _grp_id : id_type {
		grp_GENERAL,
		grp_PLAYER,
		grp_RENDERTOP,
		grp_CLIENTS,
		grp_INTERACTABLE,
		grp_HUD,
		grp_ICONS,
		grp_COLLISIONS,
		grp_THIEFS,
		grp_DESK,

		// Do not erase pls
		grp_INVALID
	};
	constexpr id_type grpNum = grp_INVALID;

	// Handlers
	enum _hdr_id : id_type {
		hdr_PLAYER1,
		hdr_PLAYER2,
		hdr_DISHCOMB,
		hdr_CLIENT,
		hdr_INVENTORY,
		hdr_SM_INVENTORY,
		hdr_MONEY_ICON,
		hdr_REPUTATION,
		hdr_TARGET,
		hdr_MONEY,
		hdr_MONEY_TEXT,
		hdr_TIME_TEXT,
		hdr_CLOCK,
		hdr_ARROW,
		hdr_STAR,
		hdr_EMPTY_STAR,
		hdr_MENU,
		hdr_PAUSE,
		hdr_BASKET,
		hdr_BIN,
		hdr_PRUEBA,
		hdr_KITCHENISLAND,
		hdr_SHOP_MENU1,
		hdr_FREEZER,
		// Do not erase pls
		hdr_INVALID
	};
	constexpr id_type hdrNum = hdr_INVALID;

	enum RenderPos { Top, Middle, Down };
}