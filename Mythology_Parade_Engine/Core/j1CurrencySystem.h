#ifndef _J1CURRENCYSYSTEM_
#define _J1CURRENCYSYSTEM_


class j1CurrencySystem
{
public:
	enum class actionSacrifice {
		RivalKilled,
		EliteKilled,
		BeastKilled,
		TempleDestroyed,
		EncampmentDestroyed,
		MonasteriesSacrifice
	};
	enum class actionPrayers {
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
	void increaseSacrifice(actionSacrifice action);
	void increasePrayers(actionPrayers action);

private:
	int sacrifices;
	int faith;
	int prayers;
};

#endif // !_J1CURRENCYSYSTEM_

