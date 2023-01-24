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
    size_t won_slots;

    Candidate():
        i{0},
        j{0},
        won_slots{0}
    {
    }
};


struct Grid {
    int8_t m_grid[BUF_SIZE];
    uint16_t m_candidates[BUF_SIZE];

    Grid()
    {
        std::memset(m_grid, 0, sizeof(m_grid));
        std::memset(m_candidates, 0, sizeof(m_candidates));
    }

    uint8_t at(int i, int j) const { return m_grid[i*GRID_SIZE+j]; }
    void set(int i, int j, int8_t val) { m_grid[i*GRID_SIZE+j] = val; }

    uint16_t candidate_at(int i, int j) const { return m_candidates[i*GRID_SIZE+j]; }
    void candidate_setfull(int i, int j, int16_t val) { m_candidates[i*GRID_SIZE+j] = 0x1FF; }
    void candidate_remove(int i, int j, int8_t val) { m_candidates[i*GRID_SIZE+j] &= ~(1 << (val-1)); }
    bool has_candidate(int i, int j, int8_t val) const { return (m_candidates[i*GRID_SIZE+j] & (1 << (val-1))) != 0; }

    void read();
    void draw() const;
    void draw_simple() const;
    void draw_simple(char *) const;
    void basic_fill();
    bool fill_basic_candidates(std::vector<Candidate> &candidates, size_t& num_slot);

    void solve(std::unordered_map<std::string,bool> &already_done, std::string &key, int depth=0);
    size_t get_num_slot();

    bool verify_grid() const;

};

void Grid::read()
{
    int i = 0;
    int num_space = 0;

    while (!std::cin.eof())
    {
        char c = std::cin.get();
        if ('1' <= c && c <= '9') {
            int val = c - '0';
            m_grid[i++] = val;
        } else if (c == ' ') {
            ++num_space;
            if (num_space == 3) {
                m_grid[i++] = 0;
                num_space = 0;
            }
        } else if (c == '-') {
            m_grid[i++] = 0;
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
            m_grid[i*GRID_SIZE+j] = (i+j)%GRID_SIZE + 1;
        }
    }
}


size_t Grid::get_num_slot()
{
    size_t num_slot = 0;
    for (auto i = 0; i < GRID_SIZE; ++i)
    {
        for (auto j = 0; j < GRID_SIZE; ++j)
        {
            num_slot += at(i, j) == 0;
        }
    }
    return num_slot;
}

bool Grid::fill_basic_candidates(std::vector<Candidate> &candidates, size_t& num_slot)
{
    // fill basic_candidates
    for (auto k = 0; k < BUF_SIZE; ++k)
    {
        int i = k / GRID_SIZE;
        int j = k % GRID_SIZE;
        if (at(i, j) != 0) {
            // not a slot
            continue;
        }

        // fill all candidates
        Candidate& c = candidates[i * GRID_SIZE + j];
        for (int8_t v = 1; v < GRID_SIZE + 1; ++v) {
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
                auto val = at(3 * IS + is, 3 * JS + js);
                if (val == 0) continue;
                // already in same square
                c.values.erase(val);
            }
        }

        auto vsize = c.values.size();
        if (vsize == 0) {
            // std::cout<<"unsolvable!\n";
            //already_done[key] = true;
            return false;
        }

        if (vsize == 1) {
            auto val = *c.values.begin();
            c.values.clear();
            --num_slot;
            set(i, j, val);
            if (num_slot == 0) {
                return true;
            }
            // propagate back to row i
            for (int m = 0; m < GRID_SIZE; ++m)
            {
                Candidate& d = candidates[i * GRID_SIZE + m];
                d.values.erase(val);
            }
            // propagate back to col j
            for (int m = 0; m < GRID_SIZE; ++m)
            {
                Candidate& d = candidates[m * GRID_SIZE + j];
                d.values.erase(val);
            }
            // propagate back to square of ij
            auto IS = i / 3;
            auto JS = j / 3;
            for (int is = 0; is < 3; ++is) {
                for (int js = 0; js < 3; ++js) {
                    Candidate& d = candidates[is * GRID_SIZE + js];
                    d.values.erase(val);
                }
            }
        }
    }
    return true;
}

void Grid::solve(
    std::unordered_map<std::string,bool> &already_done,
    std::string &key,
    int depth)
{
    auto it = already_done.find(key);
    if (it != already_done.end()) {
        return;
    }

    size_t previous_num_slot = 0;
    size_t num_slot = get_num_slot();
    if (num_slot == 0) {
        return;
    }
    std::vector<Candidate> candidates;
    candidates.resize(BUF_SIZE);

    while (num_slot != previous_num_slot)
    {
        previous_num_slot = num_slot;
        if (!fill_basic_candidates(candidates, num_slot)) {
            already_done[key] = true;
            return;
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
            if (num_appearing == 1) {
                set(i, pos, v);
                candidates[i*GRID_SIZE+pos].values.clear();
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
            if (num_appearing == 1) {
                set(pos, i, v);
                candidates[pos*GRID_SIZE+i].values.clear();
                //++num_found_with_single;
                break;
            }
        }
    }
    num_slot = get_num_slot();
    if (num_slot == 0) {
        std::cout<<"solved!\n";
        return;
    }
    // basic heuristic not enough
    // try to to perform arbitrary filling and backtrack if not working
    std::vector<Candidate> candidates2;
    for (auto k = 0; k < BUF_SIZE; ++k) {
        auto i = k / GRID_SIZE;
        auto j = k % GRID_SIZE;
        auto &c = candidates[i*GRID_SIZE+j];
        auto size = c.values.size();
        if (size > 0) {
            c.i = i;
            c.j = j;
            candidates2.push_back(c);
        }
    }

    for (auto &c : candidates2) {
        for (auto &v : c.values) {
            size_t new_slots = num_slot;
            Grid g = *this;
            g.set(c.i, c.j, v);
            std::vector<Candidate> cc{ BUF_SIZE };
            g.fill_basic_candidates(cc, new_slots);
            c.won_slots += num_slot - new_slots;
        }
        c.won_slots -= c.values.size();
    }

    std::sort(
        candidates2.begin(),
        candidates2.end(),
        [](const auto &a, const auto &b) {
            if (a.won_slots > b.won_slots) {
                return true;
            }
            if (a.won_slots < b.won_slots) {
                return false;
            }
            return a.values.size() < b.values.size();
        });

    for (auto &c : candidates2)
    {
        for (auto &v : c.values)
        {
            Grid g = *this;
            g.set(c.i, c.j, v);
            std::string q = key;
            q[c.i*GRID_SIZE+c.j+c.i] = to_rep_simple(v)[0];
            if (q == key) {
                throw std::exception();
            }
            g.solve(already_done, q, depth+1);
            if (g.get_num_slot() == 0) {
                *this = g;
                return;
            }
        }
    }
}

bool Grid::verify_grid() const
{
    for (int k = 0; k < GRID_SIZE; ++k)
    {
        // row k
        for (int v = 1; v < GRID_SIZE + 1; ++v)
        {
            int count = 0;

            for (int m = 0; m < GRID_SIZE; ++m)
            {
                count += at(k, m) == v;
            }
            if (count > 1) {
                return false;
            }
        }
        // col k
        for (int v = 1; v < GRID_SIZE + 1; ++v)
        {
            int count = 0;

            for (int m = 0; m < GRID_SIZE; ++m)
            {
                count += at(m, k) == v;
            }
            if (count > 1) {
                return false;
            }
        }
    }

    for (int I = 0; I < GRID_SIZE; I+=3)
    {
        for (int J = 0; J < GRID_SIZE; J+=3)
        {
            for (int v = 1; v < GRID_SIZE + 1; ++v)
            {
                int count = 0;
                for (int i = I; i < I + 3; ++i)
                {
                    for (int j = J; j < J + 3; ++j)
                    {
                        count += at(i, j) == v;
                    }
                }
                if (count > 1)
                {
                    return false;
                }
            }
        }
    }
    return true;
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
    if (g.verify_grid()) {
        std::cout << "valid!\n";
    }
    else {
        std::cout << "invalid!\n";
        return 1;
    }

    char rep[100] = {0};
    g.draw_simple(rep);
    std::string key{rep};
    g.solve(already_done, key);
    g.draw();
    if (g.verify_grid()) {
        std::cout << "valid!\n";
    }
    else {
        std::cout << "invalid!\n";
    }
    return 0;
}
