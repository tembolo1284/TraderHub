#ifndef PRIORITIZABLE_VALUE_ST_HPP
#define PRIORITIZABLE_VALUE_ST_HPP

#include <unordered_map>
#include <map>
#include <memory>
#include <vector>
#include <optional>
#include <functional>
#include "order.hpp"

template<typename K, typename V>
class PrioritizableValueST {
private:
    struct Entry {
        K key;
        std::reference_wrapper<V> value;
        double price;
        int64_t timestamp;
        bool isBuyOrder;

        Entry(K init_key, V& init_value, double init_price, 
              int64_t init_timestamp, bool init_isBuyOrder)
            : key(std::move(init_key))
            , value(init_value)
            , price(init_price)
            , timestamp(init_timestamp)
            , isBuyOrder(init_isBuyOrder)
        {}

        // Rule of five
        Entry(const Entry&) = delete;
        Entry& operator=(const Entry&) = delete;
        Entry(Entry&&) noexcept = default;
        Entry& operator=(Entry&&) noexcept = default;
        ~Entry() = default;
    };

    struct CompositeKey {
        double price;
        int64_t timestamp;
        bool isBuyOrder;

        CompositeKey(double init_price, 
                    int64_t init_timestamp, 
                    bool init_isBuyOrder)
            : price(init_price)
            , timestamp(init_timestamp)
            , isBuyOrder(init_isBuyOrder)
        {}

        std::partial_ordering operator<=>(const CompositeKey& other) const {
            if (this->isBuyOrder) {
                if (price != other.price) {
                    if (price > other.price) return std::partial_ordering::less;
                    if (price < other.price) return std::partial_ordering::greater;
                }
            } else {
                if (price != other.price) {
                    if (price < other.price) return std::partial_ordering::less;
                    if (price > other.price) return std::partial_ordering::greater;
                }
            }
            
            if (timestamp < other.timestamp) return std::partial_ordering::less;
            if (timestamp > other.timestamp) return std::partial_ordering::greater;
            return std::partial_ordering::equivalent;
        }

        bool operator==(const CompositeKey& other) const {
            return price == other.price &&
                   timestamp == other.timestamp &&
                   isBuyOrder == other.isBuyOrder;
        }
    };

    std::unordered_map<K, std::shared_ptr<Entry>> entries_;
    std::map<CompositeKey, std::shared_ptr<Entry>> priorityMap_;

public:
    PrioritizableValueST() 
        : entries_()
        , priorityMap_()
    {}

    // Rule of five
    PrioritizableValueST(const PrioritizableValueST&) = delete;
    PrioritizableValueST& operator=(const PrioritizableValueST&) = delete;
    PrioritizableValueST(PrioritizableValueST&&) noexcept = default;
    PrioritizableValueST& operator=(PrioritizableValueST&&) noexcept = default;
    ~PrioritizableValueST() = default;

    void put(const K& key_in, double price_in, int64_t timestamp_in, V& value_in, bool isBuyOrder_in) {
        auto entry = std::make_shared<Entry>(key_in, value_in, price_in, timestamp_in, isBuyOrder_in);
    
        auto it = entries_.find(key_in);
        if (it != entries_.end()) {
            auto oldEntry = it->second;
            priorityMap_.erase(CompositeKey(
                oldEntry->price, oldEntry->timestamp, oldEntry->isBuyOrder));
        }

        entries_[key_in] = entry;
        priorityMap_.emplace(
            CompositeKey(price_in, timestamp_in, isBuyOrder_in), entry);
    }    

    [[nodiscard]] std::optional<std::reference_wrapper<const V>> get(const K& key_in) const {
        auto it = entries_.find(key_in);
        if (it != entries_.end()) {
            return std::cref(it->second->value);
        }
        return std::nullopt;
    }

    [[nodiscard]] std::optional<std::reference_wrapper<V>> get(const K& key_in) {
        auto it = entries_.find(key_in);
        if (it != entries_.end()) {
            return std::ref(it->second->value);
        }
        return std::nullopt;
    }

    void delete_(const K& key_in) {
        auto it = entries_.find(key_in);
        if (it != entries_.end()) {
            auto entry = it->second;
            priorityMap_.erase(CompositeKey(
                entry->price, entry->timestamp, entry->isBuyOrder));
            entries_.erase(it);
        }
    }

    [[nodiscard]] std::optional<std::reference_wrapper<V>> deleteMin() {
        if (entries_.empty()) {
            return std::nullopt;
        }

        auto it = priorityMap_.begin();
        if (it != priorityMap_.end()) {
            auto entry = it->second;
            auto& value = entry->value.get();
            entries_.erase(entry->key);
            priorityMap_.erase(it);
            return std::ref(value);
        }
        return std::nullopt;
    }

    [[nodiscard]] std::vector<std::reference_wrapper<V>> getAllValues() const {
        std::vector<std::reference_wrapper<V>> values;
        values.reserve(entries_.size());
    
        for (const auto& [_, entry] : entries_) {
            values.emplace_back(std::ref(entry->value));
        }
        return values;
    }

    [[nodiscard]] bool contains(const K& key_in) const {
        return entries_.contains(key_in);
    }

    [[nodiscard]] bool isEmpty() const {
        return entries_.empty();
    }

    [[nodiscard]] size_t size() const {
        return entries_.size();
    }
};

#endif
