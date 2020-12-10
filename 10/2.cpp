#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <forward_list>
#include <algorithm>
#include <iterator>
#include <utility>
#include <numeric>

// Define a custom Graph type (outbound adjacency list)
using Graph = std::map<unsigned int, std::vector<unsigned int>>;

// We'll need that later...
struct DFSVisitor {
	bool fresh;
	unsigned int name;
	unsigned int value;
	DFSVisitor* parent;
};

int main()
{
	// As before read the list of adapters, add the power source and sink
	// and sort it
	std::vector<unsigned int> adapters{0};
	{
		std::ifstream input{"input"};
		std::copy(std::istream_iterator<unsigned int>{input},
			  std::istream_iterator<unsigned int>{},
			  std::back_inserter(adapters)
			 );
	}
	std::sort(adapters.begin(), adapters.end());
	adapters.push_back(adapters.back() + 3);

	// Now compute the graph of adapters
	// The nodes are the adapters (or better said, the adapters' jolt value)
	// and there's an edge from one adapter to another if the latter can be
	// connected onto the former.
	Graph g;
	// To be able to compute the answer, we divide the graph in subgraphs at
	// nodes having only one inbound edge. Due to the way it's built, we can
	// expect many such nodes, and computing the number of paths inside each
	// one will be kept fast.
	std::vector<unsigned int> cutpoints{0};
	for (unsigned int i=0 ; i<adapters.size() - 1 ; i++) {
		// There's always an edge from one element in the graph to the
		// next (otherwise, the number of path would trivially be 0)
		std::vector<unsigned int> next{adapters[i+1]};
		// We may skip a node or two in the chain as long as the
		// difference between two adapters is <= 3 so add those edges
		// as well.
		if (i < adapters.size() - 2 && adapters[i+2] - adapters[i] <= 3)
			next.push_back(adapters[i+2]);
		if (i < adapters.size() - 3 && adapters[i+3] - adapters[i] <= 3)
			next.push_back(adapters[i+3]);

		// Populate the graph, one node at a time
		g.emplace(adapters[i], std::move(next));

		// Finally, add a cutpoint if we have a difference in jolt value
		// of 3 since this guarantees that there's no way to skip an
		// adapter there.
		// Note that this doesn't find all cutpoints (for example, two
		// consecutive differences of 2 jolts would also make the second
		// edge the unique inbound edge of the last node). It would be
		// necessary to reverse the graph and make a second pass to
		// find those. Whether it's beneficial to the overall
		// computation is unclear.
		if (adapters[i+1] - adapters[i] == 3)
			cutpoints.push_back(adapters[i+1]);
	}

	// Print the graph
	for (unsigned int i : adapters) {
		for (unsigned int k : g[i])
			std::cerr << i << " -> " << k << std::endl;
	}

	// Please use a big type to store the result, I got bit by this the
	// first time I submitted an answer, despite the warning.
	unsigned long long int total = 1u;

	// Finally compute the number of paths in each subgraph and multiply
	// them together
	for (unsigned int j=0 ; j<cutpoints.size() - 1 ; j++) {
		unsigned start = cutpoints[j];
		unsigned end = cutpoints[j+1];
		std::cerr << "Visiting subgraph from " << start << " to " << end << std::endl;

		// We use the numerical ordering on the adapters as a natural
		// and very convenient topological ordering on the graph.
		// We do a Depth-First-Visit with a nice optimisation to compute
		// the total number of path.
		std::forward_list<DFSVisitor> nexts{{true, start, 0, nullptr}};
		// This is the optimisation path. Due to the nature of the
		// graph (quite compact), we often visit the same nodes so it's
		// convenient to memoize the number of paths starting from each
		// node during the computation to prune branches during the DFS
		// visit (by the way, it's also why the DFS visit is more
		// interesting than the BFS visit here).
		std::map<unsigned int, unsigned int> nbPaths;

		// We do a non-recursive visit so we push nodes to visit inside
		// a list.
		// We start the loop with only the root node in the list, and we
		// end it when we reach back the root node at the end of the
		// visit.
		do {
			DFSVisitor& elt = nexts.front();
			std::cerr << "\tVisiting node " << elt.name << std::endl;

			// The freshness of a node indicates whether we are
			// getting down the tree or sideways (to a sibling
			// node), in which case the node is fresh, or whether we
			// climb back up after having visited the entire
			// subtree, in which case the node is no longer fresh.
			if (!elt.fresh) {
				std::cerr << "\tThis is a node we've finished visiting" << std::endl;
				// We have finished the visit of the subtree,
				// the current value field holds the total
				// number of paths in it. Add that to the
				// parent's total.
				elt.parent->value += elt.value;
				// Memoize the value for the next time we meet
				// the same node in the visit.
				nbPaths[elt.name] = elt.value;
				// Remove the node from the list and continue;
				nexts.pop_front();
			} else if (elt.name == end) {
				std::cerr << "\tThis is the end node" << std::endl;
				// We met a leaf. Increment the parent value,
				// since we've just found one complete path.
				elt.parent->value++;
				// Nothing else to do, no need to memoize this
				// trivial value. Just remove the node.
				nexts.pop_front();
			} else {
				// In the general case, we are either going down
				// the tree or going sideways to a sibling
				// branch of the same parent element and we are
				// not at a leaf.
				//
				// First check if we've already met that node
				// previously in the visit.
				auto it = nbPaths.find(elt.name);
				if (it == nbPaths.end()) {
					// Apparently no, so, push all
					// descendants in front of the node in
					// the list to visit them.
					std::cerr << "\tThis is a fresh node with descendants we've got to visit" << std::endl;
					for (unsigned int n : g[elt.name]) {
						// We record in each node
						// the current element as the
						// parent since the structure
						// of the list itself cannot
						// tell us which node it is.
						nexts.push_front({true, n, 0, &elt});
					}
					// We've unpacked the node, we are about
					// to go down its subtree. Mark it as
					// not fresh for when we meet it when we
					// climb back the tree.
					elt.fresh = false;
					// Note that this is the only case in
					// which we don't remove the node from
					// the list, we can only remove a node
					// once we've visited its complete
					// subtree.
				} else {
					std::cerr << "\tThis is a node for which we already know the number of paths" << std::endl;
					// We're lucky, this is a memoized value.
					// Instead of visiting the subtree, we
					// can directly take its count of
					// subpath from the map.
					elt.parent->value += it->second;
					// Don't forget to remove the node (and
					// implicitly its subtree) from the
					// visit.
					nexts.pop_front();
				}
			}
		} while (nexts.front().parent != nullptr);

		// At the end of the visit, we leave the root node with its
		// computed value in the visit list.
		unsigned int partial = nexts.front().value;
		std::cerr << "Subgraph has " << partial << " paths" << std::endl;

		// The total number of paths in the graphs is the product of the
		// number of paths in all the subgraphs (they're chained one to
		// the next by a single edge).
		total *= partial;
	}

	// And finally, we're done!
	std::cout << total << std::endl;
}
