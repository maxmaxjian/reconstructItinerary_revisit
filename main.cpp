#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <iterator>

class solution {
  public:
    std::vector<std::string> findItinerary(const std::vector<std::pair<std::string,std::string>> & tickets) {
        std::unordered_map<std::string, std::vector<std::string>> dict;
        for (auto & pr : tickets) {
            if (dict.find(pr.first) == dict.end())
                dict[pr.first] = std::vector<std::string>();
            dict[pr.first].push_back(pr.second);
        }

        auto paths = getPaths(dict, std::vector<std::string>{"JFK"});
        std::for_each(paths.begin(), paths.end(),
                      [](const std::vector<std::string> & vec){
                          std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(std::cout, " "));
                          std::cout << std::endl;
                      });

        return *std::min_element(paths.begin(), paths.end(),
                                 [](const std::vector<std::string> & v1, const std::vector<std::string> & v2){
                                     return std::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end());
                                 });
    }

  private:
    std::vector<std::vector<std::string>> getPaths(std::unordered_map<std::string,std::vector<std::string>> & dict, const std::vector<std::string> & curr) {        
        std::vector<std::vector<std::string>> result;
        std::vector<std::string> next = getNext(dict, curr);
        if (next.empty())
            result.push_back(curr);
        else {
            for (auto nx : next) {
                auto dict_copy = dict;
                if (dict_copy[curr.back()].size() == 1)
                    dict_copy.erase(curr.back());
                else {
                    auto it = std::find(dict_copy[curr.back()].begin(), dict_copy[curr.back()].end(), nx);
                    dict_copy[curr.back()].erase(it);
                }
                auto cpy = curr;
                cpy.push_back(nx);
                
                auto temp = getPaths(dict_copy, cpy);
                std::copy(temp.begin(), temp.end(), std::inserter(result, result.end()));
            }
        }
        return result;
    }

    std::vector<std::string> getNext(std::unordered_map<std::string,std::vector<std::string>> & dict, const std::vector<std::string> & curr) {
        std::vector<std::string> next;
        if (dict.find(curr.back()) != dict.end())
            std::copy(dict[curr.back()].begin(), dict[curr.back()].end(), std::inserter(next,next.end()));
        return next;
    }
};

int main() {
    std::vector<std::pair<std::string,std::string>> tickets;
    tickets.push_back(std::make_pair("JFK","SFO"));
    tickets.push_back(std::make_pair("JFK","ATL"));
    tickets.push_back(std::make_pair("SFO","ATL"));
    tickets.push_back(std::make_pair("ATL","JFK"));
    tickets.push_back(std::make_pair("ATL","SFO"));

    solution soln;
    auto rtn = soln.findItinerary(tickets);
    std::cout << "The smallest itinerary in lexicographical order is:\n";
    std::copy(rtn.begin(), rtn.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << std::endl;
}
