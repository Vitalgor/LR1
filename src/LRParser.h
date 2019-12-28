//
// Created by Виталий on 021 21.12.19.
//

#ifndef LR1_LRPARSER_H
#define LR1_LRPARSER_H

#include <tuple>
#include <string>
#include <vector>
#include <set>
#include <map>

class LRParser {
public:
    LRParser(std::vector<std::pair<char, std::string>> grammar, std::set<char> grammar_symbols, char start_symbol);
    void build_lr_table();
    bool check_word(std::string s);

private:
    typedef std::tuple<int, int, char> LRItem; //number of rule in grammar, position of point and next char
    std::vector<std::pair<char, std::string>> grammar_;
    std::vector<std::set<LRItem>> states_;
    std::map<char, std::set<char>> first_;
    //action table. Second pair is action and additional info. 0 is shift, 1 - reduce, 2 - accept, 3 - reject
    std::map<std::pair<int, char>, std::pair<int, int>> lr_action_;
    //goto table. 0 is shift, 1 is reject
    std::map<std::pair<int, char>, std::pair<int, int>> lr_goto_;
    char start_symbol_;
    std::set<char> grammar_symbols_;
    void build_states_();
    void closure_(std::set<LRItem>& state);
    std::set<LRItem> goto_(const std::set<LRItem>& state, char c);
    void compute_first_();
    void add_state_(const std::set<LRItem>& state);
};


#endif //LR1_LRPARSER_H
