#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "myutils.h"

#define WORDGROUP_SIZE 3

using std::vector;
using std::string;

vector<string> dictionary;
vector<vector<int>> encoded_data;

struct Wordgroup
{

	int words[WORDGROUP_SIZE];

	vector<int> next_words;

	bool operator ==(Wordgroup other)
	{
		for (int i = 0; i < WORDGROUP_SIZE; i++)
		{
			if (words[i] != other.words[i])
			{
				return false;
			}
		}

		return true;
	}
};

vector<Wordgroup> wordgroups;
vector<int> seed_wordgroups;

void genText(Wordgroup* wordgroup)
{
	// match the wordgroup to the stored wordgroups

	for (int i = 0; i < wordgroups.size(); i++)
	{
		if (wordgroups[i] == *wordgroup)
		{
			wordgroup = &wordgroups[i];
			break;
		}
	}

	if (wordgroup->next_words.size() == 0)
	{
		return;
	}

	int nextword = wordgroup->next_words[rand() % wordgroup->next_words.size()];
	std::cout << dictionary[nextword] << " ";

	
	Wordgroup nextgroup;
	for (int i = 0; i < WORDGROUP_SIZE - 1; i++)
	{
		nextgroup.words[i] = wordgroup->words[i + 1];
	}

	nextgroup.words[WORDGROUP_SIZE - 1] = nextword;

	genText(&nextgroup); // stack overflow time
	
}

int main()
{
	std::ifstream file;
	file.open("data.txt");

	string line;

	while (getline(file, line))
	{
		vector<string> words = split(line, ' ');
		encoded_data.push_back(vector<int>());
		
		for (int i = 0; i < words.size(); i++)
		{

			int j;

			for (j = 0; j < dictionary.size(); j++)
			{
				if (strcmp(&dictionary[j][0], &words[i][0]) == 0)
				{
					break;
				}
			}

			if (j == dictionary.size())
			{
				dictionary.push_back(words[i]);
			}

			encoded_data.back().push_back(j);
		}
	}

	// ---
	// get word groups
	for (int line = 0; line < encoded_data.size(); line++)
	{
		for (int i = 0; i < encoded_data[line].size() - WORDGROUP_SIZE; i++)
		{
			if (encoded_data[line].size() < WORDGROUP_SIZE + 1)
			{
				break;
			}

			int j;

			// current word group as it iterates through the data
			Wordgroup cur_group;
			for (j = 0; j < WORDGROUP_SIZE; j++)
			{
				// just setting the current group's words that it contains
				cur_group.words[j] = encoded_data[line][i + j];
			}

			// the word that comes after the word group
			int next_word = encoded_data[line][i + WORDGROUP_SIZE];
			// ---

			// check if the current word group already exists in the list of word groups
			for (j = 0; j < wordgroups.size(); j++)
			{
				if (cur_group == wordgroups[j])
				{
					break;
				}
			}

			// it's a unique word group
			if (j == wordgroups.size())
			{
				if (i == 0)
				{
					seed_wordgroups.push_back(wordgroups.size());
				}

				wordgroups.push_back(cur_group);

				std::cout << wordgroups.size() << std::endl;
			}

			// append the next word to the word group's list of next words
			wordgroups[j].next_words.push_back(next_word);
			std::cout << wordgroups.size() << std::endl;
		}
	}
	// ---

	while (true)
	{
		int seed = seed_wordgroups[rand() % seed_wordgroups.size()];

		for (int i = 0; i < WORDGROUP_SIZE; i++)
		{
			std::cout << dictionary[wordgroups[seed].words[i]] << " ";
		}

		genText(&wordgroups[seed]);

		std::cin.get();
	}
	
	return 0;
}
