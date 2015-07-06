/**
  *  This file is part of the OpenCL-ConsLaws framework
  *  Copyright (C) 2014, 2015 Jens Kristoffer Reitan Markussen
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  */


#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#ifndef GLEW_MX
#define GLEW_MX
#endif

#include <GL/glew.h>
#if defined (WIN32)
#include <GL/wglew.h>
#elif defined (__linux__)
#include <GL/glx.h> // < I hope this works
//#include <GL/glxew.h> // < Because this didn't work
#elif defined (__APPLE__)
#include <OpenGL/OpenGL.h>
#endif
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include <thread>
#include <mutex>
#include <iostream>
#include "Logger.h"

typedef struct {
	GLFWwindow* window_handle;
	GLEWContext context_handle;
} context_ptr;

static context_ptr* current_context;
#define glewGetContext() &current_context->context_handle

static void switchContext(context_ptr* context){
	if(current_context != context){
		current_context = context;
		glfwMakeContextCurrent(context->window_handle);
	}
}

namespace ocls {

typedef std::function<void(int, int)> OnWindowSizeUpdate;
typedef std::function<void(int, int, int, int)> OnKeyPress;
typedef std::function<void(int, int, int )> OnMousebutton;
typedef std::function<void(double, double)> OnCursorMotion;
typedef std::function<void(double, double)> OnScrollEvent;

typedef std::function<void()> OnRepaint;
typedef std::function<void()> OnCreate;
typedef std::function<void()> OnDestory;

using namespace std::placeholders;

class RenderWindow {
public:
    RenderWindow(context_ptr* share_context, std::mutex* render_mutex,
                 const std::string& name_, const int& width_, const int& height_);
    ~RenderWindow();
    
    /**
     * Notify this window that it has been updated.
     */
    void notify();

    /**
    * Check if the window is ready for processing
    */
    volatile bool ready();
    
    /**
     * Makes the window appear if hidden.
     */
    void showWindow();
    
    /**
     * Hides the window if visible
     */
    void hideWindow();
    
    /**
     * Set the title of window
     */
    void setWindowTitle(const std::string& name_);
    
    /**
     * Sets the size of the window
     */
    void resize(const int& width_, const int& height_);
    
    /**
     * Returns the size of the framebuffer
     */
    int getFramebufferWidth();
    
    /**
     * Returns the size of the framebuffer
     */
    int getFramebufferHeight();

    /**
    * Return the window handle
    */
    GLFWwindow* getHandle();

    /**
    * Returns the window title
    */
    std::string getTitle();

    /**
    * Check if this window has focus
    */
    bool hasFocus();

    /**
    * Check if the window has any pending updates
    */
    volatile bool hasUpdatesPending();

    /**
    * Return the ID of the render thread
    */
    std::thread::id getRenderThreadID();

    /**
    * Set callbacks
    */
    void setWindowSizeUpdateCallback(OnWindowSizeUpdate callback);
    void setKeyPressCallback(OnKeyPress callback);
    void setMousebuttonCallback(OnMousebutton callback);
    void setCursorMotionCallback(OnCursorMotion callback);
    void setScrollEventCallback(OnScrollEvent callback);

    void setRepaintCallback(OnRepaint callback);
    void setCreateCallback(OnCreate callback);
    void setDestoryCallback(OnDestory callback);

    template<typename T>
    void setWindowSizeUpdateCallback(void (T::*callback)(int, int), T* calle){
        setWindowSizeUpdateCallback(std::bind(callback, calle, _1, _2));
    }
    template<typename T>
    void setKeyPressCallback(void (T::*callback)(int, int, int, int), T* calle){
        setKeyPressCallback(std::bind(callback, calle, _1, _2, _3, _4));
    }
    template<typename T>
    void setMousebuttonCallback(void (T::*callback)(int, int, int), T* calle){
        setMousebuttonCallback(std::bind(callback, calle, _1, _2, _3));
    }
    template<typename T>
    void setCursorMotionCallback(void (T::*callback)(double, double), T* calle){
        setCursorMotionCallback(std::bind(callback, calle, _1, _2));
    }
    template<typename T>
    void setScrollEventCallback(void (T::*callback)(double, double), T* calle){
        setScrollEventCallback(std::bind(callback, calle, _1, _2));
    }

    template<typename T>
    void setRepaintCallback(void (T::*callback)(), T* calle){
        setRepaintCallback(std::bind(callback, calle));
    }
    template<typename T>
    void setCreateCallback(void (T::*callback)(), T* calle){
        setCreateCallback(std::bind(callback, calle));
    }
    template<typename T>
    void setDestoryCallback(void (T::*callback)(), T* calle){
        setDestoryCallback(std::bind(callback, calle));
    }

    /**
    * Invokes called from Render context
    */
    void invokeSizeChange(int width, int height);
    void invokeFramebufferSizeChange(int width, int height);
    void invokeKeyPress(int key, int scancode, int action, int mods);
    void invokeMouseButtonPress(int button, int action, int mods);
    void invokeCursorMotion(double x, double y);
    void invokeScrollEvent(double xoff, double yoff);
    
private:
    /**
     * Window thread entry point, keeping the window alive
     */
    void run();

private:
    std::thread m_render_thread;
    std::mutex* m_render_mutex;
    
    int m_framebuffer_width;
    int m_framebuffer_height;
    
    std::string m_title;

    volatile bool m_has_update;
    volatile bool m_is_ready;
    volatile bool m_create_callback_assigned;
    
    context_ptr* m_window_ptr;
    GLFWwindow* m_share_context;

    // callbacks
    OnWindowSizeUpdate m_window_size_callback;
    OnKeyPress m_key_press_callback;
    OnMousebutton m_mouse_button_callback;
    OnCursorMotion m_cursor_motion_callback;
    OnScrollEvent m_scroll_event_callback;

    OnRepaint m_repaint_callback;
    OnCreate m_create_callback;
    OnDestory m_destory_callback;
};
    
/**
 * OpenGL context and window that can directly access OpenCL device data
 * for accelerated visualization.
 */
class RenderContext{
public:
    /**
     * Construct the render context
     */
    RenderContext();
    
    /**
     * Tear down the render context
     */
    ~RenderContext();
    
    /**
     * Create a new window
     */
    RenderWindow* createWindow(const std::string& name_, const int& width_, const int& height_);

    /**
    * Request repaint for window
    */
    void requestRepaint(RenderWindow* window);

    /**
    * Request repaint for window
    * No locks, very dirty
    */
    void requestRepaintDirty(RenderWindow* window);

    /**
     * Get the context pointer
     */
    context_ptr* getContext();

    /**
     *
     */
    bool isContextValid();

    /**
    * GLFW callback functions
    */
    static void windowSizeCB(GLFWwindow* window, int width, int height);
    static void windowFramebufferSizeCB(GLFWwindow* window, int width, int height);
    static void inputKeyCB(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void inputMouseCB(GLFWwindow* window, int button, int action, int mods);
    static void inputCursorCB(GLFWwindow* window, double x, double y);
    static void inputScrollCB(GLFWwindow* window, double xoff, double yoff);

private:
    context_ptr* m_context;
    std::mutex m_render_mutex;
    bool m_valid_context;

    static size_t m_window_count;
    static size_t m_window_capacity;
    static RenderWindow** m_windows;
    //static std::vector<RenderWindow*> m_windows;

    RenderWindow* m_activeWindow;
};

} //cls

#endif //RENDER_CONTEXT_H
