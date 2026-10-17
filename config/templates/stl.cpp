/**
 * ==============================================================================
 * ByteC: Starter Template: C++20 STL Containers and Algorithms
 * ==============================================================================
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

void printVector(const std::vector<int> &vec)
{
    if (vec.empty())
    {
        std::cout << "[empty]\n";
        return;
    }
    for (int val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

int main()
{
    std::vector<int> numbers;
    std::set<int> uniqueSet;
    std::map<std::string, int> scoreMap;
    int choice;

    std::cout << "--- C++20 STL Lab Menu ---\n";
    std::cout << "1. Push integer to std::vector and std::set\n";
    std::cout << "2. Sort and display std::vector and std::set\n";
    std::cout << "3. Insert or update key-value in std::map\n";
    std::cout << "4. Display std::map entries\n";
    std::cout << "5. Exit\n";

    while (true)
    {
        std::cout << "\nEnter choice (1-5): ";
        if (!(std::cin >> choice))
        {
            break;
        }

        if (choice == 1)
        {
            int val;
            std::cout << "Enter integer: ";
            if (std::cin >> val)
            {
                numbers.push_back(val);
                uniqueSet.insert(val);
                std::cout << "Inserted " << val << " into vector and set.\n";
            }
        }
        else if (choice == 2)
        {
            std::sort(numbers.begin(), numbers.end());
            std::cout << "Sorted std::vector: ";
            printVector(numbers);

            std::cout << "Ordered std::set:   ";
            if (uniqueSet.empty())
            {
                std::cout << "[empty]\n";
            }
            else
            {
                for (int val : uniqueSet)
                {
                    std::cout << val << " ";
                }
                std::cout << "\n";
            }
        }
        else if (choice == 3)
        {
            std::string key;
            int score;
            std::cout << "Enter student name and score: ";
            if (std::cin >> key >> score)
            {
                scoreMap[key] = score;
                std::cout << "Stored " << key << " -> " << score << "\n";
            }
        }
        else if (choice == 4)
        {
            if (scoreMap.empty())
            {
                std::cout << "Map is empty.\n";
            }
            else
            {
                std::cout << "std::map contents:\n";
                for (const auto &[name, score] : scoreMap)
                {
                    std::cout << "  " << name << ": " << score << "\n";
                }
            }
        }
        else if (choice == 5)
        {
            std::cout << "Exiting.\n";
            return 0;
        }
        else
        {
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
