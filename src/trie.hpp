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

#pragma once

#include <array>
#include <cstdlib>
#include <initializer_list>
#include <string>
#include <vector>

namespace __dc_detail {
	constexpr static int ALPHABET_SIZE = 26;

	class TrieNode {
	public:
		TrieNode():
			m_Children({}),
			m_EndOfWord(false)
		{}

		~TrieNode() {
			for (auto& node: m_Children) {
				delete node;
			}
		}

		// Allow accessing children by index. The nodes can be modified.
		TrieNode*& operator[](int index) {
			return m_Children[index];
		}

		// Allow accessing children by index. The nodes cannot be modified.
		const TrieNode* operator[](int index) const {
			return m_Children[index];
		}

		void makeEnd() {
			m_EndOfWord = true;
		}

		// Return true if this node is the end of a word, false otherwise.
		constexpr bool isEnd() const {
			return m_EndOfWord;
		}

	private:
		std::array<TrieNode*, ALPHABET_SIZE> m_Children;
		bool m_EndOfWord;
	};
}

class Trie {
	using key_type		  = std::string;
	using value_type      = char;
	using pointer         = char*;
	using const_pointer   = const char*;
	using reference       = value_type&;
	using const_reference = const value_type&;
	using size_type       = std::size_t;
	using TrieNode		  = __dc_detail::TrieNode;

public:
	Trie();
	Trie(std::initializer_list<key_type> keys);
	~Trie();

	void insert(const key_type& key);
	bool search(const key_type& key, bool matchSubKeys = true) const;
	TrieNode* getNode(const key_type& key) const;
	std::vector<key_type> findMatches(const key_type& subkey);

private:
	TrieNode* m_Root;
};