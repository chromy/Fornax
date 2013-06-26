// Various included headers
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>

// Various macro definitions
#define WIDTH   800
#define HEIGHT  600
#define URL     "http://www.google.com"

using namespace Awesomium;

int main(int argc, const char * argv[]) {
    // Create the WebCore singleton with default configuration
    WebCore* web_core = WebCore::Initialize(WebConfig());

    /*
    // Create a new WebView instance with a certain width and height
    WebView* view = web_core->CreateWebView(WIDTH, HEIGHT);

    // Load a certain URL into our WebView instance
    WebURL url(WSLit(URL));
    view->LoadURL(url);

    // Wait for our WebView to finish loading
    while (view->IsLoading()) {
        web_core->Update();
    }

    // Sleep a bit and update once more to give scripts and plugins
    // on the page a chance to finish loading.
    sleep(300);
    web_core->Update();

    // Get the WebView's rendering Surface. The default Surface is of
    // type 'BitmapSurface', we must cast it before we can use it.
    BitmapSurface* surface = (BitmapSurface*)view->surface();

    // Make sure our surface is not NULL-- it may be NULL if the WebView
    // process has crashed.
    if (surface != 0) {
      // Save our BitmapSurface to a JPEG image in the current
      // working directory.
      surface->SaveToJPEG(WSLit("./result.jpg"));
    }

    view->Destroy();
    */
    WebCore::Shutdown();
}

