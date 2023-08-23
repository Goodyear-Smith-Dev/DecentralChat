//
// This file is part of DecentralChat
// Copyright (C) 2023 Ryan Smith, Jake Goodyear
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>
//
// Filename: trie.hpp
// Description: Implement a trie data structure
//

#include "trie.hpp"
#include <iostream>

Trie::Trie():
	m_Root(new TrieNode)
{}

Trie::Trie(std::initializer_list<key_type> keys):
	Trie()
{
	for (const auto& key: keys) {
		insert(key);
	}
}

Trie::~Trie() {
	delete m_Root;
}

// Insert a new key into the trie
void Trie::insert(const key_type& key) {
	TrieNode* currentNode = m_Root;

	for (value_type letter: key) {
		// Get the trie node's storage index from the letter. Lowercase a
		// maps to 0, b maps to 1, and so on.
		int index = letter - 'a';

		// Allocate space for the next letter
		if (!(*currentNode)[index]) {
			(*currentNode)[index] = new TrieNode;
		}

		currentNode = (*currentNode)[index];
	}

	currentNode->makeEnd();
}

// Search for a key in the trie. Return true if it exists, false otherwise
bool Trie::search(const key_type& key, bool matchSubKeys) const {
	TrieNode* currentNode = m_Root;
	bool substrExists = matchSubKeys;

	for (value_type letter: key) {
		int index = letter - 'a';

		if (matchSubKeys) {
			substrExists = (*currentNode)[index] != nullptr;
			if (!substrExists) {
				return false;
			}
		}

		currentNode = (*currentNode)[index];
	}

	return currentNode->isEnd() || substrExists;
}

Trie::TrieNode* Trie::getNode(const key_type& key) const {
	TrieNode* currentNode = m_Root;
	bool substrExists = true;

	for (value_type letter: key) {
		int index = letter - 'a';

		substrExists = (*currentNode)[index] != nullptr;
		if (!substrExists) {
			return nullptr;
		}

		currentNode = (*currentNode)[index];
	}

	return (currentNode->isEnd() || substrExists)? currentNode : nullptr;
}

std::vector<Trie::key_type> Trie::findMatches(const key_type& subkey) {
	auto node = getNode(subkey);
	std::vector<key_type> result;

	for (int i = 0; i < __dc_detail::ALPHABET_SIZE; i++) {
		// Skip any empty (null) children
		if (!(*node)[i]) {
			continue;
		}

		std::string newSubKey = subkey + static_cast<char>(i + 'a'); // Get the next letter of the word
		std::cout << "Searching: " << newSubKey << "\n";

		// Search the new subkey to see if it's the end of a word
		if (search(newSubKey, false)) {
			std::cout << "Found match for " << newSubKey << "\n";
			result.push_back(newSubKey);
		}

		// Add any found matches to the main result vector
		if (auto matches = findMatches(newSubKey); !matches.empty()) {
			result.reserve(matches.size());
			result.insert(result.end(), matches.begin(), matches.end());
		}
}

	return result;
}