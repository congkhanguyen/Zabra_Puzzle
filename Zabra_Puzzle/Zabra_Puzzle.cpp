// Zabra_Puzzle.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <stdio.h>
#include <string.h>

enum HouseStatus { Invalid, Underfull, Valid };

enum Attrib { C, N, D, A, CA };

//Unfilled attributes are represented by -1
enum Colors { Red, Green, Ivory, Yellow, Blue };
enum Nationalities { English, Spaniard, Ukrainian, Japanese, Norwegian };
enum Drinks { Tea, Coffee, Milk, Orange_juice, Water };
enum Animals { Dog, Snails, Fox, Horse, Zebra };
enum Candies { Smarties , Kit_Kats, Hershey_bars, Snickers, Milky_Ways };

//Print correspondent atributes for each house
void printHouses(int house[5][5]) {
	const char *color[] =  { "Red", "Green", "Ivory", "Yellow", "Blue" };
	const char *nat[]   =  { "English", "Spaniard", "Ukrainian", "Japanese", "Norwegian" };
	const char *drink[] =  { "Tea", "Coffee", "Milk", "Orange Juice", "Water" };
	const char *animal[]=  { "Dog", "Snails", "Fox", "Horse", "Zebra" };
	const char *candy[] =  { "Smarties", "Kit Kats", "Hershey bars", "Snickers", "Milky Ways" };

	printf("%-13.13s%-13.13s%-13.13s%-13.13s%-13.13s%-13.13s\n",
		"House", "Color", "Nationality", "Drink", "Animal", "Candy");

	for (int i = 0; i < 5; i++) {
		printf("%-13d", i);
		if (house[i][C] >= 0)
			printf("%-13.13s", color[house[i][C]]);
		else
			printf("%-13.13s", "-");
		if (house[i][N] >= 0)
			printf("%-13.13s", nat[house[i][N]]);
		else
			printf("%-13.13s", "-");
		if (house[i][D] >= 0)
			printf("%-13.13s", drink[house[i][D]]);
		else
			printf("%-13.13s", "-");
		if (house[i][A] >= 0)
			printf("%-13.13s", animal[house[i][A]]);
		else
			printf("%-13.13s", "-");
		if (house[i][CA] >= 0)
			printf("%-13.13s\n", candy[house[i][CA]]);
		else
			printf("-\n");
	}
}

// Check constrains for each house 
int checkHouses(int house[5][5]) {
	int c_add = 0, c_or = 0;
	int m_add = 0, m_or = 0;
	int d_add = 0, d_or = 0;
	int a_add = 0, a_or = 0;
	int ca_add = 0, ca_or = 0;

	// Unary Constraint
	// Constraint 3: The Norwegian lives in the first house on the left
	if (house[0][N] >= 0 && house[0][N] != Norwegian)
		return Invalid;

	// Constraint 14: Milk is drunk in the middle house
	if (house[2][D] >= 0 && house[2][D] != Milk)
		return Invalid;

	for (int i = 0; i < 5; i++) {
		// Uni constraint.
		if (house[i][C] >= 0) {
			c_add += (1 << house[i][C]);
			c_or |= (1 << house[i][C]);
		}
		if (house[i][N] >= 0) {
			m_add += (1 << house[i][N]);
			m_or |= (1 << house[i][N]);
		}
		if (house[i][D] >= 0) {
			d_add += (1 << house[i][D]);
			d_or |= (1 << house[i][D]);
		}
		if (house[i][A] >= 0) {
			a_add += (1 << house[i][A]);
			a_or |= (1 << house[i][A]);
		}
		if (house[i][CA] >= 0) {
			ca_add += (1 << house[i][CA]);
			ca_or |= (1 << house[i][CA]);
		}
		// Binary constraint.
		// Constraint 1: The English man lives in the red house.
		if ((house[i][N] >= 0 && house[i][C] >= 0) &&
			((house[i][N] == English && house[i][C] != Red) || // Checking both
			(house[i][N] != English && house[i][C] == Red)))  // to make things quicker.
			return Invalid;

		// Constraint 2: The Spaniard  has a dog.
		if ((house[i][N] >= 0 && house[i][A] >= 0) &&
			((house[i][N] == Spaniard && house[i][A] != Dog) ||
			(house[i][N] != Spaniard && house[i][A] == Dog)))
			return Invalid;

		// Constraint 4:The green house is immediately to the right of the ivory house
		if ((i > 0 && house[i][C] >= 0 /*&& ha[i-1][C] >= 0 */ ) &&
			((house[i - 1][C] == Green && house[i][C] != Ivory) ||
			(house[i - 1][C] != Green && house[i][C] == Ivory)))
			return Invalid;

		//Non-binary constraint
		// Constraint 5: The man who eats Hershey bars lives in the house next to the man with the fox.
		if (house[i][CA] == Hershey_bars) {
			if (i == 0 && house[i + 1][A] >= 0 && house[i + 1][A] != Fox)
				return Invalid;
			else if (i == 4 && house[i - 1][A] != Fox)
				return Invalid;
			else if (house[i + 1][A] >= 0 && house[i + 1][A] != Fox && house[i - 1][A] != Fox)
				return Invalid;
		}

		// Binary constraint.
		// Constraint 6: Kit Kats are eaten in the yellow house.
		if ((house[i][CA] >= 0 && house[i][C] >= 0) &&
			((house[i][CA] == Kit_Kats && house[i][C] != Yellow) ||
			(house[i][CA] != Kit_Kats && house[i][C] == Yellow)))
			return Invalid;

		// Constraint 7: The Norwegian lives next to the blue house.
		if (house[i][N] == Norwegian &&
			((i < 4 && house[i + 1][C] >= 0 && house[i + 1][C] != Blue) ||
			(i > 0 && house[i - 1][C] != Blue)))
			return Invalid;

		// Constraint 8: The Smarties eater owns snails
		if ((house[i][CA] >= 0 && house[i][A] >= 0) &&
			((house[i][CA] == Smarties && house[i][A] != Snails) ||
			(house[i][CA] != Smarties && house[i][A] == Snails)))
			return Invalid;

		// Constraint 9: The Snickers eater drinks orange juice
		if ((house[i][CA] >= 0 && house[i][D] >= 0) &&
			((house[i][CA] == Snickers && house[i][D] != Orange_juice) ||
			(house[i][CA] != Snickers && house[i][D] == Orange_juice)))
			return Invalid;

		// Constraint 10: The Ukrainian drinks tea.
		if ((house[i][N] >= 0 && house[i][D] >= 0) &&
			((house[i][N] == Ukrainian && house[i][D] != Tea) ||
			(house[i][N] != Ukrainian && house[i][D] == Tea)))
			return Invalid;

		// Constraint 11: The Japanese eats Milky Ways
		if ((house[i][N] >= 0 && house[i][CA] >= 0) &&
			((house[i][N] == Japanese && house[i][CA] != Milky_Ways) ||
			(house[i][N] != Japanese && house[i][CA] == Milky_Ways)))
			return Invalid;

		// Non-binary constraint
		// Constraint 12: Kit Kats are eaten in a house next to the house where the horse is kept.
		if (house[i][CA] == Kit_Kats) {
			if (i == 0 && house[i + 1][A] >= 0 && house[i + 1][A] != Horse)
				return Invalid;
			else if (i == 4 && house[i - 1][A] != Horse)
				return Invalid;
			else if (house[i + 1][A] >= 0 && house[i + 1][A] != Horse && house[i - 1][A] != Horse)
				return Invalid;
		}

		// Constraint 13: Coffee is drunk in the green house
		if ((house[i][C] >= 0 && house[i][D] >= 0) &&
			((house[i][C] == Green && house[i][D] != Coffee) ||
			(house[i][C] != Green && house[i][D] == Coffee)))
			return Invalid;

		//Non-binary constraint
		// Constraint 15: They drink water in a house next to the house where they eat Hershey_bars.
		if (house[i][CA] == Hershey_bars) {
			if (i == 0 && house[i + 1][D] >= 0 && house[i + 1][D] != Water)
				return Invalid;
			else if (i == 4 && house[i - 1][D] != Water)
				return Invalid;
			else if (house[i + 1][D] >= 0 && house[i + 1][D] != Water && house[i - 1][D] != Water)
				return Invalid;
		}

	}

	if ((c_add != c_or) || (m_add != m_or) || (d_add != d_or)
		|| (a_add != a_or) || (ca_add != ca_or)) {
			return Invalid;
	}
	//Shift maximum 5 bit ~ 31
	if ((c_add != 31) || (m_add != 31) || (d_add != 31)
		|| (a_add != 31) || (ca_add != 31)) {
			return Underfull;
	}

	return Valid;
}

//Assign values to houses sequently and return valid if all houses are satisfied to constraints
int bruteFill(int house[5][5], int hno, int attr) {
	int stat = checkHouses(house);
	if ((stat == Valid) || (stat == Invalid))
		return stat;

	int htemp[5][5];// Temp house 2-dimensional array
	memcpy(htemp, house, sizeof(int) * 5 * 5);
	for (int i = 0; i < 5; i++) {
		htemp[hno][attr] = i;
		stat = checkHouses(htemp);
		if (stat != Invalid) {
			int nexthno, nextattr;
			if (attr < 4) {
				nextattr = attr + 1;
				nexthno = hno;
			} else {
				nextattr = 0;
				nexthno = hno + 1;
			}

			stat = bruteFill(htemp, nexthno, nextattr);
			if (stat != Invalid) {
				memcpy(house, htemp, sizeof(int) * 5 * 5);
				return stat;
			}
		}
	}

	//If none of the attr values assigned were valid
	return Invalid;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int ha[5][5] = {{-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1}};

	bruteFill(ha, 0, 0);
	printHouses(ha);

	return 0;
}