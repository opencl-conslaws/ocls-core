#ifndef SURFACE_PLOT_H
#define SURFACE_PLOT_H

#include "RenderContext.h"
#include "Data.h"
#include "GLUtils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include "VirtualTrackball.h"

#include "Plot.h"

class SurfacePlot : public Plot{

public:
    struct TriMesh{
        glm::ivec2 size;
        size_t count;
        size_t restart_token;
        GLUtils::BO<GL_ARRAY_BUFFER>* vertices;
        GLUtils::BO<GL_ELEMENT_ARRAY_BUFFER>* indices;
        GLuint vao;
    };

    SurfacePlot(ocls::Module* module, ocls::RenderContext* context, std::string name, ocls::Data* data, ocls::Domain* domain);
    ~SurfacePlot();

    void update(ocls::Data* data);

private:
    void windowResized(int width,int height);
    void cursorMotion(double x, double y);
    void mouseButton(int button, int action, int mods);
    void keypress(int key, int scancode, int action, int mods);

    void repaint();
    void init();
    void clean();

    TriMesh* createMesh(unsigned int Nx, unsigned int Ny);

private:
    float scale;

    TriMesh* m_mesh;
    GLUtils::Program* m_shader;

    glm::mat4 m_model;
    struct {
        glm::mat4 projection;
        glm::mat4 view;
    } m_camera;

    VirtualTrackball m_trackball;
    int m_pos[2];
};

#endif
