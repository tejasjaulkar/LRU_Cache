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
    // Create a cache with capacity 5
    LRUCache cache(5);

    // Add 10 key-value pairs (to test eviction)
    for (int i = 1; i <= 10; i++) {
        cache.put(i, i * 100); // key=i, value=i*100
        cout << "After inserting (" << i << "," << i * 100 << "):\n";
        cache.display();
    }

    // Access some keys to make them most recently used
    cout << "\nAccess key 7: " << cache.get(7) << "\n";
    cache.display();

    cout << "\nAccess key 3: " << cache.get(3) << "\n"; // likely evicted
    cache.display();

    cout << "\nAccess key 10: " << cache.get(10) << "\n";
    cache.display();

    return 0;
}

