
#include "SurfacePlot.h"
#include "Shaders.h"
#include "SimplePlotModule.h"

SurfacePlot::SurfacePlot(ocls::Module* module, ocls::RenderContext* context, std::string name, ocls::Data *data, ocls::Domain *domain) :
        Plot(module, context, name, data, domain){
    if(domain->getDimensions() != 2){
        throw std::runtime_error("Surface plot only supports 2D data");
    }

    m_context = context;
    m_shader = NULL;
    m_mesh = NULL;

    m_window = m_context->createWindow(name, 800, 600);
    if(m_window == NULL){
        return;
    }

    m_trackball.setWindowSize(800, 600);

    m_window->setWindowSizeUpdateCallback(&SurfacePlot::windowResized, this);
    m_window->setCursorMotionCallback(&SurfacePlot::cursorMotion, this);
    m_window->setMousebuttonCallback(&SurfacePlot::mouseButton, this);
    m_window->setRepaintCallback(&SurfacePlot::repaint, this);
    m_window->setCreateCallback(&SurfacePlot::init, this);
    m_window->setDestoryCallback(&SurfacePlot::clean, this);
    m_window->setKeyPressCallback(&SurfacePlot::keypress, this);

    m_camera.projection = (glm::mat4)glm::perspective(45.0f,
            m_window->getFramebufferWidth() / (float) m_window->getFramebufferHeight(), 1.0f, 50.0f);
    m_camera.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.8f));

    m_domain = domain;
    update(data);
}

SurfacePlot::~SurfacePlot() {

}

void SurfacePlot::update(ocls::Data *data) {
    if(m_closed){
        return;
    }

    if(data != NULL) {
        m_data = data;
    }
    m_context->requestRepaint(m_window);
}

SurfacePlot::TriMesh *SurfacePlot::createMesh(unsigned int Nx, unsigned int Ny) {
    TriMesh* mesh = new TriMesh();

    float dx = 1.0f/static_cast<float>(Nx);
    float dy = 1.0f/static_cast<float>(Ny);

    std::vector<GLfloat>    vertices;
    std::vector<GLuint>     indices;

    //Vertices
    vertices.reserve((Nx+1)*(Ny+1));
    for (unsigned int j=0; j<=Ny; ++j) {
        for (unsigned int i=0; i<=Nx; ++i) {
            vertices.push_back(i*dx);	//x
            vertices.push_back(j*dy);	//y
        }
    }
    mesh->vertices = new GLUtils::BO<GL_ARRAY_BUFFER>(vertices.data(),sizeof(GLfloat)*(GLuint)vertices.size());

    mesh->restart_token = Nx * Ny * 2;

    //Indices
    for (unsigned int j=0; j<Ny; ++j) {
        for (unsigned int i=0; i<=Nx; ++i) {
            indices.push_back(    j*(Nx+1)+i);
            indices.push_back((j+1)*(Nx+1)+i);
        }
        indices.push_back(mesh->restart_token);
    }
    mesh->count = (GLuint)indices.size();
    mesh->indices = new GLUtils::BO<GL_ELEMENT_ARRAY_BUFFER>(indices.data(),sizeof(GLfloat)*(GLuint)indices.size());

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    m_shader->use();
    mesh->vertices->bind();
    m_shader->setAttributePointer("position", 2);
    mesh->indices->bind();
    glBindVertexArray(0);

    return mesh;
}

void SurfacePlot::windowResized(int width, int height) {
    m_trackball.setWindowSize(width, height);
    m_camera.projection = (glm::mat4)glm::perspective(45.0f,
            width / (float) height, 1.0f, 50.0f);
    //update(m_data);
    m_context->requestRepaint(m_window);
}

void SurfacePlot::cursorMotion(double x, double y) {
    m_pos[0] = (int)x;
    m_pos[1] = (int)y;
    m_trackball.rotate(m_pos[0], m_pos[1]);
    if(m_trackball.isRotating())
        m_context->requestRepaint(m_window);
        //update(m_data);
}

void SurfacePlot::mouseButton(int button, int action, int mods) {
    if(action > 0){
        m_trackball.rotateBegin(m_pos[0], m_pos[1]);
    }else{
        m_trackball.rotateEnd(m_pos[0], m_pos[1]);
    }
    m_context->requestRepaint(m_window);
    //update(m_data);
}

void SurfacePlot::repaint() {
    glClearColor(1.0f,1.0f,1.0f,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->use();

    glUniform1i(m_shader->getUniform("tex"),0);

    float minmax[2];// = {-25.1f,25.0f};
    m_data->minmax(&minmax[0]);

    float tmp = std::fabs(minmax[1])+std::fabs(minmax[0]);
    scale = tmp != 0.0f ? 1.0f/tmp : 1.0f;
    float center = ((minmax[1]+minmax[0])*scale)*0.5f;
    m_model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -center,-0.5f));

    glUniformMatrix4fv(m_shader->getUniform("projection_matrix"), 1, 0, glm::value_ptr(m_camera.projection));
    glUniformMatrix4fv(m_shader->getUniform("model_matrix"), 1, 0, glm::value_ptr(m_model));
    if (syncing) {
        if (m_window->hasFocus()) {
            transform = m_trackball.getTransform();
            // Request a repaint on everyone except me
            dynamic_cast<SimplePlot*>(m_module)->requestRepaintAllExcept(getID());
        }
        glUniformMatrix4fv(m_shader->getUniform("view_matrix"), 1, 0, glm::value_ptr(m_camera.view*transform));
    }else {
        glUniformMatrix4fv(m_shader->getUniform("view_matrix"), 1, 0, glm::value_ptr(m_camera.view * m_trackball.getTransform()));
    }
    glUniform2fv(m_shader->getUniform("h_minmax"), 1, &minmax[0]);
    glUniform1f(m_shader->getUniform("scale"), scale);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_data->getTextureID());

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(m_mesh->restart_token);

    glBindVertexArray(m_mesh->vao);
    glDrawElements(GL_TRIANGLE_STRIP, m_mesh->count, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);

    m_shader->disuse();

    glDisable(GL_PRIMITIVE_RESTART);
}

void SurfacePlot::init() {
    if(m_shader == NULL){
        m_shader = new GLUtils::Program(surface_vert::source, surface_frag::source);
    }

    if(m_mesh == NULL){
        m_mesh = createMesh(m_domain->getMesh(ocls::Domain::Dim::X), m_domain->getMesh(ocls::Domain::Dim::Y));
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    m_closed = false;
}

void SurfacePlot::clean() {
    delete m_mesh; // < TODO: mem leak, fix later
    m_mesh = NULL;

    delete m_shader;
    m_shader = NULL;

    m_closed = true;
}

void SurfacePlot::keypress(int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS && key == GLFW_KEY_LEFT_SHIFT){
        startSync();
    }
    else if(action == GLFW_RELEASE && key == GLFW_KEY_LEFT_SHIFT){
        endSync();
    }
    m_context->requestRepaint(m_window);
}
