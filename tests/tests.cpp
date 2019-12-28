#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testRegParser

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/LRParser.h"

TEST(SimpleTests, SimpleTest1){
    std::vector<std::pair<char, std::string>> grammar;
    grammar.push_back(std::make_pair('S', "T"));
    grammar.push_back(std::make_pair('T', "(T)T"));
    grammar.push_back(std::make_pair('T', ""));
    std::set<char> grammar_symbols;
    grammar_symbols.insert('S');
    grammar_symbols.insert('T');
    LRParser parser = LRParser(grammar, grammar_symbols, 'S');
    EXPECT_NO_THROW(parser.build_lr_table());
    EXPECT_TRUE(parser.check_word("()"));
    EXPECT_TRUE(parser.check_word(""));
    EXPECT_TRUE(parser.check_word("()()"));
    EXPECT_TRUE(parser.check_word("(())"));
    EXPECT_FALSE(parser.check_word("(()"));
    EXPECT_FALSE(parser.check_word("(()))"));
}

TEST(SimpleTests, SimpleTest2){
    std::vector<std::pair<char, std::string>> grammar;
    grammar.push_back(std::make_pair('S', "T"));
    grammar.push_back(std::make_pair('T', "aT"));
    grammar.push_back(std::make_pair('T', ""));
    std::set<char> grammar_symbols;
    grammar_symbols.insert('S');
    grammar_symbols.insert('T');
    LRParser parser = LRParser(grammar, grammar_symbols, 'S');
    EXPECT_NO_THROW(parser.build_lr_table());
    EXPECT_TRUE(parser.check_word(""));
    EXPECT_TRUE(parser.check_word("a"));
    EXPECT_TRUE(parser.check_word("aaaaaaa"));
    EXPECT_FALSE(parser.check_word("aaaaaaab"));
    EXPECT_FALSE(parser.check_word("b"));
}

TEST(SimpleTests, SimpleTest3){
    std::vector<std::pair<char, std::string>> grammar;
    grammar.push_back(std::make_pair('S', "E"));
    grammar.push_back(std::make_pair('E', "E+T"));
    grammar.push_back(std::make_pair('E', "T"));
    grammar.push_back(std::make_pair('T', "T*F"));
    grammar.push_back(std::make_pair('T', "F"));
    grammar.push_back(std::make_pair('F', "(E)"));
    grammar.push_back(std::make_pair('F', "x"));
    std::set<char> grammar_symbols;
    grammar_symbols.insert('S');
    grammar_symbols.insert('T');
    grammar_symbols.insert('E');
    grammar_symbols.insert('F');
    LRParser parser = LRParser(grammar, grammar_symbols, 'S');
    EXPECT_NO_THROW(parser.build_lr_table());
    EXPECT_TRUE(parser.check_word("x"));
    EXPECT_TRUE(parser.check_word("(x)"));
    EXPECT_TRUE(parser.check_word("(x+x)*x"));
    EXPECT_TRUE(parser.check_word("x*x+(x*x)"));
    EXPECT_TRUE(parser.check_word("x+x+(x*x)"));
    EXPECT_TRUE(parser.check_word("x*x+(x+x)"));
    EXPECT_TRUE(parser.check_word("((x*x+(x+x)))"));
    EXPECT_TRUE(parser.check_word("((x*x+(x+x)))+x+x*x+x*x+x+(((x+x)))"));
    EXPECT_FALSE(parser.check_word("((x*x+((x+x)))"));
    EXPECT_FALSE(parser.check_word("x+"));
    EXPECT_FALSE(parser.check_word("+x"));
    EXPECT_FALSE(parser.check_word(""));
    EXPECT_FALSE(parser.check_word("x+x+"));
    EXPECT_FALSE(parser.check_word("x+x*"));
    EXPECT_FALSE(parser.check_word("x+x)"));
    EXPECT_FALSE(parser.check_word("x(+x"));
}

TEST(SimpleTests, SimpleTest4){
    std::vector<std::pair<char, std::string>> grammar;
    grammar.push_back(std::make_pair('S', "T"));
    grammar.push_back(std::make_pair('T', "aTa"));
    grammar.push_back(std::make_pair('T', "bTb"));
    grammar.push_back(std::make_pair('T', "a"));
    grammar.push_back(std::make_pair('T', "b"));
    grammar.push_back(std::make_pair('T', ""));
    std::set<char> grammar_symbols;
    grammar_symbols.insert('S');
    grammar_symbols.insert('T');
    LRParser parser = LRParser(grammar, grammar_symbols, 'S');
    EXPECT_ANY_THROW(parser.build_lr_table());
}

TEST(TaskTests, TaskTests1){
    std::vector<std::pair<char, std::string>> grammar;
    grammar.push_back(std::make_pair('S', "T"));
    grammar.push_back(std::make_pair('T', "C"));
    grammar.push_back(std::make_pair('T', "TC"));
    grammar.push_back(std::make_pair('C', "cD"));
    grammar.push_back(std::make_pair('D', "aDb"));
    grammar.push_back(std::make_pair('D', ""));
    std::set<char> grammar_symbols;
    grammar_symbols.insert('S');
    grammar_symbols.insert('T');
    grammar_symbols.insert('C');
    grammar_symbols.insert('D');
    LRParser parser = LRParser(grammar, grammar_symbols, 'S');
    EXPECT_NO_THROW(parser.build_lr_table());
    EXPECT_TRUE(parser.check_word("caaaabbbbcaabbcabccccaaaabbbbccccabcab"));
    EXPECT_TRUE(parser.check_word("cccccaabbcabcabcabcaaaaabbbbb"));
    EXPECT_FALSE(parser.check_word("caaaabbbbcaabbb"));
    EXPECT_FALSE(parser.check_word("caaaabbbbcaabbcca"));
    EXPECT_FALSE(parser.check_word("caaaabbbbcaabbccb"));
    EXPECT_FALSE(parser.check_word("caacbb"));
    EXPECT_FALSE(parser.check_word("caabbcabb"));
    EXPECT_FALSE(parser.check_word("caabbcacb"));
}

TEST(TaskTests, TaskTests2){
    std::vector<std::pair<char, std::string>> grammar;
    grammar.push_back(std::make_pair('S', "T"));
    grammar.push_back(std::make_pair('T', "Ta"));
    grammar.push_back(std::make_pair('D', "cD"));
    grammar.push_back(std::make_pair('T', "TTb"));
    grammar.push_back(std::make_pair('T', "C"));
    grammar.push_back(std::make_pair('C', "Dd"));
    grammar.push_back(std::make_pair('D', ""));
    std::set<char> grammar_symbols;
    grammar_symbols.insert('S');
    grammar_symbols.insert('T');
    grammar_symbols.insert('C');
    grammar_symbols.insert('D');
    LRParser parser = LRParser(grammar, grammar_symbols, 'S');
    EXPECT_NO_THROW(parser.build_lr_table());
    EXPECT_TRUE(parser.check_word("cccccdcccdaaabdaaaaacdabb"));
    EXPECT_TRUE(parser.check_word("daaaaaaaaaa"));
    EXPECT_TRUE(parser.check_word("cccccddbdaaaaacdaaabb"));
    EXPECT_TRUE(parser.check_word("cccdaaaaaaaaaa"));
    EXPECT_FALSE(parser.check_word("ccccccccdaaaaaaaaaacdabb"));
    EXPECT_FALSE(parser.check_word("ccccccccdaaaaaaaaaacdabb"));
    EXPECT_FALSE(parser.check_word("ccccccccdaaaabaaaaaac"));
    EXPECT_FALSE(parser.check_word("ccccccccdaaaaaaaaaacd"));
    EXPECT_FALSE(parser.check_word("ccccccccdaaaaaaaaaab"));
}

