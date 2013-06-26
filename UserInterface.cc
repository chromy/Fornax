#include <Awesomium/WebCore.h>
#include <Awesomium/DataSource.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/BitmapSurface.h>

#include "UserInterface.hh"

using namespace Awesomium;

namespace UserInterface {

// Helpers
char* get_file_contents(const char* file_name);
MouseButton glfw_to_awesomium(int button);

static WebCore *web_core = NULL;
static WebSession *web_session = NULL;

class DiskDataSource : public DataSource {
    public:
        DiskDataSource() {}
        virtual ~DiskDataSource() {}
        void OnRequest(int request_id, const WebString& path);
  //      void OnRequest(int request_id, const WebString& path);
};

void DiskDataSource::OnRequest(int request_id, const WebString& path) {
    if (path == WSLit("index.html")) {
        char* html_str = get_file_contents("index.html");
        SendResponse(request_id, strlen(html_str), (unsigned char *)html_str, WSLit("text/html"));
        free((void*)html_str);
    }
}

void initialise(void) {
    // Create
    WebConfig web_config = WebConfig();
    web_config.remote_debugging_port = 1337;
    web_core = WebCore::Initialize(web_config);

    //
    web_session = web_core->CreateWebSession(WSLit(""), WebPreferences());

    //
    DataSource* data_source = new DiskDataSource();
    web_session->AddDataSource(WSLit("MyApplication"), data_source);
}

void update(void) {
    // more
    web_core->Update();
}

void finalise(void) {
    // more
    WebCore::Shutdown();
}

View::View(int width, int height) {
    web_view = web_core->CreateWebView(width, height, web_session);
    web_view->SetTransparent(true);
    web_view->LoadURL(WebURL(WSLit("asset://MyApplication/index.html")));
    // TODO: fix this.
    while (web_view->IsLoading()) web_core->Update();
    glGenTextures(1, &texture);
}

View::~View() {
    glDeleteTextures(1, &texture);
}

int View::width(void) {
    BitmapSurface* sur = (BitmapSurface*)web_view->surface();
    if (sur == NULL) return 1;
    return sur->width();
}

int View::height(void) {
    BitmapSurface* sur = (BitmapSurface*)web_view->surface();
    if (sur == NULL) return 1;
    return sur->height();
}

void View::resize(int width, int height) {
    web_view->Resize(width, height);
}

void View::mouse_button_down(int button) {
    MouseButton web_button = glfw_to_awesomium(button);
    web_view->InjectMouseDown(web_button);
}

void View::mouse_button_up(int button) {
    MouseButton web_button = glfw_to_awesomium(button);
    web_view->InjectMouseUp(web_button);
}

void View::mouse_move(int x, int y) {
    web_view->InjectMouseMove(x, y);
}

void View::mouse_wheel(int pos) {
    web_view->InjectMouseWheel(pos, 0);
}

GLuint View::get_texture(void) {
    BitmapSurface* surface = (BitmapSurface*)web_view->surface();
    if (surface == NULL) return texture;

    int width = surface->width();
    int height = surface->height();

    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->buffer());
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    surface->set_is_dirty(false);
    return texture;
}

MouseButton glfw_to_awesomium(int button) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        return kMouseButton_Left;
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        return kMouseButton_Middle;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        return kMouseButton_Right;
    }
    return kMouseButton_Left;

}

char* get_file_contents(const char* file_name) {
    FILE *fp;
    size_t file_size;
    char *buffer;

    fp = fopen(file_name, "rb");
    if (!fp) {
        perror(file_name);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    // allocate memory for entire content
    buffer = (char*)calloc(1, file_size+1);
    if (!buffer) {
        fclose(fp);
        fputs("memory alloc fails", stderr);
        exit(1);
    }

    // copy the file into the buffer
    if (1 != fread(buffer, file_size, 1, fp)) {
        fclose(fp);
        free(buffer);
        fputs("entire read fails", stderr);
        exit(1);
    }

    //
    fclose(fp);
    return buffer;
}
}
