#pragma once

#include <string>
#include <vector>
#include <unordered_map>

template <typename Identifier, typename Reference>
class S_Container {
public:
    using Pair = std::pair<Identifier, Reference>;
    using Underlying_Type = std::unordered_map<Identifier, Reference>;

    void insert(const Pair& data);

    Reference* find(const Identifier& key) const;

    const std::vector<std::string>& get_key_list() { return _key_list; };

    typename Underlying_Type::const_iterator begin() const { return _hash_map.cbegin(); }

    typename Underlying_Type::const_iterator end() const { return _hash_map.cend(); }

private:
    Underlying_Type _hash_map;
    std::vector<Identifier> _key_list;
};

template <typename Identifier, typename Reference>
void S_Container<Identifier, Reference>::insert(const Pair& data) {
    _hash_map.insert(data);

    _key_list.emplace_back(data.first);
}

template <typename Identifier, typename Reference>
Reference* S_Container<Identifier, Reference>::find(
    const Identifier& key) const {
    auto fr = _hash_map.find(key);

    if (fr != _hash_map.end()) {
        return &(fr->second);
    } else {
        return nullptr;
    }
}
