#include <iostream>
#include <cstring>
#include <cstdint>


enum CONSTANTS {
    GRID_SIZE = 9,
    BUF_SIZE = GRID_SIZE * GRID_SIZE,
};

struct Grid {
    int8_t grid[BUF_SIZE];

    Grid()
    {
        std::memset(grid, 0, sizeof(grid));
    }

    uint8_t at(int i, int j) const { return grid[i*GRID_SIZE+j]; }
    void set(int i, int j, int8_t val) { grid[i*GRID_SIZE+j] = val; }

    void read();
    void draw();
    void draw_simple();
    void basic_fill();

};

void Grid::read()
{
    char c;
    int i = 0;
    int num_space = 0;
    while (!std::cin.eof())
    {
        char c = std::cin.get();
        if ('1' <= c && c <= '9') {

            int val = c - '0';
            grid[i++] = val;
        } else if (c == ' ') {
            ++num_space;
            if (num_space==3) {
                grid[i++] = 0;
            }
            num_space = 0;
        } else if (c == '-') {
            grid[i++] = 0;
        }
        if (i >= BUF_SIZE) {
            while (!std::cin.eof())
                std::cin.get();
            break;
        }
    }
}

void draw_top() {
    std::cout <<"╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n";
}

void draw_mid() {
    std::cout <<"╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n";
}

void draw_bot() {
    std::cout <<"╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n";
}

void draw_regular() {
    std::cout <<"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n";
}

std::string to_rep(int8_t val)
{
    if (val == 0)
        return " ";
    else
        return std::to_string((int) val);
}

std::string to_rep_simple(int8_t val)
{
    if (val == 0)
        return "-";
    else
        return std::to_string((int) val);
}

void Grid::draw_simple()
{
   for (auto i = 0; i < GRID_SIZE; ++i)
   {
       for (auto j = 0; j < GRID_SIZE; ++j) {
           std::cout <<to_rep_simple( at(i,j));
       }
       if (i < GRID_SIZE-1) {
           std::cout<<"/";
       }
   }
   std::cout<<"\n";
}

void Grid::draw()
{
    for (auto i = 0; i < GRID_SIZE; ++i)
    {
        if (i==0) {
            draw_top();
        } else if (i == 3 || i == 6) {
            draw_mid();
        }
        std::cout
            << "║ "
            << to_rep(grid[i*GRID_SIZE + 0])
            << " │ "
            << to_rep(grid[i*GRID_SIZE + 1])
            << " │ "
            << to_rep(grid[i*GRID_SIZE + 2])
            << " ║ "
            << to_rep(grid[i*GRID_SIZE + 3])
            << " │ "
            << to_rep(grid[i*GRID_SIZE + 4])
            << " │ "
            << to_rep(grid[i*GRID_SIZE + 5])
            << " ║ "
            << to_rep(grid[i*GRID_SIZE + 6])
            << " │ "
            << to_rep(grid[i*GRID_SIZE + 7])
            << " │ "
            << to_rep(grid[i*GRID_SIZE + 8])
            << " ║\n";
        // std::cout  <<"\n";

        if (i == 8) {
            draw_bot();
        } else if (i != 2 && i != 5) {
            draw_regular();
        }
    }
}

void Grid::basic_fill()
{
    for (auto i = 0; i < GRID_SIZE; ++i)
    {
        for (auto j = 0; j < GRID_SIZE; ++j)
        {
            grid[i*GRID_SIZE+j] = (i+j)%GRID_SIZE + 1;
        }
    }
}

void show_empty_grid(void)
{
    Grid g;
    g.draw();
}

int main(int argc, char *argv[])
{
    Grid g;
    g.basic_fill();
    g.draw_simple();
    g.draw();
    return 0;
}
