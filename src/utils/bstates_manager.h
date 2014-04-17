#ifndef BOARD_STATE_MANAGER_H
#define BOARD_STATE_MANAGER_H

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "board.h"

typedef std::shared_ptr<std::vector<std::shared_ptr<Board>>> Boards;

class BoardStateManager
{
public:
    static void generate(const std::string& path);

    static std::unique_ptr<std::vector<std::unique_ptr<Board>>> load(
            const std::string& path);

    static std::shared_ptr<Board> get_base_case(std::shared_ptr<Board> board);
    static void rotate(std::shared_ptr<Board> board);
    static void sym(std::shared_ptr<Board> board);
    
private:
    static void genrec(std::shared_ptr<Board> board, Boards boards, char p);

    static bool eq(std::shared_ptr<Board> b1, std::shared_ptr<Board> b2);


    static void save(Boards boards, const std::string& path);
    
    static bool is_generated(std::shared_ptr<Board> board);
    //static bool is_transform(std::shared_ptr<Board> board,
            //std::shared_ptr<Board> comp);

    static int hash(std::shared_ptr<Board> board);
    
    static std::map<int, std::shared_ptr<std::vector<std::shared_ptr<Board>>>>
        cache;

    static int metric1(std::shared_ptr<Board> board);
    static int metric2(std::shared_ptr<Board> board);
    static int metric3(std::shared_ptr<Board> board);
    static int metric4(std::shared_ptr<Board> board);

};

typedef BoardStateManager BSM;

#endif
