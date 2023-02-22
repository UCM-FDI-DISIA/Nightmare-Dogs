#pragma once
#include <cstdint>
#include <vector>
using namespace std;
namespace _ecs {
	using id_type = uint8_t;
	using Dish = uint8_t;
	using Ingredient = uint8_t;
	const uint8_t NUM_DISH = 28;
	const uint8_t NUM_MEAT_DISH = 5;
	enum _dish_id : id_type {
		HUEVO_FRITO,
		PAN,
		MERMELADA,
		PAN_AJO,
		FRENCH_FRIES,
		PANCAKE,
		TARTA_FRESA,
		TARTA_CHOC,
		TARTA_FRAM,
		GALLETAS,
		OMELETTE,
		TARTA_MAN,
		TARTA_QUESO,
		BURRITO,
		PURE,
		HELADO,
		NACHOS,
		HOTDOG,
		SANDWICH,
		SUSHI,
		CURRY_ARROZ,
		PIZZA,
		POLLO_ASADO,
		FILETE,
		ALBONDIGAS,
		ESPAGUETI,
		BURGUER,
		RAMEN,
		NONE_DISH
	};
	enum _ingredients_id : id_type {
		HARINA=29,
		HUEVO,
		PATATA,
		CHOCOLATE,
		LECHUGA,
		MAIZ,
		FRESA,
		ZANAHORIA,
		AJO,
		LECHE,
		ARROZ,
		TOMATE,
		MANZANA,
		MANTEQUILLA,
		FRAMBUESA,
		MOSTAZA,
		FIDEOS,
		CURRY,
		QUESO,
		SALCHICHA,
		POLLO,
		GAMBAS,
		SALMON,
		CARNE,
		NONE_ING
	};

	struct DishInfo {
		_dish_id id;
		vector<_ingredients_id>ingredients;
		float cookingTime;
		int price;
	};

	static DishInfo Dishes[NUM_DISH] = {
		{HUEVO_FRITO,{HUEVO},0.5,4},
		{PAN,{HARINA},0.7,4},
		{MERMELADA,{HARINA,FRAMBUESA},1,8},
		{PAN_AJO,{HARINA,AJO},0.7,7},
		{FRENCH_FRIES,{PATATA,TOMATE},1,8},
		{PANCAKE,{HARINA,HUEVO,LECHE},1.5,8},
		{TARTA_FRESA,{HARINA,HUEVO,FRESA},2.5,9},
		{TARTA_CHOC,{HARINA,HUEVO,CHOCOLATE},2.5,9},
		{TARTA_FRAM,{HARINA,HUEVO,FRAMBUESA},2,10},
		{GALLETAS,{HARINA,CHOCOLATE,MANTEQUILLA},1.7,11},
		{OMELETTE,{HARINA,HUEVO,MAIZ,MANTEQUILLA,},2,14},
		{TARTA_MAN,{HARINA,HUEVO,MANZANA},2.5,10},
		{TARTA_QUESO,{HARINA,HUEVO,QUESO},2.5,12},
		{BURRITO,{HARINA,LECHUGA,ARROZ},3,11},
		{PURE,{PATATA,ZANAHORIA,AJO},3.5,11},
		{HELADO,{CHOCOLATE,FRESA,LECHE,FRAMBUESA},2.5,17},
		{NACHOS,{HARINA,MAIZ,MOSTAZA},3,16},
		{HOTDOG,{HARINA,MOSTAZA,SALCHICHA},2.7,19},
		{SANDWICH,{HARINA,LECHUGA,SALMON},1,20},
		{SUSHI,{ARROZ,SALMON},3.7,21},
		{CURRY_ARROZ,{PATATA,AJO,ARROZ,CURRY},3,22},
		{PIZZA,{HARINA,QUESO,SALCHICHA},2,25},
		{POLLO_ASADO,{ZANAHORIA,TOMATE,POLLO},6,28},
		{FILETE,{ZANAHORIA,CARNE},4,15},
		{ALBONDIGAS,{HARINA,CURRY,CARNE},5.5,18},
		{ESPAGUETI,{TOMATE,FIDEOS,CARNE},5,20},
		{BURGUER,{HARINA,LECHUGA,TOMATE,QUESO,CARNE},6.5,30},
		{RAMEN,{HUEVO,AJO,FIDEOS,GAMBAS,CARNE},7,38}
	};

	static DishInfo MeatDishes[NUM_MEAT_DISH] = {
		{FILETE,{ZANAHORIA,CARNE},4,15},
		{ALBONDIGAS,{HARINA,CURRY,CARNE},5.5,18},
		{ESPAGUETI,{TOMATE,FIDEOS,CARNE},5,20},
		{BURGUER,{HARINA,LECHUGA,TOMATE,QUESO,CARNE},6.5,30},
		{RAMEN,{HUEVO,AJO,FIDEOS,GAMBAS,CARNE},7,38}
	};

};