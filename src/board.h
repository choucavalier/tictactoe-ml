#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <vector>

typedef std::tuple<char, char, char> Line;

class Board
{
public:
    Board ();
    virtual ~Board ();
    
    void print() const;

    void update(unsigned char move, unsigned char player_id);
    void undo(unsigned char move);

    unsigned char get_winner_id() const;

    std::unique_ptr<std::vector<unsigned char>> get_features(
            unsigned char player_id);

    std::vector<unsigned char> const* get_cells() const;

    bool is_over();

    void set_cell(int const& index, char const& value);

    void swap(int const& i, int const& j);

    std::shared_ptr<Board> copy();

    std::string in_string();

private:
    std::unique_ptr<std::vector<unsigned char>> cells;
    static const std::vector<Line> END_LINES;
    Line get_line(const Line& end_line) const;
};

#endif
