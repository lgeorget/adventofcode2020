#include <fstream>
#include <iostream>
#include <sstream>

#include <unordered_map>
#include <map>
#include <vector>
#include <string>

#include <algorithm>
#include <iterator>

using AllergenMatching = std::unordered_map<std::string, std::vector<std::string>>;
using Product = std::pair<std::vector<std::string>, std::vector<std::string>>;

AllergenMatching allergens;
std::vector<Product> products;
std::map<std::string, std::string> allergensFound;

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
	allergensFound.emplace(allergen, product);
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

	// maps are already sorted by their key
	// It's always a bit annoying to print a list with separators in C++
	// because the separator is also output after the last element.
	std::transform(allergensFound.begin(), std::next(allergensFound.end(), -1),
		std::ostream_iterator<const std::string&>(std::cout, ","),
		[](const auto& ap){ return ap.second; }
	);
	std::cout << std::next(allergensFound.end(), -1)->second << std::endl;
}
