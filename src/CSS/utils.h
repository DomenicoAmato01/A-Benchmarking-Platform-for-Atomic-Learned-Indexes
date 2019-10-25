//
// Created by Giorgio Vinciguerra on 01/07/2018.
//

#ifndef TOPDOWNINDEX_UTILS_H
#define TOPDOWNINDEX_UTILS_H

#include <cstddef>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstring>


template<typename TypeIn, typename TypeOut>
std::vector<TypeOut> read_dataset_binary(const std::string &filename,
                                         size_t max_size = std::numeric_limits<size_t>::max()) {
    try {
        std::fstream in(filename, std::ios::in |  std::ios::ate);
//        in.exceptions(std::ios::failbit | std::ios::badbit);
	 auto size=0;
//        auto size = std::min(max_size, static_cast<size_t>(in.tellg() / sizeof(TypeIn)));
        //std::vector<TypeIn> dataset(size);
        in.seekg(0);
	int a;
	 while (in >> a)
    	{
//        	printf("%d ", a);
		size++;
//		dataset.push_back(a);
    	}

	std::vector<TypeIn> dataset(size);
	std::fstream in2(filename, std::ios::in |  std::ios::ate);
	in2.seekg(0);
	int i=0;
	std::cout <<"Load dataset"<<std::endl;
	while(in2 >> a){
//		printf("%d ",a);
		dataset[i]=a;
		i++;
	}

//	for (auto q: dataset){
//            std::cout << "key "<<q<< std::endl;
//        }

//        in.read((char *) dataset.data(), size * sizeof(TypeIn));
        if constexpr (std::is_same<TypeIn, TypeOut>::value)
            return dataset;

	

        return std::vector<TypeOut>(dataset.begin(), dataset.end());
    }
    catch (std::ios_base::failure& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << std::strerror(errno) << std::endl;
        exit(1);
    }
}

template<class T>
void do_not_optimize(T const &value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

#endif //TOPDOWNINDEX_UTILS_H
