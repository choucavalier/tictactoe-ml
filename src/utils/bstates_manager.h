#ifndef BOARD_STATE_MANAGER_H
#define BOARD_STATE_MANAGER_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <utility>

class BoardStateManager
{
public:
    static std::map<std::string, std::vector<std::string>> states;

    static void generate(std::string const& path);

    static void load(std::string const& path);

    static std::string get_base_case(std::string const& board);

    static void print(std::string const& board);
    
    static void rotate(std::string& board);
    static void sym(std::string& board);

    static std::string board(std::string const& base);

private:
    static void genrec(std::string& board, char p);

    static std::unique_ptr<std::vector<std::string>> get_transforms(
            std::string const& board);

    static void save(std::string const& path);


    static int metric1(std::string const& board);
    static int metric2(std::string const& board);
    static int metric3(std::string const& board);
    static int metric4(std::string const& board);

    static bool is_successor(std::string s, std::string f);
};

typedef BoardStateManager BSM;

#endif

