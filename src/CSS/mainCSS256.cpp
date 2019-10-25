#include <iostream>
#include <random>
#include "utils.h"
#include "csstree.h"
#include <btree.h>
#include <chrono>

using payload_t = std::array<uint8_t, 64>;

template<typename K, size_t NodeSize>
struct custom_traits : stx::btree_default_set_traits<K> {
    static const size_t leafslots = BTREE_MAX(8, NodeSize / sizeof(K));
    static const size_t innerslots = BTREE_MAX(8, NodeSize / (sizeof(K) + sizeof(void *)));
};

template<size_t PageSize>
using custom_bptree = stx::btree<int64_t, payload_t, std::pair<int64_t, payload_t>, std::less<>,
        custom_traits<int64_t, PageSize>, true>;

template<int First, int Last, typename Lambda>
inline void static_for_pow_two(Lambda const &f) {
    if constexpr (First <= Last) {
        f(std::integral_constant<int, First>{});
        static_for_pow_two<First << 1, Last>(f);
    }
}

template<typename K, typename T>
void run_baseline_experiments(std::vector<K> &keys, std::vector<T> &dataset,char **argv,int argc) {
    std::mt19937 random{std::random_device{}()};

    // –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
    // CSS-tree
    // –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

    std::cout << std::endl << "# CSS-tree 256" << std::endl;
    std::cout << "page size (bytes),height,size (bytes),query time (ns)" << std::endl;
    static_for_pow_two<256, 256>([&](auto page_size) {

        auto t0 = std::chrono::high_resolution_clock::now();
	CSSTree<page_size> cssTree(keys);
        auto t1 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        std::cout << page_size << "," << cssTree.height() << "," << cssTree.size_in_bytes() << ","
                  << long(time) << std::endl;

if(argc>=3){
	auto query = read_dataset_binary<int, int64_t>(argv[2]);
        int correct=0;
        std::cout << "Numero queries: " << query.size() << std::endl;
        double timeQuery=0;
        t0 = std::chrono::high_resolution_clock::now();
        for (auto q: query){
//            std::cout << "Query "<<q<<" -> "<<bptree.exists(q)<< std::endl;
//            std::cout << "Query "<<q<<" -> "<<(bptree.find(q) != bptree.end())<< std::endl;
            t0 = std::chrono::high_resolution_clock::now();
            cssTree.find(q);          
            t1 = std::chrono::high_resolution_clock::now();
            timeQuery = timeQuery + std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        }
        timeQuery = timeQuery / query.size();
        std::cout << "Correct: " << correct << std::endl;
        std::cout << "Uncorrect: " << (query.size()-correct) << std::endl;
        std::cout << "Time (ns) " << double(timeQuery) << std::endl;
	}
    });
}

int main(int argc, char **argv) {

	if(argc<2) 
    { 
        printf("\nProgram usage: ./CSS256 PAGE_SIZE PATH_TO_DATASET_FILE [PATH_TO_QUERY_FILE]"); 
	return 1;
	}

 // Read the dataset
    auto keys = read_dataset_binary<int, int64_t>(argv[1]);
    std::vector<std::pair<int64_t, payload_t>> dataset;
    dataset.reserve(keys.size());
    for (auto k: keys)
        dataset.emplace_back(k, payload_t());

    run_baseline_experiments(keys, dataset,argv,argc);

    return 0;
}
