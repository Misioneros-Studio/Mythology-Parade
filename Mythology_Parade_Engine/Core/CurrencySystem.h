#ifndef _CURRENCYSYSTEM_
#define _CURRENCYSYSTEM_


class CurrencySystem
{
public:
	enum class ActionSacrifice {
		RivalKilled,
		EliteKilled,
		BeastKilled,
		TempleDestroyed,
		EncampmentDestroyed,
		MonasteriesSacrifice
	};
	enum class ActionPrayers {
		UnitConverted,
		TempleConverted,
		EncampmentConverted,
		MonasteryConverted,
		BeastConverted,
		MonasteriesPrayers
	};

public:
	CurrencySystem();
	~CurrencySystem();

	void increaseFaith(int number);
	void decreaseFaith(int number);
	void increaseSacrifice(ActionSacrifice action);
	void increasePrayers(ActionPrayers action);

public:
	int sacrifices;
	int faith;
	int prayers;
};

#endif // !_J1CURRENCYSYSTEM_

