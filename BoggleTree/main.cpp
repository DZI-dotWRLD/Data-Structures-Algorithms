#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set> // For avoiding duplicate words

using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    bool end_of_word;
    char letter;

    TrieNode() {
        end_of_word = false;
        for (int i = 0; i < 26; i++) {
            children[i] = NULL;
        }
        letter = '\0';
    }
};

class Trie {
private:
    TrieNode root;

public:
    void Insert(string str) {
        TrieNode* current = &root;

        for (size_t i = 0; i < str.size(); i++) {
            if (current->children[str.at(i) - 'A'] == NULL) {
                current->children[str.at(i) - 'A'] = new TrieNode;
                current->children[str.at(i) - 'A']->letter = str.at(i);
            }
            current = current->children[str.at(i) - 'A'];
        }
        current->end_of_word = true;
    }

    int Search(string str) {
        TrieNode* current = &root;

        for (size_t i = 0; i < str.size(); i++) {
            if (current->children[str.at(i) - 'A'] != NULL) {
                current = current->children[str.at(i) - 'A'];
            }
            else {
                return 0;
            }
        }

        if (current->end_of_word == true)
            return 2; // Word exists
        return 1;     // Prefix exists
    }
};

class Boggle {
private:
    vector<vector<char>> grid;
    vector<vector<bool>> visited;
    set<string> found_words; // To store unique found words
    string current_str;
    Trie wordtrie;

public:
    Boggle() {}

    Boggle(vector<vector<char>> arg_grid, Trie& arg_trie) {
        wordtrie = arg_trie;
        grid = arg_grid;
        visited.resize(grid.size());

        for (auto& v : visited)
            v.resize(grid[0].size());
    }

    void SearchWord(int row, int col) {
        int ret = wordtrie.Search(current_str);

        if (ret != 0) {
            if (ret == 2 && found_words.find(current_str) == found_words.end()) {
                // Only print if the word is found for the first time
                found_words.insert(current_str);
                cout << "Found: " << current_str << endl;
            }
            // Explore neighbors
            for (int r = row - 1; r <= row + 1; r++) {
                for (int c = col - 1; c <= col + 1; c++) {
                    if (r >= 0 && r < grid.size() && c >= 0 && c < grid[0].size() && !visited[r][c]) {
                        visited[r][c] = true;
                        current_str.push_back(grid[r][c]);
                        SearchWord(r, c);
                        current_str.pop_back();
                        visited[r][c] = false;
                    }
                }
            }
        }
    }

    void TraverseGrid() {
        for (int row = 0; row < grid.size(); row++) {
            for (int col = 0; col < grid[0].size(); col++) {
                current_str = "";
                visited[row][col] = true;
                current_str.push_back(grid[row][col]);
                SearchWord(row, col);
                current_str.pop_back();
                visited[row][col] = false;
            }
        }
    }
};

int main() {
    vector<vector<char>> grid{
        { 'C', 'T', 'A', 'S' },
        { 'D', 'O', 'G', 'E' },
        { 'H', 'E', 'A', 'R' },
        { 'L', 'I', 'N', 'K' }
    };

    Trie wordtrie;

    wordtrie.Insert("CAT");
    wordtrie.Insert("DOG");
    wordtrie.Insert("HEAR");
    wordtrie.Insert("LINK");
    wordtrie.Insert("CAR");
    wordtrie.Insert("HERO");
    wordtrie.Insert("GO");
    wordtrie.Insert("INK");
    wordtrie.Insert("EAGK");
    wordtrie.Insert("LIAR");


    Boggle b(grid, wordtrie);
    b.TraverseGrid();

    return 0;
}
