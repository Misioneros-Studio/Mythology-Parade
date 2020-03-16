#ifndef _J1CURRENCYSYSTEM_
#define _J1CURRENCYSYSTEM_


class j1CurrencySystem
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
	j1CurrencySystem();
	~j1CurrencySystem();

	void increaseFaith(int number);
	void decreaseFaith(int number);
	void increaseSacrifice(ActionSacrifice action);
	void increasePrayers(ActionPrayers action);

private:
	int sacrifices;
	int faith;
	int prayers;
};

#endif // !_J1CURRENCYSYSTEM_

