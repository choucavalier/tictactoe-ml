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
    static std::map<std::string, std::vector<std::pair<short, std::string>>>
        states;
    static std::map<std::string, std::vector<std::pair<short, std::string>>>
        strategies;

    static void generate(std::string const& path);

    static void load(std::string const& path);

    static std::string get_base_case(std::string const& board);

    static std::string str(std::string const& board);

    static std::string board(std::string const& base);

    static void print_strategy();

    static void rotate(std::string& board);
    static void sym(std::string& board);

    static short get_move(std::string f, std::string t, short move);

    static short whos_turn(std::string const& board);

private:
    static void genrec(std::string& board, char p, std::string lastbc);

    static std::unique_ptr<std::vector<std::string>> get_transforms(
            std::string const& board);

    static void save(std::string const& path);

    static short diff1(std::string const& b1, std::string const& b2);
    
    static int metric1(std::string const& board);
    static int metric2(std::string const& board);
    static int metric3(std::string const& board);
    static int metric4(std::string const& board);
};

typedef BoardStateManager BSM;

#endif

