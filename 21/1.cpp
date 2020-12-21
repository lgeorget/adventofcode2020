#include <fstream>
#include <iostream>
#include <sstream>

#include <unordered_map>
#include <map>
#include <vector>
#include <string>

#include <algorithm>
#include <numeric>
#include <iterator>

using AllergenMatching = std::unordered_map<std::string, std::vector<std::string>>;
using Product = std::pair<std::vector<std::string>, std::vector<std::string>>;

AllergenMatching allergens;
std::vector<Product> products;

void doSimplify(AllergenMatching::iterator it)
{
	std::string allergen = it->first;
	std::string product = it->second.back();
	for (auto& p : products) {
		p.first.erase(std::remove(p.first.begin(), p.first.end(), product), p.first.end());
		p.second.erase(std::remove(p.second.begin(), p.second.end(), allergen), p.second.end());
	}
	allergens.erase(it);
	for (auto& [a, products]: allergens) {
		products.erase(std::remove(products.begin(), products.end(), product), products.end());
	}
}

void simplify(AllergenMatching::iterator it)
{
	while (it != allergens.end()) {
		doSimplify(it);
		it = std::find_if(allergens.begin(), allergens.end(),
			[](auto& a){ return a.second.size() == 1; });
	}
}

int main()
{
	std::ifstream input{"input"};
	std::string line;
	while (std::getline(input, line)) {
		std::istringstream productStream{line};
		std::vector<std::string> ingredients;
		for (;;) {
			std::string ingredient;
			productStream >> ingredient;
			if (ingredient[0] == '(')
				break;
			ingredients.push_back(std::move(ingredient));
		}
		std::sort(ingredients.begin(), ingredients.end());

		std::vector<std::string> allergenicIngredients;
		for (;;) {
			std::string allergenicIngredient;
			productStream >> allergenicIngredient;
			if (allergenicIngredient.empty())
				break;
			allergenicIngredient.pop_back();
			allergenicIngredients.push_back(allergenicIngredient);

			auto it = allergens.find(allergenicIngredient);
			if (it == allergens.end()) {
				allergens.emplace(allergenicIngredient, ingredients);
			} else {
				std::vector<std::string> newList;
				std::set_intersection(it->second.begin(), it->second.end(), ingredients.begin(), ingredients.end(), std::back_inserter(newList));
				std::swap(it->second, newList);
			}
		}
		std::sort(allergenicIngredients.begin(), allergenicIngredients.end());

		products.emplace_back(ingredients, allergenicIngredients);
	}

	auto it = std::find_if(allergens.begin(), allergens.end(),
		[](auto& a){ return a.second.size() == 1; });
	simplify(it);

	/*
	std::cerr << "Allergens :\n";
	for (const auto& [allergen, products] : allergens) {
		std::cerr << allergen << ": ";
		for (const auto& p : products) {
			std::cerr << p << " ";
		}
		std::cerr << std::endl;
	}
	std::cerr << "\n";


	std::cerr << "Products :\n";
	for (const auto& pp : products) {
		for (const auto& p : pp.first) {
			std::cerr << p << " ";
		}
		std::cerr << ": ";
		for (const auto& a : pp.second) {
			std::cerr << a << " ";
		}
		std::cerr << std::endl;
	}
	std::cerr << "\n";
	*/

	std::cout << std::accumulate(products.begin(), products.end(), 0,
			[](int total, const auto& pp){ return total + pp.first.size(); })
		  << std::endl;
}
