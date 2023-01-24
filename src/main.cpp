#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

#include <cstring>
#include <cstdint>
#ifdef _WIN32
#include  <Windows.h>
#endif


enum CONSTANTS {
    GRID_SIZE = 9,
    BUF_SIZE = GRID_SIZE * GRID_SIZE,
};

struct Candidate {
    int i; //position i
    int j; //position j
    std::set<int8_t> values;
    int won_holes;

    Candidate():
        i{0},
        j{0},
        won_holes{0}
    {
    }
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
    void draw() const;
    void draw_simple() const;
    void draw_simple(char *) const;
    void basic_fill();
    bool fill_basic_candidates(std::vector<Candidate> &candidates);

    void solve(std::unordered_map<std::string,bool> &already_done, std::string &key, int depth=0);
    int get_num_hole();

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
            if (num_space == 3) {
                grid[i++] = 0;
                num_space = 0;
            }
        } else if (c == '-') {
            grid[i++] = 0;
        } else {
            num_space = 0;
        }
        if (i >= BUF_SIZE) {
            while (!std::cin.eof())
                std::cin.get();
            break;
        }
    }
}

// void draw_top() {
//     std::cout <<"╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n";
// }

// void draw_bot() {
//     std::cout <<"╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n";
// }

// void draw_mid() {
//     std::cout <<"╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n";
// }

// void draw_regular() {
//     std::cout <<"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n";
// }
// heavy
void draw_top() {
    std::cout <<"┏━━━┯━━━┯━━━┳━━━┯━━━┯━━━┳━━━┯━━━┯━━━┓\n";
}

void draw_bot() {
    std::cout <<"┗━━━┷━━━┷━━━┻━━━┷━━━┷━━━┻━━━┷━━━┷━━━┛\n";
}

void draw_mid() {
    std::cout <<"┣━━━┿━━━┿━━━╋━━━┿━━━┿━━━╋━━━┿━━━┿━━━┫\n";
}

void draw_regular() {
    std::cout <<"┠───┼───┼───╂───┼───┼───╂───┼───┼───┨\n";
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

void Grid::draw_simple() const
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

void Grid::draw_simple(char *out) const
{
    int k = 0;
    for (auto i = 0; i < GRID_SIZE; ++i)
    {
        for (auto j = 0; j < GRID_SIZE; ++j) {
            out[k++] = to_rep_simple( at(i,j))[0];
        }
        if (i < GRID_SIZE-1) {
            out[k++] = '/';
        }
    }
    std::cout<<"\n";
}


void Grid::draw() const
{
    for (auto i = 0; i < GRID_SIZE; ++i)
    {
        if (i==0) {
            draw_top();
        } else if (i == 3 || i == 6) {
            draw_mid();
        }
        std::cout
            // << "║ "
            << "┃ "
            << to_rep(at(i, 0))
            << " │ "
            << to_rep(at(i, 1))
            << " │ "
            << to_rep(at(i, 2))
            // << " ║ "
            << " ┃ "
            << to_rep(at(i, 3))
            << " │ "
            << to_rep(at(i, 4))
            << " │ "
            << to_rep(at(i, 5))
            // << " ║ "
            << " ┃ "
            << to_rep(at(i, 6))
            << " │ "
            << to_rep(at(i, 7))
            << " │ "
            << to_rep(at(i, 8))
            << " ┃\n";
        // << " ║\n";
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


int Grid::get_num_hole()
{
    int num_hole = 0;
    for (auto i = 0; i < GRID_SIZE; ++i)
    {
        for (auto j = 0; j < GRID_SIZE; ++j)
        {
            num_hole += at(i, j) == 0;
        }
    }
    return num_hole;
}

bool Grid::fill_basic_candidates(std::vector<Candidate> &candidates)
{
    //candidates.reserve(num_hole);
    candidates.resize(BUF_SIZE);
    int32_t num_found_with_basic = 0;
    int32_t num_found_with_single = 0;


    bool found_with_basic_candidates = false;

    // fill basic_candidates
    for (auto i = 0; i < GRID_SIZE; ++i)
    {
        for (auto j = 0; j < GRID_SIZE; ++j)
        {
            if (at(i, j) != 0) {
                // not a hole
                continue;
            }
            bool square_found = false;
            Candidate &c = candidates[i*GRID_SIZE+j];
            for (int8_t v = 1; v< GRID_SIZE+1; ++v) {
                c.values.emplace(v);
            }

            // remove candidates for row
            for (int k = 0; k < GRID_SIZE; ++k) {
                auto val = at(i, k);
                if (val == 0) continue;
                // already in same row
                c.values.erase(val);
            }

            // remove candidates for column
            for (int k = 0; k < GRID_SIZE; ++k) {
                auto val = at(k, j);
                if (val == 0) continue;
                // already in same column
                c.values.erase(val);
            }

            // remove candidates for square
            auto IS = i / 3;
            auto JS = j / 3;
            for (int is = 0; is < 3; ++is) {
                for (int js = 0; js < 3; ++js) {
                    auto val = at(3*IS+is, 3*JS+js);
                    if (val == 0) continue;
                    // already in same square
                    c.values.erase(val);
                }
            }

            if (c.values.size() == 0) {
                // std::cout<<"unsolvable!\n";
                return false;
            }
            if (c.values.size() == 1) {
                set(i, j, *c.values.begin());
                return false;
            }
        }
    }
    return true;
}

void Grid::solve(std::unordered_map<std::string,bool> &already_done, std::string &key, int depth)
{
    auto it = already_done.find(key);
    if (it != already_done.end()) {
        return;
    }

    int previous_num_hole = 0;
    int num_hole = get_num_hole();
    if (num_hole == 0) {
        return;
    }
    // std::cout<<num_hole<<" holes !\n";
    while (num_hole != previous_num_hole) {
        std::vector<Candidate> candidates;
        //candidates.reserve(num_hole);
        candidates.resize(BUF_SIZE);
        int32_t num_found_with_basic = 0;
        int32_t num_found_with_single = 0;


        bool found_with_basic_candidates = false;

        // fill basic_candidates
        for (auto i = 0; i < GRID_SIZE; ++i)
        {
            for (auto j = 0; j < GRID_SIZE; ++j)
            {
                if (at(i, j) != 0) {
                    // not a hole
                    continue;
                }
                bool square_found = false;
                Candidate &c = candidates[i*GRID_SIZE+j];
                for (int8_t v = 1; v< GRID_SIZE+1; ++v) {
                    c.values.emplace(v);
                }

                // remove candidates for row
                for (int k = 0; k < GRID_SIZE; ++k) {
                    auto val = at(i, k);
                    if (val == 0) continue;
                    // already in same row
                    c.values.erase(val);
                }

                // remove candidates for column
                for (int k = 0; k < GRID_SIZE; ++k) {
                    auto val = at(k, j);
                    if (val == 0) continue;
                    // already in same column
                    c.values.erase(val);
                }

                // remove candidates for square
                auto IS = i / 3;
                auto JS = j / 3;
                for (int is = 0; is < 3; ++is) {
                    for (int js = 0; js < 3; ++js) {
                        auto val = at(3*IS+is, 3*JS+js);
                        if (val == 0) continue;
                        // already in same square
                        c.values.erase(val);
                    }
                }

                if (c.values.size() == 1) {
                    set(i,j, *c.values.begin());
                    found_with_basic_candidates = true;
                    square_found = true;
                    ++num_found_with_basic;
                }

                if (c.values.size() == 0) {
                    // std::cout<<"unsolvable!\n";
                    already_done[key] = true;
                    return;
                }
            }
        }
        // find candidates that appear in a single place
        for (auto v = 1; v < GRID_SIZE+1; ++v)
        {
            for (auto i = 0; i < GRID_SIZE; ++i)
            {
                int num_appearing = 0;
                // in row i
                int pos = -1;
                for (int j = 0; j < GRID_SIZE; ++j) {
                    auto &c = candidates[i*GRID_SIZE+j];
                    if (c.values.count(v)) {
                        ++num_appearing;
                        pos = j;
                    }
                }
                //std::cout<<"candidate v="<<v<< " in row "<<i<<" num="<<num_appearing<<"\n";
                if (num_appearing == 1) {
                    set(i, pos, v);
                    candidates[i*GRID_SIZE+pos].values.clear();
                    ++num_found_with_single;
                    break;
                }
                num_appearing = 0;

                // in col i
                pos = -1;
                for (int j = 0; j < GRID_SIZE; ++j) {
                    auto &c = candidates[j*GRID_SIZE+i];
                    if (c.values.count(v)) {
                        ++num_appearing;
                        pos = j;
                    }
                }
                //std::cout<<"candidate v="<<v<< " in col "<<i<<" num="<<num_appearing<<"\n";
                if (num_appearing == 1) {
                    set(pos, i, v);
                    candidates[pos*GRID_SIZE+i].values.clear();
                    ++num_found_with_single;
                    break;
                }
            }
        }
        previous_num_hole = num_hole;
        num_hole = get_num_hole();

        // std::cout<< "basic="<<num_found_with_basic
        //          << " single="<<num_found_with_single
        //          << " remaining_holes="<<num_hole<<"\n";
        if (num_hole == 0) {
            std::cout<<"solved!\n";
            break;
        }

    }
    if (num_hole != 0) {
        // basic heuristic not enough
        // try to to perform arbitrary filling and backtrack if not working
        std::vector<Candidate> candidates;
        std::vector<Candidate> candidates2;
        bool ok = fill_basic_candidates(candidates);
        if (!ok) {
            already_done[key] = true;
            return;
        }
        for (auto k = 0; k < BUF_SIZE; ++k) {
            auto i = k / GRID_SIZE;
            auto j = k % GRID_SIZE;
            auto &c = candidates[i*GRID_SIZE+j];
            auto size = c.values.size();
            if (size > 0) {
                // std::cout <<"at ("<<i<<","<<j<<") num_candidates="<<size<< " [ ";
                // for (auto &v: c.values) {
                //     std::cout<<(int)v<< " ";
                // }
                // std::cout<<"]\n";
                c.i = i;
                c.j = j;
                candidates2.push_back(c);
            }
        }
        std::sort(
            candidates2.begin(),
            candidates2.end(),
            [](const auto &a, const auto &b) {
                return a.values.size() < b.values.size();
            });

        for (auto &c : candidates2) {
            for (auto &v : c.values) {
                Grid g = *this;
                g.set(c.i, c.j, v);
                std::vector<Candidate> cc;
                g.fill_basic_candidates(cc);
                auto new_holes = g.get_num_hole();
                c.won_holes += num_hole - new_holes;
            }
            c.won_holes -= c.values.size();
        }

        std::sort(
            candidates2.begin(),
            candidates2.end(),
            [](const auto &a, const auto &b) {
                if (a.won_holes > b.won_holes) {
                    return true;
                }
                if (a.won_holes < b.won_holes) {
                    return false;
                }
                return a.values.size() < b.values.size();
            });
        for (auto &c : candidates2) {
            // std::cout <<"at ("<<c.i<<","<<c.j<<") num_candidates="<<c.values.size()<< " [ ";
            // for (auto &v: c.values) {
            //     std::cout<<(int)v<< " ";
            // }
            // std::cout<<"] won_holes="<<c.won_holes<<"\n";
        }


        for (auto &c : candidates2) {
            for (auto &v : c.values) {
                Grid g = *this;
                g.set(c.i, c.j, v);
                // std::cout<<"try SET "<<(int)v<< " at ("<<c.i<<","<<c.j<<") at depth="<<depth<<"\n";
                std::string q = key;
                q[c.i*GRID_SIZE+c.j+c.i] = to_rep_simple(v)[0];
                if (q == key) {
                    throw std::exception();
                }

                // std::cout<<"key="<<key<<" q="<<q<<"\n";
                g.solve(already_done, q, depth+1);
                if (g.get_num_hole() == 0) {
                    *this = g;
                    return;
                }
                //g.set(c.i, c.j, 0);
            }
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
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    Grid g;
    std::unordered_map<std::string,bool> already_done;
    g.read();
    g.draw();

    char rep[100] = {0};
    g.draw_simple(rep);
    std::string key{rep};
    g.solve(already_done, key);
    g.draw();
    return 0;
}
