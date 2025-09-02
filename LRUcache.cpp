#include <iostream>
#include <unordered_map>
#include <list>
#include <stdexcept>

using namespace std;

class LRUCache {
private:
    int capacity;
    list<pair<int, int>> dll; // {key, value}, front = most recent
    unordered_map<int, list<pair<int, int>>::iterator> mp;

public:
    // Constructor
    explicit LRUCache(int cap) {
        if (cap <= 0)
            throw invalid_argument("Capacity must be greater than 0");
        capacity = cap;
    }

    // Get value by key
    int get(int key) {
        auto it = mp.find(key);
        if (it == mp.end())
            return -1; // Key not found

        // Move accessed node to front (most recent)
        dll.splice(dll.begin(), dll, it->second);
        return it->second->second; // Return value
    }

    // Put key-value pair
    void put(int key, int value) {
        auto it = mp.find(key);

        // If key exists, update value and move to front
        if (it != mp.end()) {
            it->second->second = value;
            dll.splice(dll.begin(), dll, it->second);
            return;
        }

        // If cache is full, remove least recently used (back)
        if ((int)dll.size() >= capacity) {
            auto last = dll.back();
            mp.erase(last.first);
            dll.pop_back();
        }

        // Insert new key-value at front
        dll.emplace_front(key, value);
        mp[key] = dll.begin();
    }

    // Display cache from MRU ? LRU
    void display() const {
        cout << "Cache state (MRU ? LRU): ";
        for (const auto &p : dll)
            cout << "[" << p.first << ":" << p.second << "] ";
        cout << "\n";
    }
};

int main() {
    LRUCache cache(3);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    cout << "get(2): " << cache.get(2) << "\n"; // Access key 2

    cache.display();

    cache.put(4, 400); // Evicts key 1
    cache.display();

    cout << "get(1): " << cache.get(1) << "\n"; // Should return -1
}

