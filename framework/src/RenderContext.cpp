#include "RenderContext.h"
#include <sstream>
#include <iomanip>
#include <Logger.h>
#include <RenderContext.h>


void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

namespace ocls {

RenderWindow::RenderWindow(context_ptr* share_context, std::mutex* render_mutex,
                            const std::string& name_, const int& width_, const int& height_){
    m_render_mutex		 	= render_mutex;
    m_framebuffer_height 	= height_;
    m_framebuffer_width 	= width_;
    m_share_context 		= share_context->window_handle;
    m_has_update 			= false;
    m_is_ready              = false;
    m_title					= name_;

    m_create_callback_assigned = false;

    m_render_thread = std::thread(&RenderWindow::run, this);

    logger->log(Logger::DEBUG, "Render window created");
}
RenderWindow::~RenderWindow(){

    m_render_thread.join();
	if(m_window_ptr != NULL){
		glfwSetWindowShouldClose(m_window_ptr->window_handle, GL_TRUE);
	}
}
    
void RenderWindow::notify(){
    m_has_update = true;
}

volatile bool RenderWindow::ready(){
    return m_is_ready;
}

void RenderWindow::showWindow(){
    glfwShowWindow(m_window_ptr->window_handle);
}
    
void RenderWindow::hideWindow(){
    glfwHideWindow(m_window_ptr->window_handle);
}
    
    
void RenderWindow::setWindowTitle(const std::string& name_){
    glfwSetWindowTitle(m_window_ptr->window_handle,name_.c_str());
}
    
void RenderWindow::resize(const int& width_, const int& height_){
    glfwSetWindowSize (m_window_ptr->window_handle, width_, height_);
}
    
int RenderWindow::getFramebufferWidth(){
    return m_framebuffer_width;
}
int RenderWindow::getFramebufferHeight(){
    return m_framebuffer_height;
}

GLFWwindow *RenderWindow::getHandle() {
    if(m_window_ptr == NULL){
        return NULL;
    }
    return m_window_ptr->window_handle;
}

std::string RenderWindow::getTitle() {
    return m_title;
}

bool RenderWindow::hasFocus(){
    return (bool)glfwGetWindowAttrib(m_window_ptr->window_handle, GLFW_FOCUSED);
}

void RenderWindow::run(){
	m_render_mutex->lock();

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

    GLFWwindow* window_handle = glfwCreateWindow(m_framebuffer_width, m_framebuffer_height, m_title.c_str(), NULL, m_share_context);
    if (window_handle == NULL) {
        logger->log(Logger::ERROR, "Failed to create render window");
    }

    m_window_ptr = new context_ptr();
    m_window_ptr->window_handle = window_handle;
    switchContext(m_window_ptr);

    // Get the actual render size
    glfwGetFramebufferSize(m_window_ptr->window_handle, &m_framebuffer_width, &m_framebuffer_height);

    glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK) {
        logger->log(Logger::ERROR, "Failed to initialize OpenGL extensions library (GLEW)");
	}

    // Window events
    glfwSetWindowSizeCallback(window_handle, &RenderContext::windowSizeCB);
    glfwSetFramebufferSizeCallback(window_handle, &RenderContext::windowFramebufferSizeCB);

    // Input events
    glfwSetKeyCallback(window_handle, &RenderContext::inputKeyCB);
    glfwSetMouseButtonCallback(window_handle, &RenderContext::inputMouseCB);
    glfwSetCursorPosCallback(window_handle, &RenderContext::inputCursorCB);
    glfwSetScrollCallback(window_handle, &RenderContext::inputScrollCB);

    m_is_ready = true;

    while(!m_create_callback_assigned){
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    m_create_callback();
    m_render_mutex->unlock();

    while(!glfwWindowShouldClose(m_window_ptr->window_handle)){
    	glfwPollEvents();
    	if(m_has_update && m_is_ready){
    		m_render_mutex->lock();
            logger->log(Logger::DEBUG, "Rendering content to window %s", m_title.c_str());

    		switchContext(m_window_ptr);

    		//render();
            glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);
            m_repaint_callback();

    		m_has_update = false;
    		glFlush();
    		glFinish();
    		m_render_mutex->unlock();
    	}else{
    		//std::this_thread::yield();
    		std::this_thread::sleep_for(std::chrono::milliseconds(15));
    	}
    	glfwSwapBuffers(m_window_ptr->window_handle);
    }

    m_render_mutex->lock();
    switchContext(m_window_ptr);
    m_destory_callback();
    m_render_mutex->unlock();

    glfwDestroyWindow(m_window_ptr->window_handle);
    delete m_window_ptr;
    m_window_ptr = NULL;
    m_has_update = false;
}

void RenderWindow::setWindowSizeUpdateCallback(OnWindowSizeUpdate callback) {
    m_window_size_callback = callback;
}

/*void RenderWindow::setWindowSizeUpdateCallback(void (*callback)(int, int), const void *instance) {

}*/

void RenderWindow::setKeyPressCallback(OnKeyPress callback) {
    m_key_press_callback = callback;
}

void RenderWindow::setMousebuttonCallback(OnMousebutton callback) {
    m_mouse_button_callback = callback;
}

void RenderWindow::setCursorMotionCallback(OnCursorMotion callback) {
    m_cursor_motion_callback = callback;
}

void RenderWindow::setScrollEventCallback(OnScrollEvent callback) {
    m_scroll_event_callback = callback;
}

void RenderWindow::setRepaintCallback(OnRepaint callback) {
    m_repaint_callback = callback;
}

void RenderWindow::setCreateCallback(OnCreate callback) {
    m_create_callback = callback;
    m_create_callback_assigned = true;
}

void RenderWindow::setDestoryCallback(OnDestory callback) {
    m_destory_callback = callback;
}


void RenderWindow::invokeSizeChange(int width, int height) {
}

void RenderWindow::invokeFramebufferSizeChange(int width, int height) {
    glfwGetFramebufferSize(m_window_ptr->window_handle, &m_framebuffer_width, &m_framebuffer_height);

    if(m_window_size_callback){
        m_window_size_callback(width, height);
    }else{
        notify();
    }
}

void RenderWindow::invokeKeyPress(int key, int scancode, int action, int mods) {
    if(m_key_press_callback){
        m_key_press_callback(key, scancode, action, mods);
    }
}

void RenderWindow::invokeMouseButtonPress(int button, int action, int mods) {
    //logger->log(Logger::DEBUG, "Window %s registrated mouse button event on button %d", m_title.c_str(), button);
    if(m_mouse_button_callback){
        m_mouse_button_callback(button, action, mods);
    }
}

void RenderWindow::invokeCursorMotion(double x, double y) {
    //logger->log(Logger::DEBUG, "Window %s registrated mouse motion to pos [%f,%f]", m_title.c_str(), x, y);
    if(m_cursor_motion_callback){
        m_cursor_motion_callback(x, y);
    }
}

void RenderWindow::invokeScrollEvent(double xoff, double yoff) {
    //logger->log(Logger::DEBUG, "Window %s registrated mouse scroll event [%f,%f]", m_title.c_str(), xoff, yoff);
    if(m_scroll_event_callback){
        m_scroll_event_callback(xoff, yoff);
    }
}

/**
*  RENDER CONTEXT
*/

// Static definition
size_t RenderContext::m_window_capacity;
size_t RenderContext::m_window_count;
RenderWindow** RenderContext::m_windows;
//std::vector<RenderWindow*> RenderContext::m_windows;

RenderContext::RenderContext() : m_activeWindow(NULL){
    //Initialize the library
    if (glfwInit() != GL_TRUE) {
        logger->log(Logger::WARNING, "Failed to initialize window management library (GLFW)");
        m_valid_context = false;
        return;
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);


    GLFWwindow* window_handle = glfwCreateWindow(1, 1, "Context", NULL, NULL);
    if (window_handle == NULL) {
        logger->log(Logger::WARNING, "Failed to create an OpenGL context with compatible profile");

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window_handle = glfwCreateWindow(1, 1, "Context", NULL, NULL);

        if (window_handle == NULL) {
            logger->log(Logger::WARNING, "Failed to create an OpenGL context with core profile");
            m_valid_context = false;
            return;
        }
    }
    m_valid_context = true;

    m_context = new context_ptr();
    m_context->window_handle = window_handle;

    //Make the window's context current
    switchContext(m_context);

    glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK) {
        logger->log(Logger::ERROR, "Failed to initialize OpenGL extensions library (GLEW)");
	}

	// Unfortunately glewInit generates an OpenGL error, but does what it's
	// supposed to (setting function pointers for core functionality).
	// Lets do the ugly thing of swallowing the error....
	glGetError();

    logger->log(Logger::DEBUG, "OpenGL context created");

    m_window_capacity = 5;
    m_window_count = 0;
    m_windows = (RenderWindow**)malloc(m_window_capacity*sizeof(RenderWindow*));
}

RenderContext::~RenderContext(){
    logger->log(Logger::DEBUG, "Destroying render context");
	for (size_t i = 0; i < m_window_count; ++ i) {
		delete m_windows[i];
        m_windows[i] = NULL;
	}

    if (m_valid_context) {
        glfwDestroyWindow(m_context->window_handle);
        delete m_context;
        m_context = NULL;
    }
    glfwTerminate();
}
    
RenderWindow* RenderContext::createWindow(const std::string& name_, const int& width_, const int& height_){
	if (!m_valid_context){
        logger->log(Logger::WARNING, "Unable to create window, no valid render context.");
        return NULL;
    }

    RenderWindow* window = new RenderWindow(m_context, &m_render_mutex, name_, width_, height_);

    if(m_window_count >= m_window_capacity){
        RenderWindow** tmp = m_windows;
        m_window_capacity+=5;
        m_windows = (RenderWindow**)malloc(m_window_capacity*sizeof(RenderWindow*));
        memcpy(m_windows[0], tmp[0], m_window_count*sizeof(RenderWindow*));
    }
    m_windows[m_window_count] = window;
    m_window_count++;


    while (!window->ready()){};
	return window;
}

void RenderContext::requestRepaint(RenderWindow *window) {
    // Wait if the window is not yet initialized
    while (!window->ready()){};

    if(window->hasUpdatesPending()) {
        return;
    }

    m_render_mutex.lock();
    window->notify();
    m_render_mutex.unlock();

    // Block only if called from another thread then the render context itself.
    if(std::this_thread::get_id() != window->getRenderThreadID()) {
        while (window->hasUpdatesPending()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }
}

void RenderContext::requestRepaintDirty(RenderWindow *window) {
    // Wait if the window is not yet initialized
    while (!window->ready()){};
    window->notify();
}

context_ptr* RenderContext::getContext(){
    return m_context;
}

bool RenderContext::isContextValid(){
    return m_valid_context;
}

void RenderContext::windowSizeCB(GLFWwindow *window, int width, int height) {
    for (int i = 0; i < m_window_count; ++i) {
        if(m_windows != NULL && m_windows[i] != NULL && m_windows[i]->getHandle() == window){
            m_windows[i]->invokeSizeChange(width, height);
            break;
        }
    }
}


void RenderContext::windowFramebufferSizeCB(GLFWwindow *window, int width, int height) {
    for (int i = 0; i < m_window_count; ++i) {
        if(m_windows != NULL && m_windows[i] != NULL && m_windows[i]->getHandle() == window){
            m_windows[i]->invokeFramebufferSizeChange(width, height);
            break;
        }
    }
}

void RenderContext::inputKeyCB(GLFWwindow *window, int key, int scancode, int action, int mods) {
    for (int i = 0; i < m_window_count; ++i) {
        if(m_windows != NULL && m_windows[i] != NULL && m_windows[i]->getHandle() == window){
            m_windows[i]->invokeKeyPress(key, scancode, action, mods);
            break;
        }
    }
}

void RenderContext::inputMouseCB(GLFWwindow *window, int button, int action, int mods) {
    for (int i = 0; i < m_window_count; ++i) {
        if(m_windows != NULL && m_windows[i] != NULL && m_windows[i]->getHandle() == window){
            m_windows[i]->invokeMouseButtonPress(button, action, mods);
            break;
        }
    }
}

void RenderContext::inputCursorCB(GLFWwindow *window, double x, double y) {
    for (int i = 0; i < m_window_count; ++i) {
        if(m_windows != NULL && m_windows[i] != NULL && m_windows[i]->getHandle() == window){
            m_windows[i]->invokeCursorMotion(x, y);
            break;
        }
    }
}

void RenderContext::inputScrollCB(GLFWwindow *window, double xoff, double yoff) {
    for (int i = 0; i < m_window_count; ++i) {
        if(m_windows != NULL && m_windows[i] != NULL && m_windows[i]->getHandle() == window){
            m_windows[i]->invokeScrollEvent(xoff, yoff);
            break;
        }
    }
}

volatile bool RenderWindow::hasUpdatesPending() {
    return m_has_update;
}

std::thread::id RenderWindow::getRenderThreadID() {
    return m_render_thread.get_id();
}
}
