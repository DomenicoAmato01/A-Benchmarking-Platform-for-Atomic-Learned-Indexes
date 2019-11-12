//#define BTREE_DEBUG

#include <iostream>
#include <random>
#include "utils.h"
#include <btree.h>
#include <chrono>
#include <btree_set.h>


using payload_t = std::array<uint8_t, 128>;

template<typename K, size_t NodeSize>
struct custom_traits : stx::btree_default_set_traits<K> {
    static const size_t leafslots = BTREE_MAX(8, NodeSize / sizeof(K));
    static const size_t innerslots = BTREE_MAX(8, NodeSize / (sizeof(K) + sizeof(void *)));
};

template<size_t PageSize>
using custom_bptree = stx::btree_set<int64_t, std::less<>,custom_traits<int64_t, PageSize>>;

template<int First, int Last, typename Lambda>
inline void static_for_pow_two(Lambda const &f) {
    if constexpr (First <= Last) {
        f(std::integral_constant<int, First>{});
        static_for_pow_two<First << 1, Last>(f);
    }
}

template<typename K, typename T>
void run_baseline_experiments(std::vector<K> &keys, std::vector<T> &dataset, char **argv,int argc) {
    std::mt19937 random{std::random_device{}()};

    // –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
    // B+ tree
    // –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

    std::cout << std::endl << "# B+ tree" << std::endl;
    std::cout << "page size (bytes),height,size (bytes),query time (ns)" << std::endl;
    static_for_pow_two<256,256>([&](auto page_size) {
        auto t0 = std::chrono::high_resolution_clock::now();
        custom_bptree<page_size> bptree;
        bptree.bulk_load(keys.begin(), keys.end());
        auto t1 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();

        auto stats = bptree.get_stats();

//	auto leaves_bytes = sizeof(typename custom_bptree<page_size>::btree_impl::leaf_node)
//                            - sizeof(custom_bptree<page_size>::btree_impl::leaf_node::slotdata)
//                            - sizeof(custom_bptree<page_size>::btree_impl::leaf_node::slotkey);

        auto leaves_bytes = sizeof(typename custom_bptree<page_size>::btree_impl::leaf_node);
//                            - sizeof(custom_bptree<page_size>::btree_impl::leaf_node::slotkey);
        auto inner_bytes = sizeof(typename custom_bptree<page_size>::btree_impl::inner_node);
	auto keysize = keys.size()*sizeof(int64_t);
        auto bytes = stats.leaves * leaves_bytes + inner_bytes * stats.innernodes;
	std::cout << "Total bytes: "  << bytes << " DataStructure size: " << (bytes-keysize) << std::endl;

// 	bptree.print(std::cout);

        std::cout << "page size (bytes),height,size (bytes),query time (ns)" << std::endl;
        std::cout << page_size << "," << bytes << "," << long(time) << std::endl;

	std::cout << "Numero di foglie: " << stats.leaves << std::endl;
        std::cout << "Numero di chiavi: " << bptree.size() << std::endl;

	 //Leggo File query
	if(argc>=3){
	        auto query = read_dataset_binary<int, int64_t>(argv[2]);
        	int correct=0;
        	std::cout << "Numero queries: " << query.size() << std::endl;
		double timeQuery=0;
        	t0 = std::chrono::high_resolution_clock::now();
        	for (auto q: query){
		    t0 = std::chrono::high_resolution_clock::now();
		    if(bptree.exists(q)){ 
	                correct++;
                    }
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
	int pageBlock;

    if(argc<2) 
    { 
        printf("\nProgram usage: ./Btree PAGE_SIZE PATH_TO_DATASET_FILE [PATH_TO_QUERY_FILE]"); 
	return 1;
	}

    // Read the dataset
    auto keys = read_dataset_binary<int, int64_t>(argv[1]);
    std::cout << "Dataset size: " << keys.size() << std::endl;
    std::vector<std::pair<int64_t, payload_t>> dataset;
    run_baseline_experiments(keys, dataset, argv,argc);

    return 0;
}
