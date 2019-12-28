//
// Created by Виктор on 021 21.12.19.
//

#include "LRParser.h"
#include <set>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <iostream>

LRParser::LRParser(std::vector<std::pair<char, std::string>> grammar, std::set<char> grammar_symbols, char start_symbol):
        grammar_{grammar},
        grammar_symbols_{grammar_symbols},
        start_symbol_{start_symbol}
{
}

void LRParser::add_state_(const std::set<LRParser::LRItem> &state) {
    for (auto s: states_){
        if (s == state)
            return;
    }
    states_.push_back(state);
}

void LRParser::compute_first_() {
    bool flag = true;
    for (auto symbol: grammar_symbols_){
        first_[symbol] = std::set<char>();
    }
    while (flag) {
        flag = false;
        for (auto rule: grammar_) {
            // rule A -> beta
            char A = rule.first;
            std::set<char> prev_set = first_[A];
            std::string beta = rule.second;
            if (beta == "") {
                first_[A].insert('\0');
                if (prev_set != first_[A])
                    flag = true;
                continue;
            }
            if (grammar_symbols_.find(beta[0]) == grammar_symbols_.end()) {
                first_[A].insert(beta[0]);
                if (prev_set != first_[A])
                    flag = true;
                continue;
            }
            for (auto c: first_[beta[0]]) {
                if (c != '\0')
                    first_[A].insert(c);
            }
            int i = 0;
            while (i < beta.length() - 1 && first_[beta[i]].count('\0')) {
                if (grammar_symbols_.find(beta[i+1]) != grammar_symbols_.end()) {
                    for (auto c: first_[beta[i+1]]) {
                        if (c != '\0')
                            first_[A].insert(c);
                    }
                    i++;
                }
                else{
                    first_[A].insert(beta[i+1]);
                    break;
                }
            }
            if (grammar_symbols_.find(beta[i]) != grammar_symbols_.end()) {
                if (i == beta.length()-1 && first_[beta[i]].count('\0'))
                    first_[A].insert('\0');
            }

            if (prev_set != first_[A])
                flag = true;
        }
    }
}

void LRParser::closure_(std::set<LRParser::LRItem> &state) {
    size_t size = 0;
    while (state.size() > size){
        size = state.size();
        for (auto item : state){
            auto curr_rule = grammar_[std::get<0>(item)];
            if (curr_rule.second.length() == std::get<1>(item))
                continue;
            char B = curr_rule.second[std::get<1>(item)];
            for (int i = 0; i < grammar_.size(); i++){
                auto rule = grammar_[i];
                if (rule.first == B) {
                    if (curr_rule.second.length() == std::get<1>(item) + 1) {
                        state.insert(std::make_tuple(i, 0, std::get<2>(item)));
                        continue;
                    }
                    if (grammar_symbols_.find(curr_rule.second[std::get<1>(item) + 1]) == grammar_symbols_.end()) {
                        state.insert(std::make_tuple(i, 0, curr_rule.second[std::get<1>(item) + 1]));
                        continue;
                    }
                    for (auto c: first_[curr_rule.second[std::get<1>(item) + 1]]) {
                        state.insert(std::make_tuple(i, 0, c));
                    }
                }
            }
        }
    }
}

std::set<LRParser::LRItem> LRParser::goto_(const std::set<LRParser::LRItem> &state, char c) {
    std::set<LRParser::LRItem> new_state;
    for (auto item: state) {
        auto curr_rule = grammar_[std::get<0>(item)];
        if (curr_rule.second.length() == std::get<1>(item))
            continue;
        char B = curr_rule.second[std::get<1>(item)];
        if (B == c) {
            new_state.insert(std::make_tuple(std::get<0>(item), std::get<1>(item) + 1, std::get<2>(item)));
        }
    }
    closure_(new_state);
    return new_state;
}

void LRParser::build_states_() {
    compute_first_();
    std::set<LRItem> start;
    for (int i = 0; i < grammar_.size(); i++) {
        auto r = grammar_[i];
        if (r.first == start_symbol_) {
            start.insert(std::make_tuple(i, 0, '\0'));
        }
    }
    closure_(start);
    states_.push_back(start);
    int i = 0;
    while (i < states_.size()){
        auto curr_state = states_[i];
        for (char c = -127; c < 127; c++){
            auto state = goto_(curr_state, c);
            if (!state.empty())
                add_state_(state);
        }
        i++;
    }
}

void LRParser::build_lr_table() {
    build_states_();

    for (int i = 0; i < states_.size(); i++) {
        for (auto rule: states_[i]) {
            if (grammar_[std::get<0>(rule)].first == start_symbol_ && std::get<1>(rule) == 1) {
                if (lr_action_.find(std::make_pair(i, '\0')) != lr_action_.end()) {
                    if (lr_action_[std::make_pair(i, '\0')] != std::make_pair(2, -1))
                        throw std::logic_error("Bad grammar");
                }
                else
                    lr_action_[std::make_pair(i, '\0')] = std::make_pair(2, -1);
            }
        }
        for (char c = -127; c < 127; c++){
            for (auto rule: states_[i]) {
                if (std::get<1>(rule) < grammar_[std::get<0>(rule)].second.length()) {
                    char a = grammar_[std::get<0>(rule)].second[std::get<1>(rule)];
                    if (a != c)
                        continue;
                    if (!grammar_symbols_.count(a)) {
                        for (int j = 0; j < states_.size(); j++) {
                            if (goto_(states_[i], a) == states_[j]) {
                                if (lr_action_.find(std::make_pair(i, a)) != lr_action_.end()){
                                    if (lr_action_[std::make_pair(i, a)] != std::make_pair(0, j))
                                        throw std::logic_error("Bad grammar");
                                }
                                else {
                                    lr_action_[std::make_pair(i, a)] = std::make_pair(0, j);
                                    break;
                                }
                            }
                        }
                    }
                    else
                        continue;
                } else {
                    char a = std::get<2>(rule);
                    if (a != c)
                        continue;
                    if (grammar_[std::get<0>(rule)].first == start_symbol_)
                        continue;
                    if (!grammar_symbols_.count(a)) {
                        if (lr_action_.find(std::make_pair(i, a)) != lr_action_.end()) {
                            if (lr_action_[std::make_pair(i, a)] != std::make_pair(1, std::get<0>(rule)))
                                throw std::logic_error("Bad grammar");
                        } else
                            lr_action_[std::make_pair(i, a)] = std::make_pair(1, std::get<0>(rule));
                    }
                }
            }
        }
        for (char c = -127; c < 127; c++){
            if (lr_action_.find(std::make_pair(i, c)) == lr_action_.end()){
                lr_action_[std::make_pair(i, c)] = std::make_pair(3, -1);
            }
        }
    }
    for (int i = 0; i < states_.size(); i++) {
        for (auto a: grammar_symbols_) {
            for (int j = 0; j < states_.size(); j++) {
                if (goto_(states_[i], a) == states_[j]) {
                    lr_goto_[std::make_pair(i, a)] = std::make_pair(0, j);
                }
            }
            if (lr_goto_.find(std::make_pair(i, a)) == lr_goto_.end())
                lr_goto_[std::make_pair(i, a)] = std::make_pair(1, -1);
        }
    }
}

bool LRParser::check_word(std::string s) {
    std::vector<std::pair<int, char>> LRStack;
    int pos = 0;
    s += '\0';
    LRStack.push_back(std::make_pair(0, '\0'));
    while (pos <= s.length()){
        char a = s[pos];
        std::pair<int, char> t = LRStack.back();
        if (lr_action_[std::make_pair(t.first, a)].first == 0) {
            pos++;
            LRStack.push_back(std::make_pair(lr_action_[std::make_pair(t.first, a)].second, a));
            continue;
        }
        if (lr_action_[std::make_pair(t.first, a)].first == 1) {
            std::pair<char, std::string> rule = grammar_[lr_action_[std::make_pair(t.first, a)].second];
            for (int i = 0; i < rule.second.length(); i++) {
                LRStack.pop_back();
            }
            int q_new = LRStack.back().first;
            char A = rule.first;
            if (lr_goto_[std::make_pair(q_new, A)].first == 0) {
                LRStack.push_back(std::make_pair(lr_goto_[std::make_pair(q_new, A)].second, A));
            }
            else{
                break;
            }
            continue;
        }
        if (lr_action_[std::make_pair(t.first, a)].first == 2){
            return true;
        }
        if (lr_action_[std::make_pair(t.first, a)].first == 3){
            break;
        }
    }
    return false;
}