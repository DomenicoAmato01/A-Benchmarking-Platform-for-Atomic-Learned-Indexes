//
// Created by Giorgio Vinciguerra on 22/07/2018.
//

#ifndef TOPDOWNINDEX_CSSTREE_H
#define TOPDOWNINDEX_CSSTREE_H

/**
 * A static tree stored implicitly, without pointers.
 * @tparam NodeSize the size of a node of the tree in bytes
 */
template<size_t NodeSize, typename K = int64_t>
class CSSTree {
    size_t tree_height;
    size_t half_marker;
    size_t n_internal_nodes;
    std::vector<K> tree;
    const std::vector<K> &leaves;
    const size_t slots_per_node = NodeSize / sizeof(K);

    template<class Iterator>
    inline typename std::vector<K>::const_iterator find_in_leaves(Iterator lo, Iterator hi, K key) const {
        for (; lo != hi && *lo < key; ++lo);
        return leaves.cbegin() == lo || key == *lo ? lo : lo - 1;
    }

public:
    explicit CSSTree(const std::vector<K> &dataset) : leaves(dataset) {
        const auto n = leaves.size();
        const auto leaf_nodes = ceil(n / double(slots_per_node));
        tree_height = size_t(ceil(log(leaf_nodes) / log(slots_per_node + 1)));
        const auto expp = size_t(pow(slots_per_node + 1, tree_height));
        const auto last_internal_node = size_t((expp - leaf_nodes) / slots_per_node);
        n_internal_nodes = (size_t) ((expp - 1) / slots_per_node) - last_internal_node;
        tree = std::vector<K>(n_internal_nodes * slots_per_node);
        half_marker = (expp - 1) / slots_per_node;

        size_t i = tree.size();
        while (i-- > 0) {
            auto node = i / slots_per_node;
            auto child = node * (slots_per_node + 1) + 1 + i % slots_per_node;
            while (child < n_internal_nodes) // follow rightmost branch
                child = child * (slots_per_node + 1) + slots_per_node + 1;

            // child is a leaf -> map it to an index in the tree
            long diff = (child - half_marker) * slots_per_node;
            if (diff < 0)
                tree[i] = leaves[diff + n + slots_per_node - 1];
            else if (diff + slots_per_node - 1 < n - last_internal_node * slots_per_node)
                tree[i] = leaves[diff + slots_per_node - 1];
            else
                // special case: fill ancestor of the last leaf node with the
                // last element of (the biggest in) the first half of the tree
                tree[i] = leaves[n - last_internal_node * slots_per_node - 1];
        }
    }

    inline typename std::vector<K>::const_iterator find(K key) const {
        if (n_internal_nodes == 0)
            return find_in_leaves(leaves.cbegin(), leaves.cend(), key);

        size_t child = 0;
        while (child < n_internal_nodes) {
            auto index_in_tree = child * slots_per_node;
            if (NodeSize > 256) { // use binary search for large pages and scan for smaller ones
                auto lo = tree.cbegin() + index_in_tree;
                auto hi = std::min(tree.cend(), lo + slots_per_node + 1);
                auto pos = std::lower_bound(lo, hi, key);
                if (pos == hi)
                    --pos;
                child = child * (slots_per_node + 1) + 1 + std::distance(lo, pos);
            } else {
                size_t lo;
                for (lo = 0; lo < slots_per_node && tree[index_in_tree + lo] < key; ++lo);
                child = child * (slots_per_node + 1) + 1 + lo;
            }
        }

        long diff = (long(child) - long(half_marker)) * slots_per_node;
        if (diff < 0)
            diff += leaves.size();
        assert(diff >= 0);

        auto lo = leaves.cbegin() + std::min(leaves.size(), size_t(diff));
        auto hi = leaves.cbegin() + std::min(leaves.size(), diff + slots_per_node);
        return find_in_leaves(lo, hi, key);
    }

    /**
     * Returns the size in bytes of the tree.
     * @return the size in bytes
     */
    size_t size_in_bytes() const {
        return tree.size() * sizeof(K);
    }

    /**
     * Returns the height of the tree.
     * @return the height
     */
    size_t height() const {
        return tree_height;
    }
};

#endif //TOPDOWNINDEX_CSSTREE_H
