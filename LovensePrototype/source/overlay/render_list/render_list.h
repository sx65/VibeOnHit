// nord-external forked

#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include <imgui/imgui.h>

namespace render {
    struct base {
        virtual void render( ImDrawList* draw_list ) = 0;
        virtual ~base() = default;
    };

    struct rect : public base {
        rect( ImVec2 p1, ImVec2 p2, ImColor col, bool filled = false, float thickness = 1.0f )
            : p1{ p1 },
            p2{ p2 },
            col{ col },
            filled{ filled },
            thickness{ thickness }
        {
        }

        ImVec2 p1, p2;
        ImColor col;
        bool filled;
        float thickness;

        void render( ImDrawList* draw_list ) override;
    };

    struct glowing_rect : public base {
        glowing_rect( ImVec2 p1, ImVec2 p2, ImColor col, float glow_size = 1.0f, bool filled = false )
            : p1{ p1 },
            p2{ p2 },
            col{ col },
            glow_size{ glow_size },
            filled{ filled }
        {
        }

        ImVec2 p1, p2;
        ImColor col;
        float glow_size;
        bool filled;

        void render( ImDrawList* draw_list ) override;
    };

    struct line : public base {
        line( ImVec2 p1, ImVec2 p2, ImColor col, float thickness = 1.0f )
            : p1{ p1 },
            p2{ p2 },
            col{ col },
            thickness{ thickness } 
        {
        }

        ImVec2 p1, p2;
        ImColor col;
        float thickness;

        void render( ImDrawList* draw_list ) override;
    };

    struct text : public base {
        text( ImVec2 p, float size, ImColor col, const std::string& text, bool center )
            : p{ p },
            size{ size },
            col{ col },
            val{ text },
            center{ center } 
        {
        }

        ImVec2 p;
        float size;
        ImColor col;
        std::string val;
        bool center;

        void render( ImDrawList* draw_list ) override;
    };

    struct circle : public base {
        circle( ImVec2 p, float rad, ImColor col, float thickness = 1.0f ) 
            : p{ p },
            rad{ rad }, 
            col{ col }, 
            thickness{ thickness } 
        {
        }

        ImVec2 p;
        float rad;
        ImColor col;
        float thickness;

        void render( ImDrawList* draw_list ) override;
    };

    struct triangle : public base {
        triangle( ImVec2 p1, ImVec2 p2, ImVec2 p3, ImColor col, float thickness = 1.0f, bool filled = false )
            : p1{ p1 },
            p2{ p2 },
            p3{ p3 },
            col{ col },
            thickness{ thickness },
            filled{ filled }
        {
        }

        ImVec2 p1, p2, p3;
        ImColor col;
        float thickness;
        bool filled;

        void render( ImDrawList* draw_list ) override;
    };

    class render_list {
    public:
        template< typename T, typename... Args >
        void add( const Args&... args ) {
            temp.emplace_back( new T( args... ) );
        }

        void clear( void );
        void draw( ImDrawList* draw_list );
        void update( void ) noexcept;

    private:
        std::vector<std::unique_ptr<base>> temp;
        std::vector<std::unique_ptr<base>> list;

        std::mutex mutex;
    };
}
