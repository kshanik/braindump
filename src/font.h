#ifndef _FONT_H
#define _FONT_H

typedef struct {
    unsigned char  c;
    unsigned short x;
    unsigned short y;
    unsigned short width;
    unsigned short height;
    short x_offset;
    short y_offset;
    unsigned x_advance;
} glyph;

std::ostream& operator << (std::ostream& o, const glyph& a)
{
    o << "c = " << (unsigned short) a.c << " "
    << "x = " << a.x << " "
    << "y = " << a.y << " "
    << "width = " << a.width << " "
    << "height = " << a.height << " "
    << "x_offset = " << a.x_offset << " "
    << "y_offset = " << a.y_offset << " "
    << "x_advance = " << a.x_advance << std::endl;
    return o;
}

typedef struct {
    std::map<unsigned short, glyph> glyphs;
} font_data;

font_data g_font_data;

void load_font_file(std::string file_path);


#ifdef _FONT_IMPLEMENTATION_

void skip_n_character(std::string::iterator &it, int count)
{
    int i = 0;
    while (i < count) {
        i++;
        ++it;
    }
}

void skip_space(std::string::iterator &it)
{
    while (*it == ' ') it++;
}

int get_number(std::string::iterator &it)
{
    std::string value;
    while (*it != ' ') {
        value += *it;
        it++;
    }
    return std::stoi(value);
}

void load_font_file(std::string file_path)
{
    std::ifstream file;
    std::string line;
    file.open(file_path);
    std::flush(std::cout);
    while (file) {
        glyph g;
        getline(file, line);
        std::string::iterator it = line.begin();
        skip_n_character(it, strlen("char id="));
        g.c = get_number(it);

        skip_space(it);
        skip_n_character(it, strlen("x="));
        g.x = get_number(it);

        skip_space(it);
        skip_n_character(it, strlen("y="));
        g.y = get_number(it);

        skip_space(it);
        skip_n_character(it, strlen("width="));
        g.width = get_number(it);


        skip_space(it);
        skip_n_character(it, strlen("height="));
        g.height = get_number(it);

        skip_space(it);
        skip_n_character(it, strlen("xoffset="));
        g.x_offset = get_number(it);

        skip_space(it);
        skip_n_character(it, strlen("yoffset="));
        g.y_offset = get_number(it);

        skip_space(it);
        skip_n_character(it, strlen("xadvance="));
        g.x_advance = get_number(it);

        std::cout << g;
        g_font_data.glyphs[g.c] = g;
    }
}

#endif //_FONT_IMPLEMENTATION_

#endif // _FONT_H
