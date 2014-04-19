#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <vector>

typedef std::tuple<short, short, short> Line;

class Board
{
public:
    Board ();
    virtual ~Board ();
    
    void print() const;

    static void print(std::string const& board_map);

    void update(short move, short player_id);
    void undo(short move);

    short get_winner_id() const;
    static short get_winner_id(std::string const& board);

    bool is_over();

    static bool is_over(std::string const& board);

    void set_cell(int const& index, short const& value);

    short operator[](int index);

    void swap(int i, int j);

    std::string const& get_map();

    short level();

private:
    std::string map;
    static const std::vector<Line> END_LINES;
    Line get_line(const Line& end_line) const;
    static Line get_line(const Line& end_line, const std::string& board);
};

#endif
