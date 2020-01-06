#include <iostream>
#include <LRParser.h>
#include <string>
#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char* argv[]){
    int mode;
    std::cout << "enter mode (0 - testing, 1 - custom)" << std::endl;
    std::cin >> mode;
    if (!mode) {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } else{
        std::string file = argv[1];
        std::string::size_type sz;
        int cnt = std::stoi(argv[2], &sz);
        std::ifstream input;
        input.open(file);
        std::string symbols;
        char start_symbol;
        input >> start_symbol;
        input >> symbols;
        std::vector<std::pair<char, std::string>> grammar;
        std::string rule = "";
        while (!input.eof()){
            input >> rule;
            char left = rule[0];
            std::string right = rule.substr(3);
            grammar.push_back(std::make_pair(left, right));
        }
        std::set<char> grammar_symbols;
        for (auto c: symbols){
            grammar_symbols.insert(c);
        }
        LRParser parser = LRParser(grammar, grammar_symbols, start_symbol);
        parser.build_lr_table();
        std::string _;
        std::getline(std::cin, _);
        for (int i = 0; i < cnt; i++){
            std::string word;
            std::getline(std::cin, word);
            bool result = parser.check_word(word);
            if (result)
                std::cout << "Accepted!" << std::endl;
            else
                std::cout << "Rejected!" << std::endl;
        }
    }
}