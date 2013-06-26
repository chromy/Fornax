#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <GL/glfw.h>
#include <Awesomium/WebView.h>

namespace UserInterface {

    void initialise(void);
    void update(void);
    void finalise(void);
    void map_files();

    class View {
        public:
            View(int width, int height);
            virtual ~View();
            int width(void);
            int height(void);
            void resize(int width, int height);
            void mouse_move(int width, int height);
            void mouse_left_click(void);
            void mouse_button_up(int button);
            void mouse_button_down(int button);
            void mouse_wheel(int pos);

            GLuint get_texture(void);
        private:
            GLuint texture;
            Awesomium::WebView* web_view;
    };
}

#endif

