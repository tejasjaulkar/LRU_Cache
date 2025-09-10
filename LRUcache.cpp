#include <iostream>
#include <unordered_map>
#include <list>
#include <stdexcept>
#include <optional>  // for std::optional (C++17+)

using namespace std;

class LRUCache {
private:
    int capacity;
    int pageFaults = 0;   // count of page faults (misses)
    int hits = 0;         // count of cache hits
    list<pair<int, int>> dll; // {key, value}, front = most recent
    unordered_map<int, list<pair<int, int>>::iterator> mp;

public:
    // Constructor
    LRUCache(int cap) {
        if (cap <= 0)
            throw invalid_argument("Capacity must be greater than 0");
        capacity = cap;
    }

    // Get value by key
    int get(int key) {
        auto it = mp.find(key);
        if (it == mp.end()) {
            pageFaults++; // miss
            return -1; 
        }
        hits++; // hit
        // Move accessed node to front (most recent)
        dll.splice(dll.begin(), dll, it->second);
        return it->second->second;
    }

    // Put key-value pair, returns evicted {key,value} if any
    optional<pair<int,int>> put(int key, int value) {
        auto it = mp.find(key);

        // If key exists, update value and move to front
        if (it != mp.end()) {
            it->second->second = value;
            dll.splice(dll.begin(), dll, it->second);
            hits++; // updating existing key = hit
            return nullopt;
        }

        pageFaults++; // new page -> fault
        optional<pair<int,int>> evicted = nullopt;

        // If cache is full, remove least recently used (back)
        if ((int)dll.size() >= capacity) {
            auto last = dll.back();
            mp.erase(last.first);
            dll.pop_back();
            evicted = last; // store evicted key/value
        }

        // Insert new key-value at front
        dll.emplace_front(key, value);
        mp[key] = dll.begin();
        return evicted;
    }

    // Display cache from MRU ? LRU
    void display() const {
        cout << "Cache state (MRU ? LRU): ";
        for (const auto &p : dll)
            cout << "[" << p.first << ":" << p.second << "] ";
        cout << "\n";
    }

    // Check if a key exists
    bool contains(int key) const {
        return mp.find(key) != mp.end();
    }

    // Current size of cache
    int size() const {
        return dll.size();
    }

    // Remove a key from cache
    bool remove(int key) {
        auto it = mp.find(key);
        if (it == mp.end()) return false;
        dll.erase(it->second);
        mp.erase(it);
        return true;
    }

    // Stats
    int getPageFaults() const { return pageFaults; }
    int getHits() const { return hits; }
    double getHitRatio() const { 
        int total = hits + pageFaults;
        return total == 0 ? 0.0 : (double)hits / total;
    }
};

int main() {
    // Create a cache with capacity 5
    LRUCache cache(5);

    // Add 10 key-value pairs (to test eviction)
    for (int i = 1; i <= 10; i++) {
        auto evicted = cache.put(i, i * 100); // key=i, value=i*100
        cout << "After inserting (" << i << "," << i * 100 << "):\n";
        cache.display();
        if (evicted) {
            cout << "Evicted: [" << evicted->first << ":" << evicted->second << "]\n";
        }
    }

    // Access some keys to make them most recently used
    cout << "\nAccess key 7: " << cache.get(7) << "\n";
    cache.display();

    cout << "\nAccess key 3: " << cache.get(3) << "\n"; // likely evicted
    cache.display();

    cout << "\nAccess key 10: " << cache.get(10) << "\n";
    cache.display();

    // Test remove()
    cout << "\nRemoving key 7...\n";
    cache.remove(7);
    cache.display();

    // Final stats
    cout << "\n=== Stats ===\n";
    cout << "Page Faults: " << cache.getPageFaults() << "\n";
    cout << "Hits: " << cache.getHits() << "\n";
    cout << "Hit Ratio: " << cache.getHitRatio() << "\n";

    return 0;
}

